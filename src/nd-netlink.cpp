#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <sys/select.h>

#include <iostream>

#include "nd-except.hpp"
#include "nd-netlink.hpp"

using namespace std;

static int ndNetlink_ProcessMessageCallback(
    const struct nlmsghdr *nlh, void *data) {
    ndNetlink *nl = reinterpret_cast<ndNetlink *>(data);
    return nl->ProcessMessage(nlh);
}

ndNetlink::ndNetlink(int bus) : dump(true) {
    socket_nl = mnl_socket_open(bus);
    if (socket_nl == nullptr)
        throw ndException("mnl_socket_open: %s", strerror(errno));

    if (mnl_socket_bind(socket_nl, 0, MNL_SOCKET_AUTOPID) < 0)
        throw ndException("mnl_socket_bind: %s", strerror(errno));

    port_id = mnl_socket_get_portid(socket_nl);
}

ndNetlink::ndNetlink(int bus, unsigned int groups) {
    socket_nl = mnl_socket_open2(bus, SOCK_NONBLOCK);
    if (socket_nl == nullptr)
        throw ndException("mnl_socket_open: %s", strerror(errno));

    if (mnl_socket_bind(socket_nl, groups, MNL_SOCKET_AUTOPID) < 0)
        throw ndException("mnl_socket_bind: %s", strerror(errno));
}

bool ndNetlink::Recv(void)
{
    int rc;
    ssize_t bytes;

    do {
        bytes = mnl_socket_recvfrom(
            socket_nl, socket_buffer.data(), socket_buffer.size());

        if (bytes < 0)
            throw ndException("mnl_socket_recvfrom: %s", strerror(errno));
#if 0
        nd_dprintf("%s: read %ld byte(s), seq_id: %u, port_id: %u\n",
            __PRETTY_FUNCTION__, bytes,
            dump ? seq_id : 0, dump ? port_id : 0);
#endif
        rc = mnl_cb_run(
            socket_buffer.data(), (size_t)bytes,
            dump ? seq_id : 0, dump ? port_id : 0,
            ndNetlink_ProcessMessageCallback,
            static_cast<void *>(this));

    } while (dump && rc == MNL_CB_OK);

    return (rc == MNL_CB_OK);
}

bool ndNetlink::Send(nlmsghdr *nlh)
{
    if (dump) {
        nlh->nlmsg_seq = seq_id = time(nullptr);
        nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    }

    if (mnl_socket_sendto(socket_nl, nlh, nlh->nlmsg_len) < 0)
        throw ndException("mnl_socket_sendto: %s", strerror(errno));

    return true;
}
