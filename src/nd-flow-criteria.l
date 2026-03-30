%option noinput nounput noyywrap 8bit nodefault
%option yylineno
%option reentrant bison-bridge bison-locations
%option header-file="nd-flow-criteria.h"

%{
// Netify Agent
// Copyright (C) 2015-2024 eGloo Incorporated <http://www.egloo.ca>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "nd-flow-parser.h"

#include "nd-flow-expr.hpp"
%}

%%
app_category_id         return FLOW_APPLICATION_CATEGORY_ID;
app_category            return FLOW_APPLICATION_CATEGORY;
app_id                  return FLOW_APPLICATION;
application_category_id return FLOW_APPLICATION_CATEGORY_ID;
application_category    return FLOW_APPLICATION_CATEGORY;
application_id          return FLOW_APPLICATION;
application             return FLOW_APPLICATION;
app                     return FLOW_APPLICATION;
category_id             return FLOW_CATEGORY_ID;
category                return FLOW_CATEGORY;
cat_id                  return FLOW_CATEGORY_ID;
cat                     return FLOW_CATEGORY;
conntrack_id            return FLOW_CONNTRACK_ID;
conntrack_mark          return FLOW_CONNTRACK_MARK;
ct_id                   return FLOW_CONNTRACK_ID;
ct_mark                 return FLOW_CONNTRACK_MARK;
ct_reply_dst_ip         return FLOW_CONNTRACK_REPLY_DST_IP;
ct_reply_src_ip         return FLOW_CONNTRACK_REPLY_SRC_IP;
detected_hostname       return FLOW_DETECTED_HOSTNAME;
detection_complete      return FLOW_DETECTION_COMPLETE;
detection_guessed       return FLOW_DETECTION_GUESSED;
detection_init          return FLOW_DETECTION_INIT;
detection_updated       return FLOW_DETECTION_UPDATED;
dhc_hit                 return FLOW_DHC_HIT;
dns_hostname            return FLOW_DNS_HOSTNAME;
domain_category_id      return FLOW_DOMAIN_CATEGORY_ID;
domain_category         return FLOW_DOMAIN_CATEGORY;
dom_category_id         return FLOW_DOMAIN_CATEGORY_ID;
dom_category            return FLOW_DOMAIN_CATEGORY;
dst_ip                  return FLOW_DST_IP;
dst_mac                 return FLOW_DST_MAC;
dst_net_category_id     return FLOW_DST_NETWORK_CATEGORY_ID;
dst_net_category        return FLOW_DST_NETWORK_CATEGORY;
dst_network_category_id return FLOW_DST_NETWORK_CATEGORY_ID;
dst_network_category    return FLOW_DST_NETWORK_CATEGORY;
dst_port                return FLOW_DST_PORT;
expired                 return FLOW_EXPIRED;
expiring                return FLOW_EXPIRING;
fhc_hit                 return FLOW_FHC_HIT;
iface_nfq_dst           return FLOW_IFACE_NFQ_DST;
iface_nfq_src           return FLOW_IFACE_NFQ_SRC;
iface                   return FLOW_IFACE;
intel_[a-zA-Z0-9][a-z0-9_]+ strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return FLOW_INTEL;
ip_dscp                 return FLOW_IP_DSCP;
ip_nat                  return FLOW_IP_NAT;
ip_protocol             return FLOW_IP_PROTO;
ip                      return FLOW_ANY_IP;
ip_version              return FLOW_IP_VERSION;
local_ip                return FLOW_LOCAL_IP;
local_mac               return FLOW_LOCAL_MAC;
local_net_category_id   return FLOW_LOCAL_NETWORK_CATEGORY_ID;
local_net_category      return FLOW_LOCAL_NETWORK_CATEGORY;
local_network_category_id  return FLOW_LOCAL_NETWORK_CATEGORY_ID;
local_network_category  return FLOW_LOCAL_NETWORK_CATEGORY;
local_port              return FLOW_LOCAL_PORT;
mac                     return FLOW_ANY_MAC;
ndpi_risk_client        return FLOW_NDPI_RISK_SCORE_CLIENT;
ndpi_risk_score         return FLOW_NDPI_RISK_SCORE;
ndpi_risk_server        return FLOW_NDPI_RISK_SCORE_SERVER;
net_category_id         return FLOW_NETWORK_CATEGORY_ID;
net_category            return FLOW_NETWORK_CATEGORY;
network_category_id     return FLOW_NETWORK_CATEGORY_ID;
network_category        return FLOW_NETWORK_CATEGORY;
origin_local            yylval->us_number = _NDFP_ORIGIN_LOCAL; return FLOW_ORIGIN_LOCAL;
origin_other            yylval->us_number = _NDFP_ORIGIN_OTHER; return FLOW_ORIGIN_OTHER;
origin                  return FLOW_ORIGIN;
origin_unknown          yylval->us_number = _NDFP_ORIGIN_UNKNOWN; return FLOW_ORIGIN_UNKNOWN;
other_broadcast         yylval->us_number = _NDFP_OTHER_BROADCAST; return FLOW_OTHER_BROADCAST;
other_error             yylval->us_number = _NDFP_OTHER_ERROR; return FLOW_OTHER_ERROR;
other_ip                return FLOW_OTHER_IP;
other_local             yylval->us_number = _NDFP_OTHER_LOCAL; return FLOW_OTHER_LOCAL;
other_mac               return FLOW_OTHER_MAC;
other_multicast         yylval->us_number = _NDFP_OTHER_MULTICAST; return FLOW_OTHER_MULTICAST;
other_net_category_id   return FLOW_OTHER_NETWORK_CATEGORY_ID;
other_net_category      return FLOW_OTHER_NETWORK_CATEGORY;
other_network_category_id  return FLOW_OTHER_NETWORK_CATEGORY_ID;
other_network_category  return FLOW_OTHER_NETWORK_CATEGORY;
other_port              return FLOW_OTHER_PORT;
other_remote            yylval->us_number = _NDFP_OTHER_REMOTE; return FLOW_OTHER_REMOTE;
other_type              return FLOW_OTHER_TYPE;
other_unknown           yylval->us_number = _NDFP_OTHER_UNKNOWN; return FLOW_OTHER_UNKNOWN;
other_unsupported       yylval->us_number = _NDFP_OTHER_UNSUPPORTED; return FLOW_OTHER_UNSUPPORTED;
port                    return FLOW_ANY_PORT;
proto_category_id       return FLOW_PROTOCOL_CATEGORY_ID;
proto_category          return FLOW_PROTOCOL_CATEGORY;
protocol_category_id    return FLOW_PROTOCOL_CATEGORY_ID;
protocol_category       return FLOW_PROTOCOL_CATEGORY;
protocol_id             return FLOW_PROTOCOL;
protocol                return FLOW_PROTOCOL;
proto_id                return FLOW_PROTOCOL;
proto                   return FLOW_PROTOCOL;
risks                   return FLOW_RISKS;
soft_dissector          return FLOW_SOFT_DISSECTOR;
src_ip                  return FLOW_SRC_IP;
src_mac                 return FLOW_SRC_MAC;
src_net_category_id     return FLOW_SRC_NETWORK_CATEGORY_ID;
src_net_category        return FLOW_SRC_NETWORK_CATEGORY;
src_network_category_id return FLOW_SRC_NETWORK_CATEGORY_ID;
src_network_category    return FLOW_SRC_NETWORK_CATEGORY;
src_port                return FLOW_SRC_PORT;
ssl_cipher              return FLOW_TLS_CIPHER;
ssl_version             return FLOW_TLS_VERSION;
tag_category_id         return FLOW_TAG_CATEGORY_ID;
tag_category            return FLOW_TAG_CATEGORY;
tag                     return FLOW_TAG;
tls_cipher              return FLOW_TLS_CIPHER;
tls_ech                 return FLOW_TLS_ECH;
tls_ja4                 return FLOW_TLS_JA4;
tls_version             return FLOW_TLS_VERSION;
tunnel_gtp              yylval->us_number = _NDFP_TUNNEL_GTP; return FLOW_TUNNEL_GTP;
tunnel_none             yylval->us_number = _NDFP_TUNNEL_NONE; return FLOW_TUNNEL_NONE;
tunnel_type             return FLOW_TUNNEL_TYPE;
vlan_id                 return FLOW_VLAN_ID;
[t|T][r|R][u|U][e|E]    yylval->bool_number=true; return VALUE_TRUE;
[f|F][a|A][l|L][s|S][e|E] yylval->bool_number=false; return VALUE_FALSE;
[a-fA-F0-9]{2}[:]{1}[a-fA-F0-9]{2}[:]{1}[a-fA-F0-9]{2}[:]{1}[a-fA-F0-9]{2}[:]{1}[a-fA-F0-9]{2}[:]{1}[a-fA-F0-9]{2} strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_MAC;
[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\/[0-9]+ strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_IPV4_CIDR;
[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3} strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_IPV4;
::1                     strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_IPV6;
[a-fA-F0-9]{1,4}[:]{1,2}[a-fA-F0-9:]*\/[0-9]+ strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_IPV6_CIDR;
[a-fA-F0-9]{1,4}[:]{1,2}[a-fA-F0-9:]* strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_IPV6;
\'rx:[a-zA-Z0-9:\^\$\/\._+()\[\]\*\{ -]+\'  strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_REGEX;
\"rx:[a-zA-Z0-9:\^\$\/\._+()\[\]\*\{ -]+\"  strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_REGEX;
\'[a-zA-Z0-9][a-zA-Z0-9:\/\._+() -]+\' strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_NAME;
\"[a-zA-Z0-9][a-zA-Z0-9:\/\._+() -]+\" strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_NAME;
@[a-zA-Z0-9][a-zA-Z0-9:\/\._+-]+ strncpy(yylval->buffer, yytext, _NDFP_MAX_BUFLEN); return VALUE_ADDR_TAG;
0x[a-fA-F0-9-]+         yylval->ul_number=(unsigned long)strtold(yytext, NULL); return VALUE_UNSIGNED;
[-]+[ ]*[0-9]+          yylval->ul_number=(unsigned long)strtold(yytext, NULL); return VALUE_SIGNED;
[+]*[0-9]+              yylval->ul_number=(unsigned long)strtold(yytext, NULL); return VALUE_UNSIGNED;
[0-9-]+\.[0-9]+         yylval->fl_number=strtof(yytext, NULL); return VALUE_FLOAT;
==                      return CMP_EQUAL;
!=                      return CMP_NOTEQUAL;
\>=                     return CMP_GTHANEQUAL;
\<=                     return CMP_LTHANEQUAL;
&&                      return BOOL_AND;
[a|A][n|N][d|D]         return BOOL_AND;
\|\|                    return BOOL_OR;
[o|O][r|R]              return BOOL_OR;
[ \t\n]+                /* Skip whitespace */
.                       return (int)yytext[0];
%%
