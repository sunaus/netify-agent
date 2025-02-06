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

#include <atomic>

#include <linux/rtnetlink.h>

#include "nd-netlink.hpp"
#include "nd-thread.hpp"

class ndNetlinkRouteThread : public ndThread, public ndNetlink
{
public:
    ndNetlinkRouteThread(void);
    virtual ~ndNetlinkRouteThread();

    virtual void *Entry();

    virtual int ProcessMessage(const struct nlmsghdr *nlh);

    void Dump(void);

    typedef struct {
        struct {
            std::atomic<uint32_t> added;
            std::atomic<uint32_t> removed;
            std::atomic<uint32_t> error;
        } addr;
        struct {
            std::atomic<uint32_t> added;
            std::atomic<uint32_t> removed;
            std::atomic<uint32_t> error;
        } net;
        struct {
            std::atomic<uint32_t> added;
            std::atomic<uint32_t> removed;
            std::atomic<uint32_t> error;
        } link;
        struct {
            std::atomic<uint32_t> added;
            std::atomic<uint32_t> removed;
            std::atomic<uint32_t> error;
        } neigh;
    } Stats;

    void GetStats(Stats &stats);
    void PrintStats(const Stats &stats) const;

    typedef std::array<struct nlattr *, IFA_MAX + 1> AttributesAddr;
    typedef std::array<struct nlattr *, RTA_MAX + 1> AttributesRoute;
    typedef std::array<struct nlattr *, IFLA_MAX + 1> AttributesLink;
    typedef std::array<struct nlattr *, NDA_MAX + 1> AttributesNeigh;

protected:
    int AddRemoveAddress(const struct nlmsghdr *nlh, bool add = true);
    int AddRemoveNetwork(const struct nlmsghdr *nlh, bool add = true);
    int AddRemoveLink(const struct nlmsghdr *nlh, bool add = true);
    int AddRemoveNeighbor(const struct nlmsghdr *nlh, bool add = true);

    Stats stats;
};
