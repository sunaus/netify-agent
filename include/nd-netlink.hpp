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

#include <array>
#include <cstdint>

#include <libmnl/libmnl.h>
#include <linux/netfilter/nfnetlink.h>

#include "nd-addr.hpp"

#define _ND_NLEX_CHECK_ATTR(tag, attr) \
    if (attr == nullptr) { \
        throw ndException("%s: required attribute not found: " #attr, tag); \
    }
#define _ND_NLRT_CHECK_ATTR(tag, attr, rtn) \
    if (attr == nullptr) { \
        nd_dprintf("%s: required attribute not found: " #attr "\n", tag); \
        return rtn; \
    }

#ifndef MNL_SOCKET_DUMP_SIZE
constexpr size_t ND_NETLINK_BUFFER_SIZE = 32768; // 32KB
#else
constexpr size_t ND_NETLINK_BUFFER_SIZE = MNL_SOCKET_DUMP_SIZE;
#endif

class ndNetlink
{
public:
    ndNetlink(int bus);
    ndNetlink(int bus, unsigned int groups);

    virtual ~ndNetlink() {
        if (socket_nl != nullptr) {
            mnl_socket_close(socket_nl);
            socket_nl = nullptr;
        }
    }

    inline int GetDescriptor(void) {
        if (socket_nl != nullptr)
            return mnl_socket_get_fd(socket_nl);
        return -1;
    }

    virtual int ProcessMessage(const struct nlmsghdr *nlh) = 0;

    bool Recv(void);

protected:
    bool dump = { false };
    struct mnl_socket *socket_nl = { nullptr };
    std::array<uint8_t, ND_NETLINK_BUFFER_SIZE> socket_buffer = { 0 };
    unsigned int port_id = { 0 }, seq_id = { 0 };

    bool Send(nlmsghdr *nlh);
};
