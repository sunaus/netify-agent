// Netify Agent
// Copyright (C) 2025 eGloo Incorporated
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

#include <array>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <thread>

#include <pthread.h>
#include <sys/select.h>

#include "nd-config.hpp"
#include "nd-instance.hpp"
#include "nd-server.hpp"
#include "nd-util.hpp"

using namespace std;
using json = nlohmann::json;
using json_ordered = nlohmann::ordered_json;

ndServer *ndServer::instance = nullptr;
ndSocketServer *ndServer::skt = nullptr;

static atomic<bool> ndServer_ClientTerminate = false;

static void ndServer_ClientResponse(FILE *hf, const json_ordered &jpayload) {
    string payload;
    nd_json_to_string(jpayload, payload, ndGC_DEBUG);

    fprintf(hf, "%s\n", payload.c_str());
    fflush(hf);
}

static void ndServer_ClientResponse(FILE *hf,
    ndServer::ResponseCode code, const string &message) {
    json_ordered j;

    j["code"] = code;
    j["message"] = message;

    ndServer_ClientResponse(hf, j);
}

static void ndServer_ClientResponse(FILE *hf,
    ndServer::ResponseCode code, const string &message, const json &jdata) {
    json_ordered j;

    j["code"] = code;
    j["message"] = message;
    j["data"] = jdata;

    ndServer_ClientResponse(hf, j);
}

static bool ndServer_ClientRequest(FILE *hf) {
    bool result = false;

    try {
        ndServerSaxConsumer ssc;

        result = json::sax_parse(
          hf, &ssc, json::input_format_t::json,
            false, // strict?
            true // ignore whitespace?
        );

        if (feof(hf) || ferror(hf)) return false;

        if (result) {
            json jdata;

            switch (ssc.command) {
            case ndServer::CommandCode::NONE:
                ndServer_ClientResponse(hf,
                  ndServer::ResponseCode::ERROR, "No command specified.");
                break;

            case ndServer::CommandCode::STATUS:
                ndInstance::GetInstance().GetStatus().Encode(jdata);
                ndServer_ClientResponse(hf,
                  ndServer::ResponseCode::OK, "Ok.", jdata);
                break;

            default:
                ndServer_ClientResponse(hf,
                  ndServer::ResponseCode::ERROR, "Unimplemented command.");
                break;
            }
        }

    } catch (exception &e) {
        nd_dprintf("ndServer: %s\n", e.what());
        ndServer_ClientResponse(hf,
          ndServer::ResponseCode::PARSE_ERROR, e.what());
    }

    return result;
}

static void ndServer_ClientEntry(ndSocket *client) {
    int rc;
    fd_set fds_read;
    int client_fd = client->GetDescriptor();

    FILE *hf = fdopen(client_fd, "a+");
    ndServer_ClientResponse(hf, ndServer::ResponseCode::HELLO,
        nd_get_version_and_features().c_str());

    while (! ndServer_ClientTerminate.load()) {
        FD_ZERO(&fds_read);
        FD_SET(client_fd, &fds_read);

        struct timeval tv = { 1, 0 };

        rc = select(client_fd + 1, &fds_read, nullptr, nullptr, &tv);

        if (rc == -1) {
            throw ndExceptionSystemError(
              __PRETTY_FUNCTION__, "select");
        }

        if (rc > 0 && FD_ISSET(client_fd, &fds_read) &&
          ! ndServer_ClientRequest(hf)) break;
    }

    delete client;
    nd_dprintf("ndServer: client exit.\n");
}

ndServer &ndServer::Create(void) {
    if (instance != nullptr) {
        throw ndException("%s: %s: %s", __PRETTY_FUNCTION__,
          "ndServer", strerror(EEXIST));
    }

    instance = new ndServer;

    return *instance;
}

void ndServer::Destroy(void) {
    if (instance == nullptr) {
        throw ndException("%s: %s: %s", __PRETTY_FUNCTION__,
          "ndServer", strerror(EINVAL));
    }

    delete instance;
}

void ndServer::Reload(const string &path_socket) {
    if (path_socket.empty() || path_socket != this->path_socket)
        Close();

    if (! path_socket.empty()) {
        ndSocketServerLocal *server = new ndSocketServerLocal(path_socket);
        server->SetBlockingMode(false);
        skt = server;

        this->path_socket = path_socket;
    }
    else this->path_socket.clear();
}

void ndServer::Close() {
    if (skt != nullptr) {
        delete skt;
        skt = nullptr;
    }
}

void ndServer::CreateClientSession() {
    ndSocket *skt_client = nullptr;

    try {
        skt_client = skt->Accept();
    }
    catch (exception &e) {
        if (skt_client != nullptr) delete skt_client;
        throw;
    }

//    DestroyClientSessions();

//    clients.emplace(
//      skt_client->GetDescriptor(), thread(ndServer_ClientEntry, skt_client));
    thread(ndServer_ClientEntry, skt_client).detach();
}

void ndServer::DestroyClientSessions(void) {
/*
    for (auto i = clients.begin(); i != clients.end(); ) {
        void *result __attribute__((unused));
        if (pthread_tryjoin_np(i->second.native_handle(), &result) == 0) {
            i->second.join();
            i = clients.erase(i);
            continue;
        }

        i++;
    }
*/
}

ndServer::~ndServer() {
    Close();
    ndServer_ClientTerminate = true;
    //for (auto &i : clients) i.second.join();
}

bool ndServerSaxConsumer::number_integer(number_integer_t val) {
    if (! key_current.empty()) {
    }
    return true;
}

bool ndServerSaxConsumer::number_unsigned(number_unsigned_t val) {
    if (! key_current.empty()) {
    }
    return true;
}

bool ndServerSaxConsumer::string(string_t &val) {
    if (! key_current.empty()) {
        if (key_stack.size() == 1 && key_current == "command") {
            if (command != ndServer::CommandCode::NONE)
                throw ndException("parse_error: command already set");

            if (val == "status")
                command = ndServer::CommandCode::STATUS;
            else
                throw ndException("parse_error: invalid command");
        }
    }

    return true;
}

bool ndServerSaxConsumer::start_object(std::size_t elements) {
    key_stack.push((key_current.empty()) ? "root" : key_current);
    nd_dprintf("ndServer: object start: %s\n", key_stack.top().c_str());
    return true;
}

bool ndServerSaxConsumer::end_object() {
    nd_dprintf("ndServer: object end: %s\n", key_stack.top().c_str());
    key_stack.pop();
    return true;
}

bool ndServerSaxConsumer::key(string_t &val) {
    key_current = val;
    return true;
}

bool ndServerSaxConsumer::parse_error(std::size_t position,
  const std::string &last_token,
  const json::exception &ex) {
    throw ndException(
      "parse_error: position: %u, token: %s, "
      "reason: %s",
      position, last_token.c_str(), ex.what());
}

