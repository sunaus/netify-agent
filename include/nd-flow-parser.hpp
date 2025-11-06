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

#include <cstdint>
#include <memory>
#include <string>
#include <regex>
#include <unordered_map>

#include "nd-flow-parser.h"

#include "nd-flow.hpp"
#include "nd-instance.hpp"

#define _NDFP_flow \
    ((ndFlowParser *)yyget_extra(scanner))->flow
#define _NDFP_local_mac \
    ((ndFlowParser *)yyget_extra(scanner))->local_mac
#define _NDFP_other_mac \
    ((ndFlowParser *)yyget_extra(scanner))->other_mac
#define _NDFP_src_mac \
    ((ndFlowParser *)yyget_extra(scanner))->src_mac
#define _NDFP_dst_mac \
    ((ndFlowParser *)yyget_extra(scanner))->dst_mac
#define _NDFP_local_ip \
    ((ndFlowParser *)yyget_extra(scanner))->local_ip
#define _NDFP_other_ip \
    ((ndFlowParser *)yyget_extra(scanner))->other_ip
#define _NDFP_src_ip \
    ((ndFlowParser *)yyget_extra(scanner))->src_ip
#define _NDFP_dst_ip \
    ((ndFlowParser *)yyget_extra(scanner))->dst_ip
#define _NDFP_local_port \
    ((ndFlowParser *)yyget_extra(scanner))->local_port
#define _NDFP_other_port \
    ((ndFlowParser *)yyget_extra(scanner))->other_port
#define _NDFP_src_port \
    ((ndFlowParser *)yyget_extra(scanner))->src_port
#define _NDFP_dst_port \
    ((ndFlowParser *)yyget_extra(scanner))->dst_port
#define _NDFP_local_net_cat \
    ((ndFlowParser *)yyget_extra(scanner))->local_net_cat
#define _NDFP_other_net_cat \
    ((ndFlowParser *)yyget_extra(scanner))->other_net_cat
#define _NDFP_src_net_cat \
    ((ndFlowParser *)yyget_extra(scanner))->src_net_cat
#define _NDFP_dst_net_cat \
    ((ndFlowParser *)yyget_extra(scanner))->dst_net_cat
#define _NDFP_origin \
    ((ndFlowParser *)yyget_extra(scanner))->origin
#define _NDFP_params_intel \
    ((ndFlowParser *)yyget_extra(scanner))->params_intel
#define _NDFP_result \
    ((ndFlowParser *)yyget_extra(scanner))->expr_result
#define _NDFP_categories \
    ((ndFlowParser *)yyget_extra(scanner))->ndi.categories
#define _NDFP_regex_search(rx, search) \
    ((ndFlowParser *)yyget_extra(scanner))->RegExSearch(rx, search)

class ndFlowParser : public ndInstanceClient
{
public:
    ndFlowParser();
    virtual ~ndFlowParser();

    typedef std::unordered_map<std::string, nlohmann::json> Params;

    bool Parse(
      ndFlow::Ptr const &flow, const Params &params, const std::string &expr);
    inline bool Parse(ndFlow::Ptr const &flow, const std::string &expr) {
        Params params;
        return Parse(flow, params, expr);
    }

    bool RegExSearch(std::string &expr, const std::string &search);

    ndFlow::Ptr flow = { nullptr };
    const char *local_mac = { nullptr };
    const char *other_mac = { nullptr };
    const char *src_mac = { nullptr };
    const char *dst_mac = { nullptr };
    const ndAddr *local_ip = { nullptr };
    const ndAddr *other_ip = { nullptr };
    const ndAddr *src_ip = { nullptr };
    const ndAddr *dst_ip = { nullptr };
    uint16_t local_port = { 0 };
    uint16_t other_port = { 0 };
    uint16_t src_port = { 0 };
    uint16_t dst_port = { 0 };
    nd_cat_id_t local_net_cat = { ND_CAT_UNKNOWN };
    nd_cat_id_t other_net_cat = { ND_CAT_UNKNOWN };
    nd_cat_id_t src_net_cat = { ND_CAT_UNKNOWN };
    nd_cat_id_t dst_net_cat = { ND_CAT_UNKNOWN };
    uint16_t origin = { 0 };
    const nlohmann::json *params_intel = { nullptr };

    bool expr_result = { false };

protected:
    void *scanner = { nullptr };

    typedef std::unordered_map<std::string, std::unique_ptr<std::regex>> rx_cache_map;
    rx_cache_map rx_cache;
};
