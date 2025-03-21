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
#include <vector>

#include <net/if.h>

#include <linux/if.h>
#include <linux/if_link.h>

#include "nd-except.hpp"
#include "nd-instance.hpp"
#include "nd-netlink-rt.hpp"
#include "nd-util.hpp"

// Enable Conntrack debug logging
// #define _ND_DEBUG_LOG   1

using namespace std;

constexpr const char *TAG = { "netlink-rt" };

static int ndNetlinkRoute_attr_parse_ifav4(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    if (mnl_attr_type_valid(attr, IFA_MAX) < 0)
        return MNL_CB_OK;

    switch(type) {
    case IFA_LOCAL:
    case IFA_ADDRESS:
    case IFA_BROADCAST:
        if (mnl_attr_validate(attr, MNL_TYPE_U32) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U32");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkRoute_attr_parse_ifav6(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    if (mnl_attr_type_valid(attr, IFA_MAX) < 0)
        return MNL_CB_OK;

    switch(type) {
    case IFA_LOCAL:
    case IFA_ADDRESS:
        if (mnl_attr_validate2(
            attr, MNL_TYPE_BINARY, sizeof(struct in6_addr)) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "BINARY");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkRoute_attr_parse_rtav4(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    if (mnl_attr_type_valid(attr, RTA_MAX) < 0)
        return MNL_CB_OK;

    switch(type) {
    case RTA_DST:
    case RTA_OIF:
        if (mnl_attr_validate(attr, MNL_TYPE_U32) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U32");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkRoute_attr_parse_rtav6(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    if (mnl_attr_type_valid(attr, RTA_MAX) < 0)
        return MNL_CB_OK;

    switch(type) {
    case RTA_DST:
        if (mnl_attr_validate2(
            attr, MNL_TYPE_BINARY, sizeof(struct in6_addr)) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "BINARY");
            return MNL_CB_ERROR;
        }
        break;
    case RTA_OIF:
        if (mnl_attr_validate(attr, MNL_TYPE_U32) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "U32");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkRoute_attr_parse_ifla(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    if (mnl_attr_type_valid(attr, IFLA_MAX) < 0)
        return MNL_CB_OK;

    switch(type) {
    case IFLA_ADDRESS:
        if (mnl_attr_validate(attr, MNL_TYPE_BINARY) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "BINARY");
            return MNL_CB_ERROR;
        }
        break;

    case IFLA_WIRELESS:
        if (mnl_attr_validate(attr, MNL_TYPE_BINARY) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "BINARY");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

static int ndNetlinkRoute_attr_parse_nda(
    const struct nlattr *attr, void *data) {
    const struct nlattr **tb = static_cast<const struct nlattr **>(data);
    int type = mnl_attr_get_type(attr);

    if (mnl_attr_type_valid(attr, NDA_MAX) < 0)
        return MNL_CB_OK;

    switch(type) {
    case NDA_DST:
    case NDA_LLADDR:
        if (mnl_attr_validate(attr, MNL_TYPE_BINARY) < 0) {
            nd_dprintf("%s: invalid attribute type, expected: %s\n",
                TAG, "BINARY");
            return MNL_CB_ERROR;
        }
        break;
    }

    tb[type] = attr;

    return MNL_CB_OK;
}

class ndNetlinkRouteDump : public ndNetlink
{
public:
    ndNetlinkRouteDump(ndNetlink *parent)
        : ndNetlink(NETLINK_ROUTE), parent(parent) {
    }

    int ProcessMessage(const struct nlmsghdr *nlh) {
        return parent->ProcessMessage(nlh);
    }

    bool Dump(uint16_t type, sa_family_t family = AF_UNSPEC) {
        fill(
            socket_buffer.begin(),
            socket_buffer.begin() + sizeof(nlmsghdr), '\0'
        );

        struct nlmsghdr *nlh = mnl_nlmsg_put_header(socket_buffer.data());
        nlh->nlmsg_type = type;

        struct ndmsg *ndh = static_cast<struct ndmsg *>(
            mnl_nlmsg_put_extra_header(nlh, sizeof(struct ndmsg))
        );
        ndh->ndm_family = family;

        try {
            if (Send(nlh)) return (Recv());
        }
        catch (exception &e) {
            nd_printf("Exception while dumping RT table: %s\n", e.what());
        }

        return false;
    }

protected:
    ndNetlink *parent;
};

ndNetlinkRouteThread::ndNetlinkRouteThread(void)
    : ndThread(TAG),
        ndNetlink(NETLINK_ROUTE,
            RTMGRP_IPV4_IFADDR | RTMGRP_IPV4_ROUTE |
            RTMGRP_IPV6_IFADDR | RTMGRP_IPV6_ROUTE |
            RTMGRP_LINK | RTMGRP_NEIGH
        ) { }

ndNetlinkRouteThread::~ndNetlinkRouteThread() {
    Join();
}

void *ndNetlinkRouteThread::Entry(void) {
    int rc;
    fd_set fds_read;
    int fd = GetDescriptor();

    while (! ShouldTerminate()) {
        FD_ZERO(&fds_read);
        FD_SET(fd, &fds_read);

        struct timeval tv = { 1, 0 };
        rc = select(fd + 1, &fds_read, nullptr, nullptr, &tv);

        if (rc == -1) {
            throw ndExceptionSystemError("select", strerror(errno));
        }

        if (FD_ISSET(fd, &fds_read)) {
            try {
                if (! Recv())
                    nd_printf("Error while reading RT message.\n");
            }
            catch (exception &e) {
                nd_printf("Exception while reading RT message: %s\n", e.what());
            }
        }
    }

    nd_dprintf("%s: Exit.\n", tag.c_str());

    return nullptr;
}

int ndNetlinkRouteThread::ProcessMessage(const struct nlmsghdr *nlh) {
#if 0
    struct nfgenmsg *nfg = static_cast<struct nfgenmsg *>(
        mnl_nlmsg_get_payload(nlh)
    );
    nd_dprintf(
        "%s: NLMSG: %hu, len: %u (%u, %u), flags: 0x%x, seq: %u, pid: %u, family: %hhu\n",
        tag.c_str(), nlh->nlmsg_type, nlh->nlmsg_len,
        NLMSG_HDRLEN, NLMSG_LENGTH(nlh->nlmsg_len),
        nlh->nlmsg_flags, nlh->nlmsg_seq, nlh->nlmsg_pid, nfg->nfgen_family);
#endif
    int rc;

    switch (nlh->nlmsg_type) {
    case RTM_NEWADDR:
        if ((rc = AddRemoveAddress(nlh)) < 0) stats.addr.error++;
        else stats.addr.added += rc;
        break;

    case RTM_DELADDR:
        if ((rc = AddRemoveAddress(nlh, false)) < 0) stats.addr.error++;
        else stats.addr.removed += rc;
        break;

    case RTM_NEWROUTE:
        if ((rc = AddRemoveNetwork(nlh)) < 0) stats.net.error++;
        else stats.net.added += rc;
        break;

    case RTM_DELROUTE:
        if ((rc = AddRemoveNetwork(nlh, false)) < 0) stats.net.error++;
        else stats.net.removed += rc;
        break;

    case RTM_NEWLINK:
        if ((rc = AddRemoveLink(nlh)) < 0) stats.link.error++;
        else stats.link.added += rc;
        break;

    case RTM_DELLINK:
        if ((rc = AddRemoveLink(nlh, false)) < 0) stats.link.error++;
        else stats.link.removed += rc;
        break;

    case RTM_NEWNEIGH:
        if ((rc = AddRemoveNeighbor(nlh)) < 0) stats.neigh.error++;
        else stats.neigh.added += rc;
        break;

    case RTM_DELNEIGH:
        if ((rc = AddRemoveNeighbor(nlh, false)) < 0) stats.neigh.error++;
        else stats.neigh.removed += rc;
        break;
    }

    return MNL_CB_OK;
}

void ndNetlinkRouteThread::Dump(void) {
    ndNetlinkRouteDump rt_dump(this);

    rt_dump.Dump(RTM_GETADDR);
    rt_dump.Dump(RTM_GETROUTE);
    rt_dump.Dump(RTM_GETLINK);
    rt_dump.Dump(RTM_GETNEIGH);
}

void ndNetlinkRouteThread::GetStats(Stats &stats) {
    stats.addr.added = this->stats.addr.added.exchange(0);
    stats.addr.removed = this->stats.addr.removed.exchange(0);
    stats.addr.error = this->stats.addr.error.exchange(0);

    stats.net.added = this->stats.net.added.exchange(0);
    stats.net.removed = this->stats.net.removed.exchange(0);
    stats.net.error = this->stats.net.error.exchange(0);

    stats.link.added = this->stats.link.added.exchange(0);
    stats.link.removed = this->stats.link.removed.exchange(0);
    stats.link.error = this->stats.link.error.exchange(0);

    stats.neigh.added = this->stats.neigh.added.exchange(0);
    stats.neigh.removed = this->stats.neigh.removed.exchange(0);
    stats.neigh.error = this->stats.neigh.error.exchange(0);
}

void ndNetlinkRouteThread::PrintStats(const Stats &stats) const {
    nd_dprintf("%s: addresses added: %lu, removed: %lu, errors: %lu\n",
        tag.c_str(), stats.addr.added.load(),
        stats.addr.removed.load(), stats.addr.error.load()
    );
    nd_dprintf("%s: networks added: %lu, removed: %lu, errors: %lu\n",
        tag.c_str(), stats.net.added.load(),
        stats.net.removed.load(), stats.net.error.load()
    );
    nd_dprintf("%s: links added: %lu, removed: %lu, errors: %lu\n",
        tag.c_str(), stats.link.added.load(),
        stats.link.removed.load(), stats.link.error.load()
    );
    nd_dprintf("%s: neighbors added: %lu, removed: %lu, errors: %lu\n",
        tag.c_str(), stats.neigh.added.load(),
        stats.neigh.removed.load(), stats.neigh.error.load()
    );
}

int ndNetlinkRouteThread::AddRemoveAddress(const struct nlmsghdr *nlh, bool add) {
    int rc = -1;

    const struct ifaddrmsg *ifa = static_cast<const struct ifaddrmsg *>(
        mnl_nlmsg_get_payload(nlh)
    );

    char ifname[IFNAMSIZ] = { '\0' };
    if (if_indextoname(ifa->ifa_index, ifname) == nullptr) {
        nd_dprintf("%s: %s: if_indextoname(%d): %s\n",
            TAG, __PRETTY_FUNCTION__, ifa->ifa_index, strerror(errno));
        return rc;
    }

    AttrsAddr attrs = { 0 };

    switch(ifa->ifa_family) {
    case AF_INET:
        if (mnl_attr_parse(nlh, sizeof(*ifa),
            ndNetlinkRoute_attr_parse_ifav4, attrs.data()) != MNL_CB_OK)
            return rc;
        break;
    case AF_INET6:
        if (mnl_attr_parse(nlh, sizeof(*ifa),
            ndNetlinkRoute_attr_parse_ifav6, attrs.data()) != MNL_CB_OK)
            return rc;
        break;
    default:
        nd_dprintf(
            "%s: unsupported address family: %d\n",
            TAG, ifa->ifa_family
        );
        return rc;
    }

    rc = 0;
    for (unsigned i = 0; i < attrs.size(); i++) {
        ndAddr addr;
        ndAddr::Type type = ndAddr::Type::LOCAL;

        switch(ifa->ifa_family) {
        case AF_INET:
#if 0
            if (i == IFA_LOCAL && attrs[IFA_LOCAL]) {
                ndAddr::Create(addr,
                    static_cast<struct in_addr *>(
                        mnl_attr_get_payload(attrs[IFA_LOCAL])
                    )
                );
            }
#endif
            if (i == IFA_ADDRESS && attrs[IFA_ADDRESS]) {
                ndAddr::Create(addr,
                    static_cast<struct in_addr *>(
                        mnl_attr_get_payload(attrs[IFA_ADDRESS])
                    )
                );
            }
            if (i == IFA_BROADCAST && attrs[IFA_BROADCAST]) {
                type = ndAddr::Type::BROADCAST;
                ndAddr::Create(addr,
                    static_cast<struct in_addr *>(
                        mnl_attr_get_payload(attrs[IFA_BROADCAST])
                    )
                );
            }
            break;

        case AF_INET6:
#if 0
            if (i == IFA_LOCAL && attrs[IFA_LOCAL]) {
                ndAddr::Create(addr,
                    static_cast<struct in6_addr *>(
                        mnl_attr_get_payload(attrs[IFA_LOCAL])
                    )
                );
            }
#endif
            if (i == IFA_ADDRESS && attrs[IFA_ADDRESS]) {
                ndAddr::Create(addr,
                    static_cast<struct in6_addr *>(
                        mnl_attr_get_payload(attrs[IFA_ADDRESS])
                    )
                );
            }
            break;
        }

        if (addr.IsValid()) {
            ndInstance &ndi = ndInstance::GetInstance();
            if (add) {
#ifdef _ND_DEBUG_LOG
                nd_dprintf("%s: adding address: %s: %s\n", TAG, ifname,
                    addr.GetString().c_str()
                );
#endif
                if (ndi.addr_lookup.AddAddress(type, addr, ifname)) rc++;
            }
            else {
#ifdef _ND_DEBUG_LOG
                nd_dprintf("%s: removing address: %s: %s\n", TAG, ifname,
                    addr.GetString().c_str()
                );
#endif
                if (ndi.addr_lookup.RemoveAddress(addr, ifname)) rc++;
            }
        }
    }

    return rc;
}

int ndNetlinkRouteThread::AddRemoveNetwork(const struct nlmsghdr *nlh, bool add) {
    const struct rtmsg *rtm = static_cast<const struct rtmsg *>(
        mnl_nlmsg_get_payload(nlh)
    );

    if (rtm->rtm_type != RTN_UNICAST) return 0;

    AttrsRoute attrs = { 0 };

    switch(rtm->rtm_family) {
    case AF_INET:
        if (mnl_attr_parse(nlh, sizeof(*rtm),
            ndNetlinkRoute_attr_parse_rtav4, attrs.data()) != MNL_CB_OK)
            return -1;
        break;

    case AF_INET6:
        if (mnl_attr_parse(nlh, sizeof(*rtm),
            ndNetlinkRoute_attr_parse_rtav6, attrs.data()) != MNL_CB_OK)
            return -1;
        break;
    default:
        nd_dprintf(
            "%s: unsupported address family: %d\n",
            TAG, rtm->rtm_family
        );
        return 0;
    }

    _ND_NLRT_CHECK_ATTR(TAG, attrs[RTA_DST], -1);
    _ND_NLRT_CHECK_ATTR(TAG, attrs[RTA_OIF], -1);

    ndAddr addr;
    switch(rtm->rtm_family) {
    case AF_INET:
        ndAddr::Create(addr,
            static_cast<struct in_addr *>(
                mnl_attr_get_payload(attrs[RTA_DST])
            ), rtm->rtm_dst_len
        );
        break;
    case AF_INET6:
        ndAddr::Create(addr,
            static_cast<struct in6_addr *>(
                mnl_attr_get_payload(attrs[RTA_DST])
            ), rtm->rtm_dst_len
        );
        break;
    }

    char ifname[IFNAMSIZ] = { '\0' };
    if (if_indextoname(mnl_attr_get_u32(attrs[RTA_OIF]), ifname) == nullptr) {
        nd_dprintf("%s: %s: if_indextoname(%u): %s\n", TAG, __PRETTY_FUNCTION__,
            mnl_attr_get_u32(attrs[RTA_OIF]), strerror(errno));
        return -1;
    }

    if (addr.IsValid() && ifname[0] != '\0') {
        ndInstance &ndi = ndInstance::GetInstance();
        if (add) {
#ifdef _ND_DEBUG_LOG
            nd_dprintf("%s: adding network: %s: %s\n", TAG, ifname,
                addr.GetString(ndAddr::MakeFlags::PREFIX).c_str());
#endif
            return (ndi.addr_lookup.AddAddress(
              ndAddr::Type::LOCAL, addr, ifname)) ? 1 : 0;
        }
        else {
#ifdef _ND_DEBUG_LOG
            nd_dprintf("%s: removing network: %s: %s\n", TAG, ifname,
                addr.GetString(ndAddr::MakeFlags::PREFIX).c_str());
#endif
            return (ndi.addr_lookup.RemoveAddress(
                addr, ifname)) ? 1 : 0;
        }
    }

    return 0;
}

int ndNetlinkRouteThread::AddRemoveLink(const struct nlmsghdr *nlh, bool add) {
    const struct ifinfomsg *ifi = static_cast<const struct ifinfomsg *>(
        mnl_nlmsg_get_payload(nlh)
    );

    int rc = 0;
    ndInstance &ndi = ndInstance::GetInstance();

    if (add) {
        char ifname[IFNAMSIZ] = { '\0' };
        if (if_indextoname(ifi->ifi_index, ifname) == nullptr) {
            nd_dprintf("%s: %s: if_indextoname(%d): %s\n",
                TAG, __PRETTY_FUNCTION__, ifi->ifi_index, strerror(errno));
            return -1;
        }

        nd_dprintf("%s: RTM_NEWLINK: %s\n", TAG, ifname);

        if (ndi.addr_lookup.AddLinkName(ifi->ifi_index, ifname)) rc = 1;

        AttrsLink attrs = { 0 };

        if (mnl_attr_parse(nlh, sizeof(*ifi),
            ndNetlinkRoute_attr_parse_ifla, attrs.data()) != MNL_CB_OK)
            return -1;

        if (attrs[IFLA_WIRELESS] != nullptr) {
            string essid;
            if (nd_get_essid(ifname, essid)) {
                bool added = ndi.addr_lookup.AddUpdateLinkESSID(
                    ifi->ifi_index, essid);
                nd_dprintf("%s: %s: ESSID %s: %s\n",
                    TAG, ifname, (added) ? "added" : "updated", essid.c_str());
            }
        }

        _ND_NLRT_CHECK_ATTR(TAG, attrs[IFLA_ADDRESS], -1);

        ndAddr addr;
        ndAddr::Create(addr,
            static_cast<const uint8_t *>(
                mnl_attr_get_payload(attrs[IFLA_ADDRESS])), ETH_ALEN
        );

#ifdef _ND_DEBUG_LOG
        nd_dprintf("%s: adding link: %u: %s\n", TAG,
            ifi->ifi_index,
            addr.GetString().c_str()
        );
#endif
        if (ndi.addr_lookup.AddLinkAddress(ifi->ifi_index, addr)) rc = 1;
    }
    else {
#ifdef _ND_DEBUG_LOG
        nd_dprintf("%s: removing link: %d\n", TAG,
            ifi->ifi_index
        );
#endif
        if (ndi.addr_lookup.RemoveLinkAddress(ifi->ifi_index)) rc = 1;
        if (ndi.addr_lookup.RemoveLinkName(ifi->ifi_index)) rc = 1;
    }

    return rc;
}

int ndNetlinkRouteThread::AddRemoveNeighbor(const struct nlmsghdr *nlh, bool add) {
    const struct ndmsg *ndm = static_cast<const struct ndmsg *>(
        mnl_nlmsg_get_payload(nlh)
    );

    AttrsNeigh attrs = { 0 };

    if (mnl_attr_parse(nlh, sizeof(*ndm),
        ndNetlinkRoute_attr_parse_nda, attrs.data()) != MNL_CB_OK)
        return -1;

    if (add)
        _ND_NLRT_CHECK_ATTR(TAG, attrs[NDA_DST], -1);
    _ND_NLRT_CHECK_ATTR(TAG, attrs[NDA_LLADDR], -1);

    ndAddr addr;

    if (add) {
        switch(ndm->ndm_family) {
        case AF_INET:
            ndAddr::Create(addr,
                static_cast<struct in_addr *>(
                    mnl_attr_get_payload(attrs[NDA_DST])
                )
            );
            break;
        case AF_INET6:
            ndAddr::Create(addr,
                static_cast<struct in6_addr *>(
                    mnl_attr_get_payload(attrs[NDA_DST])
                )
            );
            break;

        default:
            nd_dprintf(
                "%s: unsupported address family: %d\n",
                TAG, ndm->ndm_family
            );
            return 0;
        }

        if (! addr.IsValid()) {
            nd_dprintf("%s: %s: invalid IP address.\n",
                TAG, __PRETTY_FUNCTION__);
            return -1;
        }
    }

    ndAddr lladdr;
    ndAddr::Create(lladdr,
        static_cast<const uint8_t *>(
            mnl_attr_get_payload(attrs[NDA_LLADDR])), ETH_ALEN
    );

    if (! lladdr.IsValid()) {
        nd_dprintf("%s: %s: invalid LL address.\n",
            TAG, __PRETTY_FUNCTION__);
        return -1;
    }

#ifdef _ND_DEBUG_LOG
    string state;
    switch(ndm->ndm_state) {
    case NUD_INCOMPLETE:
        state = "incomplete";
        break;
    case NUD_REACHABLE:
        state = "reachable";
        break;
    case NUD_STALE:
        state = "stale";
        break;
    case NUD_DELAY:
        state = "delay";
        break;
    case NUD_PROBE:
        state = "probe";
        break;
    case NUD_FAILED:
        state = "failed";
        break;
    case NUD_NOARP:
        state = "noarp";
        break;
    case NUD_PERMANENT:
        state = "permanent";
        break;
    default:
        state = "unknown: " + to_string(ndm->ndm_state);
        break;
    }
#endif

    bool rc = false;
    ndInstance &ndi = ndInstance::GetInstance();

    if (add) {
#ifdef _ND_DEBUG_LOG
        nd_dprintf("%s: adding neighbor: %s: %s [%s]\n", TAG,
            lladdr.GetString().c_str(),
            addr.GetString().c_str(), state.c_str()
        );
#endif
        rc = ndi.addr_lookup.AddNeighbor(addr, lladdr);
    }
    else {
#ifdef _ND_DEBUG_LOG
        nd_dprintf("%s: removing neighbor: %s [%s]\n", TAG,
            lladdr.GetString().c_str(), state.c_str()
        );
#endif
        rc = ndi.addr_lookup.RemoveNeighbor(lladdr);
    }

    return rc;
}
