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

#include <string>
#include <stack>
#include <thread>
//#include <unordered_map>

#include <nlohmann/json.hpp>

#include "nd-except.hpp"
#include "nd-socket.hpp"
#include "nd-util.hpp"

class ndServer
{
public:
    enum class CommandCode : uint16_t {
        NONE = 0,
        STATUS = 100,
    };

    enum class ResponseCode : uint16_t {
        OK = 100,
        HELLO = 101,
        ERROR = 500,
        PARSE_ERROR = 501
    };

    static ndServer& Create(void);
    static void Destroy(void);

    ndServer(const ndServer &) = delete;
    ndServer &operator=(const ndServer &) = delete;

    static inline ndServer &GetInstance() {
        if (instance == nullptr) {
            throw ndException("%s: instance not found",
              __PRETTY_FUNCTION__);
        }
        return *instance;
    }

    static inline bool IsValid(void) {
        return (instance != nullptr && skt != nullptr &&
            skt->GetDescriptor() != -1);
    }

    void Reload(const std::string &path_socket);

    static inline int GetDescriptor(void) {
        return (skt != nullptr) ? skt->GetDescriptor() : -1; }

    void CreateClientSession(void);
    void DestroyClientSessions(void);

protected:
    static ndServer *instance;

    std::string path_socket;
    static ndSocketServer *skt;
/*
    using Clients = std::unordered_map<int, std::thread>;
    Clients clients;
*/
    void Close(void);

private:
    ndServer() { }
    virtual ~ndServer();
};

class ndServerSaxConsumer : public nlohmann::json::json_sax_t
{
public:
    std::string key_current;
    std::stack<std::string> key_stack;

    ndServer::CommandCode command = ndServer::CommandCode::NONE;

    bool null() override {
        return true;
    }

    bool boolean(bool val) override {
        return true;
    }

    bool number_integer(nlohmann::json::number_integer_t val) override;

    bool number_unsigned(nlohmann::json::number_unsigned_t val) override;

    bool number_float(
      nlohmann::json::number_float_t val, const std::string& s) override {
        return true;
    }

    bool string(std::string& val) override;

    bool start_object(std::size_t elements) override;

    bool end_object() override;

    bool start_array(std::size_t elements) override {
        return true;
    }

    bool end_array() override {
        return true;
    }

    bool key(std::string& val) override;

    bool binary(nlohmann::json::binary_t& val) override {
        return true;
    }

    bool parse_error(
      std::size_t position, const std::string& last_token,
      const nlohmann::json::exception& ex) override;
};
