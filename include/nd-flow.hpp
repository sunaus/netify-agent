// Netify Agent
// Copyright (C) 2015-2023 eGloo Incorporated
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

#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

#include "nd-addr.hpp"
#include "nd-apps.hpp"
#include "nd-category.hpp"
#include "nd-protos.hpp"
#include "nd-serializer.hpp"

using namespace std;

// TLS SNI hostname/common-name length. Reference: RFC 4366
#define ND_FLOW_TLS_CNLEN 256

class ndFlowStats
{
public:
    ndFlowStats()
      : lower_bytes(0), upper_bytes(0), total_bytes(0),
        lower_packets(0), upper_packets(0),
        total_packets(0), detection_packets(0)
#ifdef _ND_EXTENDED_STATS
        ,
        lower_rate_samples(ndGC.update_interval, 0),
        upper_rate_samples(ndGC.update_interval, 0),
        lower_rate(0), upper_rate(0), tcp_seq_errors(0),
        tcp_resets(0), tcp_retrans(0)
#endif
    {
    }

    ndFlowStats(const ndFlowStats &stats)
      : lower_bytes(stats.lower_bytes.load()),
        upper_bytes(stats.upper_bytes.load()),
        total_bytes(stats.total_bytes.load()),
        lower_packets(stats.lower_packets.load()),
        upper_packets(stats.upper_packets.load()),
        total_packets(stats.total_packets.load()),
        detection_packets(stats.detection_packets.load())
#ifdef _ND_EXTENDED_STATS
        ,
        lower_rate_samples(ndGC.update_interval, 0),
        upper_rate_samples(ndGC.update_interval, 0),
        lower_rate(stats.lower_rate.load()),
        upper_rate(stats.upper_rate.load()),
        tcp_seq_errors(stats.tcp_seq_errors.load()),
        tcp_resets(stats.tcp_resets.load()),
        tcp_retrans(stats.tcp_retrans.load())
#endif
    {
    }

    inline ndFlowStats &operator=(const ndFlowStats &fs) {
        lower_bytes = fs.lower_bytes.load();
        upper_bytes = fs.upper_bytes.load();
        total_bytes = fs.total_bytes.load();
        lower_packets = fs.lower_packets.load();
        upper_packets = fs.upper_packets.load();
        total_packets = fs.total_packets.load();
        detection_packets = fs.detection_packets.load();
#ifdef _ND_EXTENDED_STATS
        lower_rate = fs.lower_rate.load();
        upper_rate = fs.upper_rate.load();
        tcp_seq_errors = fs.tcp_seq_errors.load();
        tcp_resets = fs.tcp_resets.load();
        tcp_retrans = fs.tcp_retrans.load();
#endif
        return *this;
    };

    inline void Reset(bool full_reset = false) {
        lower_bytes = 0;
        upper_bytes = 0;
        lower_packets = 0;
        upper_packets = 0;
#ifdef _ND_EXTENDED_STATS
        lower_rate_samples.assign(ndGC.update_interval, 0);
        tcp_seq_errors = 0;
        tcp_resets = 0;
        tcp_retrans = 0;
#endif
        if (full_reset) {
            detection_packets = 0;
#ifdef _ND_EXTENDED_STATS
#endif
        }
    }

    atomic<uint64_t> lower_bytes;
    atomic<uint64_t> upper_bytes;
    atomic<uint64_t> total_bytes;

    atomic<uint32_t> lower_packets;
    atomic<uint32_t> upper_packets;
    atomic<uint32_t> total_packets;

    atomic<uint8_t> detection_packets;

#ifdef _ND_EXTENDED_STATS
    vector<float> lower_rate_samples;
    vector<float> upper_rate_samples;
    atomic<float> lower_rate;
    atomic<float> upper_rate;

    void UpdateRate(bool lower, uint64_t timestamp, uint64_t bytes);

    atomic<uint32_t> tcp_seq_errors;
    atomic<uint32_t> tcp_resets;
    atomic<uint32_t> tcp_retrans;
#endif
};

class ndFlow : public ndSerializer
{
public:
    nd_iface_ptr iface;

    ndAddr lower_mac;
    ndAddr upper_mac;

    ndAddr lower_addr;
    ndAddr upper_addr;

    ndAddr::Type lower_type;
    ndAddr::Type upper_type;

    int direction;

    uint8_t ip_version;
    uint8_t ip_protocol;

    uint16_t vlan_id;

    uint64_t ts_first_seen;
    atomic<uint64_t> ts_last_seen;

    enum LowerMap {
        LOWER_UNKNOWN = 0,
        LOWER_LOCAL = 1,
        LOWER_OTHER = 2
    };

    LowerMap lower_map;

    enum OtherType {
        OTHER_UNKNOWN = 0,
        OTHER_UNSUPPORTED = 1,
        OTHER_LOCAL = 2,
        OTHER_MULTICAST = 3,
        OTHER_BROADCAST = 4,
        OTHER_REMOTE = 5,
        OTHER_ERROR = 6
    };

    OtherType other_type;

    enum TunnelType { TUNNEL_NONE = 0, TUNNEL_GTP = 1 };

    TunnelType tunnel_type;

    enum Origin {
        ORIGIN_UNKNOWN,
        ORIGIN_LOWER,
        ORIGIN_UPPER,
    };

    Origin origin;

    enum PrivacyMask {
        PRIVATE_LOWER = 0x01,
        PRIVATE_UPPER = 0x02
    };

    uint8_t privacy_mask;

    enum AddrType {
        TYPE_LOWER,
        TYPE_UPPER,

        TYPE_MAX
    };

    vector<uint8_t> digest_lower;
    vector<uint8_t> digest_mdata;

    struct {
        atomic<bool> detection_complete;
        atomic<bool> detection_guessed;
        atomic<bool> detection_init;
        atomic<bool> detection_updated;
        atomic<bool> dhc_hit;
        atomic<bool> fhc_hit;
        atomic<bool> expired;
        atomic<bool> expiring;
        atomic<bool> ip_nat;
        atomic<bool> soft_dissector;
    } flags;

    struct {
        atomic<uint8_t> fin_ack;
        atomic<uint32_t> last_seq;
    } tcp;

#if defined(_ND_USE_CONNTRACK) && defined(_ND_WITH_CONNTRACK_MDATA)
    struct {
        uint32_t id;
        uint32_t mark;
    } conntrack;
#endif

    struct {
        uint8_t version;
        uint8_t ip_version;
        uint32_t lower_teid;
        uint32_t upper_teid;
        ndAddr::Type lower_type;
        ndAddr::Type upper_type;
        ndAddr lower_addr;
        ndAddr upper_addr;
        LowerMap lower_map;
        OtherType other_type;
    } gtp;

    string dns_host_name;
    string host_server_name;

    nd_proto_id_t detected_protocol;
    string detected_protocol_name;

    nd_app_id_t detected_application;
    string detected_application_name;

    struct {
        nd_cat_id_t application;
        nd_cat_id_t protocol;
        nd_cat_id_t domain;
        nd_cat_id_t network;
    } category;

    struct {
        string user_agent;
        string url;
    } http;

    struct {
        string fingerprint;
        string class_ident;
    } dhcp;

    struct {
        string client_agent;
        string server_agent;
    } ssh;

    struct {
        uint16_t version;
        uint16_t cipher_suite;

        string subject_dn;
        string issuer_dn;
        string server_cn;
        string client_ja3;
        string server_ja3;
        vector<uint8_t> cert_fingerprint;
        vector<string> alpn, alpn_server;
    } tls;

    struct {
        bool tls;
    } smtp;

    struct {
        vector<uint8_t> info_hash;
    } bt;

    struct {
        string domain_name;
    } mdns;

    struct {
        typedef set<nd_risk_id_t> Risks;
        Risks risks;

        uint16_t ndpi_score;
        uint16_t ndpi_score_client;
        uint16_t ndpi_score_server;
    } risk;

    ndFlowStats stats;

    mutable recursive_mutex lock;

    int16_t dpi_thread_id;

    struct ndpi_flow_struct *ndpi_flow;

    ndFlow(nd_iface_ptr &iface);
    ndFlow(const ndFlow &flow);
    virtual ~ndFlow();

    void Hash(const string &device, bool hash_mdata = false,
      const uint8_t *key = nullptr, size_t key_length = 0);

    void Reset(bool full_reset = false);

    void Release(void);

    nd_proto_id_t GetMasterProtocol(void) const;

    bool HasDhcpFingerprint(void) const;
    bool HasDhcpClassIdent(void) const;
    bool HasHttpUserAgent(void) const;
    bool HasHttpURL(void) const;
    bool HasSSHClientAgent(void) const;
    bool HasSSHServerAgent(void) const;
    bool HasTLSClientSNI(void) const;
    bool HasTLSServerCN(void) const;
    bool HasTLSIssuerDN(void) const;
    bool HasTLSSubjectDN(void) const;
    bool HasTLSClientJA3(void) const;
    bool HasTLSServerJA3(void) const;
    bool HasBTInfoHash(void) const;
    bool HasSSDPUserAgent(void) const;
    bool HasMDNSDomainName(void) const;

    enum PrintFlags {
        PRINTF_NONE = 0x00,
        PRINTF_HASHES = 0x01,
        PRINTF_MACS = 0x02,
        PRINTF_METADATA = 0x04,
        PRINTF_STATS = 0x08,
        PRINTF_STATS_FULL = 0x10,
        PRINTF_RISKS = 0x20,
        PRINTF_ALL = (PRINTF_HASHES | PRINTF_MACS | PRINTF_METADATA |
          PRINTF_STATS | PRINTF_STATS_FULL | PRINTF_RISKS)
    };

    void Print(uint8_t pflags = PRINTF_METADATA,
      const string &prefix = "") const;

    void UpdateLowerMaps(void);
    void GetLowerMap(ndAddr::Type lt, ndAddr::Type ut,
      LowerMap &lm, OtherType &ot);

    enum EncodeFlags {
        ENCODE_NONE = 0x00,
        ENCODE_METADATA = 0x01,
        ENCODE_TUNNELS = 0x02,
        ENCODE_STATS = 0x04,
        ENCODE_ALL = (ENCODE_METADATA | ENCODE_TUNNELS | ENCODE_STATS)
    };

    template <class T>
    void Encode(T &output, const ndFlowStats &stats,
      uint8_t encode_includes = ENCODE_ALL) const {

        lock_guard<recursive_mutex> lg(lock);

        string _other_type = "unknown";
        string _lower_mac = "local_mac",
               _upper_mac = "other_mac";
        string _lower_ip = "local_ip",
               _upper_ip = "other_ip";
        string _lower_gtp_ip = "local_ip",
               _upper_gtp_ip = "other_ip";
        string _lower_port = "local_port",
               _upper_port = "other_port";
        string _lower_gtp_port = "local_port",
               _upper_gtp_port = "other_port";
        string _lower_bytes = "local_bytes",
               _upper_bytes = "other_bytes";
        string _lower_packets = "local_packets",
               _upper_packets = "other_packets";
#ifdef _ND_EXTENDED_STATS
        string _lower_rate = "local_rate";
        string _upper_rate = "other_rate";
#endif
        string digest;
        if (! digest_mdata.empty()) {
            nd_sha1_to_string(digest_mdata, digest);
            serialize(output, { "digest" }, digest);
        }
        else {
            nd_sha1_to_string(digest_lower, digest);
            serialize(output, { "digest" }, digest);
        }

        serialize(output, { "last_seen_at" }, ts_last_seen.load());

        switch (lower_map) {
        case LOWER_LOCAL:
            _lower_mac = "local_mac";
            _lower_ip = "local_ip";
            _lower_port = "local_port";
            _lower_bytes = "local_bytes";
            _lower_packets = "local_packets";
            _upper_mac = "other_mac";
            _upper_ip = "other_ip";
            _upper_port = "other_port";
            _upper_bytes = "other_bytes";
            _upper_packets = "other_packets";
#ifdef _ND_EXTENDED_STATS
            _lower_rate = "local_rate";
            _upper_rate = "other_rate";
#endif
            break;
        case LOWER_OTHER:
            _lower_mac = "other_mac";
            _lower_ip = "other_ip";
            _lower_port = "other_port";
            _lower_bytes = "other_bytes";
            _lower_packets = "other_packets";
            _upper_mac = "local_mac";
            _upper_ip = "local_ip";
            _upper_port = "local_port";
            _upper_bytes = "local_bytes";
            _upper_packets = "local_packets";
#ifdef _ND_EXTENDED_STATS
            _lower_rate = "other_rate";
            _upper_rate = "local_rate";
#endif
            break;
        default: break;
        }

        switch (other_type) {
        case OTHER_LOCAL: _other_type = "local"; break;
        case OTHER_MULTICAST:
            _other_type = "multicast";
            break;
        case OTHER_BROADCAST:
            _other_type = "broadcast";
            break;
        case OTHER_REMOTE: _other_type = "remote"; break;
        case OTHER_UNSUPPORTED:
            _other_type = "unsupported";
            break;
        case OTHER_ERROR: _other_type = "error"; break;
        default: break;
        }

        if (encode_includes & ENCODE_METADATA) {
            serialize(output, { "ip_nat" },
              (bool)flags.ip_nat.load());
            serialize(output, { "dhc_hit" },
              (bool)flags.dhc_hit.load());
            serialize(output, { "soft_dissector" },
              (bool)flags.soft_dissector.load());
#if defined(_ND_USE_CONNTRACK) && defined(_ND_WITH_CONNTRACK_MDATA)
            serialize(output, { "ct_id" }, ct_id);
            serialize(output, { "ct_mark" }, ct_mark);
#endif
            serialize(output, { "ip_version" }, (unsigned)ip_version);
            serialize(output, { "ip_protocol" },
              (unsigned)ip_protocol);
            serialize(output, { "vlan_id" }, (unsigned)vlan_id);
            serialize(output, { "other_type" }, _other_type);

            switch (origin) {
            case ORIGIN_UPPER:
                serialize(output, { "local_origin" },
                  (_lower_ip == "local_ip") ? false : true);
                break;
            case ORIGIN_LOWER:
            default:
                serialize(output, { "local_origin" },
                  (_lower_ip == "local_ip") ? true : false);
                break;
            }

            // 00-52-14 to 00-52-FF: Unserialized (small
            // allocations)
            serialize(output, { _lower_mac },
              (privacy_mask & PRIVATE_LOWER) ?
                "00:52:14:00:00:00" :
                (lower_mac.IsValid()) ?
                lower_mac.GetString() :
                "00:00:00:00:00:00");
            serialize(output, { _upper_mac },
              (privacy_mask & PRIVATE_UPPER) ?
                "00:52:ff:00:00:00" :
                (upper_mac.IsValid()) ?
                upper_mac.GetString() :
                "00:00:00:00:00:00");

            if (privacy_mask & PRIVATE_LOWER) {
                if (ip_version == 4)
                    serialize(output, { _lower_ip },
                      ND_PRIVATE_IPV4 "253");
                else
                    serialize(output, { _lower_ip },
                      ND_PRIVATE_IPV6 "fd");
            }
            else
                serialize(output, { _lower_ip },
                  lower_addr.GetString());

            if (privacy_mask & PRIVATE_UPPER) {
                if (ip_version == 4)
                    serialize(output, { _upper_ip },
                      ND_PRIVATE_IPV4 "254");
                else
                    serialize(output, { _upper_ip },
                      ND_PRIVATE_IPV6 "fe");
            }
            else
                serialize(output, { _upper_ip },
                  upper_addr.GetString());

            serialize(output, { _lower_port },
              (unsigned)lower_addr.GetPort());
            serialize(output, { _upper_port },
              (unsigned)upper_addr.GetPort());

            serialize(output, { "detected_protocol" },
              (unsigned)detected_protocol);
            serialize(output, { "detected_protocol_name" },
              (detected_protocol_name.empty()) ?
                "Unknown" :
                detected_protocol_name);

            serialize(output, { "detected_application" },
              (unsigned)detected_application);
            serialize(output, { "detected_application_name" },
              (detected_application_name.empty()) ?
                "Unknown" :
                detected_application_name);

            serialize(output, { "detection_guessed" },
              flags.detection_guessed.load());
            serialize(output, { "detection_updated" },
              flags.detection_updated.load());

            serialize(output, { "category", "application" },
              category.application);
            serialize(output, { "category", "protocol" },
              category.protocol);
            serialize(output, { "category", "domain" },
              category.domain);
            serialize(output, { "category", "network" },
              category.network);

            if (! dns_host_name.empty())
                serialize(output, { "dns_host_name" }, dns_host_name);

            if (! host_server_name.empty())
                serialize(output, { "host_server_name" },
                  host_server_name);

            if (HasHttpUserAgent() || HasHttpURL()) {
                if (HasHttpUserAgent())
                    serialize(output,
                      { "http", "user_agent" }, http.user_agent);
                if (HasHttpURL())
                    serialize(output, { "http", "url" },
                      http.url);
            }

            if (HasDhcpFingerprint() || HasDhcpClassIdent()) {
                if (HasDhcpFingerprint())
                    serialize(output,
                      { "dhcp", "fingerprint" }, dhcp.fingerprint);

                if (HasDhcpClassIdent())
                    serialize(output,
                      { "dhcp", "class_ident" }, dhcp.class_ident);
            }

            if (HasSSHClientAgent() || HasSSHServerAgent()) {
                if (HasSSHClientAgent())
                    serialize(output, { "ssh", "client" },
                      ssh.client_agent);

                if (HasSSHServerAgent())
                    serialize(output, { "ssh", "server" },
                      ssh.server_agent);
            }

            if (GetMasterProtocol() == ND_PROTO_TLS ||
              detected_protocol == ND_PROTO_QUIC)
            {
                char tohex[7];

                sprintf(tohex, "0x%04hx", tls.version);
                serialize(output, { "ssl", "version" }, tohex);

                sprintf(tohex, "0x%04hx", tls.cipher_suite);
                serialize(output, { "ssl", "cipher_suite" }, tohex);

                if (HasTLSClientSNI())
                    serialize(output,
                      { "ssl", "client_sni" }, host_server_name);

                if (HasTLSServerCN())
                    serialize(output,
                      { "ssl", "server_cn" }, tls.server_cn);

                if (HasTLSIssuerDN())
                    serialize(output,
                      { "ssl", "issuer_dn" }, tls.issuer_dn);

                if (HasTLSSubjectDN())
                    serialize(output,
                      { "ssl", "subject_dn" }, tls.subject_dn);

                if (HasTLSClientJA3())
                    serialize(output,
                      { "ssl", "client_ja3" }, tls.client_ja3);

                if (HasTLSServerJA3())
                    serialize(output,
                      { "ssl", "server_ja3" }, tls.server_ja3);

                if (! tls.cert_fingerprint.empty()) {
                    nd_sha1_to_string(tls.cert_fingerprint, digest);
                    serialize(output,
                      { "ssl", "fingerprint" }, digest);
                }

                if (! tls.alpn.empty())
                    serialize(output, { "ssl", "alpn" }, tls.alpn);
                if (! tls.alpn_server.empty())
                    serialize(output,
                      { "ssl", "alpn_server" }, tls.alpn_server);
            }

            if (HasBTInfoHash()) {
                nd_sha1_to_string(bt.info_hash, digest);
                serialize(output, { "bt", "info_hash" }, digest);
            }

            if (HasSSDPUserAgent()) {
                if (HasSSDPUserAgent()) {
                    serialize(output,
                      { "ssdp", "user_agent" }, http.user_agent);
                }
            }
#if 0
            if (HasMiningVariant())
                serialize(output, { "mining", "variant" }, mining.variant);
#endif
            if (HasMDNSDomainName())
                serialize(output, { "mdns", "answer" },
                  mdns.domain_name);

            serialize(output, { "first_seen_at" }, ts_first_seen);

            serialize(output, { "risks", "risks" }, risk.risks);
            serialize(output, { "risks", "ndpi_risk_score" },
              risk.ndpi_score);
            serialize(output,
              { "risks", "ndpi_risk_score_client" },
              risk.ndpi_score_client);
            serialize(output,
              { "risks", "ndpi_risk_score_server" },
              risk.ndpi_score_server);
        }

        if (encode_includes & ENCODE_TUNNELS) {
            string _lower_teid = "local_teid",
                   _upper_teid = "other_teid";

            switch (tunnel_type) {
            case TUNNEL_GTP:
                switch (gtp.lower_map) {
                case LOWER_LOCAL:
                    _lower_ip = "local_ip";
                    _lower_port = "local_port";
                    _lower_teid = "local_teid";
                    _upper_ip = "other_ip";
                    _upper_port = "other_port";
                    _upper_teid = "other_teid";
                    break;
                case LOWER_OTHER:
                    _lower_ip = "other_ip";
                    _lower_port = "other_port";
                    _lower_teid = "other_teid";
                    _upper_ip = "local_ip";
                    _upper_port = "local_port";
                    _upper_teid = "local_teid";
                    break;
                default: break;
                }

                switch (gtp.other_type) {
                case OTHER_LOCAL:
                    _other_type = "local";
                    break;
                case OTHER_REMOTE:
                    _other_type = "remote";
                    break;
                case OTHER_ERROR:
                    _other_type = "error";
                    break;
                case OTHER_UNSUPPORTED:
                default: _other_type = "unsupported"; break;
                }

                serialize(output, { "gtp", "version" }, gtp.version);
                serialize(output, { "gtp", "ip_version" },
                  gtp.ip_version);
                serialize(output, { "gtp", _lower_ip },
                  gtp.lower_addr.GetString());
                serialize(output, { "gtp", _upper_ip },
                  gtp.upper_addr.GetString());
                serialize(output, { "gtp", _lower_port },
                  (unsigned)gtp.lower_addr.GetPort());
                serialize(output, { "gtp", _upper_port },
                  (unsigned)gtp.upper_addr.GetPort());
                serialize(output, { "gtp", _lower_teid },
                  htonl(gtp.lower_teid));
                serialize(output, { "gtp", _upper_teid },
                  htonl(gtp.upper_teid));
                serialize(output, { "gtp", "other_type" },
                  _other_type);

                break;
            default: break;
            }
        }

        if (encode_includes & ENCODE_STATS) {
            serialize(output, { _lower_bytes },
              stats.lower_bytes.load());
            serialize(output, { _upper_bytes },
              stats.upper_bytes.load());
            serialize(output, { _lower_packets },
              stats.lower_packets.load());
            serialize(output, { _upper_packets },
              stats.upper_packets.load());
            serialize(output, { "total_packets" },
              stats.total_packets.load());
            serialize(output, { "total_bytes" },
              stats.total_bytes.load());
            serialize(output, { "detection_packets" },
              stats.detection_packets.load());

#ifdef _ND_EXTENDED_STATS
            serialize(output, { _lower_rate },
              stats.lower_rate.load());
            serialize(output, { _upper_rate },
              stats.upper_rate.load());

            if (ip_protocol == IPPROTO_TCP) {
                serialize(output,
                  { "tcp", "seq_errors" },
                  stats.tcp_seq_errors.load());
                serialize(output, { "tcp", "resets" },
                  stats.tcp_resets.load());
                serialize(output, { "tcp", "retrans" },
                  stats.tcp_retrans.load());
            }
#endif
        }
    }
};

typedef shared_ptr<ndFlow> nd_flow_ptr;
typedef unordered_map<string, nd_flow_ptr> nd_flow_map;
typedef map<string, nd_flow_map *> nd_flows;
typedef pair<string, nd_flow_ptr> nd_flow_pair;
typedef pair<nd_flow_map::iterator, bool> nd_flow_insert;
