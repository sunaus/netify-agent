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

#include <algorithm>
#include <array>
#include <sstream>
#include <string>

#include "nd-except.hpp"
#include "nd-instance.hpp"
#include "nd-netlink-ct.hpp"

// Enable Conntrack debug logging
//#define _ND_DEBUG_LOG   1

constexpr const char *TAG = { "nd-netlink-ct" };

using namespace std;

static int ndNetlinkConntrack_attr_parse(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    switch(type) {
    case CTA_ID:
    case CTA_MARK:
        if (mnl_attr_validate(attr, MNL_TYPE_U32) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U32");
            return MNL_CB_ERROR;
        }
        break;
    case CTA_COUNTERS_ORIG:
    case CTA_COUNTERS_REPLY:
    case CTA_TUPLE_ORIG:
    case CTA_TUPLE_REPLY:
        if (mnl_attr_validate(attr, MNL_TYPE_NESTED) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "TUPLE");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkConntrack_attr_parse_tuple(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);

    int type = mnl_attr_get_type(attr);

    switch(type) {
    case CTA_TUPLE_IP:
        if (mnl_attr_validate(attr, MNL_TYPE_NESTED) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "NESTED");
            return MNL_CB_ERROR;
        }
        break;
    case CTA_TUPLE_PROTO:
        if (mnl_attr_validate(attr, MNL_TYPE_NESTED) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "NESTED");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkConntrack_attr_parse_ip(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);

    int type = mnl_attr_get_type(attr);

    switch(type) {
    case CTA_IP_V4_SRC:
    case CTA_IP_V4_DST:
        if (mnl_attr_validate(attr, MNL_TYPE_U32) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U32");
            return MNL_CB_ERROR;
        }
        break;
    case CTA_IP_V6_SRC:
    case CTA_IP_V6_DST:
        if (mnl_attr_validate2(attr,
            MNL_TYPE_BINARY, sizeof(struct in6_addr)) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "BINARY");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkConntrack_attr_parse_proto(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);

    int type = mnl_attr_get_type(attr);

    switch(type) {
    case CTA_PROTO_NUM:
        if (mnl_attr_validate(attr, MNL_TYPE_U8) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U8");
            return MNL_CB_ERROR;
        }
        break;
    case CTA_PROTO_SRC_PORT:
    case CTA_PROTO_DST_PORT:
        if (mnl_attr_validate(attr, MNL_TYPE_U16) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U16");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkConntrack_attr_parse_counters(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);

    int type = mnl_attr_get_type(attr);

    switch(type) {
    case CTA_COUNTERS_PACKETS:
    case CTA_COUNTERS_BYTES:
        if (mnl_attr_validate(attr, MNL_TYPE_U64) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U64");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

class ndNetlinkConntrackDump : public ndNetlink
{
public:
    ndNetlinkConntrackDump(ndNetlink *parent)
        : ndNetlink(NETLINK_NETFILTER), parent(parent) {
    }

    int ProcessMessage(const struct nlmsghdr *nlh) {
        return parent->ProcessMessage(nlh);
    }

    bool Dump(bool zero_counters = false, sa_family_t family = AF_UNSPEC) {
        fill(
            socket_buffer.begin(),
            socket_buffer.begin() + sizeof(nlmsghdr), '\0'
        );

        struct nlmsghdr *nlh = mnl_nlmsg_put_header(socket_buffer.data());
        nlh->nlmsg_type = (NFNL_SUBSYS_CTNETLINK << 8) |
            (zero_counters ? IPCTNL_MSG_CT_GET_CTRZERO : IPCTNL_MSG_CT_GET);

        struct nfgenmsg *nfh = static_cast<struct nfgenmsg *>(
            mnl_nlmsg_put_extra_header(nlh, sizeof(struct nfgenmsg))
        );
        nfh->nfgen_family = family;
        nfh->version = NFNETLINK_V0;
        nfh->res_id = 0;

        try {
            if (Send(nlh)) return (Recv());
        }
        catch (exception &e) {
            nd_printf(
                "%s: Exception while dumping CT table: %s\n", TAG, e.what()
            );
        }

        return false;
    }

protected:
    ndNetlink *parent;
};

ndNetlinkConntrackThread::ndNetlinkConntrackThread(void)
    : ndThread(TAG),
    ndNetlink(NETLINK_NETFILTER,
            NF_NETLINK_CONNTRACK_NEW |
            NF_NETLINK_CONNTRACK_UPDATE |
            NF_NETLINK_CONNTRACK_DESTROY) {
}

ndNetlinkConntrackThread::~ndNetlinkConntrackThread() {
    Join();
}

void *ndNetlinkConntrackThread::Entry(void) {
    int rc;
    fd_set fds_read;
    int fd = GetDescriptor();

    while (! ShouldTerminate()) {
        FD_ZERO(&fds_read);
        FD_SET(fd, &fds_read);

        struct timeval tv = { 1, 0 };
        rc = select(fd + 1, &fds_read, nullptr, nullptr, &tv);

        if (rc == -1) {
            throw ndExceptionSystemError("select", errno);
        }

        if (FD_ISSET(fd, &fds_read)) {
            try {
                if (! Recv())
                    nd_printf("%s: Error while reading CT message.\n", TAG);
            }
            catch (exception &e) {
                nd_printf("%s: Exception while reading CT message: %s\n",
                    TAG, e.what());
            }
        }
    }

    nd_dprintf("%s: Exit.\n", TAG);

    return nullptr;
}

int ndNetlinkConntrackThread::ProcessMessage(const struct nlmsghdr *nlh) {
    struct nfgenmsg *nfg = static_cast<struct nfgenmsg *>(
        mnl_nlmsg_get_payload(nlh)
    );
#if 0
    nd_dprintf(
        "%s: NLMSG: %hu, len: %u (%u, %u), flags: 0x%x, seq: %u, pid: %u, family: %hhu\n",
        TAG, nlh->nlmsg_type, nlh->nlmsg_len,
        NLMSG_HDRLEN, NLMSG_LENGTH(nlh->nlmsg_len),
        nlh->nlmsg_flags, nlh->nlmsg_seq, nlh->nlmsg_pid, nfg->nfgen_family);
#endif
    Attributes attrs = { 0 };
    if (mnl_attr_parse(nlh, sizeof(*nfg),
        ndNetlinkConntrack_attr_parse, attrs.data()) != MNL_CB_OK) {
        return MNL_CB_OK;
    }

    nf_conntrack_msg_type type = NFCT_T_UNKNOWN;
    switch(nlh->nlmsg_type & 0xFF) {
    case IPCTNL_MSG_CT_NEW:
        if (nlh->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL))
            type = NFCT_T_NEW;
        else
            type = NFCT_T_UPDATE;
        break;
    case IPCTNL_MSG_CT_DELETE:
        type = NFCT_T_DESTROY;
        break;
    default:
        nd_dprintf("%s: unknown type: 0x%04x\n", TAG, type);
        return MNL_CB_OK;
    }

    _ND_NLRT_CHECK_ATTR(TAG, attrs[CTA_ID], MNL_CB_OK);
    _ND_NLRT_CHECK_ATTR(TAG, attrs[CTA_TUPLE_ORIG], MNL_CB_OK);
    _ND_NLRT_CHECK_ATTR(TAG, attrs[CTA_TUPLE_REPLY], MNL_CB_OK);

    uint32_t id = ntohl(mnl_attr_get_u32(attrs[CTA_ID]));

    try {
        switch (type) {
        case NFCT_T_NEW:
            if (! NewFlow(id, attrs)) UpdateFlow(id, attrs);
            break;

        case NFCT_T_UPDATE:
            if (! UpdateFlow(id, attrs)) NewFlow(id, attrs);
            break;

        case NFCT_T_DESTROY:
            DestroyFlow(id);
            break;

        default:
            nd_dprintf("%s: %lu: unhandled CT message type: 0x%04x\n",
                TAG, id, type);
        }
    }
    catch (exception &e) {
        nd_printf("%s: Exception: %s.\n", TAG, e.what());
        //return MNL_CB_STOP;
    }

    return MNL_CB_OK;
}

bool ndNetlinkConntrackThread::NewFlow(uint32_t id, const Attributes &attrs) {
#ifdef _ND_DEBUG_LOG
    nd_dprintf("%s: %lu: new flow.\n", TAG, id);
#endif
    lock_guard<mutex> lg(map_lock);

    auto id_digest = id_digests.find(id);
    if (id_digest != id_digests.end()) {
#ifdef _ND_DEBUG_LOG
        nd_dprintf(
            "%s: %lu: ID found in map.\n",
            TAG, id);
#endif
        return false;
    }

    unique_ptr<ndConntrackFlow> flow(new ndConntrackFlow(id, attrs));
#ifdef _ND_DEBUG_LOG
    flow->Print();
#endif
    auto flow_iter = flows.find(flow->digest);
    if (flow_iter != flows.end()) {
#ifdef _ND_DEBUG_LOG
        flow_iter->second->Print();
#endif
        flows.erase(flow_iter);
    }

    auto id_digests_insert = id_digests.insert(make_pair(id, flow->digest));
    if (! id_digests_insert.second)
        throw ndException("duplicate flow ID on insert");

    auto flows_insert = flows.insert(
        make_pair(flow->digest, std::move(flow))
    );
    if (! flows_insert.second) {
        id_digests.erase(id_digests_insert.first);
        throw ndException("duplicate flow digest on insert");
    }

    stats.created++;

    return true;
}

bool ndNetlinkConntrackThread::UpdateFlow(uint32_t id, const Attributes &attrs) {
#ifdef _ND_DEBUG_LOG
    nd_dprintf("%s: %lu: update flow\n", TAG, id);
#endif
    lock_guard<mutex> lg(map_lock);

    auto id_digest = id_digests.find(id);
    if (id_digest == id_digests.end()) return false;

    auto flow_iter = flows.find(id_digest->second);
    if (flow_iter == flows.end()) {
#ifdef _ND_DEBUG_LOG
        nd_dprintf(
          "%s: %lu: digest not found in flow map.\n",
          TAG, id);
#endif
        id_digests.erase(id_digest);
        return false;
    }

    auto flow = *flow_iter->second;

    flow.Update(attrs);

    stats.updated++;

    return true;
}

void ndNetlinkConntrackThread::DestroyFlow(uint32_t id) {
#ifdef _ND_DEBUG_LOG
    nd_dprintf("%s: %lu: destroy flow\n", TAG, id);
#endif
    lock_guard<mutex> lg(map_lock);

    auto id_digest = id_digests.find(id);
    if (id_digest != id_digests.end()) {
        auto flow_iter = flows.find(id_digest->second);
        if (flow_iter != flows.end())
            flows.erase(flow_iter);

        id_digests.erase(id_digest);

        stats.destroyed++;
    }
}

void ndNetlinkConntrackThread::Dump(bool zero_counters) {
    ndNetlinkConntrackDump ct_dump(this);
    ct_dump.Dump(zero_counters);
}

void ndNetlinkConntrackThread::UpdateFlow(nd_flow_ptr &flow) {
    sa_family_t family;
    struct sockaddr_in *sa_orig_src = nullptr, *sa_orig_dst = nullptr;
    struct sockaddr_in *sa_reply_src = nullptr, *sa_reply_dst = nullptr;
    struct sockaddr_in6 *sa6_orig_src = nullptr, *sa6_orig_dst = nullptr;
    struct sockaddr_in6 *sa6_reply_src = nullptr, *sa6_reply_dst = nullptr;

    if (flow->ip_version == 4) family = AF_INET;
    else family = AF_INET6;

    sha1 ctx;
    sha1_init(&ctx);

    sha1_write(&ctx, (const char *)&family, sizeof(sa_family_t));
    sha1_write(&ctx, (const char *)&flow->ip_protocol, sizeof(uint8_t));

    sha1_write(&ctx, flow->lower_addr.GetAddress(),
      flow->lower_addr.GetAddressSize());
    sha1_write(&ctx, flow->upper_addr.GetAddress(),
      flow->upper_addr.GetAddressSize());

    uint16_t port = flow->lower_addr.GetPort(false);
    sha1_write(&ctx, (const char *)&port, sizeof(uint16_t));
    port = flow->upper_addr.GetPort(false);
    sha1_write(&ctx, (const char *)&port, sizeof(uint16_t));

    ndDigest digest;
    sha1_result(&ctx, digest.data());

    lock_guard<mutex> lg(map_lock);

    stats.lookups++;
    auto flow_iter = flows.find(digest);

    if (flow_iter != flows.end()) {
        if (! flow_iter->second->reply_addr[ndEnumCast(
            ndConntrackFlow::Direction, SRC)].IsValid() ||
            ! flow_iter->second->reply_addr[ndEnumCast(
            ndConntrackFlow::Direction, DST)].IsValid()) {
            throw ndException("reply SRC or DST invalid");
        }

        stats.matches++;
        auto ct_flow = *flow_iter->second;

        ct_flow.updated_at = nd_time_monotonic();

        flow->conntrack.id = ct_flow.id;
#if defined(_ND_ENABLE_CONNTRACK_MDATA)
        flow->conntrack.mark = ct_flow.mark;
#endif
        switch (ct_flow.l3_proto) {
        case AF_INET:
            sa_orig_src = reinterpret_cast<struct sockaddr_in *>(
              &ct_flow.orig_addr[ndEnumCast(ndConntrackFlow::Direction, SRC)].addr.in);
            sa_orig_dst = reinterpret_cast<struct sockaddr_in *>(
              &ct_flow.orig_addr[ndEnumCast(ndConntrackFlow::Direction, DST)].addr.in);
            sa_reply_src = reinterpret_cast<struct sockaddr_in *>(
              &ct_flow.reply_addr[ndEnumCast(ndConntrackFlow::Direction, SRC)].addr.in);
            sa_reply_dst = reinterpret_cast<struct sockaddr_in *>(
              &ct_flow.reply_addr[ndEnumCast(ndConntrackFlow::Direction, DST)].addr.in);

            if (memcmp(sa_orig_src, sa_reply_dst,
                  sizeof(struct sockaddr_in)) ||
              memcmp(sa_orig_dst, sa_reply_src,
                sizeof(struct sockaddr_in)))
                flow->flags.ip_nat = true;

            break;

        case AF_INET6:
            sa6_orig_src = reinterpret_cast<struct sockaddr_in6 *>(
              &ct_flow.orig_addr[ndEnumCast(ndConntrackFlow::Direction, SRC)].addr.in6);
            sa6_orig_dst = reinterpret_cast<struct sockaddr_in6 *>(
              &ct_flow.orig_addr[ndEnumCast(ndConntrackFlow::Direction, DST)].addr.in6);
            sa6_reply_src = reinterpret_cast<struct sockaddr_in6 *>(
              &ct_flow.reply_addr[ndEnumCast(ndConntrackFlow::Direction, SRC)].addr.in6);
            sa6_reply_dst = reinterpret_cast<struct sockaddr_in6 *>(
              &ct_flow.reply_addr[ndEnumCast(ndConntrackFlow::Direction, DST)].addr.in6);

            if (memcmp(sa6_orig_src, sa6_reply_dst,
                  sizeof(struct sockaddr_in6)) ||
              memcmp(sa6_orig_dst, sa6_reply_src,
                sizeof(struct sockaddr_in6)))
                flow->flags.ip_nat = true;

            break;
        }
    }
#ifdef _ND_DEBUG_LOG
    else {
        string _digest;
        nd_sha1_to_string(digest, _digest);
        nd_dprintf("%s: flow not found: %s\n", TAG, _digest.c_str());
        flow->Print();
    }
#endif
}

void ndNetlinkConntrackThread::UpdateFlowStats(nd_flow_ptr &flow, const struct timeval &stamp) const {
    if (flow->conntrack.id == 0) {
//        nd_dprintf("%s: id not set.\n", TAG);
        return;
    }

    lock_guard<mutex> lg(map_lock);

    auto i = id_digests.find(flow->conntrack.id);
    if (i == id_digests.end()) {
//        nd_dprintf("%s: id not found in map.\n", TAG);
        return;
    }

    auto j = flows.find(i->second);
    if (j == flows.end()) {
//        nd_dprintf("%s: flow hash not found in map.\n", TAG);
        return;
    }

    if (j->second->packets[ndEnumCast(ndConntrackFlow::Direction, SRC)] == 0 &&
        j->second->packets[ndEnumCast(ndConntrackFlow::Direction, DST)] == 0)
        return;

    unsigned idx_lower, idx_upper;

    switch (flow->origin) {
    case ndFlow::Origin::LOWER:
        idx_lower = ndEnumCast(ndConntrackFlow::Direction, SRC);
        idx_upper = ndEnumCast(ndConntrackFlow::Direction, DST);
        break;

    case ndFlow::Origin::UPPER:
        idx_lower = ndEnumCast(ndConntrackFlow::Direction, DST);
        idx_upper = ndEnumCast(ndConntrackFlow::Direction, SRC);
        break;
    default:
        nd_dprintf("%s: flow origin unknown.\n", TAG);
        return;
    }

    uint64_t ts_pkt = ((uint64_t)stamp.tv_sec) * ND_DETECTION_TICKS +
      stamp.tv_usec / (1000000 / ND_DETECTION_TICKS);

    flow->ts_last_seen = ts_pkt;

    flow->stats.lower_packets = j->second->packets[idx_lower];
    flow->stats.lower_bytes = j->second->bytes[idx_lower];
    flow->stats.upper_packets = j->second->packets[idx_upper];
    flow->stats.upper_bytes = j->second->bytes[idx_upper];

    flow->stats.total_packets += j->second->packets[idx_lower];
    flow->stats.total_bytes += j->second->bytes[idx_lower];
    flow->stats.total_packets += j->second->packets[idx_upper];
    flow->stats.total_bytes += j->second->bytes[idx_upper];
#ifdef _ND_ENABLE_EXTENDED_STATS
    flow->stats.UpdateRate(true, ts_pkt, j->second->bytes[idx_lower]);
    flow->stats.UpdateRate(false, ts_pkt, j->second->bytes[idx_upper]);
#endif
//    nd_dprintf("%s: updated flow counters.\n", TAG);
}

void ndNetlinkConntrackThread::PurgeFlows(void) {
    lock_guard<mutex> lg(map_lock);

    for (FlowMap::iterator i = flows.begin();
         i != flows.end();)
    {
        if (i->second->HasExpired()) {
            id_digests.erase(i->second->GetId());
            i = flows.erase(i);
        }
        else i++;
    }
}

ndConntrackFlow::ndConntrackFlow(uint32_t id,
    const ndNetlinkConntrackThread::Attributes &attr)
    : id(id) {
    Update(attr);
}

void ndConntrackFlow::Update(const ndNetlinkConntrackThread::Attributes &attr) {
    AttributesTuple tuple_orig = { 0 }, tuple_reply = { 0 };

    if (mnl_attr_parse_nested(attr[CTA_TUPLE_ORIG],
        ndNetlinkConntrack_attr_parse_tuple, tuple_orig.data()) != MNL_CB_OK) {
        throw ndException("origin %s invalid", "CTA_TUPLE_ORIG");
    }
    if (mnl_attr_parse_nested(attr[CTA_TUPLE_REPLY],
        ndNetlinkConntrack_attr_parse_tuple, tuple_reply.data()) != MNL_CB_OK) {
        throw ndException("reply %s invalid", "CTA_TUPLE_REPLY");
    }

    AttributesIP ip_orig = { 0 }, ip_reply = { 0 };

    if (mnl_attr_parse_nested(tuple_orig[CTA_TUPLE_IP],
        ndNetlinkConntrack_attr_parse_ip, ip_orig.data()) != MNL_CB_OK) {
        throw ndException("origin %s invalid", "CTA_TUPLE_IP");
    }
    if (mnl_attr_parse_nested(tuple_reply[CTA_TUPLE_IP],
        ndNetlinkConntrack_attr_parse_ip, ip_reply.data()) != MNL_CB_OK) {
        throw ndException("reply %s invalid", "CTA_TUPLE_IP");
    }

    if (ip_orig[CTA_IP_V4_SRC]) l3_proto = AF_INET;
    else if (ip_orig[CTA_IP_V6_SRC]) l3_proto = AF_INET6;

    switch (l3_proto) {
    case AF_INET:
        _ND_NLEX_CHECK_ATTR(TAG, ip_orig[CTA_IP_V4_SRC]);
        _ND_NLEX_CHECK_ATTR(TAG, ip_orig[CTA_IP_V4_DST]);
        _ND_NLEX_CHECK_ATTR(TAG, ip_reply[CTA_IP_V4_SRC]);
        _ND_NLEX_CHECK_ATTR(TAG, ip_reply[CTA_IP_V4_DST]);

        ndAddr::Create(orig_addr[ndEnumCast(Direction, SRC)],
            static_cast<struct in_addr *>(
                mnl_attr_get_payload(ip_orig[CTA_IP_V4_SRC])
            )
        );
        ndAddr::Create(orig_addr[ndEnumCast(Direction, DST)],
            static_cast<struct in_addr *>(
                mnl_attr_get_payload(ip_orig[CTA_IP_V4_DST])
            )
        );

        ndAddr::Create(reply_addr[ndEnumCast(Direction, SRC)],
            static_cast<struct in_addr *>(
                mnl_attr_get_payload(ip_reply[CTA_IP_V4_SRC])
            )
        );
        ndAddr::Create(reply_addr[ndEnumCast(Direction, DST)],
            static_cast<struct in_addr *>(
                mnl_attr_get_payload(ip_reply[CTA_IP_V4_DST])
            )
        );
        break;

    case AF_INET6:
        _ND_NLEX_CHECK_ATTR(TAG, ip_orig[CTA_IP_V6_SRC]);
        _ND_NLEX_CHECK_ATTR(TAG, ip_orig[CTA_IP_V6_DST]);
        _ND_NLEX_CHECK_ATTR(TAG, ip_reply[CTA_IP_V6_SRC]);
        _ND_NLEX_CHECK_ATTR(TAG, ip_reply[CTA_IP_V6_DST]);

        ndAddr::Create(orig_addr[ndEnumCast(Direction, SRC)],
            static_cast<struct in6_addr *>(
                mnl_attr_get_payload(ip_orig[CTA_IP_V6_SRC])
            )
        );
        ndAddr::Create(orig_addr[ndEnumCast(Direction, DST)],
            static_cast<struct in6_addr *>(
                mnl_attr_get_payload(ip_orig[CTA_IP_V6_DST])
            )
        );

        ndAddr::Create(reply_addr[ndEnumCast(Direction, SRC)],
            static_cast<struct in6_addr *>(
                mnl_attr_get_payload(ip_reply[CTA_IP_V6_SRC])
            )
        );
        ndAddr::Create(reply_addr[ndEnumCast(Direction, DST)],
            static_cast<struct in6_addr *>(
                mnl_attr_get_payload(ip_reply[CTA_IP_V6_DST])
            )
        );
        break;

    default:
        throw ndException("un-supported address family: %hhu", l3_proto);
    }

    AttributesProto proto_orig = { 0 }, proto_reply = { 0 };

    if (mnl_attr_parse_nested(tuple_orig[CTA_TUPLE_PROTO],
        ndNetlinkConntrack_attr_parse_proto, proto_orig.data()) != MNL_CB_OK) {
        throw ndException("origin %s invalid", "CTA_TUPLE_PROTO");
    }
    if (mnl_attr_parse_nested(tuple_reply[CTA_TUPLE_PROTO],
        ndNetlinkConntrack_attr_parse_proto, proto_reply.data()) != MNL_CB_OK) {
        throw ndException("reply %s invalid", "CTA_TUPLE_PROTO");
    }

    _ND_NLEX_CHECK_ATTR(TAG, proto_orig[CTA_PROTO_NUM]);
    _ND_NLEX_CHECK_ATTR(TAG, proto_reply[CTA_PROTO_NUM]);

    if (mnl_attr_get_u8(proto_orig[CTA_PROTO_NUM]) !=
        mnl_attr_get_u8(proto_reply[CTA_PROTO_NUM])) {
        nd_dprintf(
            "%s: WARNING: origin protocol doesn't match destination.\n", TAG);
    }

    l4_proto = mnl_attr_get_u8(proto_orig[CTA_PROTO_NUM]);

    if (proto_orig[CTA_PROTO_SRC_PORT]) {
        orig_addr[ndEnumCast(Direction, SRC)].SetPort(
            mnl_attr_get_u16(proto_orig[CTA_PROTO_SRC_PORT])
        );
    }
    if (proto_orig[CTA_PROTO_DST_PORT]) {
        orig_addr[ndEnumCast(Direction, DST)].SetPort(
            mnl_attr_get_u16(proto_orig[CTA_PROTO_DST_PORT])
        );
    }
    if (proto_reply[CTA_PROTO_SRC_PORT]) {
        reply_addr[ndEnumCast(Direction, SRC)].SetPort(
            mnl_attr_get_u16(proto_reply[CTA_PROTO_SRC_PORT])
        );
    }
    if (proto_reply[CTA_PROTO_DST_PORT]) {
        reply_addr[ndEnumCast(Direction, DST)].SetPort(
            mnl_attr_get_u16(proto_reply[CTA_PROTO_DST_PORT])
        );
    }

#if defined(_ND_ENABLE_CONNTRACK_MDATA)
    if (attr[CTA_MARK])
        mark = ntohl(mnl_attr_get_u32(attr[CTA_MARK]));
#endif

#if defined(_ND_ENABLE_CONNTRACK_COUNTERS)
    if (attr[CTA_COUNTERS_ORIG]) {
        AttributesCounters counters = { 0 };

        if (mnl_attr_parse_nested(attr[CTA_COUNTERS_ORIG],
            ndNetlinkConntrack_attr_parse_counters,
            counters.data()) != MNL_CB_OK) {
            throw ndException("origin %s is invalid", "CTA_COUNTERS_ORIG");
        }

        packets[ndEnumCast(Direction, SRC)] =
            be64toh(mnl_attr_get_u64(counters[CTA_COUNTERS_PACKETS]));
        bytes[ndEnumCast(Direction, SRC)] =
            be64toh(mnl_attr_get_u64(counters[CTA_COUNTERS_BYTES]));
#if 0
        nd_printf(
            "%s: origin packets: %llu, bytes: %llu\n",
            TAG,
            packets[ndEnumCast(Direction, SRC)],
            bytes[ndEnumCast(Direction, SRC)]);
#endif
    }

    if (attr[CTA_COUNTERS_REPLY]) {
        AttributesCounters counters = { 0 };

        if (mnl_attr_parse_nested(attr[CTA_COUNTERS_REPLY],
            ndNetlinkConntrack_attr_parse_counters,
            counters.data()) != MNL_CB_OK) {
            throw ndException("reply %s is invalid", "CTA_COUNTERS_REPLY");
        }

        packets[ndEnumCast(Direction, DST)] =
            be64toh(mnl_attr_get_u64(counters[CTA_COUNTERS_PACKETS]));
        bytes[ndEnumCast(Direction, DST)] =
            be64toh(mnl_attr_get_u64(counters[CTA_COUNTERS_BYTES]));
#if 0
        nd_printf(
            "%s: reply packets: %llu, bytes: %llu\n",
            TAG,
            packets[ndEnumCast(Direction, DST)],
            bytes[ndEnumCast(Direction, DST)]);
#endif
    }
#endif

    updated_at = nd_time_monotonic();

    Hash();
}

void ndConntrackFlow::Hash(void) {
    sha1 ctx;
    int addr_cmp = 0;
    struct sockaddr_in *sa_src = nullptr, *sa_dst = nullptr;
    struct sockaddr_in6 *sa6_src = nullptr, *sa6_dst = nullptr;

    sha1_init(&ctx);

    sha1_write(&ctx, reinterpret_cast<const char *>(&l3_proto),
      sizeof(sa_family_t));
    sha1_write(&ctx, reinterpret_cast<const char *>(&l4_proto),
      sizeof(uint8_t));

    string src, dst;

    switch (orig_addr[ndEnumCast(Direction, SRC)].addr.ss.ss_family) {
    case AF_INET:
        sa_src = reinterpret_cast<struct sockaddr_in *>(
            &reply_addr[ndEnumCast(Direction, DST)].addr.in);

        sa_dst = reinterpret_cast<struct sockaddr_in *>(
            &reply_addr[ndEnumCast(Direction, SRC)].addr.in);

        addr_cmp = memcmp(
            &sa_src->sin_addr, &sa_dst->sin_addr, sizeof(in_addr));

        if (addr_cmp < 0) {
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa_src->sin_addr),
              sizeof(struct in_addr));
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa_dst->sin_addr),
              sizeof(struct in_addr));
        }
        else {
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa_dst->sin_addr),
              sizeof(struct in_addr));
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa_src->sin_addr),
              sizeof(struct in_addr));
        }
        break;

    case AF_INET6:
        sa6_src = reinterpret_cast<struct sockaddr_in6 *>(
            &reply_addr[ndEnumCast(Direction, DST)].addr.in6);

        sa6_dst = reinterpret_cast<struct sockaddr_in6 *>(
            &reply_addr[ndEnumCast(Direction, SRC)].addr.in6);

        addr_cmp = memcmp(&sa6_src->sin6_addr,
          &sa6_dst->sin6_addr, sizeof(struct in6_addr));

        if (addr_cmp < 0) {
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa6_src->sin6_addr),
              sizeof(struct in6_addr));
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa6_dst->sin6_addr),
              sizeof(struct in6_addr));
        }
        else {
            sha1_write(&ctx,
              reinterpret_cast<const char *>(&sa6_dst->sin6_addr),
              sizeof(struct in6_addr));
            sha1_write(&ctx, (const char *)&sa6_src->sin6_addr,
              sizeof(struct in6_addr));
        }
        break;
    }

    if (addr_cmp < 0) {
        uint16_t port;

        port = reply_addr[ndEnumCast(Direction, DST)].GetPort(false);
        sha1_write(&ctx,
          reinterpret_cast<const char *>(&port), sizeof(uint16_t));
        port = reply_addr[ndEnumCast(Direction, SRC)].GetPort(false);
        sha1_write(&ctx,
          reinterpret_cast<const char *>(&port), sizeof(uint16_t));
    }
    else {
        uint16_t port;

        port = reply_addr[ndEnumCast(Direction, SRC)].GetPort(false);
        sha1_write(&ctx,
          reinterpret_cast<const char *>(&port), sizeof(uint16_t));
        port = reply_addr[ndEnumCast(Direction, DST)].GetPort(false);
        sha1_write(&ctx,
          reinterpret_cast<const char *>(&port), sizeof(uint16_t));
    }

    sha1_result(&ctx, digest.data());
#ifdef _ND_DEBUG_LOG
    string _digest;
    nd_sha1_to_string(digest, _digest);
    nd_dprintf("%s: %lu: %s\n", TAG, id, _digest.c_str());
#endif
}

void ndConntrackFlow::Print(void) {
    ndDebugLogStream dls(ndDebugLogStream::Type::FLOW);

    // ID
    dls << TAG << ": " << setw(8) << id << ": ";

    // Digest
    for (unsigned i = 0; i < 5; i++) {
        dls << setw(2) << setfill('0') << hex
            << (int)digest[i];
    }
    dls << ": ";

    // L3 proto
    switch (l3_proto) {
    case AF_INET:
        dls << setfill(' ') << dec << setw(2) << 4;
        break;
    case AF_INET6:
        dls << setfill(' ') << dec << setw(2) << 6;
        break;
    }

    // L4 proto
    dls << ": " << setfill(' ') << dec << setw(2) << (unsigned)l4_proto << ": ";

    // Origin addresses
    auto SRC = ndEnumCast(ndConntrackFlow::Direction, SRC);
    auto DST = ndEnumCast(ndConntrackFlow::Direction, DST);
    dls << orig_addr[SRC].GetString(ndAddr::MakeFlags::PORT) << " --> "
        << orig_addr[DST].GetString(ndAddr::MakeFlags::PORT) << " : "
        << reply_addr[SRC].GetString(ndAddr::MakeFlags::PORT) << " --> "
        << reply_addr[DST].GetString(ndAddr::MakeFlags::PORT) << endl;
}
