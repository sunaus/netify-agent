// Netify Agent
// Copyright (C) 2015-2024 eGloo Incorporated
// <http://www.egloo.ca>
//
// This program is free software: you can redistribute it
// and/or modify it under the terms of the GNU General
// Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <mutex>

#include <netinet/in.h>

#include "nd-apps.hpp"
#include "nd-detection.hpp"
#include "nd-except.hpp"
#include "nd-fhc.hpp"
#include "nd-protos.hpp"
#include "nd-risks.hpp"
#include "nd-sha1.h"
#include "nd-tls-alpn.hpp"
#include "nd-util.hpp"

#include "ndpi_protocol_ids.h"

using namespace std;

// Enable flow hash cache debug logging
// #define _ND_LOG_FHC 1

// Enable to log unknown protocol debug
// #define _ND_LOG_PROTO_UNKNOWN   1

// Enable to log custom domain category lookups
// #define _ND_LOG_CATEGORY_DOTD_LOOKUPS 1

// Enable to log STUN debug
// #define _ND_LOG_STUN 1

#define ndEF    entry->flow
#define ndEFNF  entry->flow->ndpi_flow
#define ndEFNFP entry->flow->ndpi_flow->protos

ndDetectionThread::ndDetectionThread(int16_t cpu, const string &tag,
#ifdef _ND_ENABLE_NETLINK
  ndNetlink *netlink,
#endif
#ifdef _ND_ENABLE_CONNTRACK
  ndConntrackThread *thread_conntrack,
#endif
  ndDNSHintCache *dhc, ndFlowHashCache *fhc, uint8_t private_addr)
  : ndThread(tag, (long)cpu, true), ndInstanceClient(),
#ifdef _ND_ENABLE_NETLINK
    netlink(netlink),
#endif
#ifdef _ND_ENABLE_CONNTRACK
    thread_conntrack(thread_conntrack),
#endif
    ndpi(nullptr), dhc(dhc), fhc(fhc), flows(0) {
    Reload();

    private_addrs.first.ss_family = AF_INET;
    nd_private_ipaddr(private_addr, private_addrs.first);

    private_addrs.second.ss_family = AF_INET6;
    nd_private_ipaddr(private_addr, private_addrs.second);

    int rc;

    pthread_condattr_t cond_attr;

    pthread_condattr_init(&cond_attr);
    pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);
    if ((rc = pthread_cond_init(&pkt_queue_cond, &cond_attr)) != 0)
    {
        throw ndExceptionSystemErrno(__PRETTY_FUNCTION__,
          "pthread_cond_init", rc);
    }
    pthread_condattr_destroy(&cond_attr);

    if ((rc = pthread_mutex_init(&pkt_queue_cond_mutex, nullptr)) != 0)
    {
        throw ndExceptionSystemErrno(__PRETTY_FUNCTION__,
          "pthread_mutex_init", rc);
    }

    nd_dprintf("%s: detection thread created on CPU: %hu\n",
      tag.c_str(), cpu);
}

ndDetectionThread::~ndDetectionThread() {
    pthread_cond_broadcast(&pkt_queue_cond);

    Join();

    pthread_cond_destroy(&pkt_queue_cond);
    pthread_mutex_destroy(&pkt_queue_cond_mutex);

    while (pkt_queue.size()) {
        ndDetectionQueueEntry *entry = pkt_queue.front();
        pkt_queue.pop();

        delete entry;
    }

    if (ndpi != nullptr) nd_ndpi_free(ndpi);

    nd_dprintf(
      "%s: detection thread destroyed, %u flows "
      "processed.\n",
      tag.c_str(), flows);
}

void ndDetectionThread::Reload(void) {
    if (ndpi != nullptr) nd_ndpi_free(ndpi);
    ndpi = nd_ndpi_init();
}

void ndDetectionThread::QueuePacket(nd_flow_ptr &flow,
  const ndPacket *packet,
  const uint8_t *data,
  uint16_t length) {
    ndDetectionQueueEntry *entry = new ndDetectionQueueEntry(
      flow, packet, data, length);

    Lock();

    pkt_queue.push(entry);

    Unlock();

    int rc;
    if ((rc = pthread_cond_broadcast(&pkt_queue_cond)) != 0) {
        throw ndExceptionSystemErrno(__PRETTY_FUNCTION__,
          "pthread_cond_broadcast", rc);
    }
}

void *ndDetectionThread::Entry(void) {
    int rc;

    do {
        if ((rc = pthread_mutex_lock(&pkt_queue_cond_mutex)) != 0)
        {
            throw ndExceptionSystemErrno(__PRETTY_FUNCTION__,
              "pthread_mutex_lock", rc);
        }

        struct timespec ts_cond;
        if (clock_gettime(CLOCK_MONOTONIC, &ts_cond) != 0) {
            throw ndExceptionSystemError(__PRETTY_FUNCTION__,
              "clock_gettime");
        }

        ts_cond.tv_sec += 1;

        if ((rc = pthread_cond_timedwait(&pkt_queue_cond,
               &pkt_queue_cond_mutex, &ts_cond)) != 0 &&
          rc != ETIMEDOUT)
        {
            throw ndExceptionSystemErrno(__PRETTY_FUNCTION__,
              "pthread_cond_timedwait", rc);
        }

        if ((rc = pthread_mutex_unlock(&pkt_queue_cond_mutex)) != 0)
        {
            throw ndExceptionSystemErrno(__PRETTY_FUNCTION__,
              "pthread_mutex_unlock", rc);
        }

        ProcessPacketQueue();
    }
    while (ShouldTerminate() == false);

    ProcessPacketQueue();

    nd_dprintf("%s: detection thread ended on CPU: %hu\n",
      tag.c_str(), cpu);

    return nullptr;
}

ndProto::Id ndDetectionThread::ProtocolLookup(uint16_t id,
  ndDetectionQueueEntry *entry) {
    if (id == NDPI_PROTOCOL_UNKNOWN)
        return ndProto::Id::UNKNOWN;

    auto it = ndProto::nDPI::Protos.find(id);
    if (it == ndProto::nDPI::Protos.end())
        return ndProto::Id::TODO;

    auto it_pm = ndProto::PortMap.find(it->second);
    if (it_pm != ndProto::PortMap.end()) {
        for (auto &it_entry : it_pm->second) {
            if (ndEF->lower_addr.GetPort() != it_entry.first &&
              ndEF->upper_addr.GetPort() != it_entry.first)
                continue;
            return it_entry.second;
        }
    }

    return it->second;
}

void ndDetectionThread::HashCacheLookup(
  ndDetectionQueueEntry *entry, bool insert) {

    if (fhc != nullptr && ! ndEF->flags.fhc_hit &&
      ndEF->ip_protocol != IPPROTO_ICMP &&
      ndEF->ip_protocol != IPPROTO_ICMPV6 &&
      ndEF->ip_protocol != IPPROTO_IGMP &&
      ndEF->GetMasterProtocol() != ndProto::Id::DNS)
    {
        if (insert) {
            if (! ndEF->flags.fhc_hit &&
              ! ndEF->flags.detection_guessed &&
              (ndEF->detected_protocol != ndProto::Id::UNKNOWN ||
                ndEF->detected_application != ND_APP_UNKNOWN))
            {
                fhc->Insert(ndEF);
            }
            return;
        }

        if (ndEF->detected_protocol == ndProto::Id::UNKNOWN ||
          ndEF->detected_application == ND_APP_UNKNOWN)
        {
            ndFlowHashCacheEntry result;
            if (fhc->Lookup(ndEF->digest_lower, result)) {
                if (! equal(result.digest.begin(),
                      result.digest.end(),
                      ndEF->digest_mdata.begin()))
                {
                    ndEF->flags.fhc_hit = true;

                    if (result.proto_id != ndProto::Id::UNKNOWN &&
                      result.proto_id != ndEF->detected_protocol)
                    {
#ifdef _ND_LOG_FHC
                        string proto_from = nd_proto_get_name(
                          ndEF->detected_protocol);
                        string proto_to = nd_proto_get_name(
                          result.proto_id);
                        nd_dprintf(
                          "%s: updated proto from flow "
                          "hash cache: %s -> %s\n",
                          tag.c_str(), proto_from.c_str(),
                          proto_to.c_str());
#endif
                        ndEF->detected_protocol = result.proto_id;
                    }

                    if (result.app_id != ND_APP_UNKNOWN &&
                      result.app_id != ndEF->detected_application)
                    {
#ifdef _ND_LOG_FHC
                        string app_from;
                        ndi.apps.Lookup(
                          ndEF->detected_application, app_from);
                        string app_to;
                        ndi.apps.Lookup(result.app_id, app_to);
                        nd_dprintf(
                          "%s: updated app from flow hash "
                          "cache: %s -> %s\n",
                          tag.c_str(), app_from.c_str(),
                          app_to.c_str());
#endif
                        ndEF->detected_application = result.app_id;
                        ndi.apps.Lookup(result.app_id,
                          ndEF->detected_application_name);
                    }

                    ndEF->digest_mdata.assign(
                      result.digest.begin(), result.digest.end());
#ifdef _ND_LOG_FHC
                    nd_dprintf(
                      "%s: resurrected flow metadata "
                      "hash.\n",
                      tag.c_str());
#endif
                }
            }
        }
    }
}

void ndDetectionThread::ProcessPacketQueue(void) {
    ndDetectionQueueEntry *entry;

    do {
        Lock();

        if (pkt_queue.size()) {
            entry = pkt_queue.front();
            pkt_queue.pop();
        }
        else entry = nullptr;

        Unlock();

        if (entry != nullptr) {
            if (ndEF->stats.detection_packets.load() == 0 ||
              (ndEF->flags.detection_complete.load() == false &&
                ndEF->flags.expiring.load() == false &&
                ndEF->stats.detection_packets.load() <
                  ndGC.max_detection_pkts))
            {
                ndEF->stats.detection_packets++;

                ProcessPacket(entry);
            }

            if (ndEF->stats.detection_packets.load() ==
                ndGC.max_detection_pkts ||
              (ndEF->flags.expiring.load() &&
                ndEF->flags.expired.load() == false))
            {
                if (ndEFNF != nullptr) {
                    if (entry->packet != nullptr)
                        ProcessPacket(entry);

                    ProcessFlow(entry);

                    if (ndEF->detected_protocol == ndProto::Id::UNKNOWN)
                        SetGuessedProtocol(entry);
                }

                SetDetectionComplete(entry);

                if (ndEF->flags.expiring.load()) {
                    ndEF->flags.expired = true;
                    ndEF->flags.expiring = false;
                }
            }

            if (ndEF->flags.detection_complete.load() &&
              ndEFNF != nullptr)
                ndEF->Release();

            delete entry;
        }
    }
    while (entry != nullptr);
}

void ndDetectionThread::ProcessPacket(ndDetectionQueueEntry *entry) {
    bool flow_update = false;

    if (ndEFNF == nullptr) {
        flows++;

        ndEFNF = (ndpi_flow_struct *)ndpi_malloc(
          sizeof(ndpi_flow_struct));
        if (ndEFNF == nullptr) {
            throw ndExceptionSystemError(__PRETTY_FUNCTION__,
              "ndpi_malloc");
        }

        memset(ndEFNF, 0, sizeof(ndpi_flow_struct));
    }

    ndpi_protocol ndpi_rc = ndpi_detection_process_packet(ndpi,
      ndEFNF, entry->data, entry->length,
      ndEF->ts_last_seen.load(), nullptr);

    lock_guard<recursive_mutex> lg(ndEF->lock);

    if (ndpi_rc.master_protocol == NDPI_PROTOCOL_STUN &&
      ndpi_rc.app_protocol != NDPI_PROTOCOL_UNKNOWN)
    {
#ifdef _ND_LOG_STUN
        nd_dprintf(
          "%s: STUN[%lu]: master: %hu, app: %hu, stack[0]: "
          "%hu, "
          "stack[1]: %hu, detected_protocol: %hu\n",
          tag.c_str(), ndEF->stats.detection_packets.load(),
          ndpi_rc.master_protocol, ndpi_rc.app_protocol,
          ndEFNF->detected_protocol_stack[0],
          ndEFNF->detected_protocol_stack[1],
          ndEF->detected_protocol);
#endif
        ndProto::Id id = ProtocolLookup(ndpi_rc.app_protocol, entry);

        if (id != ndProto::Id::TODO) {
            ndEF->detected_protocol = id;
#ifdef _ND_LOG_STUN
            nd_dprintf(
              "%s: STUN[%lu]: refined detected protocol: "
              "%hu\n",
              tag.c_str(), ndEF->stats.detection_packets.load(),
              ndEF->detected_protocol);
#endif
        }
    }

    if (ndEF->detected_protocol == ndProto::Id::UNKNOWN &&
      ndpi_rc.master_protocol != NDPI_PROTOCOL_UNKNOWN)
    {
        ndEF->detected_protocol = ProtocolLookup(
          ndpi_rc.master_protocol, entry);
    }

    if (ndEF->detected_protocol == ndProto::Id::UNKNOWN &&
      ndpi_rc.app_protocol != NDPI_PROTOCOL_UNKNOWN)
    {
        ndEF->detected_protocol = ProtocolLookup(
          ndpi_rc.app_protocol, entry);

#if _ND_LOG_PROTO_UNKNOWN
        if (ndEF->detected_protocol != ndProto::Id::UNKNOWN) {
            char proto_name[64];
            ndpi_protocol2name(ndpi, ndpi_rc, proto_name,
              sizeof(proto_name));
            nd_dprintf(
              "%s: Set detected protocol from application "
              "hint: %s\n",
              tag.c_str(), proto_name);
        }
#endif
    }

    if (ndEF->detected_protocol == ndProto::Id::TODO) {
        char proto_name[64];
        ndpi_protocol2name(ndpi, ndpi_rc, proto_name,
          sizeof(proto_name));
        nd_dprintf(
          "%s: Unmapped detected protocol: ID #%hu/%hu "
          "(%s)\n",
          tag.c_str(), ndpi_rc.master_protocol,
          ndpi_rc.app_protocol, proto_name);
    }

    bool check_extra_packets = (ndEFNF->extra_packets_func != nullptr);

    if (ndEF->flags.detection_init.load() == false &&
      ndEF->detected_protocol != ndProto::Id::UNKNOWN)
    {
        ProcessFlow(entry);
        flow_update = true;
    }
    else if (ndEF->flags.detection_init.load()) {
        // Flows with extra packet processing...
        ndEF->flags.detection_updated = false;

        switch (ndEF->GetMasterProtocol()) {
        case ndProto::Id::TLS:
        case ndProto::Id::QUIC:
            if (ndEF->tls.client_sni.empty() &&
              ! ndEF->host_server_name.empty())
            {
                flow_update = true;
                ndEF->flags.detection_updated = true;
                ndEF->tls.client_sni = ndEF->host_server_name;
            }

            if (ndEF->tls.cipher_suite == 0 &&
              ndEFNFP.tls_quic.server_cipher != 0)
            {
                ndEF->tls.cipher_suite = ndEFNFP.tls_quic.server_cipher;

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEF->tls.server_cn.empty() &&
              ndEFNFP.tls_quic.serverCN != nullptr)
            {
                nd_set_hostname(ndEF->tls.server_cn,
                  ndEFNFP.tls_quic.serverCN,
                  ND_FLOW_TLS_CNLEN);

                free(ndEFNFP.tls_quic.serverCN);
                ndEFNFP.tls_quic.serverCN = nullptr;

                // Detect application by server CN if still
                // unknown.
                if (ndEF->detected_application == ND_APP_UNKNOWN)
                {
                    SetDetectedApplication(entry,
                      ndi.apps.Find(ndEF->tls.server_cn));
                }

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEF->tls.issuer_dn.empty() &&
              ndEFNFP.tls_quic.issuerDN != nullptr)
            {
                ndEF->tls.issuer_dn.assign(ndEFNFP.tls_quic.issuerDN);

                free(ndEFNFP.tls_quic.issuerDN);
                ndEFNFP.tls_quic.issuerDN = nullptr;

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEF->tls.subject_dn.empty() &&
              ndEFNFP.tls_quic.subjectDN != nullptr)
            {
                ndEF->tls.subject_dn.assign(ndEFNFP.tls_quic.subjectDN);

                free(ndEFNFP.tls_quic.subjectDN);
                ndEFNFP.tls_quic.subjectDN = nullptr;

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEF->tls.server_ja3.empty() &&
              ndEFNFP.tls_quic.ja3_server[0] != '\0')
            {
                ndEF->tls.server_ja3.assign(
                  ndEFNFP.tls_quic.ja3_server);

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEF->tls.cert_fingerprint.empty() &&
              ndEFNFP.tls_quic.fingerprint_set)
            {
                ndEF->tls.cert_fingerprint.reserve(SHA1_DIGEST_LENGTH);
                ndEF->tls.cert_fingerprint.resize(SHA1_DIGEST_LENGTH);
                memcpy(&ndEF->tls.cert_fingerprint[0],
                  ndEFNFP.tls_quic.sha1_certificate_fingerprint,
                  SHA1_DIGEST_LENGTH);

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEFNFP.tls_quic.advertised_alpns) {
                ProcessALPN(entry, true);

                free(ndEFNFP.tls_quic.advertised_alpns);
                ndEFNFP.tls_quic.advertised_alpns = nullptr;
            }

            if (ndEFNFP.tls_quic.negotiated_alpn) {
                flow_update = ProcessALPN(entry, false);

                free(ndEFNFP.tls_quic.negotiated_alpn);
                ndEFNFP.tls_quic.negotiated_alpn = nullptr;
            }

            if (ndEF->tls.ech.version != 0 &&
              ndEFNFP.tls_quic.encrypted_ch.version != 0)
            {
                ndEF->tls.ech.version =
                  ndEFNFP.tls_quic.encrypted_ch.version;

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            if (ndEF->tls.esni.esni.empty() &&
              ndEFNFP.tls_quic.encrypted_sni.esni != nullptr)
            {
                ndEF->tls.esni.cipher_suite =
                  ndEFNFP.tls_quic.encrypted_sni.cipher_suite;
                ndEF->tls.esni.esni.assign(
                  ndEFNFP.tls_quic.encrypted_sni.esni);

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }

            break;
        case ndProto::Id::DNS:
        case ndProto::Id::LLMNR:
            if (ndEF->host_server_name.empty() &&
              ndEFNF->host_server_name[0] != '\0')
            {
                flow_update = true;
                ndEF->host_server_name.assign(ndEFNF->host_server_name);
                nd_set_hostname(ndEF->host_server_name);

                SetDetectedApplication(entry,
                  ndi.apps.Find(ndEF->host_server_name));
            }
            break;
        case ndProto::Id::MDNS:
            if (ndEF->HasMDNSDomainName()) {
                flow_update = true;
                ndEF->flags.detection_updated = true;
            }
            // nd_dprintf("mDNS flow updated: %s, %s, %s\n",
            //     ndEF->host_server_name,
            //     ndEFNF->host_server_name,
            //     ndEF->mdns.domain_name);
            break;
        case ndProto::Id::SSH:
            if (ndEF->ssh.server_agent.empty() &&
              ndEFNFP.ssh.server_signature[0] != '\0')
            {
                ndEF->ssh.server_agent.assign(
                  ndEFNFP.ssh.server_signature);

                flow_update = true;
                ndEF->flags.detection_updated = true;
            }
            break;
        case ndProto::Id::BITTORRENT:
            if (ndEF->bt.info_hash.empty() &&
              ndEFNFP.bittorrent.hash[0] != '\0')
            {
                ndEF->bt.info_hash.reserve(SHA1_DIGEST_LENGTH);
                ndEF->bt.info_hash.resize(SHA1_DIGEST_LENGTH);
                memcpy(&ndEF->bt.info_hash[0],
                  ndEFNFP.bittorrent.hash, SHA1_DIGEST_LENGTH);
            }
            break;
        default: break;
        }
    }
#if 0
    if (ndEF->detected_protocol == ndProto::Id::MDNS) {
        nd_dprintf("mDNS flow updated: packets: %lu, check extra packets: %s.\n",
            ndEF->stats.detection_packets.load(), (check_extra_packets) ? "yes" : "no");
    }
#endif
    if (ndEF->GetMasterProtocol() == ndProto::Id::TLS ||
      ndEF->GetMasterProtocol() == ndProto::Id::QUIC)
    {
        if (ndEFNF->tls_quic.certificate_processed)
            check_extra_packets = false;
#if 0
        if (ndEFNFP.tls_quic.hello_processed) {
            nd_dprintf("%s: TLS hello processed.\n", tag.c_str());
        }
        if (ndEFNF->tls_quic.certificate_processed) {
            nd_dprintf(
              "%s: TLS server certificate processed.\n",
              tag.c_str());
        }
        if (ndEF->flags.risks_checked.load()) {
            nd_dprintf("%s: Risks checked.\n", tag.c_str());
        }
        nd_dprintf(
          "%s: detection_init: %d, check_extra_packets: "
          "%d, fin_ack: %hhu\n",
          tag.c_str(), ndEF->flags.detection_init.load(),
          check_extra_packets, ndEF->flags.tcp_fin_ack.load());
#endif
    }

    if (flow_update) FlowUpdate(entry);

    // Flow detection complete?
    if (ndEF->flags.detection_init.load() && ! check_extra_packets)
        SetDetectionComplete(entry);
}

bool ndDetectionThread::ProcessALPN(
  ndDetectionQueueEntry *entry, bool client) {
    bool flow_update = false;
    const char *detected_alpn = (client) ?
      ndEFNFP.tls_quic.advertised_alpns :
      ndEFNFP.tls_quic.negotiated_alpn;
    //    nd_dprintf("%s: TLS %s ALPN: %s\n",
    //        tag.c_str(), (client) ? "client" : "server",
    //        detected_alpn);

    if (client && ndEF->tls.alpn.empty()) {
        stringstream ss(detected_alpn);

        while (ss.good()) {
            string alpn;
            getline(ss, alpn, ',');

            ndEF->tls.alpn.push_back(alpn);
        }

        flow_update = (! ndEF->tls.alpn.empty());
    }
    else if (ndEF->tls.alpn_server.empty()) {
        ndEF->tls.alpn_server.push_back(detected_alpn);

        auto alpn = nd_alpn_protos.find(detected_alpn);
        if (alpn != nd_alpn_protos.end() &&
          alpn->second != ndEF->detected_protocol)
        {
            if ((ndGC_DEBUG && ndGC_VERBOSE)) {
                nd_dprintf(
                  "%s: TLS ALPN: refined: %s: %s -> "
                  "%s\n",
                  tag.c_str(), detected_alpn,
                  ndEF->detected_protocol_name.c_str(),
                  ndProto::GetName(alpn->second));
            }

            ndEF->detected_protocol = alpn->second;
            ndEF->detected_protocol_name = ndProto::GetName(
              alpn->second);

            flow_update = true;
            ndEF->flags.detection_updated = true;
        }
    }

    return flow_update;
}

void ndDetectionThread::ProcessFlow(ndDetectionQueueEntry *entry) {

    lock_guard<recursive_mutex> lg(ndEF->lock);

    ndi.addr_lookup.Classify(ndEF->lower_type, ndEF->lower_addr);
    ndi.addr_lookup.Classify(ndEF->upper_type, ndEF->upper_addr);

    if (dhc != nullptr &&
      ndEF->GetMasterProtocol() != ndProto::Id::DNS)
    {
        string hostname;

        if (ndEF->lower_type == ndAddr::Type::OTHER)
            ndEF->flags.dhc_hit = dhc->Lookup(
              ndEF->lower_addr, hostname);

        if (! ndEF->flags.dhc_hit.load() &&
          ndEF->upper_type == ndAddr::Type::OTHER)
        {
            ndEF->flags.dhc_hit = dhc->Lookup(
              ndEF->upper_addr, hostname);
        }

        if (ndEF->flags.dhc_hit.load())
            ndEF->dns_host_name = hostname;
    }

    if (ndEFNF->host_server_name[0] != '\0') {
        ndEF->host_server_name.assign(ndEFNF->host_server_name);
        nd_set_hostname(ndEF->host_server_name);
    }

    // Determine application based on master protocol metadata
    // for Protocol / Application "Twins"
    switch (ndEF->detected_protocol) {
    case ndProto::Id::APPLE_PUSH:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.apple-push"));
        break;

    case ndProto::Id::AVAST:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.avast"));
        break;

    case ndProto::Id::FACEBOOK_VOIP:
        // TODO: netify.meta-messaging or netify.facebook-messager?
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.meta-messaging"));
        break;

    case ndProto::Id::LINE_CALL:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.line"));
        break;

    case ndProto::Id::NEST_LOG_SINK:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.nest"));
        break;

    case ndProto::Id::SPOTIFY:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.spotify"));
        break;

    case ndProto::Id::SIGNAL_CALL:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.signal"));
        break;

    case ndProto::Id::SKYPE_TEAMS_CALL:
        // TODO: netify.skype or netify.teams?
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.teams"));
        break;

    case ndProto::Id::STEAM:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.steam"));
        break;

    case ndProto::Id::SYNCTHING:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.syncthing"));
        break;

    case ndProto::Id::TEAMVIEWER:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.teamviewer"));
        break;

    case ndProto::Id::TELEGRAM_VOIP:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.telegram"));
        break;

    case ndProto::Id::TIVOCONNECT:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.tivo"));
        break;

    case ndProto::Id::TPLINK_SHP:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.tp-link"));
        break;

    case ndProto::Id::TUYA_LP:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.tuya-smart"));
        break;

    case ndProto::Id::UBNTAC2:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.ubiquiti"));
        break;

    case ndProto::Id::WHATSAPP_CALL:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.whatsapp"));
        break;

    case ndProto::Id::ZOOM:
        SetDetectedApplication(entry,
          ndi.apps.Lookup("netify.zoom"));
        break;

    default: break;
    }

    // Determine application by host_server_name if still
    // unknown.
    if (ndEF->detected_application == ND_APP_UNKNOWN) {
        if (! ndEF->host_server_name.empty())
            SetDetectedApplication(entry,
              ndi.apps.Find(ndEF->host_server_name));
    }

    // Determine application by dns_host_name if still
    // unknown.
    if (ndEF->detected_application == ND_APP_UNKNOWN) {
        if (! ndEF->dns_host_name.empty())
            SetDetectedApplication(entry,
              ndi.apps.Find(ndEF->dns_host_name));
    }

    // Determine application by network CIDR if still unknown.
    // DNS flows excluded...
    if (ndEF->GetMasterProtocol() != ndProto::Id::DNS &&
      ndEF->detected_application == ND_APP_UNKNOWN)
    {
        if (ndEF->lower_type == ndAddr::Type::OTHER) {
            SetDetectedApplication(entry,
              ndi.apps.Find(ndEF->lower_addr));

            if (ndEF->detected_application == ND_APP_UNKNOWN)
                SetDetectedApplication(entry,
                  ndi.apps.Find(ndEF->upper_addr));
        }
        else {
            SetDetectedApplication(entry,
              ndi.apps.Find(ndEF->upper_addr));

            if (ndEF->detected_application == ND_APP_UNKNOWN)
                SetDetectedApplication(entry,
                  ndi.apps.Find(ndEF->lower_addr));
        }
    }

    // Additional protocol-specific processing...
    switch (ndEF->GetMasterProtocol()) {
    case ndProto::Id::TLS:
    case ndProto::Id::QUIC:
        ndEF->tls.version = ndEFNFP.tls_quic.ssl_version;
        ndEF->tls.cipher_suite = ndEFNFP.tls_quic.server_cipher;

        if (ndEF->tls.client_sni.empty() &&
          ! ndEF->host_server_name.empty())
            ndEF->tls.client_sni = ndEF->host_server_name;

        if (ndEF->tls.server_cn.empty() &&
          ndEFNFP.tls_quic.serverCN != nullptr)
        {
            nd_set_hostname(ndEF->tls.server_cn,
              ndEFNFP.tls_quic.serverCN,
              ND_FLOW_TLS_CNLEN);

            free(ndEFNFP.tls_quic.serverCN);
            ndEFNFP.tls_quic.serverCN = nullptr;

            // Detect application by server CN if still unknown.
            if (ndEF->detected_application == ND_APP_UNKNOWN)
                SetDetectedApplication(entry,
                  ndi.apps.Find(ndEF->tls.server_cn));
        }

        if (ndEFNFP.tls_quic.issuerDN != nullptr) {
            ndEF->tls.issuer_dn.assign(ndEFNFP.tls_quic.issuerDN);

            nd_trim(ndEF->tls.issuer_dn);

            free(ndEFNFP.tls_quic.issuerDN);
            ndEFNFP.tls_quic.issuerDN = nullptr;
        }

        if (ndEFNFP.tls_quic.subjectDN != nullptr) {
            ndEF->tls.subject_dn.assign(ndEFNFP.tls_quic.subjectDN);

            nd_trim(ndEF->tls.subject_dn);

            free(ndEFNFP.tls_quic.subjectDN);
            ndEFNFP.tls_quic.subjectDN = nullptr;
        }

        if (ndEF->tls.ech.version != 0 &&
          ndEFNFP.tls_quic.encrypted_ch.version != 0)
        {
            ndEF->tls.ech.version =
              ndEFNFP.tls_quic.encrypted_ch.version;
        }

        if (ndEF->tls.esni.esni.empty() &&
          ndEFNFP.tls_quic.encrypted_sni.esni != nullptr)
        {
            ndEF->tls.esni.cipher_suite =
              ndEFNFP.tls_quic.encrypted_sni.cipher_suite;
            ndEF->tls.esni.esni.assign(
              ndEFNFP.tls_quic.encrypted_sni.esni);
        }
        break;
    case ndProto::Id::HTTP:
    case ndProto::Id::SSDP:
        if (ndEFNF->http.user_agent != nullptr) {
            ndEF->http.user_agent.assign(ndEFNF->http.user_agent);

            nd_trim(ndEF->http.user_agent);

            free(ndEFNF->http.user_agent);
            ndEFNF->http.user_agent = nullptr;
        }

        if (ndEFNF->http.url != nullptr) {
            ndEF->http.url.assign(ndEFNF->http.url);

            nd_trim(ndEF->http.url);

            free(ndEFNF->http.url);
            ndEFNF->http.url = nullptr;
        }

        break;
    case ndProto::Id::DHCP:
        if (ndEFNFP.dhcp.fingerprint[0] != '\0')
            ndEF->dhcp.fingerprint.assign(ndEFNFP.dhcp.fingerprint);
        if (ndEFNFP.dhcp.class_ident[0] != '\0')
            ndEF->dhcp.class_ident.assign(ndEFNFP.dhcp.class_ident);
        break;
    case ndProto::Id::SSH:
        if (ndEFNFP.ssh.client_signature[0] != '\0')
            ndEF->ssh.client_agent.assign(ndEFNFP.ssh.client_signature);
        if (ndEFNFP.ssh.server_signature[0] != '\0')
            ndEF->ssh.server_agent.assign(ndEFNFP.ssh.server_signature);
        break;
    case ndProto::Id::BITTORRENT:
        if (ndEFNFP.bittorrent.hash[0] != '\0') {
            ndEF->bt.info_hash.assign(SHA1_DIGEST_LENGTH, 0);
            memcpy(&ndEF->bt.info_hash[0],
              ndEFNFP.bittorrent.hash, SHA1_DIGEST_LENGTH);
        }
        break;
    default: break;
    }

    ndEF->Hash(tag, true);

    HashCacheLookup(entry);

    for (int t = 0; t < 2; t++) {
        const ndAddr *mac, *ip;
        ndAddr::Type type = ndAddr::Type::NONE;

        if (t == 0 &&
          (ndEF->lower_type == ndAddr::Type::LOCAL ||
            ndEF->lower_type == ndAddr::Type::LOCALNET ||
            ndEF->lower_type == ndAddr::Type::RESERVED))
        {
            ndi.addr_lookup.Classify(type, ndEF->lower_mac);

            mac = &ndEF->lower_mac;
            ip = &ndEF->lower_addr;
        }
        else if (
          (ndEF->upper_type == ndAddr::Type::LOCAL ||
            ndEF->upper_type == ndAddr::Type::LOCALNET ||
            ndEF->upper_type == ndAddr::Type::RESERVED))
        {
            ndi.addr_lookup.Classify(type, ndEF->upper_mac);

            mac = &ndEF->upper_mac;
            ip = &ndEF->upper_addr;
        }

        if (type != ndAddr::Type::OTHER || mac == nullptr ||
          ip == nullptr)
            continue;

        ndEF->iface->PushEndpoint(*mac, *ip);
    }

#ifdef _ND_ENABLE_CONNTRACK
    if (thread_conntrack != nullptr &&
      ndEF->iface->role == ndInterfaceRole::WAN &&
      ((ndEF->lower_type == ndAddr::Type::LOCAL &&
         ndEF->upper_type == ndAddr::Type::OTHER) ||
        (ndEF->lower_type == ndAddr::Type::OTHER &&
          ndEF->upper_type == ndAddr::Type::LOCAL)))
    {
        // Update flow with any collected information from the
        // connection tracker (CT ID, mark, NAT'd).
        thread_conntrack->UpdateFlow(ndEF);
    }
#endif
    ndEF->detected_protocol_name = ndProto::GetName(
      ndEF->detected_protocol);

    if (ndEF->detected_protocol != ndProto::Id::UNKNOWN) {
        ndEF->category.protocol = ndi.categories.Lookup(
          ndCategories::Type::PROTO,
          (unsigned)ndEF->detected_protocol);
    }

    if (ndEFNF->host_server_name[0] != '\0') {
        ndEF->category.domain = ndi.categories.LookupDotDirectory(
          ndEFNF->host_server_name);
#ifdef _ND_LOG_CATEGORY_DOTD_LOOKUPS
        nd_dprintf("%s: category.domain: %hu\n",
          tag.c_str(), ndEF->category.domain);
#endif
    }

    ndEF->UpdateLowerMaps();

    for (
      vector<uint8_t *>::const_iterator i =
        ndGC.privacy_filter_mac.begin();
      i != ndGC.privacy_filter_mac.end() &&
      ndEF->privacy_mask !=
        (ndFlow::PrivacyMask::LOWER | ndFlow::PrivacyMask::UPPER);
      i++)
    {
#if defined(__linux__)
        if (! memcmp((*i), ndEF->lower_mac.addr.ll.sll_addr, ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER;
        if (! memcmp((*i), ndEF->upper_mac.addr.ll.sll_addr, ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER;
#elif defined(__FreeBSD__)
        if (! memcmp((*i), LLADDR(&ndEF->lower_mac.addr.dl), ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER;
        if (! memcmp((*i), LLADDR(&ndEF->upper_mac.addr.dl), ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER;
#endif
    }

    for (
      vector<struct sockaddr *>::const_iterator i =
        ndGC.privacy_filter_host.begin();
      i != ndGC.privacy_filter_host.end() &&
      ndEF->privacy_mask !=
        (ndFlow::PrivacyMask::LOWER | ndFlow::PrivacyMask::UPPER);
      i++)
    {
        struct sockaddr_in *sa_in;
        struct sockaddr_in6 *sa_in6;

        switch ((*i)->sa_family) {
        case AF_INET:
            sa_in = reinterpret_cast<struct sockaddr_in *>((*i));
            if (! memcmp(&ndEF->lower_addr.addr.in.sin_addr,
                  &sa_in->sin_addr,
                  sizeof(struct in_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER;
            if (! memcmp(&ndEF->upper_addr.addr.in.sin_addr,
                  &sa_in->sin_addr,
                  sizeof(struct in_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER;
            break;
        case AF_INET6:
            sa_in6 = reinterpret_cast<struct sockaddr_in6 *>((*i));
            if (! memcmp(&ndEF->lower_addr.addr.in6.sin6_addr,
                  &sa_in6->sin6_addr,
                  sizeof(struct in6_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER;
            if (! memcmp(&ndEF->upper_addr.addr.in6.sin6_addr,
                  &sa_in6->sin6_addr,
                  sizeof(struct in6_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER;
            break;
        }

        // TODO: Update the text IP addresses that were set
        // above...
    }

    ndEF->flags.detection_init = true;
}

void ndDetectionThread::ProcessRisks(ndDetectionQueueEntry *entry) {
    if (ndEFNF->risk != NDPI_NO_RISK) {
        for (unsigned i = 0; i < NDPI_MAX_RISK; i++) {
            if (NDPI_ISSET_BIT(ndEFNF->risk, i) != 0) {
                ndEF->risk.risks.insert(ndRisk::nDPI::Find(i));
            }
        }

        ndEF->risk.ndpi_score = ndpi_risk2score(ndEFNF->risk,
          &ndEF->risk.ndpi_score_client,
          &ndEF->risk.ndpi_score_server);
    }
}

void ndDetectionThread::SetDetectedApplication(
  ndDetectionQueueEntry *entry, nd_app_id_t app_id) {
    if (app_id == ND_APP_UNKNOWN) return;

    ndEF->detected_application = app_id;
    ndi.apps.Lookup(app_id, ndEF->detected_application_name);

    ndEF->category.application = ndi.categories.Lookup(
      ndCategories::Type::APP, app_id);
}

void ndDetectionThread::SetGuessedProtocol(
  ndDetectionQueueEntry *entry) {
    uint8_t guessed = 0;
    ndpi_protocol ndpi_rc = ndpi_detection_giveup(ndpi,
      ndEFNF, 1, &guessed);

    if (guessed) {
        ndEF->detected_protocol = ProtocolLookup(
          ndpi_rc.master_protocol, entry);

        if (ndEF->detected_protocol == ndProto::Id::UNKNOWN) {
            ndEF->detected_protocol = ProtocolLookup(
              ndpi_rc.app_protocol, entry);
        }
    }

    ndEF->flags.detection_guessed = true;
}

void ndDetectionThread::SetDetectionComplete(
  ndDetectionQueueEntry *entry) {

    lock_guard<recursive_mutex> lg(ndEF->lock);

    if (ndEF->flags.detection_complete.load()) return;

    ProcessRisks(entry);

    ndEF->flags.detection_complete = true;

    FlowUpdate(entry);

    HashCacheLookup(entry, true);
}

void ndDetectionThread::FlowUpdate(ndDetectionQueueEntry *entry) {
    if (ndEF->category.application == ND_CAT_UNKNOWN &&
      ndEF->detected_application != ND_APP_UNKNOWN)
    {
        ndEF->category.application = ndi.categories.Lookup(
          ndCategories::Type::APP,
          (unsigned)ndEF->detected_application);
    }

    if (ndEF->category.protocol == ND_CAT_UNKNOWN &&
      ndEF->detected_protocol != ndProto::Id::UNKNOWN)
    {
        ndEF->category.protocol = ndi.categories.Lookup(
          ndCategories::Type::PROTO,
          (unsigned)ndEF->detected_protocol);
    }

    if (ndEF->category.domain == ND_CAT_UNKNOWN &&
      ! ndEF->host_server_name.empty())
    {
        ndEF->category.domain = ndi.categories.LookupDotDirectory(
          ndEF->host_server_name);
    }

    if (ndGC_SOFT_DISSECTORS) {
        ndSoftDissector nsd;

        if (ndi.apps.SoftDissectorMatch(ndEF, &parser, nsd)) {
            ndEF->flags.soft_dissector = true;

            if (nsd.aid > -1) {
                if (nsd.aid == ND_APP_UNKNOWN) {
                    ndEF->detected_application = 0;
                    ndEF->detected_application_name.clear();
                    ndEF->category.application = ND_CAT_UNKNOWN;
                }
                else
                    SetDetectedApplication(entry,
                      (nd_app_id_t)nsd.aid);
            }

            if (nsd.pid > -1) {
                ndEF->detected_protocol = (ndProto::Id)nsd.pid;

                ndEF->category.protocol = ndi.categories.Lookup(
                  ndCategories::Type::PROTO,
                  (unsigned)ndEF->detected_protocol);
                ndEF->detected_protocol_name =
                  ndProto::GetName(ndEF->detected_protocol);
            }

            SetDetectionComplete(entry);
        }
    }

    ndPluginProcessor::Event event = ndPluginProcessor::Event::DPI_NEW;

    if (ndEF->flags.detection_complete.load())
        event = ndPluginProcessor::Event::DPI_COMPLETE;
    else if (ndEF->flags.detection_updated.load())
        event = ndPluginProcessor::Event::DPI_UPDATE;

    ndi.plugins.BroadcastProcessorEvent(event, ndEF);

    if (ndGC_DEBUG || ndGC.h_flow != stderr) {
        bool output = false;
        ndFlags<ndFlow::PrintFlags> flags = ndFlow::PrintFlags::METADATA;

        if (ndGC.verbosity > 1)  // -vv
            flags |= ndFlow::PrintFlags::RISKS;
        if (ndGC.verbosity > 2)  // -vvv
            flags |= ndFlow::PrintFlags::STATS;
        if (ndGC.verbosity > 3)  // -vvvv
            flags |= ndFlow::PrintFlags::STATS_FULL;
        if (ndGC.verbosity > 4)  // -vvvvv
            flags |= ndFlow::PrintFlags::MACS;
        if (ndGC.verbosity > 5)  // -vvvvvv
            flags |= ndFlow::PrintFlags::HASHES;
        if (ndGC.verbosity > 6)
            flags = ndFlow::PrintFlags::ALL;

        if (ndGC.debug_flow_print_exprs.size()) {
            for (auto &it : ndGC.debug_flow_print_exprs) {
                try {
                    if (! parser.Parse(ndEF, it)) continue;
                    output = true;
                    break;
                }
                catch (string &e) {
                    nd_dprintf("%s: %s: %s\n", tag.c_str(),
                      it.c_str(), e.c_str());
                }
            }
        }
        else if (ndGC_VERBOSE || ndGC.h_flow != stderr)
            output = true;

        switch (event) {
        case ndPluginProcessor::Event::DPI_NEW:
            if (! ndFlagBoolean(ndGC.verbosity_flags,
                  ndVerbosityFlags::EVENT_DPI_NEW))
                output = false;
            break;
        case ndPluginProcessor::Event::DPI_UPDATE:
            if (! ndFlagBoolean(ndGC.verbosity_flags,
                  ndVerbosityFlags::EVENT_DPI_UPDATE))
                output = false;
            break;
        case ndPluginProcessor::Event::DPI_COMPLETE:
            if (! ndFlagBoolean(ndGC.verbosity_flags,
                  ndVerbosityFlags::EVENT_DPI_COMPLETE))
                output = false;
            break;
        default: break;
        }

        if (output) ndEF->Print(flags);
    }
}
