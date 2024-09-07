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

// Enable extra flow debug logging
// #define _ND_LOG_FLOW_DEBUG 1

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
    ndpi(nullptr), dhc(dhc), fhc(fhc) {
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

#ifdef _ND_ENABLE_DEBUG_STATS
    uint64_t pkts_flushed = 0;
#endif
    while (pkt_queue.size()) {
        ndDetectionQueueEntry *entry = pkt_queue.front();
        pkt_queue.pop();

        delete entry;
#ifdef _ND_ENABLE_DEBUG_STATS
        pkts_flushed++;
#endif
    }

    if (ndpi != nullptr) nd_ndpi_free(ndpi);

    nd_dprintf("%s: detection thread destroyed.\n", tag.c_str());
#ifdef _ND_ENABLE_DEBUG_STATS
    nd_dprintf(
      "%s: %llu flows, %llu pkts, %llu pkts flushed, max "
      "pkts queued: %llu, max pkts queue size: %llu MB.\n",
      tag.c_str(), flows, pkts, pkts_flushed,
      max_queued_pkts, (max_queued_size / 1024 / 1024));
#endif
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

#ifdef _ND_ENABLE_DEBUG_STATS
    pkts++;
    queued_pkts++;
    queued_size += (entry->length + sizeof(ndDetectionQueueEntry));

    max_queued_pkts = max(max_queued_pkts, queued_pkts);
    max_queued_size = max(max_queued_size, queued_size);
#endif
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

void ndDetectionThread::ProcessPacketQueue(void) {
    ndDetectionQueueEntry *entry;

    do {
        Lock();

        if (pkt_queue.size()) {
            entry = pkt_queue.front();
            pkt_queue.pop();
#ifdef _ND_ENABLE_DEBUG_STATS
            queued_pkts--;
            queued_size -= (entry->length +
              sizeof(ndDetectionQueueEntry));
#endif
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
                lock_guard<recursive_mutex> lg(ndEF->lock);

                if (ndEFNF != nullptr) {
                    if (entry->packet != nullptr)
                        ProcessPacket(entry);

                    if (ndEF->flags.detection_init.load() == false)
                        DetectionUpdate(entry);

                    if (ndEF->detected_protocol == ndProto::Id::UNKNOWN)
                        DetectionGuess(entry);
                }

                DetectionComplete(entry);

                if (ndEF->flags.expiring.load()) {
                    ndEF->flags.expired = true;
                    ndEF->flags.expiring = false;
                }
            }

            if (ndEF->flags.detection_complete.load())
                ndEF->Release();

            delete entry;
        }
    }
    while (entry != nullptr);
}

void ndDetectionThread::ProcessPacket(ndDetectionQueueEntry *entry) {
    if (ndEFNF == nullptr) {
        lock_guard<recursive_mutex> lg(ndEF->lock);

        ProcessFlow(entry);

        if (fhc != nullptr && ! ndEF->flags.fhc_hit.load() &&
          ndEF->ip_protocol != IPPROTO_ICMP &&
          ndEF->ip_protocol != IPPROTO_ICMPV6 &&
          ndEF->ip_protocol != IPPROTO_IGMP)
        {
            ndFlowHashCacheEntry result;
            if (fhc->Lookup(ndEF->digest_lower, result)) {
                if (result.proto_id != ndProto::Id::UNKNOWN) {
#ifdef _ND_LOG_FHC
                    string proto_from = ndProto::GetName(
                      ndEF->detected_protocol);
                    string proto_to = ndProto::GetName(
                      result.proto_id);
                    nd_dprintf(
                      "%s: FHC: updated proto from cache: "
                      "%s -> %s\n",
                      tag.c_str(), proto_from.c_str(),
                      proto_to.c_str());
#endif
                    SetDetectedProtocol(entry, result.proto_id);
                }

                if (result.app_id != ND_APP_UNKNOWN) {
#ifdef _ND_LOG_FHC
                    string app_from;
                    ndi.apps.Lookup(
                      ndEF->detected_application, app_from);
                    string app_to;
                    ndi.apps.Lookup(result.app_id, app_to);
                    nd_dprintf(
                      "%s: FHC: updated app from cache: "
                      "%s -> %s\n",
                      tag.c_str(), app_from.c_str(),
                      app_to.c_str());
#endif
                    SetDetectedApplication(entry, result.app_id);
                }

                ndEF->digest_mdata.push_back(result.digest);

                ndEF->flags.fhc_hit = true;

                DetectionUpdate(entry);
                DetectionComplete(entry);

                return;
            }
        }

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
          "%hu, stack[1]: %hu, detected_protocol: %hu\n",
          tag.c_str(), ndEF->stats.detection_packets.load(),
          ndpi_rc.master_protocol, ndpi_rc.app_protocol,
          ndEFNF->detected_protocol_stack[0],
          ndEFNF->detected_protocol_stack[1],
          ndEF->detected_protocol);
#endif
        ndProto::Id id = ProtocolLookup(ndpi_rc.app_protocol, entry);

        if (id != ndProto::Id::TODO) {
            SetDetectedProtocol(entry, id);
#ifdef _ND_LOG_STUN
            nd_dprintf(
              "%s: STUN[%lu]: refined detected protocol: "
              "%hu\n",
              tag.c_str(),
              ndEF->stats.detection_packets.load(),
              ndEF->detected_protocol);
#endif
        }
    }

    if (ndEF->detected_protocol == ndProto::Id::UNKNOWN) {
        if (ndpi_rc.master_protocol != NDPI_PROTOCOL_UNKNOWN)
        {
            SetDetectedProtocol(entry,
              ProtocolLookup(ndpi_rc.master_protocol, entry));
        }
        else if (ndpi_rc.app_protocol != NDPI_PROTOCOL_UNKNOWN)
        {
            SetDetectedProtocol(entry,
              ProtocolLookup(ndpi_rc.app_protocol, entry));
        }
    }

    if (ndEF->host_server_name.empty() &&
      ndEFNF->host_server_name[0] != '\0')
    {
        SetHostServerName(entry, ndEFNF->host_server_name);
    }

    if (ndEF->detected_protocol == ndProto::Id::TODO) {
        char proto_name[64];
        ndpi_protocol2name(ndpi, ndpi_rc, proto_name,
          sizeof(proto_name));
        nd_dprintf(
          "%s: unmapped protocol detected: ID #%hu/%hu "
          "(%s)\n",
          tag.c_str(), ndpi_rc.master_protocol,
          ndpi_rc.app_protocol, proto_name);

        DetectionComplete(entry);
        return;
    }

    if (ndEF->detected_protocol != ndProto::Id::UNKNOWN)
        DetectionUpdate(entry);

    if ((ndEF->flags.detection_init.load() &&
          ndEFNF->extra_packets_func == nullptr) ||
      ndEFNF->num_extra_packets_checked > ndEFNF->max_extra_packets_to_check ||
      ((ndEF->GetMasterProtocol() == ndProto::Id::TLS ||
         ndEF->detected_protocol == ndProto::Id::DTLS ||
         ndEF->detected_protocol == ndProto::Id::QUIC) &&
        ndEF->tls.proc_hello.load() &&
        ndEF->tls.proc_certificate.load()))
    {
        if (ndEF->detected_protocol == ndProto::Id::UNKNOWN)
            DetectionGuess(entry);

        DetectionComplete(entry);
    }
}

void ndDetectionThread::ProcessFlow(ndDetectionQueueEntry *entry) {
#ifdef _ND_ENABLE_DEBUG_STATS
    flows++;
#endif
    ndi.addr_lookup.Classify(ndEF->lower_type, ndEF->lower_addr);
    ndi.addr_lookup.Classify(ndEF->upper_type, ndEF->upper_addr);

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

    ndEF->UpdateLowerMaps();

    if (ndEF->lower_type == ndAddr::Type::OTHER) {
        ndEF->category.network =
          ndi.categories.LookupDotDirectory(ndEF->lower_addr);
    }
    else if (ndEF->upper_type == ndAddr::Type::OTHER) {
        ndEF->category.network =
          ndi.categories.LookupDotDirectory(ndEF->upper_addr);
    }

    for (
      vector<uint8_t *>::const_iterator i =
        ndGC.privacy_filter_mac.begin();
      i != ndGC.privacy_filter_mac.end() &&
      ndEF->privacy_mask !=
        (ndFlow::PrivacyMask::LOWER_MAC | ndFlow::PrivacyMask::UPPER_MAC);
      i++)
    {
#if defined(__linux__)
        if (! memcmp((*i), ndEF->lower_mac.addr.ll.sll_addr, ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER_MAC;
        if (! memcmp((*i), ndEF->upper_mac.addr.ll.sll_addr, ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER_MAC;
#elif defined(__FreeBSD__)
        if (! memcmp((*i), LLADDR(&ndEF->lower_mac.addr.dl), ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER_MAC;
        if (! memcmp((*i), LLADDR(&ndEF->upper_mac.addr.dl), ETH_ALEN))
            ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER_MAC;
#endif
    }

    for (
      vector<struct sockaddr *>::const_iterator i =
        ndGC.privacy_filter_host.begin();
      i != ndGC.privacy_filter_host.end() &&
      ndEF->privacy_mask !=
        (ndFlow::PrivacyMask::LOWER_IP | ndFlow::PrivacyMask::UPPER_IP);
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
                ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER_IP;
            if (! memcmp(&ndEF->upper_addr.addr.in.sin_addr,
                  &sa_in->sin_addr,
                  sizeof(struct in_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER_IP;
            break;
        case AF_INET6:
            sa_in6 = reinterpret_cast<struct sockaddr_in6 *>((*i));
            if (! memcmp(&ndEF->lower_addr.addr.in6.sin6_addr,
                  &sa_in6->sin6_addr,
                  sizeof(struct in6_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::LOWER_IP;
            if (! memcmp(&ndEF->upper_addr.addr.in6.sin6_addr,
                  &sa_in6->sin6_addr,
                  sizeof(struct in6_addr)))
                ndEF->privacy_mask |= ndFlow::PrivacyMask::UPPER_IP;
            break;
        }

        // TODO: Update the text IP addresses that were set
        // above...
    }

    if (dhc != nullptr) {
        string hostname;

        if (ndEF->lower_type == ndAddr::Type::OTHER)
            dhc->Lookup(ndEF->lower_addr, hostname);

        if (hostname.empty() && ndEF->upper_type == ndAddr::Type::OTHER)
            dhc->Lookup(ndEF->upper_addr, hostname);

        if (! hostname.empty())
            ndEF->dns_host_name = hostname;
    }
}

bool ndDetectionThread::ProcessALPN(
  ndDetectionQueueEntry *entry, bool client) {
    bool updated = false;
    const char *detected_alpn = (client) ?
      ndEFNFP.tls_quic.advertised_alpns :
      ndEFNFP.tls_quic.negotiated_alpn;

    if (client && ndEF->tls.alpn.empty()) {
        stringstream ss(detected_alpn);

        while (ss.good()) {
            string alpn;
            getline(ss, alpn, ',');

            ndEF->tls.alpn.push_back(alpn);
        }

        updated = (! ndEF->tls.alpn.empty());
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

            SetDetectedProtocol(entry, alpn->second);

            updated = true;
        }
    }

    return updated;
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

void ndDetectionThread::SetHostServerName(
  ndDetectionQueueEntry *entry, const char *host_server_name) {
    if (host_server_name[0] != '\0') {
        ndEF->host_server_name.assign(host_server_name);
        nd_set_hostname(ndEF->host_server_name);

        SetDetectedApplication(entry,
          ndi.apps.Find(ndEF->host_server_name));

        ndEF->category.domain = ndi.categories.LookupDotDirectory(
          ndEFNF->host_server_name);
    }
}

void ndDetectionThread::SetDetectedProtocol(
  ndDetectionQueueEntry *entry, ndProto::Id id) {
    if (id == ndProto::Id::UNKNOWN) return;

    ndEF->detected_protocol = id;
    ndEF->detected_protocol_name = ndProto::GetName(id);

    ndEF->category.protocol = ndi.categories.Lookup(
      ndCategories::Type::PROTO, (unsigned)id);
}

void ndDetectionThread::SetDetectedApplication(
  ndDetectionQueueEntry *entry, nd_app_id_t id) {
    if (id == ND_APP_UNKNOWN) return;

    ndEF->detected_application = id;
    ndi.apps.Lookup(id, ndEF->detected_application_name);

    ndEF->category.application = ndi.categories.Lookup(
      ndCategories::Type::APP, id);
}

void ndDetectionThread::DispatchEvent(ndDetectionQueueEntry *entry) {
    ndPluginProcessor::Event event = ndPluginProcessor::Event::DPI_NEW;

    if (ndEF->flags.detection_complete.load())
        event = ndPluginProcessor::Event::DPI_COMPLETE;
    else if (ndEF->flags.detection_updated.load())
        event = ndPluginProcessor::Event::DPI_UPDATE;

    ndi.plugins.BroadcastProcessorEvent(event, ndEF);

    if (ndGC_DEBUG || ndGC.h_flow != stderr) {
        bool flow_print = false;
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
                    flow_print = true;
                    break;
                }
                catch (string &e) {
                    nd_dprintf("%s: %s: %s\n", tag.c_str(),
                      it.c_str(), e.c_str());
                }
            }
        }
        else if (ndGC_VERBOSE || ndGC.h_flow != stderr)
            flow_print = true;

        switch (event) {
        case ndPluginProcessor::Event::DPI_NEW:
            if (! ndFlagBoolean(ndGC.verbosity_flags,
                  ndVerbosityFlags::EVENT_DPI_NEW))
                flow_print = false;
            break;
        case ndPluginProcessor::Event::DPI_UPDATE:
            if (! ndFlagBoolean(ndGC.verbosity_flags,
                  ndVerbosityFlags::EVENT_DPI_UPDATE))
                flow_print = false;
            break;
        case ndPluginProcessor::Event::DPI_COMPLETE:
            if (! ndFlagBoolean(ndGC.verbosity_flags,
                  ndVerbosityFlags::EVENT_DPI_COMPLETE))
                flow_print = false;
            break;
        default: break;
        }

        if (flow_print) ndEF->Print(flags);
    }
}

void ndDetectionThread::DetectionUpdate(ndDetectionQueueEntry *entry) {

    if (ndEF->flags.detection_init.load() == false) {
        // Determine application based on master protocol metadata
        // for Protocol / Application "Twins"
        nd_app_id_t app_twin = ND_APP_UNKNOWN;

        switch (ndEF->detected_protocol) {
        case ndProto::Id::APPLE_PUSH:
            app_twin = ndi.apps.Lookup("netify.apple-push");
            break;

        case ndProto::Id::AVAST:
            app_twin = ndi.apps.Lookup("netify.avast");
            break;

        case ndProto::Id::FACEBOOK_VOIP:
            // TODO: netify.meta-messaging or netify.facebook-messager?
            app_twin = ndi.apps.Lookup(
              "netify.meta-messaging");
            break;

        case ndProto::Id::LINE_CALL:
            app_twin = ndi.apps.Lookup("netify.line");
            break;

        case ndProto::Id::NEST_LOG_SINK:
            app_twin = ndi.apps.Lookup("netify.nest");
            break;

        case ndProto::Id::SPOTIFY:
            app_twin = ndi.apps.Lookup("netify.spotify");
            break;

        case ndProto::Id::SIGNAL_CALL:
            app_twin = ndi.apps.Lookup("netify.signal");
            break;

        case ndProto::Id::SKYPE_TEAMS_CALL:
            // TODO: netify.skype or netify.teams?
            app_twin = ndi.apps.Lookup("netify.teams");
            break;

        case ndProto::Id::STEAM:
            app_twin = ndi.apps.Lookup("netify.steam");
            break;

        case ndProto::Id::SYNCTHING:
            app_twin = ndi.apps.Lookup("netify.syncthing");
            break;

        case ndProto::Id::TEAMVIEWER:
            app_twin = ndi.apps.Lookup("netify.teamviewer");
            break;

        case ndProto::Id::TELEGRAM_VOIP:
            app_twin = ndi.apps.Lookup("netify.telegram");
            break;

        case ndProto::Id::TIVOCONNECT:
            app_twin = ndi.apps.Lookup("netify.tivo");
            break;

        case ndProto::Id::TPLINK_SHP:
            app_twin = ndi.apps.Lookup("netify.tp-link");
            break;

        case ndProto::Id::TUYA_LP:
            app_twin = ndi.apps.Lookup("netify.tuya-smart");
            break;

        case ndProto::Id::UBNTAC2:
            app_twin = ndi.apps.Lookup("netify.ubiquiti");
            break;

        case ndProto::Id::WHATSAPP_CALL:
            app_twin = ndi.apps.Lookup("netify.whatsapp");
            break;

        case ndProto::Id::ZOOM:
            app_twin = ndi.apps.Lookup("netify.zoom");
            break;

        default: break;
        }

        if (app_twin != ND_APP_UNKNOWN)
            SetDetectedApplication(entry, app_twin);
    }

    if (ndEFNF != nullptr) {
        bool updated = false;

        switch (ndEF->GetMasterProtocol()) {
        case ndProto::Id::TLS:
        case ndProto::Id::DTLS:
        case ndProto::Id::QUIC:
            if (ndEF->tls.version == 0 &&
              ndEFNFP.tls_quic.ssl_version != 0)
            {
                updated = true;
                ndEF->tls.version = ndEFNFP.tls_quic.ssl_version;
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set version.\n", tag.c_str());
#endif
            }

            if (ndEF->tls.cipher_suite == 0 &&
              ndEFNFP.tls_quic.server_cipher != 0)
            {
                updated = true;
                ndEF->tls.cipher_suite = ndEFNFP.tls_quic.server_cipher;
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set cipher_suite.\n",
                  tag.c_str());
#endif
            }

            if (ndEF->tls.server_ja3.empty() &&
              ndEFNFP.tls_quic.ja3_server[0] != '\0')
            {
                updated = true;
                ndEF->tls.server_ja3.assign(
                  ndEFNFP.tls_quic.ja3_server);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set ja3_server.\n",
                  tag.c_str());
#endif
            }

            if (ndEF->tls.server_cn.empty() &&
              ndEFNFP.tls_quic.serverCN != nullptr)
            {
                updated = true;
                nd_set_hostname(ndEF->tls.server_cn,
                  ndEFNFP.tls_quic.serverCN,
                  ND_FLOW_TLS_CNLEN);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set serverCN: %s\n",
                  tag.c_str(), ndEF->tls.server_cn.c_str());
#endif
                if (ndEF->detected_application == ND_APP_UNKNOWN)
                {
                    SetDetectedApplication(entry,
                      ndi.apps.Find(ndEF->tls.server_cn));
                }
            }

            if (ndEF->tls.issuer_dn.empty() &&
              ndEFNFP.tls_quic.issuerDN != nullptr)
            {
                updated = true;
                ndEF->tls.issuer_dn.assign(ndEFNFP.tls_quic.issuerDN);
                nd_trim(ndEF->tls.issuer_dn);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set issuerDN.\n", tag.c_str());
#endif
            }

            if (ndEF->tls.subject_dn.empty() &&
              ndEFNFP.tls_quic.subjectDN != nullptr)
            {
                updated = true;
                ndEF->tls.subject_dn.assign(ndEFNFP.tls_quic.subjectDN);
                nd_trim(ndEF->tls.issuer_dn);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set subjectDN.\n",
                  tag.c_str());
#endif
            }

            if (ndEF->tls.cert_fingerprint.empty() &&
              ndEFNFP.tls_quic.fingerprint_set)
            {
                updated = true;
                ndEF->tls.cert_fingerprint.reserve(SHA1_DIGEST_LENGTH);
                ndEF->tls.cert_fingerprint.resize(SHA1_DIGEST_LENGTH);
                memcpy(&ndEF->tls.cert_fingerprint[0],
                  ndEFNFP.tls_quic.sha1_certificate_fingerprint,
                  SHA1_DIGEST_LENGTH);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf(
                  "%s: TLS: set cert_fingerprint.\n", tag.c_str());
#endif
            }

            if (ndEF->tls.alpn.empty() && ndEFNFP.tls_quic.advertised_alpns)
            {
                updated = ProcessALPN(entry, true);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set ALPNs.\n", tag.c_str());
#endif
            }

            if (ndEF->tls.alpn_server.empty() &&
              ndEFNFP.tls_quic.negotiated_alpn)
            {
                updated = ProcessALPN(entry, false);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf(
                  "%s: TLS: set negotiated_alpn.\n", tag.c_str());
#endif
            }

            if (ndEF->tls.ech.version != 0 &&
              ndEFNFP.tls_quic.encrypted_ch.version != 0)
            {
                updated = true;
                ndEF->tls.ech.version =
                  ndEFNFP.tls_quic.encrypted_ch.version;
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set ECH.\n", tag.c_str());
#endif
            }

            if (ndEF->tls.esni.esni.empty() &&
              ndEFNFP.tls_quic.encrypted_sni.esni != nullptr)
            {
                updated = true;
                ndEF->tls.esni.cipher_suite =
                  ndEFNFP.tls_quic.encrypted_sni.cipher_suite;
                ndEF->tls.esni.esni.assign(
                  ndEFNFP.tls_quic.encrypted_sni.esni);
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: set ESNI.\n", tag.c_str());
#endif
            }

            if (ndEFNFP.tls_quic.hello_processed &&
              ! ndEF->tls.proc_hello.load())
            {
                ndEF->tls.proc_hello = true;
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf("%s: TLS: hello processed.\n",
                  tag.c_str());
#endif
            }

            if (ndEFNF->tls_quic.certificate_processed &&
              ! ndEF->tls.proc_certificate.load())
            {
                ndEF->tls.proc_certificate = true;
#ifdef _ND_LOG_FLOW_DEBUG
                nd_dprintf(
                  "%s: TLS: certificate processed.\n", tag.c_str());
#endif
            }

            break;
        case ndProto::Id::HTTP:
        case ndProto::Id::SSDP:
            if (ndEF->http.user_agent.empty() &&
              ndEFNF->http.user_agent != nullptr)
            {
                updated = true;
                ndEF->http.user_agent.assign(ndEFNF->http.user_agent);
                nd_trim(ndEF->http.user_agent);
            }

            if (ndEF->http.url.empty() && ndEFNF->http.url != nullptr)
            {
                updated = true;
                ndEF->http.url.assign(ndEFNF->http.url);
                nd_trim(ndEF->http.url);
            }
            break;
        case ndProto::Id::DHCP:
            if (ndEF->dhcp.fingerprint.empty() &&
              ndEFNFP.dhcp.fingerprint[0] != '\0')
            {
                updated = true;
                ndEF->dhcp.fingerprint.assign(ndEFNFP.dhcp.fingerprint);
            }
            if (ndEF->dhcp.class_ident.empty() &&
              ndEFNFP.dhcp.class_ident[0] != '\0')
            {
                updated = true;
                ndEF->dhcp.class_ident.assign(ndEFNFP.dhcp.class_ident);
            }
            break;
        case ndProto::Id::SSH:
            if (ndEF->ssh.client_agent.empty() &&
              ndEFNFP.ssh.client_signature[0] != '\0')
            {
                updated = true;
                ndEF->ssh.client_agent.assign(
                  ndEFNFP.ssh.client_signature);
                nd_trim(ndEF->ssh.client_agent);
            }
            if (ndEF->ssh.server_agent.empty() &&
              ndEFNFP.ssh.server_signature[0] != '\0')
            {
                updated = true;
                ndEF->ssh.server_agent.assign(
                  ndEFNFP.ssh.server_signature);
                nd_trim(ndEF->ssh.server_agent);
            }
            break;
        case ndProto::Id::BITTORRENT:
            if (ndEF->bt.info_hash.empty() &&
              ndEFNFP.bittorrent.hash[0] != '\0')
            {
                updated = true;
                ndEF->bt.info_hash.reserve(SHA1_DIGEST_LENGTH);
                ndEF->bt.info_hash.resize(SHA1_DIGEST_LENGTH);
                memcpy(&ndEF->bt.info_hash[0],
                  ndEFNFP.bittorrent.hash, SHA1_DIGEST_LENGTH);
            }
            break;
        default: break;
        }

        if (ndEF->flags.detection_init.load()) {
            if (updated) {
                ndEF->flags.detection_updated = true;
                ndEF->Hash(tag, true);

                DispatchEvent(entry);
            }

            return;
        }
    }

    ndEF->flags.detection_init = true;
    ndEF->Hash(tag, true);

    DispatchEvent(entry);
}

void ndDetectionThread::DetectionGuess(ndDetectionQueueEntry *entry) {
    uint8_t guessed = 0;
    ndpi_protocol ndpi_rc = ndpi_detection_giveup(ndpi,
      ndEFNF, 1, &guessed);

    if (guessed) {
        ndProto::Id id = ProtocolLookup(
          ndpi_rc.master_protocol, entry);

        if (id == ndProto::Id::UNKNOWN) {
            id = ProtocolLookup(ndpi_rc.app_protocol, entry);
            if (id != ndProto::Id::UNKNOWN) {
                SetDetectedProtocol(entry, id);
            }
        }
    }

    ndEF->flags.detection_guessed = true;

    DetectionUpdate(entry);
}

void ndDetectionThread::DetectionComplete(
  ndDetectionQueueEntry *entry) {

    if (ndEFNF != nullptr) ProcessRisks(entry);

    // Determine application by DHC (dns_host_name) if still
    // unknown.
    if (ndEF->detected_application == ND_APP_UNKNOWN) {
        if (! ndEF->dns_host_name.empty()) {
            if (! ndGC_DHC_PARTIAL_LOOKUPS ||
              ndEF->host_server_name.empty())
            {
                SetDetectedApplication(entry,
                  ndi.apps.Find(ndEF->dns_host_name));

                if (ndEF->detected_application == ND_APP_UNKNOWN)
                {
                    ndEF->flags.dhc_hit = (ndEF->detected_application !=
                      ND_APP_UNKNOWN);
                }
            }
        }
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

    if (ndGC_SOFT_DISSECTORS) {
        ndSoftDissector nsd;

        if (ndi.apps.SoftDissectorMatch(ndEF, &parser, nsd)) {
            ndEF->flags.soft_dissector = true;

            if (nsd.aid > -1) {
                if (nsd.aid == ND_APP_UNKNOWN) {
                    ndEF->detected_application = ND_APP_UNKNOWN;
                    ndEF->detected_application_name.clear();
                    ndEF->category.application = ND_CAT_UNKNOWN;
                }
                else {
                    SetDetectedApplication(entry,
                      (nd_app_id_t)nsd.aid);
                }
            }

            if (nsd.pid > -1)
                SetDetectedProtocol(entry, (ndProto::Id)nsd.pid);
        }
    }

    ndEF->Hash(tag, true);

    if (fhc != nullptr && ! ndEF->flags.fhc_hit.load() &&
      ! ndEF->flags.detection_guessed.load() &&
      (ndEF->detected_protocol != ndProto::Id::UNKNOWN ||
        ndEF->detected_application != ND_APP_UNKNOWN) &&
      ndEF->ip_protocol != IPPROTO_ICMP &&
      ndEF->ip_protocol != IPPROTO_ICMPV6 &&
      ndEF->ip_protocol != IPPROTO_IGMP &&
      ndEF->GetMasterProtocol() != ndProto::Id::DNS &&
      ndEF->detected_protocol != ndProto::Id::DHCP &&
      ndEF->detected_protocol != ndProto::Id::DHCPV6)
    {
        fhc->Insert(ndEF);
    }

    ndEF->flags.detection_complete = true;

    if (ndEFNF != nullptr) DetectionUpdate(entry);

    DispatchEvent(entry);
}
