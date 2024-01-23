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

#pragma once

#include <map>
#include <mutex>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "nd-sha1.h"
#include "netifyd.hpp"

enum nd_dhc_save {
    ndDHC_DISABLED,
    ndDHC_PERSISTENT,
    ndDHC_VOLATILE,
};

enum nd_fhc_save {
    ndFHC_DISABLED,
    ndFHC_PERSISTENT,
    ndFHC_VOLATILE,
};

enum nd_capture_type {
    ndCT_NONE = 0,
    ndCT_CMDLINE = (1 << 0),
    ndCT_PCAP = (1 << 1),
    ndCT_PCAP_OFFLINE = (1 << 2),
    ndCT_TPV3 = (1 << 3),
    ndCT_NFQ = (1 << 4),

    ndCT_USER = (1 << 31),
};

#define ndCT_TYPE(t) (t & 0xfffffffe)

enum nd_interface_role {
    ndIR_NONE,
    ndIR_LAN,
    ndIR_WAN,
};

enum nd_tpv3_fanout_mode {
    ndFOM_DISABLED,
    ndFOM_HASH,
    ndFOM_LOAD_BALANCED,
    ndFOM_CPU,
    ndFOM_ROLLOVER,
    ndFOM_RANDOM,
    ndFOM_QUEUE_MAP,
};

enum nd_tpv3_fanout_flags {
    ndFOF_NONE = 0,
    ndFOF_DEFRAG = (1 << 0),
    ndFOF_ROLLOVER = (1 << 1),
};

enum nd_global_flags {
    ndGF_NONE = 0,
    ndGF_DEBUG = (1 << 0),
    ndGF_DEBUG_CURL = (1 << 1),
    ndGF_FREE_BIT3 = (1 << 2),
    ndGF_DEBUG_NDPI = (1 << 3),
    ndGF_QUIET = (1 << 4),
    ndGF_SYN_SCAN_PROTECTION = (1 << 5),
    ndGF_PRIVATE_EXTADDR = (1 << 6),
    ndGF_SSL_USE_TLSv1 = (1 << 7),
    ndGF_SSL_VERIFY = (1 << 8),
    ndGF_USE_CONNTRACK = (1 << 9),
    ndGF_USE_NETLINK = (1 << 10),
    ndGF_USE_NAPI = (1 << 11),
    ndGF_USE_DHC = (1 << 12),
    ndGF_USE_FHC = (1 << 13),
    ndGF_EXPORT_JSON = (1 << 14),
    ndGF_VERBOSE = (1 << 15),
    ndGF_FREE_BIT17 = (1 << 16),
    ndGF_REPLAY_DELAY = (1 << 17),
    ndGF_REMAIN_IN_FOREGROUND = (1 << 18),
    ndGF_ALLOW_UNPRIV = (1 << 19),
    ndGF_IGNORE_IFACE_CONFIGS = (1 << 20),
    ndGF_UPLOAD_ENABLED = (1 << 21),
    ndGF_UPLOAD_NAT_FLOWS = (1 << 22),
    ndGF_AUTO_FLOW_EXPIRY = (1 << 23),
    ndGF_SOFT_DISSECTORS = (1 << 24),
    ndGF_DOTD_CATEGORIES = (1 << 25),
    ndGF_RUN_WITHOUT_SOURCES = (1 << 26),
};

#define ndGC_DEBUG \
    (ndGlobalConfig::GetInstance().flags & ndGF_DEBUG)
#define ndGC_DEBUG_CURL \
    (ndGlobalConfig::GetInstance().flags & ndGF_DEBUG_CURL)
#define ndGC_DEBUG_NDPI \
    (ndGlobalConfig::GetInstance().flags & ndGF_DEBUG_NDPI)
#define ndGC_QUIET \
    (ndGlobalConfig::GetInstance().flags & ndGF_QUIET)
#define ndGC_OVERRIDE_LEGACY_CONFIG \
    (ndGlobalConfig::GetInstance().flags & ndGF_OVERRIDE_LEGACY_CONFIG)
#define ndGC_SYN_SCAN_PROTECTION \
    (ndGlobalConfig::GetInstance().flags & ndGF_SYN_SCAN_PROTECTION)
#define ndGC_PRIVATE_EXTADDR \
    (ndGlobalConfig::GetInstance().flags & ndGF_PRIVATE_EXTADDR)
#define ndGC_SSL_USE_TLSv1 \
    (ndGlobalConfig::GetInstance().flags & ndGF_SSL_USE_TLSv1)
#define ndGC_SSL_VERIFY \
    (ndGlobalConfig::GetInstance().flags & ndGF_SSL_VERIFY)
#define ndGC_USE_CONNTRACK \
    (ndGlobalConfig::GetInstance().flags & ndGF_USE_CONNTRACK)
#define ndGC_USE_NETLINK \
    (ndGlobalConfig::GetInstance().flags & ndGF_USE_NETLINK)
#define ndGC_USE_NAPI \
    (ndGlobalConfig::GetInstance().flags & ndGF_USE_NAPI)
#define ndGC_USE_DHC \
    (ndGlobalConfig::GetInstance().flags & ndGF_USE_DHC)
#define ndGC_USE_FHC \
    (ndGlobalConfig::GetInstance().flags & ndGF_USE_FHC)
#define ndGC_EXPORT_JSON \
    (ndGlobalConfig::GetInstance().flags & ndGF_EXPORT_JSON)
#define ndGC_VERBOSE \
    (ndGlobalConfig::GetInstance().flags & ndGF_VERBOSE)
#define ndGC_REPLAY_DELAY \
    (ndGlobalConfig::GetInstance().flags & ndGF_REPLAY_DELAY)
#define ndGC_REMAIN_IN_FOREGROUND \
    (ndGlobalConfig::GetInstance().flags & ndGF_REMAIN_IN_FOREGROUND)
#define ndGC_ALLOW_UNPRIV \
    (ndGlobalConfig::GetInstance().flags & ndGF_ALLOW_UNPRIV)
#define ndGC_IGNORE_IFACE_CONFIGS \
    (ndGlobalConfig::GetInstance().flags & ndGF_IGNORE_IFACE_CONFIGS)
#define ndGC_UPLOAD_ENABLED \
    (ndGlobalConfig::GetInstance().flags & ndGF_UPLOAD_ENABLED)
#define ndGC_UPLOAD_NAT_FLOWS \
    (ndGlobalConfig::GetInstance().flags & ndGF_UPLOAD_NAT_FLOWS)
#define ndGC_AUTO_FLOW_EXPIRY \
    (ndGlobalConfig::GetInstance().flags & ndGF_AUTO_FLOW_EXPIRY)
#define ndGC_SOFT_DISSECTORS \
    (ndGlobalConfig::GetInstance().flags & ndGF_SOFT_DISSECTORS)
#define ndGC_DOTD_CATEGORIES \
    (ndGlobalConfig::GetInstance().flags & ndGF_DOTD_CATEGORIES)
#define ndGC_RUN_WITHOUT_SOURCES \
    (ndGlobalConfig::GetInstance().flags & ndGF_RUN_WITHOUT_SOURCES)

#define ndGC ndGlobalConfig::GetInstance()

#define ndGC_SetFlag(flag, value) \
    if (value) \
        ndGlobalConfig::GetInstance().flags |= flag; \
    else ndGlobalConfig::GetInstance().flags &= ~flag

typedef struct nd_config_pcap_t {
    std::string capture_filename;

    inline bool operator==(const struct nd_config_pcap_t &i) const {
        return (capture_filename == i.capture_filename);
    }
} nd_config_pcap;

typedef struct nd_config_tpv3_t {
    unsigned fanout_mode{ ndFOM_DISABLED };
    unsigned fanout_flags{ ndFOF_NONE };
    unsigned fanout_instances{ 0 };
    unsigned rb_block_size{ ND_TPV3_RB_BLOCK_SIZE };
    unsigned rb_frame_size{ ND_TPV3_RB_FRAME_SIZE };
    unsigned rb_blocks{ ND_TPV3_RB_BLOCKS };

    inline bool operator==(const struct nd_config_tpv3_t &i) const {
        if (fanout_mode != i.fanout_mode) return false;
        if (fanout_flags != i.fanout_flags) return false;
        if (fanout_instances != i.fanout_instances)
            return false;
        if (rb_block_size != i.rb_block_size) return false;
        if (rb_frame_size != i.rb_frame_size) return false;
        if (rb_blocks != i.rb_blocks) return false;
        return true;
    }
} nd_config_tpv3;

typedef struct nd_config_nfq_t {
    unsigned queue_id{ 0 };
    unsigned instances{ 0 };

    inline bool operator==(const struct nd_config_nfq_t &i) const {
        return (queue_id == i.queue_id && instances == i.instances);
    }
} nd_config_nfq;

class ndGlobalConfig
{
public:
    bool napi_tls_verify;
    std::string napi_vendor;
    std::string path_agent_status;
    std::string path_app_config;
    std::string path_cat_config;
    std::string path_categories;
    std::string path_config;
    std::string path_export_json;
    std::string path_functions;
    std::string path_interfaces;
    std::string path_legacy_config;
    std::string path_pid_file;
    std::string path_plugins;
    std::string path_shared_data;
    std::string path_state_persistent;
    std::string path_state_volatile;
    std::string path_uuid;
    std::string path_uuid_serial;
    std::string path_uuid_site;
    std::string url_napi_bootstrap;
    enum nd_dhc_save dhc_save;
    enum nd_fhc_save fhc_save;
    enum nd_capture_type capture_type;
    unsigned capture_read_timeout;
    nd_config_tpv3 tpv3_defaults;
    FILE *h_flow;
    int16_t ca_capture_base;
    int16_t ca_conntrack;
    int16_t ca_detection_base;
    int16_t ca_detection_cores;
    size_t max_packet_queue;
    uint16_t max_capture_length;
    uint32_t flags;
    uint8_t digest_app_config[SHA1_DIGEST_LENGTH];
    uint8_t digest_legacy_config[SHA1_DIGEST_LENGTH];
    uint8_t verbosity;
    uint8_t verbosity_flags;
    unsigned fhc_purge_divisor;
    unsigned fm_buckets;
    unsigned max_detection_pkts;
    unsigned max_dhc;
    unsigned max_fhc;
    unsigned max_flows;
    unsigned ttl_capture_delay;
    unsigned ttl_idle_flow;
    unsigned ttl_idle_tcp_flow;
    unsigned ttl_napi_tick;
    unsigned ttl_napi_update;
    unsigned update_imf;
    unsigned update_interval;

    enum VerbosityFlags {
        VFLAG_NONE = 0,
        VFLAG_EVENT_DPI_NEW = (1 << 0),
        VFLAG_EVENT_DPI_UPDATE = (1 << 1),
        VFLAG_EVENT_DPI_COMPLETE = (1 << 2),
    };

    typedef std::vector<std::pair<std::string, std::string>> SocketHosts;
    SocketHosts socket_host;

    typedef std::vector<std::string> SocketPaths;
    SocketPaths socket_path;

    typedef std::vector<struct sockaddr *> PrivacyFilterHosts;
    PrivacyFilterHosts privacy_filter_host;

    typedef std::vector<uint8_t *> PrivacyFilterMACs;
    PrivacyFilterMACs privacy_filter_mac;

    typedef std::vector<std::pair<std::regex *, std::string>> PrivacyFilterRegex;
    PrivacyFilterRegex privacy_regex;

    typedef std::map<std::string, std::string> InterfaceFilters;
    InterfaceFilters interface_filters;

    typedef std::map<std::string,
      std::pair<std::string, std::map<std::string, std::string>>>
      Plugins;
    Plugins plugin_processors;
    Plugins plugin_sinks;

    typedef std::map<std::string, std::string> CustomHeaders;
    CustomHeaders custom_headers;

    typedef std::map<std::string, std::string> Protocols;
    Protocols protocols;

    typedef std::map<std::string, std::pair<unsigned, void *>> Interfaces;
    typedef std::map<nd_interface_role, Interfaces> InterfacesByRole;
    InterfacesByRole interfaces;

    typedef std::map<std::string, std::set<std::string>> InterfaceAddrs;
    InterfaceAddrs interface_addrs;

    typedef std::map<std::string, std::string> InterfacePeers;
    InterfacePeers interface_peers;

    typedef std::map<std::string, std::string> ConfVars;
    ConfVars conf_vars;

    typedef std::vector<std::string> FlowDebugExpressions;
    FlowDebugExpressions debug_flow_print_exprs;

    ndGlobalConfig(const ndGlobalConfig &) = delete;
    ndGlobalConfig &operator=(const ndGlobalConfig &) = delete;

    static inline ndGlobalConfig &GetInstance() {
        static ndGlobalConfig config;
        return config;
    }

    bool Open(const std::string &filename);
    void Close(void);

    bool Load(const std::string &filename);

    enum UUID {
        UUID_NONE,
        UUID_AGENT,
        UUID_SITE,
        UUID_SERIAL,
    };

    bool LoadUUID(UUID which, std::string &uuid);
    bool SaveUUID(UUID which, const std::string &uuid);
    void GetUUID(UUID which, std::string &uuid);

    bool ForceReset(void);

    bool LoadInterfaces(const std::string &filename);

    bool AddInterface(const std::string &iface,
      nd_interface_role role,
      unsigned type = ndCT_NONE,
      void *config = nullptr);

    bool AddInterfaceAddress(const std::string &iface,
      const std::string &addr);
    bool AddInterfacePeer(const std::string &iface,
      const std::string &peer);

    bool AddInterfaceFilter(const std::string &iface,
      const std::string &filter);

protected:
    void *reader;
    std::mutex lock_uuid;

    std::string uuid;
    std::string uuid_serial;
    std::string uuid_site;

    bool LoadInterfaces(void *config_reader);
    void ClearInterfaces(bool cmdline_entries = false);

    enum nd_capture_type LoadCaptureType(void *config_reader,
      const std::string &section, const std::string &key) const;
    bool LoadCaptureSettings(void *config_reader,
      const std::string &section, unsigned &type, void *config) const;

    bool AddPlugin(const std::string &filename);

    void UpdatePaths(void);
    void UpdateConfigVars(void);

private:
    ndGlobalConfig();
    virtual ~ndGlobalConfig();
};
