// Netify Agent
// Copyright (C) 2015-2025 eGloo Incorporated
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

#include <ctime>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <libnetfilter_conntrack/libnetfilter_conntrack.h>

#include "nd-addr.hpp"
#include "nd-flow.hpp"
#include "nd-netlink.hpp"
#include "nd-thread.hpp"
#include "nd-util.hpp"

constexpr time_t _ND_CT_FLOW_TTL = 900;

class ndConntrackFlow;

class ndNetlinkConntrackThread : public ndThread, public ndNetlink
{
public:
    ndNetlinkConntrackThread(void);
    virtual ~ndNetlinkConntrackThread();

    virtual void *Entry(void);

    virtual int ProcessMessage(const struct nlmsghdr *nlh);

    void Dump(bool zero_counters = false);

    void UpdateFlow(nd_flow_ptr &flow);
    void UpdateFlowStats(nd_flow_ptr &flow, const struct timeval &stamp) const;

    void PurgeFlows(void);

    typedef struct {
        uint32_t created = { 0 };
        uint32_t updated = { 0 };
        uint32_t destroyed = { 0 };
        uint32_t lookups = { 0 };
        uint32_t matches = { 0 };
        uint32_t total_ids = { 0 };
        uint32_t total_flows = { 0 };
    } Stats;

    void GetStats(Stats &stats) {
        std::lock_guard<std::mutex> lg(map_lock);

        stats = this->stats;
        stats.total_ids = (uint32_t)id_digests.size();
        stats.total_flows = (uint32_t)flows.size();

        this->stats.lookups = this->stats.matches = 0;
        this->stats.created = this->stats.updated = this->stats.destroyed = 0;
    };

    void PrintStats(const Stats &stats) {
        nd_dprintf("%s: created: %lu, updated: %lu, destroyed: %lu, "
            "lookups: %lu, matches: %lu, total ids: %lu, total flows: %lu\n",
            tag.c_str(),
            stats.created, stats.updated, stats.destroyed,
            stats.lookups, stats.matches,
            stats.total_ids, stats.total_flows
        );
    }

    typedef std::array<struct nlattr *, CTA_MAX + 1> Attributes;

protected:
    bool NewFlow(uint32_t id, const Attributes &attrs);
    bool UpdateFlow(uint32_t id, const Attributes &attrs);
    void DestroyFlow(uint32_t id);

    mutable std::mutex map_lock;

    typedef std::unordered_map<uint32_t, ndDigest> IdMap;
    typedef std::unordered_map<ndDigest,
        std::unique_ptr<ndConntrackFlow>, ndArrayHasher> FlowMap;

    IdMap id_digests;
    FlowMap flows;

    Stats stats;
};

class ndConntrackFlow
{
public:
    enum class Direction : uint8_t { SRC, DST };

    typedef std::pair<ndConntrackFlow *, void *> Attributes;
    typedef std::array<struct nlattr *, CTA_IP_MAX + 1> AttributesIP;
    typedef std::array<struct nlattr *, CTA_PROTO_MAX + 1> AttributesProto;
    typedef std::array<struct nlattr *, CTA_TUPLE_MAX + 1> AttributesTuple;
    typedef std::array<struct nlattr *, CTA_COUNTERS_MAX + 1> AttributesCounters;

    ndConntrackFlow(uint32_t id,
        const ndNetlinkConntrackThread::Attributes &attr);

    void Update(const ndNetlinkConntrackThread::Attributes &attr);

    inline uint32_t GetId(void) { return id; }

    inline bool HasExpired(void) {
        return (updated_at + _ND_CT_FLOW_TTL <= nd_time_monotonic()) ?
          true :
          false;
    }

    void Print(void);

protected:
    friend class ndNetlinkConntrackThread;

    void Hash(void);

    uint32_t id = { 0 };
#if defined(_ND_ENABLE_CONNTRACK_MDATA)
    uint32_t mark = { 0 };
#endif
    time_t updated_at = { 0 };
    ndDigest digest;
    sa_family_t l3_proto = { 0 };
    uint8_t l4_proto = { 0 };
    ndAddr orig_addr[2];
    ndAddr reply_addr[2];
#if defined(_ND_ENABLE_CONNTRACK_COUNTERS)
    uint64_t packets[2] = { 0, 0 };
    uint64_t bytes[2] = { 0, 0 };
#endif
};

