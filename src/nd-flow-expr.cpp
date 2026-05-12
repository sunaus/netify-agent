/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 5 "nd-flow-expr.ypp"

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <mutex>

#include <radix/radix_tree.hpp>

#include "nd-except.hpp"
#include "nd-flow-parser.hpp"
#include "nd-flow-expr.hpp"

using namespace std;

using json = nlohmann::json;

extern "C" {
#include "nd-flow-criteria.h"

    void yyerror(YYLTYPE *yyllocp, yyscan_t scanner, const char *message);
}

void yyerror(YYLTYPE *yyllocp, yyscan_t scanner, const char *message) {
    throw ndException("[%d:%d] %s",
      yyllocp->first_line, yyllocp->first_column, message);
}

static bool is_addr_equal(const ndAddr *flow_addr, const string &compr_addr) {
  typedef radix_tree<ndRadixNetworkEntry<_ND_ADDR_BITSv4>, bool> nd_rn4_addr;
  typedef radix_tree<ndRadixNetworkEntry<_ND_ADDR_BITSv6>, bool> nd_rn6_addr;

  ndAddr addr(compr_addr);
  if (! addr.IsValid() || ! addr.IsIP()) return false;
  if (! (flow_addr->IsIPv4() == addr.IsIPv4())) return false;
  if (! (flow_addr->IsIPv6() == addr.IsIPv6())) return false;

  addr.SetCompareFlags(ndAddr::CompareFlags::ADDR);

  if (! addr.IsNetwork())
    return (addr == *flow_addr);

  try {
    if (addr.IsIPv4()) {
      nd_rn4_addr rn;
      ndRadixNetworkEntry<_ND_ADDR_BITSv4> entry;
      if (! ndRadixNetworkEntry<_ND_ADDR_BITSv4>::Create(entry, addr))
        return false;

      rn[entry] = true;

      nd_rn4_addr::iterator it;
      if (ndRadixNetworkEntry<_ND_ADDR_BITSv4>::CreateQuery(entry, *flow_addr)) {
        if ((it = rn.longest_match(entry)) != rn.end())
          return true;
      }
    }
    else {
      nd_rn6_addr rn;
      ndRadixNetworkEntry<_ND_ADDR_BITSv6> entry;
      if (! ndRadixNetworkEntry<_ND_ADDR_BITSv6>::Create(entry, addr))
        return false;

      rn[entry] = true;

      nd_rn6_addr::iterator it;
      if (ndRadixNetworkEntry<_ND_ADDR_BITSv6>::CreateQuery(entry, *flow_addr)) {
        if ((it = rn.longest_match(entry)) != rn.end())
          return true;
      }
    }
  }
  catch (runtime_error &e) {
      nd_dprintf("Error adding network: %s: %s\n",
        compr_addr.c_str(), e.what());
  }

  return false;
}

static bool flow_intel(string &key, const json &jintel, json &jvalue) {
    string prefix = "intel_";
    size_t p = key.find(prefix);
    if (p == string::npos) return false;
    key.erase(p, prefix.size());

    prefix = "criteria_";
    p = key.find(prefix);
    if (p != string::npos) {
        key.erase(p, prefix.size());

        auto c = jintel.find("criteria");
        if (c == jintel.end()) return false;

        auto j = c->find(key);
        if (j == c->end()) return false;
        jvalue = j.value();
    }
    else {
        auto j = jintel.find(key);
        if (j == jintel.end()) return false;
        jvalue = j.value();
    }

    return true;
}


#line 193 "nd-flow-expr.cpp"




# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_ND_FLOW_EXPR_HPP_INCLUDED
# define YY_YY_ND_FLOW_EXPR_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 130 "nd-flow-expr.ypp"

typedef void* yyscan_t;

#line 235 "nd-flow-expr.cpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FLOW_ANY_IP = 258,             /* FLOW_ANY_IP  */
    FLOW_ANY_MAC = 259,            /* FLOW_ANY_MAC  */
    FLOW_ANY_PORT = 260,           /* FLOW_ANY_PORT  */
    FLOW_APPLICATION = 261,        /* FLOW_APPLICATION  */
    FLOW_APPLICATION_CATEGORY = 262, /* FLOW_APPLICATION_CATEGORY  */
    FLOW_APPLICATION_CATEGORY_ID = 263, /* FLOW_APPLICATION_CATEGORY_ID  */
    FLOW_CATEGORY = 264,           /* FLOW_CATEGORY  */
    FLOW_CATEGORY_ID = 265,        /* FLOW_CATEGORY_ID  */
    FLOW_CONNTRACK_ID = 266,       /* FLOW_CONNTRACK_ID  */
    FLOW_CONNTRACK_MARK = 267,     /* FLOW_CONNTRACK_MARK  */
    FLOW_CONNTRACK_REPLY_DST_IP = 268, /* FLOW_CONNTRACK_REPLY_DST_IP  */
    FLOW_CONNTRACK_REPLY_SRC_IP = 269, /* FLOW_CONNTRACK_REPLY_SRC_IP  */
    FLOW_DETECTED_HOSTNAME = 270,  /* FLOW_DETECTED_HOSTNAME  */
    FLOW_DETECTION_COMPLETE = 271, /* FLOW_DETECTION_COMPLETE  */
    FLOW_DETECTION_GUESSED = 272,  /* FLOW_DETECTION_GUESSED  */
    FLOW_DETECTION_INIT = 273,     /* FLOW_DETECTION_INIT  */
    FLOW_DETECTION_UPDATED = 274,  /* FLOW_DETECTION_UPDATED  */
    FLOW_DHC_HIT = 275,            /* FLOW_DHC_HIT  */
    FLOW_DNS_HOSTNAME = 276,       /* FLOW_DNS_HOSTNAME  */
    FLOW_DOMAIN_CATEGORY = 277,    /* FLOW_DOMAIN_CATEGORY  */
    FLOW_DOMAIN_CATEGORY_ID = 278, /* FLOW_DOMAIN_CATEGORY_ID  */
    FLOW_DST_IP = 279,             /* FLOW_DST_IP  */
    FLOW_DST_MAC = 280,            /* FLOW_DST_MAC  */
    FLOW_DST_NETWORK_CATEGORY = 281, /* FLOW_DST_NETWORK_CATEGORY  */
    FLOW_DST_NETWORK_CATEGORY_ID = 282, /* FLOW_DST_NETWORK_CATEGORY_ID  */
    FLOW_DST_PORT = 283,           /* FLOW_DST_PORT  */
    FLOW_EXPIRED = 284,            /* FLOW_EXPIRED  */
    FLOW_EXPIRING = 285,           /* FLOW_EXPIRING  */
    FLOW_FHC_HIT = 286,            /* FLOW_FHC_HIT  */
    FLOW_IFACE = 287,              /* FLOW_IFACE  */
    FLOW_IFACE_NFQ_DST = 288,      /* FLOW_IFACE_NFQ_DST  */
    FLOW_IFACE_NFQ_SRC = 289,      /* FLOW_IFACE_NFQ_SRC  */
    FLOW_IP_DSCP = 290,            /* FLOW_IP_DSCP  */
    FLOW_IP_NAT = 291,             /* FLOW_IP_NAT  */
    FLOW_IP_PROTO = 292,           /* FLOW_IP_PROTO  */
    FLOW_IP_VERSION = 293,         /* FLOW_IP_VERSION  */
    FLOW_LOCAL_IP = 294,           /* FLOW_LOCAL_IP  */
    FLOW_LOCAL_MAC = 295,          /* FLOW_LOCAL_MAC  */
    FLOW_LOCAL_NETWORK_CATEGORY = 296, /* FLOW_LOCAL_NETWORK_CATEGORY  */
    FLOW_LOCAL_NETWORK_CATEGORY_ID = 297, /* FLOW_LOCAL_NETWORK_CATEGORY_ID  */
    FLOW_LOCAL_PORT = 298,         /* FLOW_LOCAL_PORT  */
    FLOW_NDPI_RISK_SCORE = 299,    /* FLOW_NDPI_RISK_SCORE  */
    FLOW_NDPI_RISK_SCORE_CLIENT = 300, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
    FLOW_NDPI_RISK_SCORE_SERVER = 301, /* FLOW_NDPI_RISK_SCORE_SERVER  */
    FLOW_NETWORK_CATEGORY = 302,   /* FLOW_NETWORK_CATEGORY  */
    FLOW_NETWORK_CATEGORY_ID = 303, /* FLOW_NETWORK_CATEGORY_ID  */
    FLOW_ORIGIN = 304,             /* FLOW_ORIGIN  */
    FLOW_OTHER_IP = 305,           /* FLOW_OTHER_IP  */
    FLOW_OTHER_MAC = 306,          /* FLOW_OTHER_MAC  */
    FLOW_OTHER_NETWORK_CATEGORY = 307, /* FLOW_OTHER_NETWORK_CATEGORY  */
    FLOW_OTHER_NETWORK_CATEGORY_ID = 308, /* FLOW_OTHER_NETWORK_CATEGORY_ID  */
    FLOW_OTHER_PORT = 309,         /* FLOW_OTHER_PORT  */
    FLOW_OTHER_TYPE = 310,         /* FLOW_OTHER_TYPE  */
    FLOW_PROTOCOL = 311,           /* FLOW_PROTOCOL  */
    FLOW_PROTOCOL_CATEGORY = 312,  /* FLOW_PROTOCOL_CATEGORY  */
    FLOW_PROTOCOL_CATEGORY_ID = 313, /* FLOW_PROTOCOL_CATEGORY_ID  */
    FLOW_RISKS = 314,              /* FLOW_RISKS  */
    FLOW_SOFT_DISSECTOR = 315,     /* FLOW_SOFT_DISSECTOR  */
    FLOW_SRC_IP = 316,             /* FLOW_SRC_IP  */
    FLOW_SRC_MAC = 317,            /* FLOW_SRC_MAC  */
    FLOW_SRC_NETWORK_CATEGORY = 318, /* FLOW_SRC_NETWORK_CATEGORY  */
    FLOW_SRC_NETWORK_CATEGORY_ID = 319, /* FLOW_SRC_NETWORK_CATEGORY_ID  */
    FLOW_SRC_PORT = 320,           /* FLOW_SRC_PORT  */
    FLOW_TAG = 321,                /* FLOW_TAG  */
    FLOW_TAG_CATEGORY = 322,       /* FLOW_TAG_CATEGORY  */
    FLOW_TAG_CATEGORY_ID = 323,    /* FLOW_TAG_CATEGORY_ID  */
    FLOW_TLS_CIPHER = 324,         /* FLOW_TLS_CIPHER  */
    FLOW_TLS_ECH = 325,            /* FLOW_TLS_ECH  */
    FLOW_TLS_JA4 = 326,            /* FLOW_TLS_JA4  */
    FLOW_TLS_VERSION = 327,        /* FLOW_TLS_VERSION  */
    FLOW_TUNNEL_TYPE = 328,        /* FLOW_TUNNEL_TYPE  */
    FLOW_VLAN_ID = 329,            /* FLOW_VLAN_ID  */
    FLOW_VLAN = 330,               /* FLOW_VLAN  */
    FLOW_LOCAL_IF_META_SSID = 331, /* FLOW_LOCAL_IF_META_SSID  */
    FLOW_LOCAL_IF_META_PVID = 332, /* FLOW_LOCAL_IF_META_PVID  */
    FLOW_OTHER_IF_META_PVID = 333, /* FLOW_OTHER_IF_META_PVID  */
    FLOW_APP_IP_OVERRIDE = 334,    /* FLOW_APP_IP_OVERRIDE  */
    FLOW_APP_PROTO_TWINS = 335,    /* FLOW_APP_PROTO_TWINS  */
    FLOW_DHCP_CLASS_IDENT = 336,   /* FLOW_DHCP_CLASS_IDENT  */
    FLOW_DHCP_FINGERPRINT = 337,   /* FLOW_DHCP_FINGERPRINT  */
    FLOW_DST_BYTES = 338,          /* FLOW_DST_BYTES  */
    FLOW_DST_PACKETS = 339,        /* FLOW_DST_PACKETS  */
    FLOW_HTTP_URL = 340,           /* FLOW_HTTP_URL  */
    FLOW_HTTP_USER_AGENT = 341,    /* FLOW_HTTP_USER_AGENT  */
    FLOW_LOCAL_BYTES = 342,        /* FLOW_LOCAL_BYTES  */
    FLOW_LOCAL_PACKETS = 343,      /* FLOW_LOCAL_PACKETS  */
    FLOW_MDNS_DOMAIN_NAME = 344,   /* FLOW_MDNS_DOMAIN_NAME  */
    FLOW_NFQ_DST_IFINDEX = 345,    /* FLOW_NFQ_DST_IFINDEX  */
    FLOW_NFQ_SRC_IFINDEX = 346,    /* FLOW_NFQ_SRC_IFINDEX  */
    FLOW_OTHER_BYTES = 347,        /* FLOW_OTHER_BYTES  */
    FLOW_OTHER_PACKETS = 348,      /* FLOW_OTHER_PACKETS  */
    FLOW_SRC_BYTES = 349,          /* FLOW_SRC_BYTES  */
    FLOW_SRC_PACKETS = 350,        /* FLOW_SRC_PACKETS  */
    FLOW_SSH_CLIENT_AGENT = 351,   /* FLOW_SSH_CLIENT_AGENT  */
    FLOW_SSH_SERVER_AGENT = 352,   /* FLOW_SSH_SERVER_AGENT  */
    FLOW_TCP_FIN_ACK = 353,        /* FLOW_TCP_FIN_ACK  */
    FLOW_TCP_LAST_SEQ = 354,       /* FLOW_TCP_LAST_SEQ  */
    FLOW_TLS_ALPN = 355,           /* FLOW_TLS_ALPN  */
    FLOW_TLS_ISSUER_DN = 356,      /* FLOW_TLS_ISSUER_DN  */
    FLOW_TLS_SERVER_CN = 357,      /* FLOW_TLS_SERVER_CN  */
    FLOW_TLS_SUBJECT_DN = 358,     /* FLOW_TLS_SUBJECT_DN  */
    FLOW_TOTAL_BYTES = 359,        /* FLOW_TOTAL_BYTES  */
    FLOW_TOTAL_PACKETS = 360,      /* FLOW_TOTAL_PACKETS  */
    FLOW_TS_FIRST_SEEN = 361,      /* FLOW_TS_FIRST_SEEN  */
    FLOW_TS_LAST_SEEN = 362,       /* FLOW_TS_LAST_SEEN  */
    FLOW_TOTAL_LOCAL_BYTES = 363,  /* FLOW_TOTAL_LOCAL_BYTES  */
    FLOW_TOTAL_OTHER_BYTES = 364,  /* FLOW_TOTAL_OTHER_BYTES  */
    FLOW_TOTAL_SRC_BYTES = 365,    /* FLOW_TOTAL_SRC_BYTES  */
    FLOW_TOTAL_DST_BYTES = 366,    /* FLOW_TOTAL_DST_BYTES  */
    FLOW_TOTAL_LOCAL_PACKETS = 367, /* FLOW_TOTAL_LOCAL_PACKETS  */
    FLOW_TOTAL_OTHER_PACKETS = 368, /* FLOW_TOTAL_OTHER_PACKETS  */
    FLOW_TOTAL_SRC_PACKETS = 369,  /* FLOW_TOTAL_SRC_PACKETS  */
    FLOW_TOTAL_DST_PACKETS = 370,  /* FLOW_TOTAL_DST_PACKETS  */
    FLOW_DETECTION_PACKETS = 371,  /* FLOW_DETECTION_PACKETS  */
    FLOW_LOCAL_RATE = 372,         /* FLOW_LOCAL_RATE  */
    FLOW_OTHER_RATE = 373,         /* FLOW_OTHER_RATE  */
    FLOW_SRC_RATE = 374,           /* FLOW_SRC_RATE  */
    FLOW_DST_RATE = 375,           /* FLOW_DST_RATE  */
    FLOW_TCP_SEQ_ERRORS = 376,     /* FLOW_TCP_SEQ_ERRORS  */
    FLOW_TCP_RESETS = 377,         /* FLOW_TCP_RESETS  */
    FLOW_TCP_RETRANS = 378,        /* FLOW_TCP_RETRANS  */
    FLOW_TLS_CERT_FINGERPRINT = 379, /* FLOW_TLS_CERT_FINGERPRINT  */
    FLOW_TLS_ALPN_SERVER = 380,    /* FLOW_TLS_ALPN_SERVER  */
    FLOW_TLS_PROC_HELLO = 381,     /* FLOW_TLS_PROC_HELLO  */
    FLOW_TLS_PROC_CERTIFICATE = 382, /* FLOW_TLS_PROC_CERTIFICATE  */
    FLOW_SMTP_TLS = 383,           /* FLOW_SMTP_TLS  */
    FLOW_BT_INFO_HASH = 384,       /* FLOW_BT_INFO_HASH  */
    FLOW_STUN_MAPPED = 385,        /* FLOW_STUN_MAPPED  */
    FLOW_STUN_PEER = 386,          /* FLOW_STUN_PEER  */
    FLOW_STUN_RELAYED = 387,       /* FLOW_STUN_RELAYED  */
    FLOW_STUN_RESPONSE = 388,      /* FLOW_STUN_RESPONSE  */
    FLOW_STUN_OTHER = 389,         /* FLOW_STUN_OTHER  */
    FLOW_OTHER_UNKNOWN = 390,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 391,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 392,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 393,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 394,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 395,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 396,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 397,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 398,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 399,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 400,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 401,         /* FLOW_TUNNEL_GTP  */
    FLOW_INTEL = 402,              /* FLOW_INTEL  */
    CMP_EQUAL = 403,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 404,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 405,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 406,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 407,                /* BOOL_AND  */
    BOOL_OR = 408,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 409,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 410,              /* VALUE_TRUE  */
    VALUE_FALSE = 411,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 412,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 413,              /* VALUE_NAME  */
    VALUE_REGEX = 414,             /* VALUE_REGEX  */
    VALUE_ADDR_TAG = 415,          /* VALUE_ADDR_TAG  */
    VALUE_ADDR_IPV4 = 416,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 417,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 418,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 419,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 420,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 421,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 422              /* VALUE_FLOAT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define FLOW_ANY_IP 258
#define FLOW_ANY_MAC 259
#define FLOW_ANY_PORT 260
#define FLOW_APPLICATION 261
#define FLOW_APPLICATION_CATEGORY 262
#define FLOW_APPLICATION_CATEGORY_ID 263
#define FLOW_CATEGORY 264
#define FLOW_CATEGORY_ID 265
#define FLOW_CONNTRACK_ID 266
#define FLOW_CONNTRACK_MARK 267
#define FLOW_CONNTRACK_REPLY_DST_IP 268
#define FLOW_CONNTRACK_REPLY_SRC_IP 269
#define FLOW_DETECTED_HOSTNAME 270
#define FLOW_DETECTION_COMPLETE 271
#define FLOW_DETECTION_GUESSED 272
#define FLOW_DETECTION_INIT 273
#define FLOW_DETECTION_UPDATED 274
#define FLOW_DHC_HIT 275
#define FLOW_DNS_HOSTNAME 276
#define FLOW_DOMAIN_CATEGORY 277
#define FLOW_DOMAIN_CATEGORY_ID 278
#define FLOW_DST_IP 279
#define FLOW_DST_MAC 280
#define FLOW_DST_NETWORK_CATEGORY 281
#define FLOW_DST_NETWORK_CATEGORY_ID 282
#define FLOW_DST_PORT 283
#define FLOW_EXPIRED 284
#define FLOW_EXPIRING 285
#define FLOW_FHC_HIT 286
#define FLOW_IFACE 287
#define FLOW_IFACE_NFQ_DST 288
#define FLOW_IFACE_NFQ_SRC 289
#define FLOW_IP_DSCP 290
#define FLOW_IP_NAT 291
#define FLOW_IP_PROTO 292
#define FLOW_IP_VERSION 293
#define FLOW_LOCAL_IP 294
#define FLOW_LOCAL_MAC 295
#define FLOW_LOCAL_NETWORK_CATEGORY 296
#define FLOW_LOCAL_NETWORK_CATEGORY_ID 297
#define FLOW_LOCAL_PORT 298
#define FLOW_NDPI_RISK_SCORE 299
#define FLOW_NDPI_RISK_SCORE_CLIENT 300
#define FLOW_NDPI_RISK_SCORE_SERVER 301
#define FLOW_NETWORK_CATEGORY 302
#define FLOW_NETWORK_CATEGORY_ID 303
#define FLOW_ORIGIN 304
#define FLOW_OTHER_IP 305
#define FLOW_OTHER_MAC 306
#define FLOW_OTHER_NETWORK_CATEGORY 307
#define FLOW_OTHER_NETWORK_CATEGORY_ID 308
#define FLOW_OTHER_PORT 309
#define FLOW_OTHER_TYPE 310
#define FLOW_PROTOCOL 311
#define FLOW_PROTOCOL_CATEGORY 312
#define FLOW_PROTOCOL_CATEGORY_ID 313
#define FLOW_RISKS 314
#define FLOW_SOFT_DISSECTOR 315
#define FLOW_SRC_IP 316
#define FLOW_SRC_MAC 317
#define FLOW_SRC_NETWORK_CATEGORY 318
#define FLOW_SRC_NETWORK_CATEGORY_ID 319
#define FLOW_SRC_PORT 320
#define FLOW_TAG 321
#define FLOW_TAG_CATEGORY 322
#define FLOW_TAG_CATEGORY_ID 323
#define FLOW_TLS_CIPHER 324
#define FLOW_TLS_ECH 325
#define FLOW_TLS_JA4 326
#define FLOW_TLS_VERSION 327
#define FLOW_TUNNEL_TYPE 328
#define FLOW_VLAN_ID 329
#define FLOW_VLAN 330
#define FLOW_LOCAL_IF_META_SSID 331
#define FLOW_LOCAL_IF_META_PVID 332
#define FLOW_OTHER_IF_META_PVID 333
#define FLOW_APP_IP_OVERRIDE 334
#define FLOW_APP_PROTO_TWINS 335
#define FLOW_DHCP_CLASS_IDENT 336
#define FLOW_DHCP_FINGERPRINT 337
#define FLOW_DST_BYTES 338
#define FLOW_DST_PACKETS 339
#define FLOW_HTTP_URL 340
#define FLOW_HTTP_USER_AGENT 341
#define FLOW_LOCAL_BYTES 342
#define FLOW_LOCAL_PACKETS 343
#define FLOW_MDNS_DOMAIN_NAME 344
#define FLOW_NFQ_DST_IFINDEX 345
#define FLOW_NFQ_SRC_IFINDEX 346
#define FLOW_OTHER_BYTES 347
#define FLOW_OTHER_PACKETS 348
#define FLOW_SRC_BYTES 349
#define FLOW_SRC_PACKETS 350
#define FLOW_SSH_CLIENT_AGENT 351
#define FLOW_SSH_SERVER_AGENT 352
#define FLOW_TCP_FIN_ACK 353
#define FLOW_TCP_LAST_SEQ 354
#define FLOW_TLS_ALPN 355
#define FLOW_TLS_ISSUER_DN 356
#define FLOW_TLS_SERVER_CN 357
#define FLOW_TLS_SUBJECT_DN 358
#define FLOW_TOTAL_BYTES 359
#define FLOW_TOTAL_PACKETS 360
#define FLOW_TS_FIRST_SEEN 361
#define FLOW_TS_LAST_SEEN 362
#define FLOW_TOTAL_LOCAL_BYTES 363
#define FLOW_TOTAL_OTHER_BYTES 364
#define FLOW_TOTAL_SRC_BYTES 365
#define FLOW_TOTAL_DST_BYTES 366
#define FLOW_TOTAL_LOCAL_PACKETS 367
#define FLOW_TOTAL_OTHER_PACKETS 368
#define FLOW_TOTAL_SRC_PACKETS 369
#define FLOW_TOTAL_DST_PACKETS 370
#define FLOW_DETECTION_PACKETS 371
#define FLOW_LOCAL_RATE 372
#define FLOW_OTHER_RATE 373
#define FLOW_SRC_RATE 374
#define FLOW_DST_RATE 375
#define FLOW_TCP_SEQ_ERRORS 376
#define FLOW_TCP_RESETS 377
#define FLOW_TCP_RETRANS 378
#define FLOW_TLS_CERT_FINGERPRINT 379
#define FLOW_TLS_ALPN_SERVER 380
#define FLOW_TLS_PROC_HELLO 381
#define FLOW_TLS_PROC_CERTIFICATE 382
#define FLOW_SMTP_TLS 383
#define FLOW_BT_INFO_HASH 384
#define FLOW_STUN_MAPPED 385
#define FLOW_STUN_PEER 386
#define FLOW_STUN_RELAYED 387
#define FLOW_STUN_RESPONSE 388
#define FLOW_STUN_OTHER 389
#define FLOW_OTHER_UNKNOWN 390
#define FLOW_OTHER_UNSUPPORTED 391
#define FLOW_OTHER_LOCAL 392
#define FLOW_OTHER_MULTICAST 393
#define FLOW_OTHER_BROADCAST 394
#define FLOW_OTHER_REMOTE 395
#define FLOW_OTHER_ERROR 396
#define FLOW_ORIGIN_LOCAL 397
#define FLOW_ORIGIN_OTHER 398
#define FLOW_ORIGIN_UNKNOWN 399
#define FLOW_TUNNEL_NONE 400
#define FLOW_TUNNEL_GTP 401
#define FLOW_INTEL 402
#define CMP_EQUAL 403
#define CMP_NOTEQUAL 404
#define CMP_GTHANEQUAL 405
#define CMP_LTHANEQUAL 406
#define BOOL_AND 407
#define BOOL_OR 408
#define VALUE_ADDR_IPMASK 409
#define VALUE_TRUE 410
#define VALUE_FALSE 411
#define VALUE_ADDR_MAC 412
#define VALUE_NAME 413
#define VALUE_REGEX 414
#define VALUE_ADDR_TAG 415
#define VALUE_ADDR_IPV4 416
#define VALUE_ADDR_IPV4_CIDR 417
#define VALUE_ADDR_IPV6 418
#define VALUE_ADDR_IPV6_CIDR 419
#define VALUE_SIGNED 420
#define VALUE_UNSIGNED 421
#define VALUE_FLOAT 422

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 137 "nd-flow-expr.ypp"

    char buffer[_NDFP_MAX_BUFLEN];

    bool bool_number;
    unsigned short us_number;
    long sl_number;
    unsigned long ul_number;
    float fl_number;

    bool bool_result;

#line 601 "nd-flow-expr.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (yyscan_t scanner);


#endif /* !YY_YY_ND_FLOW_EXPR_HPP_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_FLOW_ANY_IP = 3,                /* FLOW_ANY_IP  */
  YYSYMBOL_FLOW_ANY_MAC = 4,               /* FLOW_ANY_MAC  */
  YYSYMBOL_FLOW_ANY_PORT = 5,              /* FLOW_ANY_PORT  */
  YYSYMBOL_FLOW_APPLICATION = 6,           /* FLOW_APPLICATION  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY = 7,  /* FLOW_APPLICATION_CATEGORY  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY_ID = 8, /* FLOW_APPLICATION_CATEGORY_ID  */
  YYSYMBOL_FLOW_CATEGORY = 9,              /* FLOW_CATEGORY  */
  YYSYMBOL_FLOW_CATEGORY_ID = 10,          /* FLOW_CATEGORY_ID  */
  YYSYMBOL_FLOW_CONNTRACK_ID = 11,         /* FLOW_CONNTRACK_ID  */
  YYSYMBOL_FLOW_CONNTRACK_MARK = 12,       /* FLOW_CONNTRACK_MARK  */
  YYSYMBOL_FLOW_CONNTRACK_REPLY_DST_IP = 13, /* FLOW_CONNTRACK_REPLY_DST_IP  */
  YYSYMBOL_FLOW_CONNTRACK_REPLY_SRC_IP = 14, /* FLOW_CONNTRACK_REPLY_SRC_IP  */
  YYSYMBOL_FLOW_DETECTED_HOSTNAME = 15,    /* FLOW_DETECTED_HOSTNAME  */
  YYSYMBOL_FLOW_DETECTION_COMPLETE = 16,   /* FLOW_DETECTION_COMPLETE  */
  YYSYMBOL_FLOW_DETECTION_GUESSED = 17,    /* FLOW_DETECTION_GUESSED  */
  YYSYMBOL_FLOW_DETECTION_INIT = 18,       /* FLOW_DETECTION_INIT  */
  YYSYMBOL_FLOW_DETECTION_UPDATED = 19,    /* FLOW_DETECTION_UPDATED  */
  YYSYMBOL_FLOW_DHC_HIT = 20,              /* FLOW_DHC_HIT  */
  YYSYMBOL_FLOW_DNS_HOSTNAME = 21,         /* FLOW_DNS_HOSTNAME  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY = 22,      /* FLOW_DOMAIN_CATEGORY  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY_ID = 23,   /* FLOW_DOMAIN_CATEGORY_ID  */
  YYSYMBOL_FLOW_DST_IP = 24,               /* FLOW_DST_IP  */
  YYSYMBOL_FLOW_DST_MAC = 25,              /* FLOW_DST_MAC  */
  YYSYMBOL_FLOW_DST_NETWORK_CATEGORY = 26, /* FLOW_DST_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_DST_NETWORK_CATEGORY_ID = 27, /* FLOW_DST_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_DST_PORT = 28,             /* FLOW_DST_PORT  */
  YYSYMBOL_FLOW_EXPIRED = 29,              /* FLOW_EXPIRED  */
  YYSYMBOL_FLOW_EXPIRING = 30,             /* FLOW_EXPIRING  */
  YYSYMBOL_FLOW_FHC_HIT = 31,              /* FLOW_FHC_HIT  */
  YYSYMBOL_FLOW_IFACE = 32,                /* FLOW_IFACE  */
  YYSYMBOL_FLOW_IFACE_NFQ_DST = 33,        /* FLOW_IFACE_NFQ_DST  */
  YYSYMBOL_FLOW_IFACE_NFQ_SRC = 34,        /* FLOW_IFACE_NFQ_SRC  */
  YYSYMBOL_FLOW_IP_DSCP = 35,              /* FLOW_IP_DSCP  */
  YYSYMBOL_FLOW_IP_NAT = 36,               /* FLOW_IP_NAT  */
  YYSYMBOL_FLOW_IP_PROTO = 37,             /* FLOW_IP_PROTO  */
  YYSYMBOL_FLOW_IP_VERSION = 38,           /* FLOW_IP_VERSION  */
  YYSYMBOL_FLOW_LOCAL_IP = 39,             /* FLOW_LOCAL_IP  */
  YYSYMBOL_FLOW_LOCAL_MAC = 40,            /* FLOW_LOCAL_MAC  */
  YYSYMBOL_FLOW_LOCAL_NETWORK_CATEGORY = 41, /* FLOW_LOCAL_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_LOCAL_NETWORK_CATEGORY_ID = 42, /* FLOW_LOCAL_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_LOCAL_PORT = 43,           /* FLOW_LOCAL_PORT  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE = 44,      /* FLOW_NDPI_RISK_SCORE  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_CLIENT = 45, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_SERVER = 46, /* FLOW_NDPI_RISK_SCORE_SERVER  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY = 47,     /* FLOW_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY_ID = 48,  /* FLOW_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_ORIGIN = 49,               /* FLOW_ORIGIN  */
  YYSYMBOL_FLOW_OTHER_IP = 50,             /* FLOW_OTHER_IP  */
  YYSYMBOL_FLOW_OTHER_MAC = 51,            /* FLOW_OTHER_MAC  */
  YYSYMBOL_FLOW_OTHER_NETWORK_CATEGORY = 52, /* FLOW_OTHER_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_OTHER_NETWORK_CATEGORY_ID = 53, /* FLOW_OTHER_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_OTHER_PORT = 54,           /* FLOW_OTHER_PORT  */
  YYSYMBOL_FLOW_OTHER_TYPE = 55,           /* FLOW_OTHER_TYPE  */
  YYSYMBOL_FLOW_PROTOCOL = 56,             /* FLOW_PROTOCOL  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY = 57,    /* FLOW_PROTOCOL_CATEGORY  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY_ID = 58, /* FLOW_PROTOCOL_CATEGORY_ID  */
  YYSYMBOL_FLOW_RISKS = 59,                /* FLOW_RISKS  */
  YYSYMBOL_FLOW_SOFT_DISSECTOR = 60,       /* FLOW_SOFT_DISSECTOR  */
  YYSYMBOL_FLOW_SRC_IP = 61,               /* FLOW_SRC_IP  */
  YYSYMBOL_FLOW_SRC_MAC = 62,              /* FLOW_SRC_MAC  */
  YYSYMBOL_FLOW_SRC_NETWORK_CATEGORY = 63, /* FLOW_SRC_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_SRC_NETWORK_CATEGORY_ID = 64, /* FLOW_SRC_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_SRC_PORT = 65,             /* FLOW_SRC_PORT  */
  YYSYMBOL_FLOW_TAG = 66,                  /* FLOW_TAG  */
  YYSYMBOL_FLOW_TAG_CATEGORY = 67,         /* FLOW_TAG_CATEGORY  */
  YYSYMBOL_FLOW_TAG_CATEGORY_ID = 68,      /* FLOW_TAG_CATEGORY_ID  */
  YYSYMBOL_FLOW_TLS_CIPHER = 69,           /* FLOW_TLS_CIPHER  */
  YYSYMBOL_FLOW_TLS_ECH = 70,              /* FLOW_TLS_ECH  */
  YYSYMBOL_FLOW_TLS_JA4 = 71,              /* FLOW_TLS_JA4  */
  YYSYMBOL_FLOW_TLS_VERSION = 72,          /* FLOW_TLS_VERSION  */
  YYSYMBOL_FLOW_TUNNEL_TYPE = 73,          /* FLOW_TUNNEL_TYPE  */
  YYSYMBOL_FLOW_VLAN_ID = 74,              /* FLOW_VLAN_ID  */
  YYSYMBOL_FLOW_VLAN = 75,                 /* FLOW_VLAN  */
  YYSYMBOL_FLOW_LOCAL_IF_META_SSID = 76,   /* FLOW_LOCAL_IF_META_SSID  */
  YYSYMBOL_FLOW_LOCAL_IF_META_PVID = 77,   /* FLOW_LOCAL_IF_META_PVID  */
  YYSYMBOL_FLOW_OTHER_IF_META_PVID = 78,   /* FLOW_OTHER_IF_META_PVID  */
  YYSYMBOL_FLOW_APP_IP_OVERRIDE = 79,      /* FLOW_APP_IP_OVERRIDE  */
  YYSYMBOL_FLOW_APP_PROTO_TWINS = 80,      /* FLOW_APP_PROTO_TWINS  */
  YYSYMBOL_FLOW_DHCP_CLASS_IDENT = 81,     /* FLOW_DHCP_CLASS_IDENT  */
  YYSYMBOL_FLOW_DHCP_FINGERPRINT = 82,     /* FLOW_DHCP_FINGERPRINT  */
  YYSYMBOL_FLOW_DST_BYTES = 83,            /* FLOW_DST_BYTES  */
  YYSYMBOL_FLOW_DST_PACKETS = 84,          /* FLOW_DST_PACKETS  */
  YYSYMBOL_FLOW_HTTP_URL = 85,             /* FLOW_HTTP_URL  */
  YYSYMBOL_FLOW_HTTP_USER_AGENT = 86,      /* FLOW_HTTP_USER_AGENT  */
  YYSYMBOL_FLOW_LOCAL_BYTES = 87,          /* FLOW_LOCAL_BYTES  */
  YYSYMBOL_FLOW_LOCAL_PACKETS = 88,        /* FLOW_LOCAL_PACKETS  */
  YYSYMBOL_FLOW_MDNS_DOMAIN_NAME = 89,     /* FLOW_MDNS_DOMAIN_NAME  */
  YYSYMBOL_FLOW_NFQ_DST_IFINDEX = 90,      /* FLOW_NFQ_DST_IFINDEX  */
  YYSYMBOL_FLOW_NFQ_SRC_IFINDEX = 91,      /* FLOW_NFQ_SRC_IFINDEX  */
  YYSYMBOL_FLOW_OTHER_BYTES = 92,          /* FLOW_OTHER_BYTES  */
  YYSYMBOL_FLOW_OTHER_PACKETS = 93,        /* FLOW_OTHER_PACKETS  */
  YYSYMBOL_FLOW_SRC_BYTES = 94,            /* FLOW_SRC_BYTES  */
  YYSYMBOL_FLOW_SRC_PACKETS = 95,          /* FLOW_SRC_PACKETS  */
  YYSYMBOL_FLOW_SSH_CLIENT_AGENT = 96,     /* FLOW_SSH_CLIENT_AGENT  */
  YYSYMBOL_FLOW_SSH_SERVER_AGENT = 97,     /* FLOW_SSH_SERVER_AGENT  */
  YYSYMBOL_FLOW_TCP_FIN_ACK = 98,          /* FLOW_TCP_FIN_ACK  */
  YYSYMBOL_FLOW_TCP_LAST_SEQ = 99,         /* FLOW_TCP_LAST_SEQ  */
  YYSYMBOL_FLOW_TLS_ALPN = 100,            /* FLOW_TLS_ALPN  */
  YYSYMBOL_FLOW_TLS_ISSUER_DN = 101,       /* FLOW_TLS_ISSUER_DN  */
  YYSYMBOL_FLOW_TLS_SERVER_CN = 102,       /* FLOW_TLS_SERVER_CN  */
  YYSYMBOL_FLOW_TLS_SUBJECT_DN = 103,      /* FLOW_TLS_SUBJECT_DN  */
  YYSYMBOL_FLOW_TOTAL_BYTES = 104,         /* FLOW_TOTAL_BYTES  */
  YYSYMBOL_FLOW_TOTAL_PACKETS = 105,       /* FLOW_TOTAL_PACKETS  */
  YYSYMBOL_FLOW_TS_FIRST_SEEN = 106,       /* FLOW_TS_FIRST_SEEN  */
  YYSYMBOL_FLOW_TS_LAST_SEEN = 107,        /* FLOW_TS_LAST_SEEN  */
  YYSYMBOL_FLOW_TOTAL_LOCAL_BYTES = 108,   /* FLOW_TOTAL_LOCAL_BYTES  */
  YYSYMBOL_FLOW_TOTAL_OTHER_BYTES = 109,   /* FLOW_TOTAL_OTHER_BYTES  */
  YYSYMBOL_FLOW_TOTAL_SRC_BYTES = 110,     /* FLOW_TOTAL_SRC_BYTES  */
  YYSYMBOL_FLOW_TOTAL_DST_BYTES = 111,     /* FLOW_TOTAL_DST_BYTES  */
  YYSYMBOL_FLOW_TOTAL_LOCAL_PACKETS = 112, /* FLOW_TOTAL_LOCAL_PACKETS  */
  YYSYMBOL_FLOW_TOTAL_OTHER_PACKETS = 113, /* FLOW_TOTAL_OTHER_PACKETS  */
  YYSYMBOL_FLOW_TOTAL_SRC_PACKETS = 114,   /* FLOW_TOTAL_SRC_PACKETS  */
  YYSYMBOL_FLOW_TOTAL_DST_PACKETS = 115,   /* FLOW_TOTAL_DST_PACKETS  */
  YYSYMBOL_FLOW_DETECTION_PACKETS = 116,   /* FLOW_DETECTION_PACKETS  */
  YYSYMBOL_FLOW_LOCAL_RATE = 117,          /* FLOW_LOCAL_RATE  */
  YYSYMBOL_FLOW_OTHER_RATE = 118,          /* FLOW_OTHER_RATE  */
  YYSYMBOL_FLOW_SRC_RATE = 119,            /* FLOW_SRC_RATE  */
  YYSYMBOL_FLOW_DST_RATE = 120,            /* FLOW_DST_RATE  */
  YYSYMBOL_FLOW_TCP_SEQ_ERRORS = 121,      /* FLOW_TCP_SEQ_ERRORS  */
  YYSYMBOL_FLOW_TCP_RESETS = 122,          /* FLOW_TCP_RESETS  */
  YYSYMBOL_FLOW_TCP_RETRANS = 123,         /* FLOW_TCP_RETRANS  */
  YYSYMBOL_FLOW_TLS_CERT_FINGERPRINT = 124, /* FLOW_TLS_CERT_FINGERPRINT  */
  YYSYMBOL_FLOW_TLS_ALPN_SERVER = 125,     /* FLOW_TLS_ALPN_SERVER  */
  YYSYMBOL_FLOW_TLS_PROC_HELLO = 126,      /* FLOW_TLS_PROC_HELLO  */
  YYSYMBOL_FLOW_TLS_PROC_CERTIFICATE = 127, /* FLOW_TLS_PROC_CERTIFICATE  */
  YYSYMBOL_FLOW_SMTP_TLS = 128,            /* FLOW_SMTP_TLS  */
  YYSYMBOL_FLOW_BT_INFO_HASH = 129,        /* FLOW_BT_INFO_HASH  */
  YYSYMBOL_FLOW_STUN_MAPPED = 130,         /* FLOW_STUN_MAPPED  */
  YYSYMBOL_FLOW_STUN_PEER = 131,           /* FLOW_STUN_PEER  */
  YYSYMBOL_FLOW_STUN_RELAYED = 132,        /* FLOW_STUN_RELAYED  */
  YYSYMBOL_FLOW_STUN_RESPONSE = 133,       /* FLOW_STUN_RESPONSE  */
  YYSYMBOL_FLOW_STUN_OTHER = 134,          /* FLOW_STUN_OTHER  */
  YYSYMBOL_FLOW_OTHER_UNKNOWN = 135,       /* FLOW_OTHER_UNKNOWN  */
  YYSYMBOL_FLOW_OTHER_UNSUPPORTED = 136,   /* FLOW_OTHER_UNSUPPORTED  */
  YYSYMBOL_FLOW_OTHER_LOCAL = 137,         /* FLOW_OTHER_LOCAL  */
  YYSYMBOL_FLOW_OTHER_MULTICAST = 138,     /* FLOW_OTHER_MULTICAST  */
  YYSYMBOL_FLOW_OTHER_BROADCAST = 139,     /* FLOW_OTHER_BROADCAST  */
  YYSYMBOL_FLOW_OTHER_REMOTE = 140,        /* FLOW_OTHER_REMOTE  */
  YYSYMBOL_FLOW_OTHER_ERROR = 141,         /* FLOW_OTHER_ERROR  */
  YYSYMBOL_FLOW_ORIGIN_LOCAL = 142,        /* FLOW_ORIGIN_LOCAL  */
  YYSYMBOL_FLOW_ORIGIN_OTHER = 143,        /* FLOW_ORIGIN_OTHER  */
  YYSYMBOL_FLOW_ORIGIN_UNKNOWN = 144,      /* FLOW_ORIGIN_UNKNOWN  */
  YYSYMBOL_FLOW_TUNNEL_NONE = 145,         /* FLOW_TUNNEL_NONE  */
  YYSYMBOL_FLOW_TUNNEL_GTP = 146,          /* FLOW_TUNNEL_GTP  */
  YYSYMBOL_FLOW_INTEL = 147,               /* FLOW_INTEL  */
  YYSYMBOL_CMP_EQUAL = 148,                /* CMP_EQUAL  */
  YYSYMBOL_CMP_NOTEQUAL = 149,             /* CMP_NOTEQUAL  */
  YYSYMBOL_CMP_GTHANEQUAL = 150,           /* CMP_GTHANEQUAL  */
  YYSYMBOL_CMP_LTHANEQUAL = 151,           /* CMP_LTHANEQUAL  */
  YYSYMBOL_BOOL_AND = 152,                 /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 153,                  /* BOOL_OR  */
  YYSYMBOL_VALUE_ADDR_IPMASK = 154,        /* VALUE_ADDR_IPMASK  */
  YYSYMBOL_VALUE_TRUE = 155,               /* VALUE_TRUE  */
  YYSYMBOL_VALUE_FALSE = 156,              /* VALUE_FALSE  */
  YYSYMBOL_VALUE_ADDR_MAC = 157,           /* VALUE_ADDR_MAC  */
  YYSYMBOL_VALUE_NAME = 158,               /* VALUE_NAME  */
  YYSYMBOL_VALUE_REGEX = 159,              /* VALUE_REGEX  */
  YYSYMBOL_VALUE_ADDR_TAG = 160,           /* VALUE_ADDR_TAG  */
  YYSYMBOL_VALUE_ADDR_IPV4 = 161,          /* VALUE_ADDR_IPV4  */
  YYSYMBOL_VALUE_ADDR_IPV4_CIDR = 162,     /* VALUE_ADDR_IPV4_CIDR  */
  YYSYMBOL_VALUE_ADDR_IPV6 = 163,          /* VALUE_ADDR_IPV6  */
  YYSYMBOL_VALUE_ADDR_IPV6_CIDR = 164,     /* VALUE_ADDR_IPV6_CIDR  */
  YYSYMBOL_VALUE_SIGNED = 165,             /* VALUE_SIGNED  */
  YYSYMBOL_VALUE_UNSIGNED = 166,           /* VALUE_UNSIGNED  */
  YYSYMBOL_VALUE_FLOAT = 167,              /* VALUE_FLOAT  */
  YYSYMBOL_168_ = 168,                     /* ';'  */
  YYSYMBOL_169_ = 169,                     /* '('  */
  YYSYMBOL_170_ = 170,                     /* ')'  */
  YYSYMBOL_171_ = 171,                     /* '!'  */
  YYSYMBOL_172_ = 172,                     /* '>'  */
  YYSYMBOL_173_ = 173,                     /* '<'  */
  YYSYMBOL_YYACCEPT = 174,                 /* $accept  */
  YYSYMBOL_exprs = 175,                    /* exprs  */
  YYSYMBOL_expr = 176,                     /* expr  */
  YYSYMBOL_expr_ip_proto = 177,            /* expr_ip_proto  */
  YYSYMBOL_expr_ip_dscp = 178,             /* expr_ip_dscp  */
  YYSYMBOL_expr_ip_version = 179,          /* expr_ip_version  */
  YYSYMBOL_expr_vlan_id = 180,             /* expr_vlan_id  */
  YYSYMBOL_expr_vlan = 181,                /* expr_vlan  */
  YYSYMBOL_expr_local_if_metadata_ssid = 182, /* expr_local_if_metadata_ssid  */
  YYSYMBOL_expr_local_if_metadata_pvid = 183, /* expr_local_if_metadata_pvid  */
  YYSYMBOL_expr_other_if_metadata_pvid = 184, /* expr_other_if_metadata_pvid  */
  YYSYMBOL_expr_other_type = 185,          /* expr_other_type  */
  YYSYMBOL_value_other_type = 186,         /* value_other_type  */
  YYSYMBOL_expr_any_mac = 187,             /* expr_any_mac  */
  YYSYMBOL_expr_local_mac = 188,           /* expr_local_mac  */
  YYSYMBOL_expr_other_mac = 189,           /* expr_other_mac  */
  YYSYMBOL_expr_src_mac = 190,             /* expr_src_mac  */
  YYSYMBOL_expr_dst_mac = 191,             /* expr_dst_mac  */
  YYSYMBOL_expr_any_ip = 192,              /* expr_any_ip  */
  YYSYMBOL_expr_local_ip = 193,            /* expr_local_ip  */
  YYSYMBOL_expr_other_ip = 194,            /* expr_other_ip  */
  YYSYMBOL_expr_src_ip = 195,              /* expr_src_ip  */
  YYSYMBOL_expr_dst_ip = 196,              /* expr_dst_ip  */
  YYSYMBOL_expr_conntrack_reply_src_ip = 197, /* expr_conntrack_reply_src_ip  */
  YYSYMBOL_expr_conntrack_reply_dst_ip = 198, /* expr_conntrack_reply_dst_ip  */
  YYSYMBOL_value_addr_ip = 199,            /* value_addr_ip  */
  YYSYMBOL_expr_any_port = 200,            /* expr_any_port  */
  YYSYMBOL_expr_local_port = 201,          /* expr_local_port  */
  YYSYMBOL_expr_other_port = 202,          /* expr_other_port  */
  YYSYMBOL_expr_src_port = 203,            /* expr_src_port  */
  YYSYMBOL_expr_dst_port = 204,            /* expr_dst_port  */
  YYSYMBOL_expr_tunnel_type = 205,         /* expr_tunnel_type  */
  YYSYMBOL_value_tunnel_type = 206,        /* value_tunnel_type  */
  YYSYMBOL_expr_detection_complete = 207,  /* expr_detection_complete  */
  YYSYMBOL_expr_detection_guessed = 208,   /* expr_detection_guessed  */
  YYSYMBOL_expr_detection_init = 209,      /* expr_detection_init  */
  YYSYMBOL_expr_detection_updated = 210,   /* expr_detection_updated  */
  YYSYMBOL_expr_dhc_hit = 211,             /* expr_dhc_hit  */
  YYSYMBOL_expr_fhc_hit = 212,             /* expr_fhc_hit  */
  YYSYMBOL_expr_ip_nat = 213,              /* expr_ip_nat  */
  YYSYMBOL_expr_expiring = 214,            /* expr_expiring  */
  YYSYMBOL_expr_expired = 215,             /* expr_expired  */
  YYSYMBOL_expr_soft_dissector = 216,      /* expr_soft_dissector  */
  YYSYMBOL_expr_app = 217,                 /* expr_app  */
  YYSYMBOL_expr_app_id = 218,              /* expr_app_id  */
  YYSYMBOL_expr_app_name = 219,            /* expr_app_name  */
  YYSYMBOL_expr_category = 220,            /* expr_category  */
  YYSYMBOL_expr_category_id = 221,         /* expr_category_id  */
  YYSYMBOL_expr_app_category = 222,        /* expr_app_category  */
  YYSYMBOL_expr_app_category_id = 223,     /* expr_app_category_id  */
  YYSYMBOL_expr_domain_category = 224,     /* expr_domain_category  */
  YYSYMBOL_expr_domain_category_id = 225,  /* expr_domain_category_id  */
  YYSYMBOL_expr_network_category = 226,    /* expr_network_category  */
  YYSYMBOL_expr_network_category_id = 227, /* expr_network_category_id  */
  YYSYMBOL_expr_local_network_category = 228, /* expr_local_network_category  */
  YYSYMBOL_expr_local_network_category_id = 229, /* expr_local_network_category_id  */
  YYSYMBOL_expr_other_network_category = 230, /* expr_other_network_category  */
  YYSYMBOL_expr_other_network_category_id = 231, /* expr_other_network_category_id  */
  YYSYMBOL_expr_src_network_category = 232, /* expr_src_network_category  */
  YYSYMBOL_expr_src_network_category_id = 233, /* expr_src_network_category_id  */
  YYSYMBOL_expr_dst_network_category = 234, /* expr_dst_network_category  */
  YYSYMBOL_expr_dst_network_category_id = 235, /* expr_dst_network_category_id  */
  YYSYMBOL_expr_tag_category = 236,        /* expr_tag_category  */
  YYSYMBOL_expr_tag_category_id = 237,     /* expr_tag_category_id  */
  YYSYMBOL_expr_proto = 238,               /* expr_proto  */
  YYSYMBOL_expr_proto_id = 239,            /* expr_proto_id  */
  YYSYMBOL_expr_proto_name = 240,          /* expr_proto_name  */
  YYSYMBOL_expr_proto_category = 241,      /* expr_proto_category  */
  YYSYMBOL_expr_proto_category_id = 242,   /* expr_proto_category_id  */
  YYSYMBOL_expr_detected_hostname = 243,   /* expr_detected_hostname  */
  YYSYMBOL_expr_dns_hostname = 244,        /* expr_dns_hostname  */
  YYSYMBOL_expr_risks = 245,               /* expr_risks  */
  YYSYMBOL_expr_risk_ndpi_score = 246,     /* expr_risk_ndpi_score  */
  YYSYMBOL_expr_risk_ndpi_score_client = 247, /* expr_risk_ndpi_score_client  */
  YYSYMBOL_expr_risk_ndpi_score_server = 248, /* expr_risk_ndpi_score_server  */
  YYSYMBOL_expr_conntrack_id = 249,        /* expr_conntrack_id  */
  YYSYMBOL_expr_conntrack_mark = 250,      /* expr_conntrack_mark  */
  YYSYMBOL_expr_iface = 251,               /* expr_iface  */
  YYSYMBOL_expr_iface_nfq_src = 252,       /* expr_iface_nfq_src  */
  YYSYMBOL_expr_iface_nfq_dst = 253,       /* expr_iface_nfq_dst  */
  YYSYMBOL_expr_intel = 254,               /* expr_intel  */
  YYSYMBOL_expr_tls_version = 255,         /* expr_tls_version  */
  YYSYMBOL_expr_tls_cipher = 256,          /* expr_tls_cipher  */
  YYSYMBOL_expr_tls_ech = 257,             /* expr_tls_ech  */
  YYSYMBOL_expr_tls_ja4 = 258,             /* expr_tls_ja4  */
  YYSYMBOL_expr_origin = 259,              /* expr_origin  */
  YYSYMBOL_value_origin_type = 260,        /* value_origin_type  */
  YYSYMBOL_expr_tag = 261,                 /* expr_tag  */
  YYSYMBOL_expr_app_ip_override = 262,     /* expr_app_ip_override  */
  YYSYMBOL_expr_app_proto_twins = 263,     /* expr_app_proto_twins  */
  YYSYMBOL_expr_ts_first_seen = 264,       /* expr_ts_first_seen  */
  YYSYMBOL_expr_ts_last_seen = 265,        /* expr_ts_last_seen  */
  YYSYMBOL_expr_nfq_src_ifindex = 266,     /* expr_nfq_src_ifindex  */
  YYSYMBOL_expr_nfq_dst_ifindex = 267,     /* expr_nfq_dst_ifindex  */
  YYSYMBOL_expr_tcp_fin_ack = 268,         /* expr_tcp_fin_ack  */
  YYSYMBOL_expr_tcp_last_seq = 269,        /* expr_tcp_last_seq  */
  YYSYMBOL_expr_total_bytes = 270,         /* expr_total_bytes  */
  YYSYMBOL_expr_total_packets = 271,       /* expr_total_packets  */
  YYSYMBOL_expr_local_bytes = 272,         /* expr_local_bytes  */
  YYSYMBOL_expr_other_bytes = 273,         /* expr_other_bytes  */
  YYSYMBOL_expr_src_bytes = 274,           /* expr_src_bytes  */
  YYSYMBOL_expr_dst_bytes = 275,           /* expr_dst_bytes  */
  YYSYMBOL_expr_local_packets = 276,       /* expr_local_packets  */
  YYSYMBOL_expr_other_packets = 277,       /* expr_other_packets  */
  YYSYMBOL_expr_src_packets = 278,         /* expr_src_packets  */
  YYSYMBOL_expr_dst_packets = 279,         /* expr_dst_packets  */
  YYSYMBOL_expr_http_user_agent = 280,     /* expr_http_user_agent  */
  YYSYMBOL_expr_http_url = 281,            /* expr_http_url  */
  YYSYMBOL_expr_dhcp_fingerprint = 282,    /* expr_dhcp_fingerprint  */
  YYSYMBOL_expr_dhcp_class_ident = 283,    /* expr_dhcp_class_ident  */
  YYSYMBOL_expr_ssh_client_agent = 284,    /* expr_ssh_client_agent  */
  YYSYMBOL_expr_ssh_server_agent = 285,    /* expr_ssh_server_agent  */
  YYSYMBOL_expr_tls_subject_dn = 286,      /* expr_tls_subject_dn  */
  YYSYMBOL_expr_tls_issuer_dn = 287,       /* expr_tls_issuer_dn  */
  YYSYMBOL_expr_tls_server_cn = 288,       /* expr_tls_server_cn  */
  YYSYMBOL_expr_mdns_domain_name = 289,    /* expr_mdns_domain_name  */
  YYSYMBOL_expr_tls_alpn = 290,            /* expr_tls_alpn  */
  YYSYMBOL_expr_tls_proc_hello = 291,      /* expr_tls_proc_hello  */
  YYSYMBOL_expr_tls_proc_certificate = 292, /* expr_tls_proc_certificate  */
  YYSYMBOL_expr_smtp_tls = 293,            /* expr_smtp_tls  */
  YYSYMBOL_expr_total_local_bytes = 294,   /* expr_total_local_bytes  */
  YYSYMBOL_expr_total_other_bytes = 295,   /* expr_total_other_bytes  */
  YYSYMBOL_expr_total_src_bytes = 296,     /* expr_total_src_bytes  */
  YYSYMBOL_expr_total_dst_bytes = 297,     /* expr_total_dst_bytes  */
  YYSYMBOL_expr_total_local_packets = 298, /* expr_total_local_packets  */
  YYSYMBOL_expr_total_other_packets = 299, /* expr_total_other_packets  */
  YYSYMBOL_expr_total_src_packets = 300,   /* expr_total_src_packets  */
  YYSYMBOL_expr_total_dst_packets = 301,   /* expr_total_dst_packets  */
  YYSYMBOL_expr_detection_packets = 302,   /* expr_detection_packets  */
  YYSYMBOL_expr_local_rate = 303,          /* expr_local_rate  */
  YYSYMBOL_expr_other_rate = 304,          /* expr_other_rate  */
  YYSYMBOL_expr_src_rate = 305,            /* expr_src_rate  */
  YYSYMBOL_expr_dst_rate = 306,            /* expr_dst_rate  */
  YYSYMBOL_expr_tcp_seq_errors = 307,      /* expr_tcp_seq_errors  */
  YYSYMBOL_expr_tcp_resets = 308,          /* expr_tcp_resets  */
  YYSYMBOL_expr_tcp_retrans = 309,         /* expr_tcp_retrans  */
  YYSYMBOL_expr_tls_alpn_server = 310,     /* expr_tls_alpn_server  */
  YYSYMBOL_expr_tls_cert_fingerprint = 311, /* expr_tls_cert_fingerprint  */
  YYSYMBOL_expr_bt_info_hash = 312,        /* expr_bt_info_hash  */
  YYSYMBOL_expr_stun_mapped = 313,         /* expr_stun_mapped  */
  YYSYMBOL_expr_stun_peer = 314,           /* expr_stun_peer  */
  YYSYMBOL_expr_stun_relayed = 315,        /* expr_stun_relayed  */
  YYSYMBOL_expr_stun_response = 316,       /* expr_stun_response  */
  YYSYMBOL_expr_stun_other = 317           /* expr_stun_other  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1525

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  144
/* YYNRULES -- Number of rules.  */
#define YYNRULES  877
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1427

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   422


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   171,     2,     2,     2,     2,     2,     2,
     169,   170,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   168,
     173,     2,   172,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   283,   283,   285,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   426,   430,
     434,   439,   443,   447,   451,   455,   459,   463,   470,   474,
     481,   485,   492,   496,   500,   504,   508,   512,   516,   520,
     527,   535,   543,   551,   559,   567,   575,   583,   594,   602,
     610,   629,   648,   661,   677,   685,   693,   701,   709,   717,
     725,   733,   744,   752,   760,   768,   776,   784,   792,   800,
     811,   817,   823,   867,   914,   915,   916,   917,   918,   919,
     920,   924,   939,   952,   968,   986,   993,   999,  1008,  1020,
    1027,  1034,  1043,  1055,  1062,  1069,  1078,  1090,  1097,  1104,
    1113,  1125,  1138,  1151,  1166,  1184,  1190,  1196,  1205,  1217,
    1223,  1229,  1238,  1250,  1257,  1264,  1274,  1287,  1294,  1301,
    1311,  1324,  1335,  1346,  1360,  1377,  1388,  1399,  1413,  1430,
    1431,  1432,  1433,  1437,  1443,  1449,  1455,  1461,  1467,  1473,
    1479,  1488,  1492,  1496,  1500,  1504,  1508,  1512,  1516,  1523,
    1527,  1531,  1535,  1539,  1543,  1547,  1551,  1558,  1562,  1566,
    1570,  1574,  1578,  1582,  1586,  1593,  1597,  1601,  1605,  1609,
    1613,  1617,  1621,  1628,  1634,  1640,  1659,  1681,  1682,  1685,
    1689,  1695,  1703,  1711,  1719,  1730,  1734,  1740,  1748,  1756,
    1764,  1775,  1779,  1785,  1793,  1801,  1809,  1820,  1824,  1830,
    1838,  1846,  1854,  1865,  1869,  1875,  1883,  1891,  1899,  1910,
    1914,  1920,  1928,  1936,  1944,  1955,  1959,  1963,  1967,  1971,
    1975,  1982,  1986,  1990,  1994,  1998,  2002,  2009,  2013,  2017,
    2021,  2025,  2029,  2036,  2040,  2044,  2048,  2052,  2056,  2063,
    2069,  2077,  2078,  2081,  2090,  2102,  2129,  2159,  2227,  2294,
    2347,  2403,  2420,  2440,  2447,  2458,  2475,  2495,  2502,  2513,
    2539,  2568,  2582,  2599,  2616,  2636,  2644,  2655,  2672,  2692,
    2700,  2711,  2728,  2748,  2756,  2767,  2784,  2804,  2812,  2823,
    2840,  2860,  2868,  2879,  2885,  2891,  2892,  2895,  2901,  2910,
    2930,  2952,  2969,  2989,  2997,  3008,  3015,  3022,  3041,  3060,
    3074,  3083,  3090,  3097,  3116,  3135,  3149,  3158,  3162,  3166,
    3185,  3204,  3217,  3233,  3237,  3241,  3245,  3249,  3253,  3257,
    3261,  3268,  3272,  3276,  3280,  3284,  3288,  3292,  3296,  3303,
    3307,  3311,  3315,  3319,  3323,  3327,  3331,  3338,  3346,  3354,
    3362,  3370,  3378,  3386,  3394,  3405,  3413,  3421,  3429,  3437,
    3445,  3453,  3461,  3472,  3479,  3486,  3504,  3522,  3536,  3545,
    3556,  3567,  3588,  3607,  3621,  3638,  3649,  3660,  3679,  3698,
    3713,  3730,  3751,  3773,  3793,  3809,  3821,  3833,  3845,  3858,
    3873,  3893,  3909,  3921,  3933,  3945,  3958,  3973,  3985,  3997,
    4011,  4023,  4035,  4049,  4061,  4073,  4087,  4099,  4111,  4126,
    4130,  4134,  4138,  4142,  4146,  4150,  4154,  4161,  4165,  4169,
    4173,  4177,  4181,  4185,  4189,  4196,  4200,  4204,  4208,  4212,
    4216,  4220,  4224,  4231,  4238,  4245,  4264,  4283,  4297,  4314,
    4318,  4322,  4326,  4333,  4334,  4335,  4339,  4346,  4353,  4381,
    4412,  4415,  4418,  4421,  4424,  4427,  4433,  4436,  4439,  4442,
    4445,  4448,  4454,  4457,  4460,  4463,  4466,  4469,  4475,  4478,
    4481,  4484,  4487,  4490,  4496,  4499,  4502,  4505,  4508,  4511,
    4517,  4520,  4523,  4526,  4529,  4532,  4538,  4541,  4544,  4547,
    4550,  4553,  4559,  4562,  4565,  4568,  4571,  4574,  4580,  4583,
    4586,  4589,  4592,  4595,  4601,  4604,  4607,  4610,  4613,  4616,
    4622,  4626,  4630,  4634,  4638,  4642,  4649,  4653,  4657,  4661,
    4665,  4669,  4676,  4680,  4684,  4688,  4692,  4696,  4703,  4707,
    4711,  4715,  4719,  4723,  4730,  4734,  4738,  4742,  4746,  4750,
    4757,  4761,  4765,  4769,  4773,  4777,  4784,  4788,  4792,  4796,
    4800,  4804,  4811,  4815,  4819,  4823,  4827,  4831,  4838,  4841,
    4844,  4854,  4864,  4871,  4881,  4884,  4887,  4897,  4907,  4914,
    4924,  4927,  4930,  4940,  4950,  4957,  4967,  4970,  4973,  4983,
    4993,  5000,  5010,  5013,  5016,  5026,  5036,  5043,  5053,  5056,
    5059,  5069,  5079,  5086,  5096,  5099,  5102,  5112,  5122,  5129,
    5139,  5142,  5145,  5155,  5165,  5172,  5182,  5185,  5188,  5198,
    5208,  5215,  5225,  5228,  5231,  5241,  5251,  5258,  5268,  5271,
    5274,  5285,  5296,  5306,  5319,  5322,  5325,  5328,  5331,  5334,
    5340,  5343,  5346,  5349,  5352,  5355,  5361,  5364,  5367,  5370,
    5373,  5376,  5382,  5390,  5398,  5406,  5414,  5422,  5433,  5441,
    5449,  5457,  5465,  5473,  5484,  5492,  5500,  5508,  5516,  5524,
    5535,  5543,  5551,  5559,  5567,  5575,  5586,  5594,  5602,  5610,
    5618,  5626,  5637,  5645,  5653,  5661,  5669,  5677,  5688,  5696,
    5704,  5712,  5720,  5728,  5739,  5747,  5755,  5763,  5771,  5779,
    5790,  5797,  5804,  5811,  5818,  5825,  5835,  5843,  5851,  5859,
    5867,  5875,  5886,  5894,  5902,  5910,  5918,  5926,  5937,  5945,
    5953,  5961,  5969,  5977,  5988,  5996,  6004,  6012,  6020,  6028,
    6039,  6046,  6053,  6060,  6067,  6074,  6084,  6091,  6098,  6105,
    6112,  6119,  6129,  6136,  6143,  6150,  6157,  6164,  6174,  6177,
    6180,  6191,  6202,  6212,  6225,  6235,  6248,  6258,  6271,  6274,
    6277,  6283,  6292,  6295,  6298,  6304,  6313,  6316,  6319,  6325,
    6334,  6337,  6340,  6346,  6355,  6358,  6361,  6367
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "FLOW_ANY_IP",
  "FLOW_ANY_MAC", "FLOW_ANY_PORT", "FLOW_APPLICATION",
  "FLOW_APPLICATION_CATEGORY", "FLOW_APPLICATION_CATEGORY_ID",
  "FLOW_CATEGORY", "FLOW_CATEGORY_ID", "FLOW_CONNTRACK_ID",
  "FLOW_CONNTRACK_MARK", "FLOW_CONNTRACK_REPLY_DST_IP",
  "FLOW_CONNTRACK_REPLY_SRC_IP", "FLOW_DETECTED_HOSTNAME",
  "FLOW_DETECTION_COMPLETE", "FLOW_DETECTION_GUESSED",
  "FLOW_DETECTION_INIT", "FLOW_DETECTION_UPDATED", "FLOW_DHC_HIT",
  "FLOW_DNS_HOSTNAME", "FLOW_DOMAIN_CATEGORY", "FLOW_DOMAIN_CATEGORY_ID",
  "FLOW_DST_IP", "FLOW_DST_MAC", "FLOW_DST_NETWORK_CATEGORY",
  "FLOW_DST_NETWORK_CATEGORY_ID", "FLOW_DST_PORT", "FLOW_EXPIRED",
  "FLOW_EXPIRING", "FLOW_FHC_HIT", "FLOW_IFACE", "FLOW_IFACE_NFQ_DST",
  "FLOW_IFACE_NFQ_SRC", "FLOW_IP_DSCP", "FLOW_IP_NAT", "FLOW_IP_PROTO",
  "FLOW_IP_VERSION", "FLOW_LOCAL_IP", "FLOW_LOCAL_MAC",
  "FLOW_LOCAL_NETWORK_CATEGORY", "FLOW_LOCAL_NETWORK_CATEGORY_ID",
  "FLOW_LOCAL_PORT", "FLOW_NDPI_RISK_SCORE", "FLOW_NDPI_RISK_SCORE_CLIENT",
  "FLOW_NDPI_RISK_SCORE_SERVER", "FLOW_NETWORK_CATEGORY",
  "FLOW_NETWORK_CATEGORY_ID", "FLOW_ORIGIN", "FLOW_OTHER_IP",
  "FLOW_OTHER_MAC", "FLOW_OTHER_NETWORK_CATEGORY",
  "FLOW_OTHER_NETWORK_CATEGORY_ID", "FLOW_OTHER_PORT", "FLOW_OTHER_TYPE",
  "FLOW_PROTOCOL", "FLOW_PROTOCOL_CATEGORY", "FLOW_PROTOCOL_CATEGORY_ID",
  "FLOW_RISKS", "FLOW_SOFT_DISSECTOR", "FLOW_SRC_IP", "FLOW_SRC_MAC",
  "FLOW_SRC_NETWORK_CATEGORY", "FLOW_SRC_NETWORK_CATEGORY_ID",
  "FLOW_SRC_PORT", "FLOW_TAG", "FLOW_TAG_CATEGORY", "FLOW_TAG_CATEGORY_ID",
  "FLOW_TLS_CIPHER", "FLOW_TLS_ECH", "FLOW_TLS_JA4", "FLOW_TLS_VERSION",
  "FLOW_TUNNEL_TYPE", "FLOW_VLAN_ID", "FLOW_VLAN",
  "FLOW_LOCAL_IF_META_SSID", "FLOW_LOCAL_IF_META_PVID",
  "FLOW_OTHER_IF_META_PVID", "FLOW_APP_IP_OVERRIDE",
  "FLOW_APP_PROTO_TWINS", "FLOW_DHCP_CLASS_IDENT", "FLOW_DHCP_FINGERPRINT",
  "FLOW_DST_BYTES", "FLOW_DST_PACKETS", "FLOW_HTTP_URL",
  "FLOW_HTTP_USER_AGENT", "FLOW_LOCAL_BYTES", "FLOW_LOCAL_PACKETS",
  "FLOW_MDNS_DOMAIN_NAME", "FLOW_NFQ_DST_IFINDEX", "FLOW_NFQ_SRC_IFINDEX",
  "FLOW_OTHER_BYTES", "FLOW_OTHER_PACKETS", "FLOW_SRC_BYTES",
  "FLOW_SRC_PACKETS", "FLOW_SSH_CLIENT_AGENT", "FLOW_SSH_SERVER_AGENT",
  "FLOW_TCP_FIN_ACK", "FLOW_TCP_LAST_SEQ", "FLOW_TLS_ALPN",
  "FLOW_TLS_ISSUER_DN", "FLOW_TLS_SERVER_CN", "FLOW_TLS_SUBJECT_DN",
  "FLOW_TOTAL_BYTES", "FLOW_TOTAL_PACKETS", "FLOW_TS_FIRST_SEEN",
  "FLOW_TS_LAST_SEEN", "FLOW_TOTAL_LOCAL_BYTES", "FLOW_TOTAL_OTHER_BYTES",
  "FLOW_TOTAL_SRC_BYTES", "FLOW_TOTAL_DST_BYTES",
  "FLOW_TOTAL_LOCAL_PACKETS", "FLOW_TOTAL_OTHER_PACKETS",
  "FLOW_TOTAL_SRC_PACKETS", "FLOW_TOTAL_DST_PACKETS",
  "FLOW_DETECTION_PACKETS", "FLOW_LOCAL_RATE", "FLOW_OTHER_RATE",
  "FLOW_SRC_RATE", "FLOW_DST_RATE", "FLOW_TCP_SEQ_ERRORS",
  "FLOW_TCP_RESETS", "FLOW_TCP_RETRANS", "FLOW_TLS_CERT_FINGERPRINT",
  "FLOW_TLS_ALPN_SERVER", "FLOW_TLS_PROC_HELLO",
  "FLOW_TLS_PROC_CERTIFICATE", "FLOW_SMTP_TLS", "FLOW_BT_INFO_HASH",
  "FLOW_STUN_MAPPED", "FLOW_STUN_PEER", "FLOW_STUN_RELAYED",
  "FLOW_STUN_RESPONSE", "FLOW_STUN_OTHER", "FLOW_OTHER_UNKNOWN",
  "FLOW_OTHER_UNSUPPORTED", "FLOW_OTHER_LOCAL", "FLOW_OTHER_MULTICAST",
  "FLOW_OTHER_BROADCAST", "FLOW_OTHER_REMOTE", "FLOW_OTHER_ERROR",
  "FLOW_ORIGIN_LOCAL", "FLOW_ORIGIN_OTHER", "FLOW_ORIGIN_UNKNOWN",
  "FLOW_TUNNEL_NONE", "FLOW_TUNNEL_GTP", "FLOW_INTEL", "CMP_EQUAL",
  "CMP_NOTEQUAL", "CMP_GTHANEQUAL", "CMP_LTHANEQUAL", "BOOL_AND",
  "BOOL_OR", "VALUE_ADDR_IPMASK", "VALUE_TRUE", "VALUE_FALSE",
  "VALUE_ADDR_MAC", "VALUE_NAME", "VALUE_REGEX", "VALUE_ADDR_TAG",
  "VALUE_ADDR_IPV4", "VALUE_ADDR_IPV4_CIDR", "VALUE_ADDR_IPV6",
  "VALUE_ADDR_IPV6_CIDR", "VALUE_SIGNED", "VALUE_UNSIGNED", "VALUE_FLOAT",
  "';'", "'('", "')'", "'!'", "'>'", "'<'", "$accept", "exprs", "expr",
  "expr_ip_proto", "expr_ip_dscp", "expr_ip_version", "expr_vlan_id",
  "expr_vlan", "expr_local_if_metadata_ssid",
  "expr_local_if_metadata_pvid", "expr_other_if_metadata_pvid",
  "expr_other_type", "value_other_type", "expr_any_mac", "expr_local_mac",
  "expr_other_mac", "expr_src_mac", "expr_dst_mac", "expr_any_ip",
  "expr_local_ip", "expr_other_ip", "expr_src_ip", "expr_dst_ip",
  "expr_conntrack_reply_src_ip", "expr_conntrack_reply_dst_ip",
  "value_addr_ip", "expr_any_port", "expr_local_port", "expr_other_port",
  "expr_src_port", "expr_dst_port", "expr_tunnel_type",
  "value_tunnel_type", "expr_detection_complete", "expr_detection_guessed",
  "expr_detection_init", "expr_detection_updated", "expr_dhc_hit",
  "expr_fhc_hit", "expr_ip_nat", "expr_expiring", "expr_expired",
  "expr_soft_dissector", "expr_app", "expr_app_id", "expr_app_name",
  "expr_category", "expr_category_id", "expr_app_category",
  "expr_app_category_id", "expr_domain_category",
  "expr_domain_category_id", "expr_network_category",
  "expr_network_category_id", "expr_local_network_category",
  "expr_local_network_category_id", "expr_other_network_category",
  "expr_other_network_category_id", "expr_src_network_category",
  "expr_src_network_category_id", "expr_dst_network_category",
  "expr_dst_network_category_id", "expr_tag_category",
  "expr_tag_category_id", "expr_proto", "expr_proto_id", "expr_proto_name",
  "expr_proto_category", "expr_proto_category_id",
  "expr_detected_hostname", "expr_dns_hostname", "expr_risks",
  "expr_risk_ndpi_score", "expr_risk_ndpi_score_client",
  "expr_risk_ndpi_score_server", "expr_conntrack_id",
  "expr_conntrack_mark", "expr_iface", "expr_iface_nfq_src",
  "expr_iface_nfq_dst", "expr_intel", "expr_tls_version",
  "expr_tls_cipher", "expr_tls_ech", "expr_tls_ja4", "expr_origin",
  "value_origin_type", "expr_tag", "expr_app_ip_override",
  "expr_app_proto_twins", "expr_ts_first_seen", "expr_ts_last_seen",
  "expr_nfq_src_ifindex", "expr_nfq_dst_ifindex", "expr_tcp_fin_ack",
  "expr_tcp_last_seq", "expr_total_bytes", "expr_total_packets",
  "expr_local_bytes", "expr_other_bytes", "expr_src_bytes",
  "expr_dst_bytes", "expr_local_packets", "expr_other_packets",
  "expr_src_packets", "expr_dst_packets", "expr_http_user_agent",
  "expr_http_url", "expr_dhcp_fingerprint", "expr_dhcp_class_ident",
  "expr_ssh_client_agent", "expr_ssh_server_agent", "expr_tls_subject_dn",
  "expr_tls_issuer_dn", "expr_tls_server_cn", "expr_mdns_domain_name",
  "expr_tls_alpn", "expr_tls_proc_hello", "expr_tls_proc_certificate",
  "expr_smtp_tls", "expr_total_local_bytes", "expr_total_other_bytes",
  "expr_total_src_bytes", "expr_total_dst_bytes",
  "expr_total_local_packets", "expr_total_other_packets",
  "expr_total_src_packets", "expr_total_dst_packets",
  "expr_detection_packets", "expr_local_rate", "expr_other_rate",
  "expr_src_rate", "expr_dst_rate", "expr_tcp_seq_errors",
  "expr_tcp_resets", "expr_tcp_retrans", "expr_tls_alpn_server",
  "expr_tls_cert_fingerprint", "expr_bt_info_hash", "expr_stun_mapped",
  "expr_stun_peer", "expr_stun_relayed", "expr_stun_response",
  "expr_stun_other", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-277)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -277,   155,  -277,  -109,   -53,  -146,   -28,   147,   159,   208,
     218,  -142,  -135,   234,   238,   245,   249,   282,   293,   470,
     474,   478,   488,   518,   527,   531,   561,   565,  -131,   569,
     573,   577,   607,   611,   615,   619,   623,  -127,   653,   657,
     661,   669,   685,  -101,   -92,   -88,   -84,   695,   814,   816,
     818,   820,   822,   824,   -75,   826,   828,   830,   832,   834,
     836,   838,   840,   842,   844,   -49,   846,   848,   850,   -45,
     -40,   852,   -36,   854,   -32,   149,   856,   189,   241,   858,
     860,   862,   864,   256,   260,   866,   868,   267,   271,   870,
     448,   452,   456,   462,   466,   492,   872,   874,   496,   501,
     876,   878,   880,   882,   505,   509,   535,   539,   543,   547,
     551,   555,   581,   585,   589,   593,   597,   601,   627,   631,
     635,   639,   643,   647,   884,   886,   888,   890,   892,   894,
     896,   898,   900,   902,   904,   673,   461,   299,  -100,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,   130,   274,   -66,   -35,
    -156,  -122,   -96,   -83,   -59,   -27,  -130,  -115,  -133,   -24,
     -17,   135,   -14,   165,   168,   175,   180,   185,   194,   197,
     237,   246,   257,   279,   443,   465,   467,   482,   678,   687,
     692,   697,   655,   897,   903,   905,   907,   909,   911,   913,
     915,   917,   919,   921,   920,   922,   167,   512,   786,   891,
     702,   707,   -31,   -22,   970,  1037,  1031,  1033,  1035,  1038,
    1039,  1040,  1042,  1043,   927,   929,   931,   933,   935,   937,
     936,   938,   940,   942,   944,   946,  1044,  1045,   951,   953,
    1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,   712,   717,
     -10,    -9,  1062,  1063,  1056,  1057,  1058,  1059,  1060,  1061,
    1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,
    1092,  1093,  1086,  1087,   519,   519,   722,   727,   146,   152,
    1096,  1097,  1090,  1091,  1094,  1095,  1098,  1099,  1100,  1101,
     312,   312,   -89,   -80,  1104,  1105,  1102,  1103,   -79,   -76,
     955,   957,   732,   737,   190,   192,  1112,  1113,  1106,  1107,
    1108,  1109,  1110,  1111,  1114,  1115,  1120,  1121,  1124,  1125,
    1118,  1119,  1122,  1123,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,   956,   958,  1136,  1137,  1138,  1139,
    1140,  1141,   973,   973,  1142,  1143,  1144,  1145,  1146,  1147,
    1148,  1149,  1150,  1151,  1152,  1153,   962,   964,  1154,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
     969,   971,   974,   976,   975,   977,   979,   981,  1166,  1167,
    1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,
     983,   985,   987,   989,  1178,  1179,  1180,  1181,  1182,  1183,
    1184,  1185,  1186,  1187,  1188,  1189,   991,   993,  1190,  1191,
    1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,
    1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1225,   995,   997,   999,  1001,  1226,  1227,
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,
    1003,  1005,  1007,  1009,  1011,  1013,  1015,  1017,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,
    1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,
    1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,
    1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,
    1310,  1311,  1312,  1313,  1314,  1315,   660,   887,  1029,  1316,
    1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1326,
    1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,
    1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,  1364,  1365,
    1019,  1021,  1026,  1028,  1030,  1032,  1034,  1036,  1366,  1367,
     742,   747,   752,   757,   762,   767,   772,   777,   782,   787,
     -13,   670,   665,   788,   791,   794,  -141,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,   461,   461,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  1041,  1041
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,     0,     0,   253,   359,     0,     0,     0,
       0,   457,   465,     0,     0,   415,   299,   305,   311,   317,
     323,   421,     0,     0,     0,     0,     0,     0,   285,   347,
     341,   329,   473,   485,   479,     0,   335,   140,     0,     0,
       0,     0,     0,   261,   433,   441,   449,     0,     0,   549,
       0,     0,     0,     0,   269,   190,   403,     0,     0,   427,
     353,     0,     0,     0,     0,   277,   556,     0,     0,   527,
     535,   543,   519,   293,   152,   160,   168,   174,   182,   560,
     566,   686,   680,     0,     0,   674,   668,     0,     0,   722,
       0,     0,     0,     0,     0,     0,   692,   698,     0,     0,
     728,   710,   716,   704,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   848,   734,   740,   746,     0,
       0,     0,     0,     0,     0,   491,     0,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    99,   361,   362,    97,
      98,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   405,   406,
     117,   118,   119,   120,   121,   122,   123,   124,   130,   131,
     132,   133,   134,   135,   125,   126,   127,   128,   129,   136,
      12,    13,    39,    40,    24,    23,    31,    32,    37,    38,
      20,    25,    27,    16,    21,    26,    28,    17,    19,    18,
      15,    14,    29,    30,    36,    34,    35,    22,    33,    59,
      60,    61,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    58,    57,
      62,    63,    64,    65,    66,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   254,   360,   458,
     466,   416,   300,   306,   312,   318,   324,   422,   286,   348,
     342,   330,   474,   486,   480,   336,   141,   262,   434,   442,
     450,   550,   270,   191,   404,   428,   354,   278,   557,   528,
     536,   544,   520,   294,   153,   161,   169,   175,   183,   561,
     567,   687,   681,   675,   669,   723,   693,   699,   729,   711,
     717,   705,   849,   735,   741,   747,   492,     0,     0,     3,
     223,   249,   250,   251,   252,   221,   224,   222,   201,   203,
     202,   204,   255,   256,   257,   258,   259,   260,   365,   363,
     366,   364,   371,   372,   373,   374,   367,   368,   369,   370,
     459,   460,   461,   462,   463,   464,   467,   468,   469,   470,
     471,   472,   247,   245,   248,   246,   243,   241,   244,   242,
     417,   419,   418,   420,   301,   302,   303,   304,   307,   308,
     309,   310,   313,   314,   315,   316,   319,   320,   321,   322,
     325,   326,   327,   328,   423,   425,   424,   426,   375,   376,
     377,   378,   239,   237,   240,   238,   217,   219,   218,   220,
     395,   396,   397,   398,   287,   288,   289,   290,   291,   292,
     349,   350,   351,   352,   343,   344,   345,   346,   331,   332,
     333,   334,   475,   477,   476,   478,   487,   489,   488,   490,
     481,   483,   482,   484,   148,   149,   337,   338,   339,   340,
     142,   143,   144,   145,   146,   147,   150,   151,   227,   225,
     228,   226,   205,   207,   206,   208,   383,   384,   385,   386,
     263,   264,   265,   266,   267,   268,   435,   436,   437,   438,
     439,   440,   443,   444,   445,   446,   447,   448,   451,   452,
     453,   454,   455,   456,   379,   380,   381,   382,   553,   554,
     555,   551,   552,   231,   229,   232,   230,   209,   211,   210,
     212,   387,   388,   389,   390,   271,   272,   273,   274,   275,
     276,   194,   195,   196,   197,   198,   199,   200,   192,   193,
     409,   407,   410,   408,   411,   412,   413,   414,   429,   431,
     430,   432,   355,   356,   357,   358,   235,   233,   236,   234,
     213,   215,   214,   216,   391,   392,   393,   394,   279,   280,
     281,   282,   283,   284,   558,   559,   399,   400,   401,   402,
     529,   530,   531,   532,   533,   534,   537,   538,   539,   540,
     541,   542,   545,   547,   546,   548,   521,   522,   523,   524,
     525,   526,   297,   298,   295,   296,   154,   155,   156,   157,
     158,   159,   162,   163,   164,   165,   166,   167,   170,   172,
     171,   173,   176,   177,   178,   179,   180,   181,   184,   185,
     186,   187,   188,   189,   562,   563,   564,   565,   568,   569,
     570,   571,   688,   690,   689,   691,   682,   684,   683,   685,
     638,   639,   640,   641,   642,   643,   662,   663,   664,   665,
     666,   667,   676,   678,   677,   679,   670,   672,   671,   673,
     620,   621,   622,   623,   624,   625,   644,   645,   646,   647,
     648,   649,   724,   726,   725,   727,   590,   591,   592,   593,
     594,   595,   584,   585,   586,   587,   588,   589,   626,   627,
     628,   629,   630,   631,   650,   651,   652,   653,   654,   655,
     632,   633,   634,   635,   636,   637,   656,   657,   658,   659,
     660,   661,   694,   696,   695,   697,   700,   702,   701,   703,
     596,   597,   598,   599,   600,   601,   602,   603,   604,   605,
     606,   607,   730,   732,   731,   733,   712,   714,   713,   715,
     718,   720,   719,   721,   706,   708,   707,   709,   608,   609,
     610,   611,   612,   613,   614,   615,   616,   617,   618,   619,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   752,   753,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
     830,   831,   832,   833,   834,   835,   836,   837,   838,   839,
     840,   841,   842,   843,   844,   845,   846,   847,   854,   855,
     850,   852,   851,   853,   736,   737,   738,   739,   742,   743,
     744,   745,   748,   749,   750,   751,   856,   857,   860,   858,
     861,   859,   864,   862,   865,   863,   868,   866,   869,   867,
     872,   870,   873,   871,   876,   874,   877,   875,   498,   499,
     493,   494,   496,   495,   497,   505,   506,   500,   501,   503,
     502,   504,   508,   507,   509,   511,   510,   512,   514,   513,
     515,   517,   516,   518,   139,   138,   137
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -277,  -277,  -136,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -108,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -276,  -277,  -277,  -277,  -277,
    -277,  -277,   725,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,   795,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,  1018,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   815,   160,   161,   162,   163,
     164,   165,  1084,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   991,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     746,   817,   280,   281,   282,   283,   296,   297,   298,   299,
     822,   807,   808,   302,   303,   304,   305,   338,   339,   340,
     341,   360,   361,   362,   363,   832,   284,   285,   828,  1424,
     300,   301,   853,   855,   857,   859,   829,   306,   307,   276,
     277,   342,   343,   830,   823,   364,   365,   376,   377,   378,
     379,   831,   807,   808,   893,   895,   382,   383,   384,   385,
     388,   389,   390,   391,   394,   395,   396,   397,   809,  1020,
     824,   380,   381,   414,   415,   416,   417,  1021,  1022,  1028,
     386,   387,  1030,   825,   392,   393,  1023,  1029,   398,   399,
    1031,   818,   949,   951,   819,   278,   279,   418,   419,   440,
     441,   442,   443,   452,   453,   454,   455,   826,   458,   459,
     460,   461,   466,   467,   468,   469,   474,   475,   476,   477,
     286,   287,   820,   444,   445,   821,   896,   456,   457,   897,
     994,   996,   462,   463,   833,   898,   470,   471,   899,   827,
     478,   479,  1398,  1399,   836,  1400,  1401,   952,   954,   834,
     953,   955,  1402,  1403,  1404,     2,  1037,  1039,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     810,   811,   812,   813,   814,   288,   289,   480,   481,   482,
     483,   835,   135,   997,   747,   748,   998,   290,   291,   999,
     749,   750,  1000,  1019,   751,   752,   753,   754,   755,   756,
     757,   484,   485,   837,   136,   888,   137,   758,   759,   760,
     761,   762,   763,   764,   838,   765,   766,   488,   489,   490,
     491,   839,   767,   768,   769,   770,   840,  1040,   771,  1042,
    1041,   841,  1043,   772,   773,   774,   292,   293,   775,   776,
     842,   492,   493,   843,   777,   778,   294,   295,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,   790,
     791,   792,   308,   309,   793,   794,   310,   311,   795,   494,
     495,   496,   497,   312,   313,   796,   797,   314,   315,   798,
     799,   800,   801,   844,   508,   509,   510,   511,   514,   515,
     516,   517,   845,   498,   499,   524,   525,   526,   527,   530,
     531,   532,   533,   846,   802,   803,   804,   805,   512,   513,
     316,   317,   518,   519,   816,   811,   812,   813,   814,   528,
     529,   318,   319,   534,   535,   847,   806,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1379,  1381,  1383,  1385,  1387,  1389,
    1391,  1393,  1395,  1397,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   538,   539,   540,   541,
     544,   545,   546,   547,   550,   551,   552,   553,   135,   848,
     556,   557,   558,   559,   562,   563,   564,   565,   320,   321,
     542,   543,   322,   323,   548,   549,   324,   325,   554,   555,
     136,   849,   137,   850,   560,   561,   326,   327,   566,   567,
     568,   569,   570,   571,   578,   579,   580,   581,   851,   584,
     585,   586,   587,   598,   599,   600,   601,   604,   605,   606,
     607,   988,   989,   990,   572,   573,   328,   329,   582,   583,
     889,  1425,  1426,   588,   589,   330,   331,   602,   603,   332,
     333,   608,   609,   610,   611,   612,   613,   616,   617,   618,
     619,   622,   623,   624,   625,   628,   629,   630,   631,   634,
     635,   636,   637,   640,   641,   642,   643,   614,   615,   334,
     335,   620,   621,   336,   337,   626,   627,   344,   345,   632,
     633,   346,   347,   638,   639,   348,   349,   644,   645,   646,
     647,   648,   649,   652,   653,   654,   655,   658,   659,   660,
     661,   664,   665,   666,   667,   670,   671,   672,   673,   676,
     677,   678,   679,   650,   651,   350,   351,   656,   657,   352,
     353,   662,   663,   354,   355,   668,   669,   356,   357,   674,
     675,   358,   359,   680,   681,   682,   683,   684,   685,   688,
     689,   690,   691,   694,   695,   696,   697,   700,   701,   702,
     703,   706,   707,   708,   709,   712,   713,   714,   715,   686,
     687,   366,   367,   692,   693,   368,   369,   698,   699,   370,
     371,   704,   705,   860,   861,   710,   711,   372,   373,   716,
     717,   740,   741,   742,   743,  1405,  1406,  1316,  1407,  1408,
    1412,  1413,  1414,   374,   375,  1409,  1410,  1411,   852,   811,
     812,   813,   814,   400,   401,   744,   745,   854,   811,   812,
     813,   814,   856,   811,   812,   813,   814,   858,   811,   812,
     813,   814,   892,   811,   812,   813,   814,   894,   811,   812,
     813,   814,   948,   811,   812,   813,   814,   950,   811,   812,
     813,   814,   993,   811,   812,   813,   814,   995,   811,   812,
     813,   814,  1036,   811,   812,   813,   814,  1038,   811,   812,
     813,   814,  1378,   811,   812,   813,   814,  1380,   811,   812,
     813,   814,  1382,   811,   812,   813,   814,  1384,   811,   812,
     813,   814,  1386,   811,   812,   813,   814,  1388,   811,   812,
     813,   814,  1390,   811,   812,   813,   814,  1392,   811,   812,
     813,   814,  1394,   811,   812,   813,   814,  1396,   811,   812,
     813,   814,   890,  1415,  1416,  1417,  1418,  1419,  1420,  1421,
    1422,  1423,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   446,   447,   448,   449,   450,   451,
     464,   465,   472,   473,   486,   487,   500,   501,   502,   503,
     504,   505,   506,   507,   520,   521,   522,   523,   536,   537,
     574,   575,   576,   577,   590,   591,   592,   593,   594,   595,
     596,   597,   718,   719,   720,   721,   722,   723,   724,   725,
     726,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,  1317,   862,   863,   891,   864,   865,
     866,   867,   868,   869,   870,   871,   872,   873,   874,   875,
     876,   877,   878,   879,   880,   881,   882,   883,   884,   885,
     886,   887,   910,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     928,   929,   930,   931,   932,   933,   936,   937,   938,   939,
    1032,  1033,  1034,  1035,  1072,  1073,  1074,  1075,  1082,  1083,
    1098,  1099,  1100,  1101,  1114,  1115,  1116,  1117,   900,  1118,
    1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,
    1129,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1162,
    1163,  1164,  1165,  1202,  1203,  1204,  1205,  1206,  1207,  1208,
    1209,  1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,
    1231,  1232,  1233,  1234,  1235,  1236,  1237,  1360,  1361,  1362,
    1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,  1372,
    1373,  1374,  1375,   807,   808,   901,  1318,   902,  1085,   903,
     992,   904,     0,     0,   905,   906,   907,     0,   908,   909,
     934,   935,   940,   941,   942,   943,   944,   945,   946,   947,
     956,   957,   958,   959,   960,   961,   962,   963,     0,     0,
     964,   965,   966,   967,   968,   969,   970,   971,   972,   973,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,  1001,  1002,  1003,  1004,     0,     0,
    1005,  1006,  1024,  1025,  1007,  1008,  1009,  1010,  1026,  1027,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1054,  1055,
    1052,  1053,  1056,  1057,  1058,  1059,     0,     0,  1060,  1061,
       0,     0,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,
    1070,  1071,  1076,  1077,  1078,  1079,  1080,  1081,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1140,  1141,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,
    1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,  1189,
    1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,
    1200,  1201,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,  1221,  1238,  1239,  1240,  1241,  1242,  1243,
    1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,
    1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,
    1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,
    1314,  1315,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,
    1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,
    1356,  1357,  1358,  1359,  1376,  1377
};

static const yytype_int16 yycheck[] =
{
     136,   277,   148,   149,   150,   151,   148,   149,   150,   151,
     166,   152,   153,   148,   149,   150,   151,   148,   149,   150,
     151,   148,   149,   150,   151,   158,   172,   173,   158,   170,
     172,   173,   308,   309,   310,   311,   166,   172,   173,   148,
     149,   172,   173,   158,   166,   172,   173,   148,   149,   150,
     151,   166,   152,   153,   330,   331,   148,   149,   150,   151,
     148,   149,   150,   151,   148,   149,   150,   151,   168,   158,
     166,   172,   173,   148,   149,   150,   151,   166,   158,   158,
     172,   173,   158,   166,   172,   173,   166,   166,   172,   173,
     166,   157,   368,   369,   160,   148,   149,   172,   173,   148,
     149,   150,   151,   148,   149,   150,   151,   166,   148,   149,
     150,   151,   148,   149,   150,   151,   148,   149,   150,   151,
     148,   149,   157,   172,   173,   160,   157,   172,   173,   160,
     406,   407,   172,   173,   158,   157,   172,   173,   160,   166,
     172,   173,   155,   156,   158,   158,   159,   157,   157,   166,
     160,   160,   165,   166,   167,     0,   432,   433,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     160,   161,   162,   163,   164,   148,   149,   148,   149,   150,
     151,   166,   147,   157,     5,     6,   160,   148,   149,   157,
      11,    12,   160,   421,    15,    16,    17,    18,    19,    20,
      21,   172,   173,   158,   169,   158,   171,    28,    29,    30,
      31,    32,    33,    34,   166,    36,    37,   148,   149,   150,
     151,   166,    43,    44,    45,    46,   166,   157,    49,   157,
     160,   166,   160,    54,    55,    56,   148,   149,    59,    60,
     166,   172,   173,   166,    65,    66,   148,   149,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   148,   149,    85,    86,   148,   149,    89,   148,
     149,   150,   151,   148,   149,    96,    97,   148,   149,   100,
     101,   102,   103,   166,   148,   149,   150,   151,   148,   149,
     150,   151,   166,   172,   173,   148,   149,   150,   151,   148,
     149,   150,   151,   166,   125,   126,   127,   128,   172,   173,
     148,   149,   172,   173,   160,   161,   162,   163,   164,   172,
     173,   148,   149,   172,   173,   166,   147,   135,   136,   137,
     138,   139,   140,   141,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   148,   149,   150,   151,
     148,   149,   150,   151,   148,   149,   150,   151,   147,   166,
     148,   149,   150,   151,   148,   149,   150,   151,   148,   149,
     172,   173,   148,   149,   172,   173,   148,   149,   172,   173,
     169,   166,   171,   166,   172,   173,   148,   149,   172,   173,
     148,   149,   150,   151,   148,   149,   150,   151,   166,   148,
     149,   150,   151,   148,   149,   150,   151,   148,   149,   150,
     151,   142,   143,   144,   172,   173,   148,   149,   172,   173,
     158,   807,   808,   172,   173,   148,   149,   172,   173,   148,
     149,   172,   173,   148,   149,   150,   151,   148,   149,   150,
     151,   148,   149,   150,   151,   148,   149,   150,   151,   148,
     149,   150,   151,   148,   149,   150,   151,   172,   173,   148,
     149,   172,   173,   148,   149,   172,   173,   148,   149,   172,
     173,   148,   149,   172,   173,   148,   149,   172,   173,   148,
     149,   150,   151,   148,   149,   150,   151,   148,   149,   150,
     151,   148,   149,   150,   151,   148,   149,   150,   151,   148,
     149,   150,   151,   172,   173,   148,   149,   172,   173,   148,
     149,   172,   173,   148,   149,   172,   173,   148,   149,   172,
     173,   148,   149,   172,   173,   148,   149,   150,   151,   148,
     149,   150,   151,   148,   149,   150,   151,   148,   149,   150,
     151,   148,   149,   150,   151,   148,   149,   150,   151,   172,
     173,   148,   149,   172,   173,   148,   149,   172,   173,   148,
     149,   172,   173,   158,   159,   172,   173,   148,   149,   172,
     173,   148,   149,   150,   151,   155,   156,   167,   158,   159,
     165,   166,   167,   148,   149,   165,   166,   167,   160,   161,
     162,   163,   164,   148,   149,   172,   173,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   160,   161,   162,   163,   164,   160,   161,   162,
     163,   164,   166,   165,   166,   167,   165,   166,   167,   165,
     166,   167,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   167,   158,   159,   166,   155,   156,
     155,   156,   155,   156,   155,   156,   155,   156,   155,   156,
     155,   156,   155,   156,   155,   156,   155,   156,   158,   159,
     158,   159,   155,   156,   155,   156,   155,   156,   155,   156,
     155,   156,   155,   156,   158,   159,   158,   159,   158,   159,
     158,   159,   158,   159,   158,   159,   155,   156,   155,   156,
     155,   156,   155,   156,   158,   159,   158,   159,   145,   146,
     158,   159,   158,   159,   155,   156,   155,   156,   158,   155,
     156,   155,   156,   158,   159,   158,   159,   158,   159,   158,
     159,   158,   159,   158,   159,   158,   159,   158,   159,   158,
     159,   158,   159,   158,   159,   158,   159,   158,   159,   158,
     159,   158,   159,   158,   159,   158,   159,   158,   159,   158,
     159,   158,   159,   158,   159,   158,   159,   158,   159,   158,
     159,   155,   156,   155,   156,   155,   156,   155,   156,   155,
     156,   155,   156,   152,   153,   158,   167,   166,   473,   166,
     405,   166,    -1,    -1,   166,   166,   166,    -1,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     158,   158,   166,   166,   166,   166,   166,   166,    -1,    -1,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     158,   158,   166,   166,   158,   158,   166,   166,    -1,    -1,
     166,   166,   158,   158,   166,   166,   166,   166,   166,   166,
     158,   158,   166,   166,   166,   166,   166,   166,   158,   158,
     166,   166,   158,   158,   166,   166,    -1,    -1,   166,   166,
      -1,    -1,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,    -1,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   158,   158,   158,   158
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   175,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   147,   169,   171,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     200,   201,   202,   203,   204,   205,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   148,   149,   148,   149,
     148,   149,   150,   151,   172,   173,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     150,   151,   172,   173,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   150,   151,   172,   173,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   150,   151,   172,   173,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   150,   151,   172,   173,   148,   149,   148,   149,
     148,   149,   148,   149,   150,   151,   172,   173,   148,   149,
     150,   151,   172,   173,   148,   149,   148,   149,   150,   151,
     172,   173,   148,   149,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   148,   149,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     150,   151,   172,   173,   148,   149,   148,   149,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     150,   151,   172,   173,   148,   149,   150,   151,   172,   173,
     148,   149,   150,   151,   172,   173,   148,   149,   150,   151,
     172,   173,   148,   149,   150,   151,   172,   173,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   148,   149,   148,   149,   148,   149,   148,   149,
     148,   149,   150,   151,   172,   173,   176,     5,     6,    11,
      12,    15,    16,    17,    18,    19,    20,    21,    28,    29,
      30,    31,    32,    33,    34,    36,    37,    43,    44,    45,
      46,    49,    54,    55,    56,    59,    60,    65,    66,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    85,    86,    89,    96,    97,   100,   101,
     102,   103,   125,   126,   127,   128,   147,   152,   153,   168,
     160,   161,   162,   163,   164,   199,   160,   199,   157,   160,
     157,   160,   166,   166,   166,   166,   166,   166,   158,   166,
     158,   166,   158,   158,   166,   166,   158,   158,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   160,   199,   160,   199,   160,   199,   160,   199,
     158,   159,   158,   159,   155,   156,   155,   156,   155,   156,
     155,   156,   155,   156,   155,   156,   155,   156,   155,   156,
     155,   156,   155,   156,   158,   159,   158,   159,   158,   158,
     166,   166,   160,   199,   160,   199,   157,   160,   157,   160,
     158,   158,   166,   166,   166,   166,   166,   166,   166,   166,
     155,   156,   155,   156,   155,   156,   155,   156,   155,   156,
     155,   156,   158,   159,   158,   159,   158,   159,   158,   159,
     158,   159,   158,   159,   166,   166,   155,   156,   155,   156,
     166,   166,   166,   166,   166,   166,   166,   166,   160,   199,
     160,   199,   157,   160,   157,   160,   158,   158,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   158,   158,   166,   166,   142,   143,
     144,   260,   260,   160,   199,   160,   199,   157,   160,   157,
     160,   158,   158,   166,   166,   166,   166,   166,   166,   166,
     166,   135,   136,   137,   138,   139,   140,   141,   186,   186,
     158,   166,   158,   166,   158,   158,   166,   166,   158,   166,
     158,   166,   155,   156,   155,   156,   160,   199,   160,   199,
     157,   160,   157,   160,   158,   158,   166,   166,   166,   166,
     166,   166,   166,   166,   158,   158,   158,   158,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   158,   159,   158,   159,   166,   166,   166,   166,
     166,   166,   145,   146,   206,   206,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   158,   159,
     158,   159,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   155,   156,   155,   156,   155,   156,
     155,   156,   158,   159,   158,   159,   158,   159,   158,   159,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   158,   159,   158,   159,   158,   159,   158,   159,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   158,   159,   158,   159,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   158,   159,   158,   159,   158,   159,   158,   159,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   158,   159,   158,   159,   158,   159,   158,   159,
     158,   159,   158,   159,   158,   159,   158,   159,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   158,   158,
     158,   159,   158,   159,   155,   156,   155,   156,   155,   156,
     155,   156,   155,   156,   155,   156,   158,   158,   160,   199,
     160,   199,   160,   199,   160,   199,   160,   199,   160,   199,
     160,   199,   160,   199,   160,   199,   160,   199,   155,   156,
     158,   159,   165,   166,   167,   155,   156,   158,   159,   165,
     166,   167,   165,   166,   167,   165,   166,   167,   165,   166,
     167,   165,   166,   167,   170,   176,   176
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   174,   175,   175,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     177,   177,   177,   177,   177,   177,   177,   177,   178,   178,
     179,   179,   180,   180,   180,   180,   180,   180,   180,   180,
     181,   181,   181,   181,   181,   181,   181,   181,   182,   182,
     182,   182,   182,   182,   183,   183,   183,   183,   183,   183,
     183,   183,   184,   184,   184,   184,   184,   184,   184,   184,
     185,   185,   185,   185,   186,   186,   186,   186,   186,   186,
     186,   187,   187,   187,   187,   188,   188,   188,   188,   189,
     189,   189,   189,   190,   190,   190,   190,   191,   191,   191,
     191,   192,   192,   192,   192,   193,   193,   193,   193,   194,
     194,   194,   194,   195,   195,   195,   195,   196,   196,   196,
     196,   197,   197,   197,   197,   198,   198,   198,   198,   199,
     199,   199,   199,   200,   200,   200,   200,   200,   200,   200,
     200,   201,   201,   201,   201,   201,   201,   201,   201,   202,
     202,   202,   202,   202,   202,   202,   202,   203,   203,   203,
     203,   203,   203,   203,   203,   204,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   205,   205,   206,   206,   207,
     207,   207,   207,   207,   207,   208,   208,   208,   208,   208,
     208,   209,   209,   209,   209,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   211,   211,   211,   211,   212,
     212,   212,   212,   212,   212,   213,   213,   213,   213,   213,
     213,   214,   214,   214,   214,   214,   214,   215,   215,   215,
     215,   215,   215,   216,   216,   216,   216,   216,   216,   217,
     217,   217,   217,   218,   218,   219,   219,   220,   220,   221,
     221,   222,   222,   223,   223,   224,   224,   225,   225,   226,
     226,   227,   227,   228,   228,   229,   229,   230,   230,   231,
     231,   232,   232,   233,   233,   234,   234,   235,   235,   236,
     236,   237,   237,   238,   238,   238,   238,   239,   239,   240,
     240,   241,   241,   242,   242,   243,   243,   243,   243,   243,
     243,   244,   244,   244,   244,   244,   244,   245,   245,   245,
     245,   245,   245,   246,   246,   246,   246,   246,   246,   246,
     246,   247,   247,   247,   247,   247,   247,   247,   247,   248,
     248,   248,   248,   248,   248,   248,   248,   249,   249,   249,
     249,   249,   249,   249,   249,   250,   250,   250,   250,   250,
     250,   250,   250,   251,   251,   251,   251,   251,   251,   252,
     252,   252,   252,   252,   252,   253,   253,   253,   253,   253,
     253,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   255,
     255,   255,   255,   255,   255,   255,   255,   256,   256,   256,
     256,   256,   256,   256,   256,   257,   257,   257,   257,   257,
     257,   257,   257,   258,   258,   258,   258,   258,   258,   259,
     259,   259,   259,   260,   260,   260,   261,   261,   261,   261,
     262,   262,   262,   262,   262,   262,   263,   263,   263,   263,
     263,   263,   264,   264,   264,   264,   264,   264,   265,   265,
     265,   265,   265,   265,   266,   266,   266,   266,   266,   266,
     267,   267,   267,   267,   267,   267,   268,   268,   268,   268,
     268,   268,   269,   269,   269,   269,   269,   269,   270,   270,
     270,   270,   270,   270,   271,   271,   271,   271,   271,   271,
     272,   272,   272,   272,   272,   272,   273,   273,   273,   273,
     273,   273,   274,   274,   274,   274,   274,   274,   275,   275,
     275,   275,   275,   275,   276,   276,   276,   276,   276,   276,
     277,   277,   277,   277,   277,   277,   278,   278,   278,   278,
     278,   278,   279,   279,   279,   279,   279,   279,   280,   280,
     280,   280,   280,   280,   281,   281,   281,   281,   281,   281,
     282,   282,   282,   282,   282,   282,   283,   283,   283,   283,
     283,   283,   284,   284,   284,   284,   284,   284,   285,   285,
     285,   285,   285,   285,   286,   286,   286,   286,   286,   286,
     287,   287,   287,   287,   287,   287,   288,   288,   288,   288,
     288,   288,   289,   289,   289,   289,   289,   289,   290,   290,
     290,   290,   290,   290,   291,   291,   291,   291,   291,   291,
     292,   292,   292,   292,   292,   292,   293,   293,   293,   293,
     293,   293,   294,   294,   294,   294,   294,   294,   295,   295,
     295,   295,   295,   295,   296,   296,   296,   296,   296,   296,
     297,   297,   297,   297,   297,   297,   298,   298,   298,   298,
     298,   298,   299,   299,   299,   299,   299,   299,   300,   300,
     300,   300,   300,   300,   301,   301,   301,   301,   301,   301,
     302,   302,   302,   302,   302,   302,   303,   303,   303,   303,
     303,   303,   304,   304,   304,   304,   304,   304,   305,   305,
     305,   305,   305,   305,   306,   306,   306,   306,   306,   306,
     307,   307,   307,   307,   307,   307,   308,   308,   308,   308,
     308,   308,   309,   309,   309,   309,   309,   309,   310,   310,
     310,   310,   310,   310,   311,   311,   312,   312,   313,   313,
     313,   313,   314,   314,   314,   314,   315,   315,   315,   315,
     316,   316,   316,   316,   317,   317,   317,   317
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     2,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     3,     3,     1,     2,     3,     3,     1,     1,     1,
       2,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     2,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     3,     3,     1,     1,     1,     1,     2,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, yyscan_t scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 137: /* expr: expr BOOL_OR expr  */
#line 422 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) || (yyvsp[0].bool_result)));
        _NDFP_debugf("OR (%d || %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 3167 "nd-flow-expr.cpp"
    break;

  case 138: /* expr: expr BOOL_AND expr  */
#line 426 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) && (yyvsp[0].bool_result)));
        _NDFP_debugf("AND (%d && %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 3176 "nd-flow-expr.cpp"
    break;

  case 139: /* expr: '(' expr ')'  */
#line 430 "nd-flow-expr.ypp"
                   { _NDFP_result = ((yyval.bool_result) = (yyvsp[-1].bool_result)); }
#line 3182 "nd-flow-expr.cpp"
    break;

  case 140: /* expr_ip_proto: FLOW_IP_PROTO  */
#line 434 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != 0));
        _NDFP_debugf(
            "IP Protocol is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3192 "nd-flow-expr.cpp"
    break;

  case 141: /* expr_ip_proto: '!' FLOW_IP_PROTO  */
#line 439 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == 0));
        _NDFP_debugf("IP Protocol is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3201 "nd-flow-expr.cpp"
    break;

  case 142: /* expr_ip_proto: FLOW_IP_PROTO CMP_EQUAL VALUE_UNSIGNED  */
#line 443 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3210 "nd-flow-expr.cpp"
    break;

  case 143: /* expr_ip_proto: FLOW_IP_PROTO CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 447 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3219 "nd-flow-expr.cpp"
    break;

  case 144: /* expr_ip_proto: FLOW_IP_PROTO CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 451 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol >= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3228 "nd-flow-expr.cpp"
    break;

  case 145: /* expr_ip_proto: FLOW_IP_PROTO CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 455 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol <= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3237 "nd-flow-expr.cpp"
    break;

  case 146: /* expr_ip_proto: FLOW_IP_PROTO '>' VALUE_UNSIGNED  */
#line 459 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol > (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3246 "nd-flow-expr.cpp"
    break;

  case 147: /* expr_ip_proto: FLOW_IP_PROTO '<' VALUE_UNSIGNED  */
#line 463 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol < (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3255 "nd-flow-expr.cpp"
    break;

  case 148: /* expr_ip_dscp: FLOW_IP_DSCP CMP_EQUAL VALUE_UNSIGNED  */
#line 470 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3264 "nd-flow-expr.cpp"
    break;

  case 149: /* expr_ip_dscp: FLOW_IP_DSCP CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 474 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3273 "nd-flow-expr.cpp"
    break;

  case 150: /* expr_ip_version: FLOW_IP_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 481 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3282 "nd-flow-expr.cpp"
    break;

  case 151: /* expr_ip_version: FLOW_IP_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 485 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3291 "nd-flow-expr.cpp"
    break;

  case 152: /* expr_vlan_id: FLOW_VLAN_ID  */
#line 492 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0));
        _NDFP_debugf("VLAN ID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3300 "nd-flow-expr.cpp"
    break;

  case 153: /* expr_vlan_id: '!' FLOW_VLAN_ID  */
#line 496 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0));
        _NDFP_debugf("VLAN ID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3309 "nd-flow-expr.cpp"
    break;

  case 154: /* expr_vlan_id: FLOW_VLAN_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 500 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3318 "nd-flow-expr.cpp"
    break;

  case 155: /* expr_vlan_id: FLOW_VLAN_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 504 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3327 "nd-flow-expr.cpp"
    break;

  case 156: /* expr_vlan_id: FLOW_VLAN_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 508 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3336 "nd-flow-expr.cpp"
    break;

  case 157: /* expr_vlan_id: FLOW_VLAN_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 512 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3345 "nd-flow-expr.cpp"
    break;

  case 158: /* expr_vlan_id: FLOW_VLAN_ID '>' VALUE_UNSIGNED  */
#line 516 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3354 "nd-flow-expr.cpp"
    break;

  case 159: /* expr_vlan_id: FLOW_VLAN_ID '<' VALUE_UNSIGNED  */
#line 520 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3363 "nd-flow-expr.cpp"
    break;

  case 160: /* expr_vlan: FLOW_VLAN  */
#line 527 "nd-flow-expr.ypp"
                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            || _NDFP_flow->if_metadata.local.pvid != 0 || _NDFP_flow->if_metadata.other.pvid != 0
#endif
        ));
        _NDFP_debugf("VLAN is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3376 "nd-flow-expr.cpp"
    break;

  case 161: /* expr_vlan: '!' FLOW_VLAN  */
#line 535 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            && _NDFP_flow->if_metadata.local.pvid == 0 && _NDFP_flow->if_metadata.other.pvid == 0
#endif
        ));
        _NDFP_debugf("VLAN is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3389 "nd-flow-expr.cpp"
    break;

  case 162: /* expr_vlan: FLOW_VLAN CMP_EQUAL VALUE_UNSIGNED  */
#line 543 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            || _NDFP_flow->if_metadata.local.pvid == (yyvsp[0].ul_number) || _NDFP_flow->if_metadata.other.pvid == (yyvsp[0].ul_number)
#endif
        ));
        _NDFP_debugf("VLAN == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3402 "nd-flow-expr.cpp"
    break;

  case 163: /* expr_vlan: FLOW_VLAN CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 551 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            && _NDFP_flow->if_metadata.local.pvid != (yyvsp[0].ul_number) && _NDFP_flow->if_metadata.other.pvid != (yyvsp[0].ul_number)
#endif
        ));
        _NDFP_debugf("VLAN != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3415 "nd-flow-expr.cpp"
    break;

  case 164: /* expr_vlan: FLOW_VLAN CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 559 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            || _NDFP_flow->if_metadata.local.pvid >= (yyvsp[0].ul_number) || _NDFP_flow->if_metadata.other.pvid >= (yyvsp[0].ul_number)
#endif
        ));
        _NDFP_debugf("VLAN >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3428 "nd-flow-expr.cpp"
    break;

  case 165: /* expr_vlan: FLOW_VLAN CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 567 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            || _NDFP_flow->if_metadata.local.pvid <= (yyvsp[0].ul_number) || _NDFP_flow->if_metadata.other.pvid <= (yyvsp[0].ul_number)
#endif
        ));
        _NDFP_debugf("VLAN <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3441 "nd-flow-expr.cpp"
    break;

  case 166: /* expr_vlan: FLOW_VLAN '>' VALUE_UNSIGNED  */
#line 575 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            || _NDFP_flow->if_metadata.local.pvid > (yyvsp[0].ul_number) || _NDFP_flow->if_metadata.other.pvid > (yyvsp[0].ul_number)
#endif
        ));
        _NDFP_debugf("VLAN > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3454 "nd-flow-expr.cpp"
    break;

  case 167: /* expr_vlan: FLOW_VLAN '<' VALUE_UNSIGNED  */
#line 583 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)
#if defined(_ND_ENABLE_INTERFACE_METADATA)
            || _NDFP_flow->if_metadata.local.pvid < (yyvsp[0].ul_number) || _NDFP_flow->if_metadata.other.pvid < (yyvsp[0].ul_number)
#endif
        ));
        _NDFP_debugf("VLAN < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3467 "nd-flow-expr.cpp"
    break;

  case 168: /* expr_local_if_metadata_ssid: FLOW_LOCAL_IF_META_SSID  */
#line 594 "nd-flow-expr.ypp"
                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (!_NDFP_flow->if_metadata.local.ssid.empty()));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta SSID set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3480 "nd-flow-expr.cpp"
    break;

  case 169: /* expr_local_if_metadata_ssid: '!' FLOW_LOCAL_IF_META_SSID  */
#line 602 "nd-flow-expr.ypp"
                                  {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.ssid.empty()));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta SSID not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3493 "nd-flow-expr.cpp"
    break;

  case 170: /* expr_local_if_metadata_ssid: FLOW_LOCAL_IF_META_SSID CMP_EQUAL VALUE_NAME  */
#line 610 "nd-flow-expr.ypp"
                                                   {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = false);
        if (!_NDFP_flow->if_metadata.local.ssid.empty()) {
            string search((yyvsp[0].buffer));
            size_t p;
            while ((p = search.find_first_of("\'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(_NDFP_flow->if_metadata.local.ssid.c_str(),
              search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta SSID == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3517 "nd-flow-expr.cpp"
    break;

  case 171: /* expr_local_if_metadata_ssid: FLOW_LOCAL_IF_META_SSID CMP_NOTEQUAL VALUE_NAME  */
#line 629 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = true);
        if (!_NDFP_flow->if_metadata.local.ssid.empty()) {
            string search((yyvsp[0].buffer));
            size_t p;
            while ((p = search.find_first_of("\'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(_NDFP_flow->if_metadata.local.ssid.c_str(),
              search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta SSID != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3541 "nd-flow-expr.cpp"
    break;

  case 172: /* expr_local_if_metadata_ssid: FLOW_LOCAL_IF_META_SSID CMP_EQUAL VALUE_REGEX  */
#line 648 "nd-flow-expr.ypp"
                                                    {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = false);
        if (!_NDFP_flow->if_metadata.local.ssid.empty()) {
            string rx((yyvsp[0].buffer)); 
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
              rx, _NDFP_flow->if_metadata.local.ssid));
        }
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta SSID =~ %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3559 "nd-flow-expr.cpp"
    break;

  case 173: /* expr_local_if_metadata_ssid: FLOW_LOCAL_IF_META_SSID CMP_NOTEQUAL VALUE_REGEX  */
#line 661 "nd-flow-expr.ypp"
                                                       {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = true);
        if (!_NDFP_flow->if_metadata.local.ssid.empty()) {
            string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(
              rx, _NDFP_flow->if_metadata.local.ssid));
        }
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta SSID !~ %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3577 "nd-flow-expr.cpp"
    break;

  case 174: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID  */
#line 677 "nd-flow-expr.ypp"
                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid != 0));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3590 "nd-flow-expr.cpp"
    break;

  case 175: /* expr_local_if_metadata_pvid: '!' FLOW_LOCAL_IF_META_PVID  */
#line 685 "nd-flow-expr.ypp"
                                  {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid == 0));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3603 "nd-flow-expr.cpp"
    break;

  case 176: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID CMP_EQUAL VALUE_UNSIGNED  */
#line 693 "nd-flow-expr.ypp"
                                                       {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3616 "nd-flow-expr.cpp"
    break;

  case 177: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 701 "nd-flow-expr.ypp"
                                                          {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3629 "nd-flow-expr.cpp"
    break;

  case 178: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 709 "nd-flow-expr.ypp"
                                                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3642 "nd-flow-expr.cpp"
    break;

  case 179: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 717 "nd-flow-expr.ypp"
                                                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3655 "nd-flow-expr.cpp"
    break;

  case 180: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID '>' VALUE_UNSIGNED  */
#line 725 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3668 "nd-flow-expr.cpp"
    break;

  case 181: /* expr_local_if_metadata_pvid: FLOW_LOCAL_IF_META_PVID '<' VALUE_UNSIGNED  */
#line 733 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.local.pvid < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Local if_meta PVID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3681 "nd-flow-expr.cpp"
    break;

  case 182: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID  */
#line 744 "nd-flow-expr.ypp"
                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid != 0));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3694 "nd-flow-expr.cpp"
    break;

  case 183: /* expr_other_if_metadata_pvid: '!' FLOW_OTHER_IF_META_PVID  */
#line 752 "nd-flow-expr.ypp"
                                  {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid == 0));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3707 "nd-flow-expr.cpp"
    break;

  case 184: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID CMP_EQUAL VALUE_UNSIGNED  */
#line 760 "nd-flow-expr.ypp"
                                                       {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3720 "nd-flow-expr.cpp"
    break;

  case 185: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 768 "nd-flow-expr.ypp"
                                                          {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3733 "nd-flow-expr.cpp"
    break;

  case 186: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 776 "nd-flow-expr.ypp"
                                                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3746 "nd-flow-expr.cpp"
    break;

  case 187: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 784 "nd-flow-expr.ypp"
                                                            {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3759 "nd-flow-expr.cpp"
    break;

  case 188: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID '>' VALUE_UNSIGNED  */
#line 792 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3772 "nd-flow-expr.cpp"
    break;

  case 189: /* expr_other_if_metadata_pvid: FLOW_OTHER_IF_META_PVID '<' VALUE_UNSIGNED  */
#line 800 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_INTERFACE_METADATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->if_metadata.other.pvid < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("Other if_meta PVID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3785 "nd-flow-expr.cpp"
    break;

  case 190: /* expr_other_type: FLOW_OTHER_TYPE  */
#line 811 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3796 "nd-flow-expr.cpp"
    break;

  case 191: /* expr_other_type: '!' FLOW_OTHER_TYPE  */
#line 817 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3807 "nd-flow-expr.cpp"
    break;

  case 192: /* expr_other_type: FLOW_OTHER_TYPE CMP_EQUAL value_other_type  */
#line 823 "nd-flow-expr.ypp"
                                                 {
        switch ((yyvsp[0].us_number)) {
        case _NDFP_OTHER_UNKNOWN:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
            );
            break;
        case _NDFP_OTHER_UNSUPPORTED:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::UNSUPPORTED
            );
            break;
        case _NDFP_OTHER_LOCAL:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::LOCAL
            );
            break;
        case _NDFP_OTHER_MULTICAST:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::MULTICAST
            );
            break;
        case _NDFP_OTHER_BROADCAST:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::BROADCAST
            );
            break;
        case _NDFP_OTHER_REMOTE:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::REMOTE
            );
            break;
        case _NDFP_OTHER_ERROR:
            _NDFP_result = (
                _NDFP_flow->other_type == ndFlow::OtherType::ERROR
            );
            break;
        default:
            _NDFP_result = false;
        }

        (yyval.bool_result) = _NDFP_result;
        _NDFP_debugf("Other type == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3856 "nd-flow-expr.cpp"
    break;

  case 193: /* expr_other_type: FLOW_OTHER_TYPE CMP_NOTEQUAL value_other_type  */
#line 867 "nd-flow-expr.ypp"
                                                    {
        switch ((yyvsp[0].us_number)) {
        case _NDFP_OTHER_UNKNOWN:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
            );
            break;
        case _NDFP_OTHER_UNSUPPORTED:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::UNSUPPORTED
            );
            break;
        case _NDFP_OTHER_LOCAL:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::LOCAL
            );
            break;
        case _NDFP_OTHER_MULTICAST:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::MULTICAST
            );
            break;
        case _NDFP_OTHER_BROADCAST:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::BROADCAST
            );
            break;
        case _NDFP_OTHER_REMOTE:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::REMOTE
            );
            break;
        case _NDFP_OTHER_ERROR:
            _NDFP_result = (
                _NDFP_flow->other_type != ndFlow::OtherType::ERROR
            );
            break;
        default:
            _NDFP_result = false;
        }

        (yyval.bool_result) = _NDFP_result;
        _NDFP_debugf("Other type != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3905 "nd-flow-expr.cpp"
    break;

  case 194: /* value_other_type: FLOW_OTHER_UNKNOWN  */
#line 914 "nd-flow-expr.ypp"
                         { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3911 "nd-flow-expr.cpp"
    break;

  case 195: /* value_other_type: FLOW_OTHER_UNSUPPORTED  */
#line 915 "nd-flow-expr.ypp"
                             { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3917 "nd-flow-expr.cpp"
    break;

  case 196: /* value_other_type: FLOW_OTHER_LOCAL  */
#line 916 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3923 "nd-flow-expr.cpp"
    break;

  case 197: /* value_other_type: FLOW_OTHER_MULTICAST  */
#line 917 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3929 "nd-flow-expr.cpp"
    break;

  case 198: /* value_other_type: FLOW_OTHER_BROADCAST  */
#line 918 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3935 "nd-flow-expr.cpp"
    break;

  case 199: /* value_other_type: FLOW_OTHER_REMOTE  */
#line 919 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3941 "nd-flow-expr.cpp"
    break;

  case 200: /* value_other_type: FLOW_OTHER_ERROR  */
#line 920 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3947 "nd-flow-expr.cpp"
    break;

  case 201: /* expr_any_mac: FLOW_ANY_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 924 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                strncasecmp(
                    _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
            ));
        }

        _NDFP_debugf("Any MAC == MAC %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3967 "nd-flow-expr.cpp"
    break;

  case 202: /* expr_any_mac: FLOW_ANY_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 939 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                strncasecmp(_NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
            ));
        }

        _NDFP_debugf("Any MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3985 "nd-flow-expr.cpp"
    break;

  case 203: /* expr_any_mac: FLOW_ANY_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 952 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == true));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
              .LookupGroupAddress(tag, *_NDFP_other_mac) == true));
        }

        _NDFP_debugf("Any MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4006 "nd-flow-expr.cpp"
    break;

  case 204: /* expr_any_mac: FLOW_ANY_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 968 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == false));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
              .LookupGroupAddress(tag, *_NDFP_other_mac) == false));
        }

        _NDFP_debugf("Any MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4026 "nd-flow-expr.cpp"
    break;

  case 205: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 986 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4038 "nd-flow-expr.cpp"
    break;

  case 206: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 993 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4049 "nd-flow-expr.cpp"
    break;

  case 207: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 999 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == true));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4063 "nd-flow-expr.cpp"
    break;

  case 208: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1008 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == false));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4077 "nd-flow-expr.cpp"
    break;

  case 209: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 1020 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4089 "nd-flow-expr.cpp"
    break;

  case 210: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 1027 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4101 "nd-flow-expr.cpp"
    break;

  case 211: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 1034 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == true));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4115 "nd-flow-expr.cpp"
    break;

  case 212: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1043 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == false));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4129 "nd-flow-expr.cpp"
    break;

  case 213: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 1055 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4141 "nd-flow-expr.cpp"
    break;

  case 214: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 1062 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4153 "nd-flow-expr.cpp"
    break;

  case 215: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 1069 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == true));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4167 "nd-flow-expr.cpp"
    break;

  case 216: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1078 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == false));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4181 "nd-flow-expr.cpp"
    break;

  case 217: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 1090 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4193 "nd-flow-expr.cpp"
    break;

  case 218: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 1097 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4205 "nd-flow-expr.cpp"
    break;

  case 219: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 1104 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == true));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4219 "nd-flow-expr.cpp"
    break;

  case 220: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1113 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == false));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4233 "nd-flow-expr.cpp"
    break;

  case 221: /* expr_any_ip: FLOW_ANY_IP CMP_EQUAL value_addr_ip  */
#line 1125 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
            ));
        }

        _NDFP_debugf("Any IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4251 "nd-flow-expr.cpp"
    break;

  case 222: /* expr_any_ip: FLOW_ANY_IP CMP_NOTEQUAL value_addr_ip  */
#line 1138 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
            ));
        }

        _NDFP_debugf("Any IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4269 "nd-flow-expr.cpp"
    break;

  case 223: /* expr_any_ip: FLOW_ANY_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1151 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == true));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
              .LookupGroupAddress(tag, *_NDFP_other_ip) == true));
        }

        _NDFP_debugf("Any IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4289 "nd-flow-expr.cpp"
    break;

  case 224: /* expr_any_ip: FLOW_ANY_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1166 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == false));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
              .LookupGroupAddress(tag, *_NDFP_other_ip) == false));
        }

        _NDFP_debugf("Any IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4309 "nd-flow-expr.cpp"
    break;

  case 225: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL value_addr_ip  */
#line 1184 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4320 "nd-flow-expr.cpp"
    break;

  case 226: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL value_addr_ip  */
#line 1190 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4331 "nd-flow-expr.cpp"
    break;

  case 227: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1196 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == true));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4345 "nd-flow-expr.cpp"
    break;

  case 228: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1205 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == false));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4359 "nd-flow-expr.cpp"
    break;

  case 229: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL value_addr_ip  */
#line 1217 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4370 "nd-flow-expr.cpp"
    break;

  case 230: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL value_addr_ip  */
#line 1223 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4381 "nd-flow-expr.cpp"
    break;

  case 231: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1229 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == true));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4395 "nd-flow-expr.cpp"
    break;

  case 232: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1238 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == false));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4409 "nd-flow-expr.cpp"
    break;

  case 233: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL value_addr_ip  */
#line 1250 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Source IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4421 "nd-flow-expr.cpp"
    break;

  case 234: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL value_addr_ip  */
#line 1257 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Source IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4433 "nd-flow-expr.cpp"
    break;

  case 235: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1264 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == true));
        _NDFP_debugf("Source IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4448 "nd-flow-expr.cpp"
    break;

  case 236: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1274 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == false));
        _NDFP_debugf("Source IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4463 "nd-flow-expr.cpp"
    break;

  case 237: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL value_addr_ip  */
#line 1287 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Destination IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4475 "nd-flow-expr.cpp"
    break;

  case 238: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL value_addr_ip  */
#line 1294 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Destination IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4487 "nd-flow-expr.cpp"
    break;

  case 239: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1301 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == true));
        _NDFP_debugf("Destination IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4502 "nd-flow-expr.cpp"
    break;

  case 240: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1311 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == false));
        _NDFP_debugf("Destination IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4517 "nd-flow-expr.cpp"
    break;

  case 241: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_EQUAL value_addr_ip  */
#line 1324 "nd-flow-expr.ypp"
                                                          {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_ct_reply_src_ip, (yyvsp[0].buffer)) == true
        ));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Source IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4533 "nd-flow-expr.cpp"
    break;

  case 242: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_NOTEQUAL value_addr_ip  */
#line 1335 "nd-flow-expr.ypp"
                                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_ct_reply_src_ip, (yyvsp[0].buffer)) == false
        ));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Source IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4549 "nd-flow-expr.cpp"
    break;

  case 243: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1346 "nd-flow-expr.ypp"
                                                           {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_ct_reply_src_ip) == true));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Source IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4568 "nd-flow-expr.cpp"
    break;

  case 244: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1360 "nd-flow-expr.ypp"
                                                              {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_ct_reply_src_ip) == false));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Source IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4587 "nd-flow-expr.cpp"
    break;

  case 245: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_EQUAL value_addr_ip  */
#line 1377 "nd-flow-expr.ypp"
                                                          {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_ct_reply_dst_ip, (yyvsp[0].buffer)) == true
        ));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Destination IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4603 "nd-flow-expr.cpp"
    break;

  case 246: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_NOTEQUAL value_addr_ip  */
#line 1388 "nd-flow-expr.ypp"
                                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_ct_reply_dst_ip, (yyvsp[0].buffer)) == false
        ));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Destination IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4619 "nd-flow-expr.cpp"
    break;

  case 247: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1399 "nd-flow-expr.ypp"
                                                           {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_ct_reply_dst_ip) == true));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Destination IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4638 "nd-flow-expr.cpp"
    break;

  case 248: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1413 "nd-flow-expr.ypp"
                                                              {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_ct_reply_dst_ip) == false));
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
        _NDFP_debugf("CT Reply Destination IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4657 "nd-flow-expr.cpp"
    break;

  case 249: /* value_addr_ip: VALUE_ADDR_IPV4  */
#line 1430 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4663 "nd-flow-expr.cpp"
    break;

  case 250: /* value_addr_ip: VALUE_ADDR_IPV4_CIDR  */
#line 1431 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4669 "nd-flow-expr.cpp"
    break;

  case 251: /* value_addr_ip: VALUE_ADDR_IPV6  */
#line 1432 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4675 "nd-flow-expr.cpp"
    break;

  case 252: /* value_addr_ip: VALUE_ADDR_IPV6_CIDR  */
#line 1433 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4681 "nd-flow-expr.cpp"
    break;

  case 253: /* expr_any_port: FLOW_ANY_PORT  */
#line 1437 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Any port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4692 "nd-flow-expr.cpp"
    break;

  case 254: /* expr_any_port: '!' FLOW_ANY_PORT  */
#line 1443 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Any port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4703 "nd-flow-expr.cpp"
    break;

  case 255: /* expr_any_port: FLOW_ANY_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1449 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4714 "nd-flow-expr.cpp"
    break;

  case 256: /* expr_any_port: FLOW_ANY_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1455 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4725 "nd-flow-expr.cpp"
    break;

  case 257: /* expr_any_port: FLOW_ANY_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1461 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4736 "nd-flow-expr.cpp"
    break;

  case 258: /* expr_any_port: FLOW_ANY_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1467 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4747 "nd-flow-expr.cpp"
    break;

  case 259: /* expr_any_port: FLOW_ANY_PORT '>' VALUE_UNSIGNED  */
#line 1473 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4758 "nd-flow-expr.cpp"
    break;

  case 260: /* expr_any_port: FLOW_ANY_PORT '<' VALUE_UNSIGNED  */
#line 1479 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4769 "nd-flow-expr.cpp"
    break;

  case 261: /* expr_local_port: FLOW_LOCAL_PORT  */
#line 1488 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        _NDFP_debugf("Local port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4778 "nd-flow-expr.cpp"
    break;

  case 262: /* expr_local_port: '!' FLOW_LOCAL_PORT  */
#line 1492 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        _NDFP_debugf("Local port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4787 "nd-flow-expr.cpp"
    break;

  case 263: /* expr_local_port: FLOW_LOCAL_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1496 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4796 "nd-flow-expr.cpp"
    break;

  case 264: /* expr_local_port: FLOW_LOCAL_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1500 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4805 "nd-flow-expr.cpp"
    break;

  case 265: /* expr_local_port: FLOW_LOCAL_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1504 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4814 "nd-flow-expr.cpp"
    break;

  case 266: /* expr_local_port: FLOW_LOCAL_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1508 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4823 "nd-flow-expr.cpp"
    break;

  case 267: /* expr_local_port: FLOW_LOCAL_PORT '>' VALUE_UNSIGNED  */
#line 1512 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4832 "nd-flow-expr.cpp"
    break;

  case 268: /* expr_local_port: FLOW_LOCAL_PORT '<' VALUE_UNSIGNED  */
#line 1516 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4841 "nd-flow-expr.cpp"
    break;

  case 269: /* expr_other_port: FLOW_OTHER_PORT  */
#line 1523 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Other port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4850 "nd-flow-expr.cpp"
    break;

  case 270: /* expr_other_port: '!' FLOW_OTHER_PORT  */
#line 1527 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Other port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4859 "nd-flow-expr.cpp"
    break;

  case 271: /* expr_other_port: FLOW_OTHER_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1531 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4868 "nd-flow-expr.cpp"
    break;

  case 272: /* expr_other_port: FLOW_OTHER_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1535 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4877 "nd-flow-expr.cpp"
    break;

  case 273: /* expr_other_port: FLOW_OTHER_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1539 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4886 "nd-flow-expr.cpp"
    break;

  case 274: /* expr_other_port: FLOW_OTHER_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1543 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4895 "nd-flow-expr.cpp"
    break;

  case 275: /* expr_other_port: FLOW_OTHER_PORT '>' VALUE_UNSIGNED  */
#line 1547 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4904 "nd-flow-expr.cpp"
    break;

  case 276: /* expr_other_port: FLOW_OTHER_PORT '<' VALUE_UNSIGNED  */
#line 1551 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4913 "nd-flow-expr.cpp"
    break;

  case 277: /* expr_src_port: FLOW_SRC_PORT  */
#line 1558 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != 0));
        _NDFP_debugf("Source port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4922 "nd-flow-expr.cpp"
    break;

  case 278: /* expr_src_port: '!' FLOW_SRC_PORT  */
#line 1562 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == 0));
        _NDFP_debugf("Source port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4931 "nd-flow-expr.cpp"
    break;

  case 279: /* expr_src_port: FLOW_SRC_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1566 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4940 "nd-flow-expr.cpp"
    break;

  case 280: /* expr_src_port: FLOW_SRC_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1570 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4949 "nd-flow-expr.cpp"
    break;

  case 281: /* expr_src_port: FLOW_SRC_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1574 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4958 "nd-flow-expr.cpp"
    break;

  case 282: /* expr_src_port: FLOW_SRC_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1578 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4967 "nd-flow-expr.cpp"
    break;

  case 283: /* expr_src_port: FLOW_SRC_PORT '>' VALUE_UNSIGNED  */
#line 1582 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4976 "nd-flow-expr.cpp"
    break;

  case 284: /* expr_src_port: FLOW_SRC_PORT '<' VALUE_UNSIGNED  */
#line 1586 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4985 "nd-flow-expr.cpp"
    break;

  case 285: /* expr_dst_port: FLOW_DST_PORT  */
#line 1593 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != 0));
        _NDFP_debugf("Destination port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4994 "nd-flow-expr.cpp"
    break;

  case 286: /* expr_dst_port: '!' FLOW_DST_PORT  */
#line 1597 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == 0));
        _NDFP_debugf("Destination port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5003 "nd-flow-expr.cpp"
    break;

  case 287: /* expr_dst_port: FLOW_DST_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1601 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5012 "nd-flow-expr.cpp"
    break;

  case 288: /* expr_dst_port: FLOW_DST_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1605 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5021 "nd-flow-expr.cpp"
    break;

  case 289: /* expr_dst_port: FLOW_DST_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1609 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5030 "nd-flow-expr.cpp"
    break;

  case 290: /* expr_dst_port: FLOW_DST_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1613 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5039 "nd-flow-expr.cpp"
    break;

  case 291: /* expr_dst_port: FLOW_DST_PORT '>' VALUE_UNSIGNED  */
#line 1617 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5048 "nd-flow-expr.cpp"
    break;

  case 292: /* expr_dst_port: FLOW_DST_PORT '<' VALUE_UNSIGNED  */
#line 1621 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5057 "nd-flow-expr.cpp"
    break;

  case 293: /* expr_tunnel_type: FLOW_TUNNEL_TYPE  */
#line 1628 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5068 "nd-flow-expr.cpp"
    break;

  case 294: /* expr_tunnel_type: '!' FLOW_TUNNEL_TYPE  */
#line 1634 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type is none? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5079 "nd-flow-expr.cpp"
    break;

  case 295: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_EQUAL value_tunnel_type  */
#line 1640 "nd-flow-expr.ypp"
                                                   {
        switch ((yyvsp[0].us_number)) {
        case _NDFP_TUNNEL_NONE:
            _NDFP_result = (
                _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
            );
            break;
        case _NDFP_TUNNEL_GTP:
            _NDFP_result = (
                _NDFP_flow->tunnel_type == ndFlow::TunnelType::GTP
            );
            break;
        default:
            _NDFP_result = false;
        }

        (yyval.bool_result) = _NDFP_result;
        _NDFP_debugf("Tunnel type == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5103 "nd-flow-expr.cpp"
    break;

  case 296: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_NOTEQUAL value_tunnel_type  */
#line 1659 "nd-flow-expr.ypp"
                                                      {
        switch ((yyvsp[0].us_number)) {
        case _NDFP_TUNNEL_NONE:
            _NDFP_result = (
                _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
            );
            break;
        case _NDFP_TUNNEL_GTP:
            _NDFP_result = (
                _NDFP_flow->tunnel_type != ndFlow::TunnelType::GTP
            );
            break;
        default:
            _NDFP_result = false;
        }

        (yyval.bool_result) = _NDFP_result;
        _NDFP_debugf("Tunnel type != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5127 "nd-flow-expr.cpp"
    break;

  case 297: /* value_tunnel_type: FLOW_TUNNEL_NONE  */
#line 1681 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 5133 "nd-flow-expr.cpp"
    break;

  case 298: /* value_tunnel_type: FLOW_TUNNEL_GTP  */
#line 1682 "nd-flow-expr.ypp"
                      { (yyval.us_number) = (yyvsp[0].us_number); }
#line 5139 "nd-flow-expr.cpp"
    break;

  case 299: /* expr_detection_complete: FLOW_DETECTION_COMPLETE  */
#line 1685 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf("Detection was complete? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5148 "nd-flow-expr.cpp"
    break;

  case 300: /* expr_detection_complete: '!' FLOW_DETECTION_COMPLETE  */
#line 1689 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf(
            "Detection was not complete? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5159 "nd-flow-expr.cpp"
    break;

  case 301: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_TRUE  */
#line 1695 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == true
        ));
        _NDFP_debugf(
            "Detection complete == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5172 "nd-flow-expr.cpp"
    break;

  case 302: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_FALSE  */
#line 1703 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == false
        ));
        _NDFP_debugf(
            "Detection complete == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5185 "nd-flow-expr.cpp"
    break;

  case 303: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_TRUE  */
#line 1711 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != true
        ));
        _NDFP_debugf(
            "Detection complete != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5198 "nd-flow-expr.cpp"
    break;

  case 304: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_FALSE  */
#line 1719 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != false
        ));
        _NDFP_debugf(
            "Detection complete != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5211 "nd-flow-expr.cpp"
    break;

  case 305: /* expr_detection_guessed: FLOW_DETECTION_GUESSED  */
#line 1730 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf("Detection was guessed? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5220 "nd-flow-expr.cpp"
    break;

  case 306: /* expr_detection_guessed: '!' FLOW_DETECTION_GUESSED  */
#line 1734 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf(
            "Detection was not guessed? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5231 "nd-flow-expr.cpp"
    break;

  case 307: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_TRUE  */
#line 1740 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == true
        ));
        _NDFP_debugf(
            "Detection guessed == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5244 "nd-flow-expr.cpp"
    break;

  case 308: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_FALSE  */
#line 1748 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == false
        ));
        _NDFP_debugf(
            "Detection guessed == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5257 "nd-flow-expr.cpp"
    break;

  case 309: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_TRUE  */
#line 1756 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != true
        ));
        _NDFP_debugf(
            "Detection guessed != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5270 "nd-flow-expr.cpp"
    break;

  case 310: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_FALSE  */
#line 1764 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != false
        ));
        _NDFP_debugf(
            "Detection guessed != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5283 "nd-flow-expr.cpp"
    break;

  case 311: /* expr_detection_init: FLOW_DETECTION_INIT  */
#line 1775 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf("Detection was init? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5292 "nd-flow-expr.cpp"
    break;

  case 312: /* expr_detection_init: '!' FLOW_DETECTION_INIT  */
#line 1779 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf(
            "Detection was not init? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5303 "nd-flow-expr.cpp"
    break;

  case 313: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_TRUE  */
#line 1785 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == true
        ));
        _NDFP_debugf(
            "Detection init == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5316 "nd-flow-expr.cpp"
    break;

  case 314: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_FALSE  */
#line 1793 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == false
        ));
        _NDFP_debugf(
            "Detection init == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5329 "nd-flow-expr.cpp"
    break;

  case 315: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1801 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != true
        ));
        _NDFP_debugf(
            "Detection init != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5342 "nd-flow-expr.cpp"
    break;

  case 316: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1809 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != false
        ));
        _NDFP_debugf(
            "Detection init != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5355 "nd-flow-expr.cpp"
    break;

  case 317: /* expr_detection_updated: FLOW_DETECTION_UPDATED  */
#line 1820 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf("Detection was updated? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5364 "nd-flow-expr.cpp"
    break;

  case 318: /* expr_detection_updated: '!' FLOW_DETECTION_UPDATED  */
#line 1824 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf(
            "Detection was not updated? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5375 "nd-flow-expr.cpp"
    break;

  case 319: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_TRUE  */
#line 1830 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == true
        ));
        _NDFP_debugf(
            "Detection updated == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5388 "nd-flow-expr.cpp"
    break;

  case 320: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_FALSE  */
#line 1838 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == false
        ));
        _NDFP_debugf(
            "Detection updated == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5401 "nd-flow-expr.cpp"
    break;

  case 321: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_TRUE  */
#line 1846 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != true
        ));
        _NDFP_debugf(
            "Detection updated != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5414 "nd-flow-expr.cpp"
    break;

  case 322: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_FALSE  */
#line 1854 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != false
        ));
        _NDFP_debugf(
            "Detection updated != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5427 "nd-flow-expr.cpp"
    break;

  case 323: /* expr_dhc_hit: FLOW_DHC_HIT  */
#line 1865 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf("DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5436 "nd-flow-expr.cpp"
    break;

  case 324: /* expr_dhc_hit: '!' FLOW_DHC_HIT  */
#line 1869 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf(
            "DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5447 "nd-flow-expr.cpp"
    break;

  case 325: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1875 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == true
        ));
        _NDFP_debugf(
            "DHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5460 "nd-flow-expr.cpp"
    break;

  case 326: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1883 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == false
        ));
        _NDFP_debugf(
            "DHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5473 "nd-flow-expr.cpp"
    break;

  case 327: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1891 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != true
        ));
        _NDFP_debugf(
            "DHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5486 "nd-flow-expr.cpp"
    break;

  case 328: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1899 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != false
        ));
        _NDFP_debugf(
            "DHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5499 "nd-flow-expr.cpp"
    break;

  case 329: /* expr_fhc_hit: FLOW_FHC_HIT  */
#line 1910 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf("FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5508 "nd-flow-expr.cpp"
    break;

  case 330: /* expr_fhc_hit: '!' FLOW_FHC_HIT  */
#line 1914 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf(
            "FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5519 "nd-flow-expr.cpp"
    break;

  case 331: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1920 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == true
        ));
        _NDFP_debugf(
            "FHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5532 "nd-flow-expr.cpp"
    break;

  case 332: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1928 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == false
        ));
        _NDFP_debugf(
            "FHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5545 "nd-flow-expr.cpp"
    break;

  case 333: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1936 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != true
        ));
        _NDFP_debugf(
            "FHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5558 "nd-flow-expr.cpp"
    break;

  case 334: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1944 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != false
        ));
        _NDFP_debugf(
            "FHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5571 "nd-flow-expr.cpp"
    break;

  case 335: /* expr_ip_nat: FLOW_IP_NAT  */
#line 1955 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5580 "nd-flow-expr.cpp"
    break;

  case 336: /* expr_ip_nat: '!' FLOW_IP_NAT  */
#line 1959 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5589 "nd-flow-expr.cpp"
    break;

  case 337: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_TRUE  */
#line 1963 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5598 "nd-flow-expr.cpp"
    break;

  case 338: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_FALSE  */
#line 1967 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5607 "nd-flow-expr.cpp"
    break;

  case 339: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_TRUE  */
#line 1971 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != true));
        _NDFP_debugf("IP NAT != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5616 "nd-flow-expr.cpp"
    break;

  case 340: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_FALSE  */
#line 1975 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != false));
        _NDFP_debugf("IP NAT != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5625 "nd-flow-expr.cpp"
    break;

  case 341: /* expr_expiring: FLOW_EXPIRING  */
#line 1982 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5634 "nd-flow-expr.cpp"
    break;

  case 342: /* expr_expiring: '!' FLOW_EXPIRING  */
#line 1986 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5643 "nd-flow-expr.cpp"
    break;

  case 343: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_TRUE  */
#line 1990 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5652 "nd-flow-expr.cpp"
    break;

  case 344: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_FALSE  */
#line 1994 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5661 "nd-flow-expr.cpp"
    break;

  case 345: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_TRUE  */
#line 1998 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != true));
        _NDFP_debugf("Flow expiring != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5670 "nd-flow-expr.cpp"
    break;

  case 346: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_FALSE  */
#line 2002 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != false));
        _NDFP_debugf("Flow expiring != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5679 "nd-flow-expr.cpp"
    break;

  case 347: /* expr_expired: FLOW_EXPIRED  */
#line 2009 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5688 "nd-flow-expr.cpp"
    break;

  case 348: /* expr_expired: '!' FLOW_EXPIRED  */
#line 2013 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5697 "nd-flow-expr.cpp"
    break;

  case 349: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_TRUE  */
#line 2017 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5706 "nd-flow-expr.cpp"
    break;

  case 350: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_FALSE  */
#line 2021 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5715 "nd-flow-expr.cpp"
    break;

  case 351: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_TRUE  */
#line 2025 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != true));
        _NDFP_debugf("Flow expired != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5724 "nd-flow-expr.cpp"
    break;

  case 352: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_FALSE  */
#line 2029 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != false));
        _NDFP_debugf("Flow expired != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5733 "nd-flow-expr.cpp"
    break;

  case 353: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR  */
#line 2036 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5742 "nd-flow-expr.cpp"
    break;

  case 354: /* expr_soft_dissector: '!' FLOW_SOFT_DISSECTOR  */
#line 2040 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5751 "nd-flow-expr.cpp"
    break;

  case 355: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_TRUE  */
#line 2044 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5760 "nd-flow-expr.cpp"
    break;

  case 356: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_FALSE  */
#line 2048 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5769 "nd-flow-expr.cpp"
    break;

  case 357: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_TRUE  */
#line 2052 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != true));
        _NDFP_debugf("Soft dissector matched != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5778 "nd-flow-expr.cpp"
    break;

  case 358: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_FALSE  */
#line 2056 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != false));
        _NDFP_debugf("Soft dissector matched != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5787 "nd-flow-expr.cpp"
    break;

  case 359: /* expr_app: FLOW_APPLICATION  */
#line 2063 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) != 0
        ));
        _NDFP_debugf("Application detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5798 "nd-flow-expr.cpp"
    break;

  case 360: /* expr_app: '!' FLOW_APPLICATION  */
#line 2069 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) == 0
        ));
        _NDFP_debugf(
            "Application not detected? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5811 "nd-flow-expr.cpp"
    break;

  case 363: /* expr_app_id: FLOW_APPLICATION CMP_EQUAL VALUE_UNSIGNED  */
#line 2081 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf(
            "Application ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5825 "nd-flow-expr.cpp"
    break;

  case 364: /* expr_app_id: FLOW_APPLICATION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2090 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = false);

        _NDFP_debugf(
            "Application ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5839 "nd-flow-expr.cpp"
    break;

  case 365: /* expr_app_name: FLOW_APPLICATION CMP_EQUAL VALUE_NAME  */
#line 2102 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->detected_application_name.empty()) {

            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(_NDFP_flow->detected_application_name.c_str(),
              search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
            else if ((p = _NDFP_flow->detected_application_name.find_first_of(".")) !=
              string::npos && strncasecmp(
                _NDFP_flow->detected_application_name.substr(p + 1).c_str(),
                search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                    _NDFP_result = ((yyval.bool_result) = true);
            }
        }

        _NDFP_debugf(
            "Application name == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5871 "nd-flow-expr.cpp"
    break;

  case 366: /* expr_app_name: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NAME  */
#line 2129 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = true);

        if (! _NDFP_flow->detected_application_name.empty()) {

            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(_NDFP_flow->detected_application_name.c_str(),
              search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
            else if ((p = _NDFP_flow->detected_application_name.find_first_of(".")) !=
              string::npos && strncasecmp(
                _NDFP_flow->detected_application_name.substr(p + 1).c_str(),
                search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                    _NDFP_result = ((yyval.bool_result) = false);
            }
        }

        _NDFP_debugf(
            "Application name != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5903 "nd-flow-expr.cpp"
    break;

  case 367: /* expr_category: FLOW_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2159 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = false);

        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        if (_NDFP_flow->category.application != ndCategory::UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.application
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.domain != ndCategory::UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.domain
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.lower_net != ndCategory::UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.lower_net
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.upper_net != ndCategory::UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.upper_net
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.overlay != ndCategory::UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.overlay
                )
            );
        }

        if (! _NDFP_result && _NDFP_params_intel) {
            json jvalue;
            string key("intel_domain_criteria");
            _NDFP_result = (
                (yyval.bool_result) = (
                    flow_intel(key, *_NDFP_params_intel, jvalue) &&
                      jvalue.is_string() && jvalue.get<string>() == category
                )
            );
        }

        _NDFP_debugf("App/domain/network/tag/intel category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 5976 "nd-flow-expr.cpp"
    break;

  case 368: /* expr_category: FLOW_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2227 "nd-flow-expr.ypp"
                                            {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_flow->category.application
            )
        );

        if (! _NDFP_result) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) != _NDFP_flow->category.domain
                )
            );
        }

        if (! _NDFP_result) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) != _NDFP_flow->category.lower_net
                )
            );
        }

        if (! _NDFP_result) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) != _NDFP_flow->category.upper_net
                )
            );
        }

        if (! _NDFP_result) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) != _NDFP_flow->category.overlay
                )
            );
        }

        if (! _NDFP_result && _NDFP_params_intel) {
            json jvalue;
            string key("intel_domain_criteria");
            _NDFP_result = (
                (yyval.bool_result) = (
                    flow_intel(key, *_NDFP_params_intel, jvalue) &&
                      jvalue.is_string() && jvalue.get<string>() != category
                )
            );
        }

        _NDFP_debugf("App/domain/network/tag/intel category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6045 "nd-flow-expr.cpp"
    break;

  case 369: /* expr_category_id: FLOW_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2294 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("App category ID == %lu? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Domain category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.lower_net) == (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Network (lower) category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.upper_net) == (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Network (upper) category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.overlay) == (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Overlay tag category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result && _NDFP_params_intel) {
            json jvalue;
            string key("intel_domain_criteria");
            _NDFP_result = (
                (yyval.bool_result) = (
                    flow_intel(key, *_NDFP_params_intel, jvalue) &&
                      jvalue.is_number() && jvalue.get<unsigned long>() == (yyvsp[0].ul_number)
                )
            );
        }

        _NDFP_debugf("App/domain/network/tag category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6103 "nd-flow-expr.cpp"
    break;

  case 370: /* expr_category_id: FLOW_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2347 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("App category ID == %lu? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Domain category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.lower_net) != (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Network (lower) category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.upper_net) != (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Network (upper) category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.overlay) != (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Overlay tag category ID == %lu? %s\n",
                (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result && _NDFP_params_intel) {
            json jvalue;
            string key("intel_domain_criteria");
            _NDFP_result = (
                (yyval.bool_result) = (
                    flow_intel(key, *_NDFP_params_intel, jvalue) &&
                      jvalue.is_number() && jvalue.get<unsigned long>() != (yyvsp[0].ul_number)
                )
            );
        }

        _NDFP_debugf("App/domain/network/tag category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6161 "nd-flow-expr.cpp"
    break;

  case 371: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2403 "nd-flow-expr.ypp"
                                                     {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_flow->category.application
            )
        );

        _NDFP_debugf("App category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6183 "nd-flow-expr.cpp"
    break;

  case 372: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2420 "nd-flow-expr.ypp"
                                                        {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_flow->category.application
            )
        );

        _NDFP_debugf("App category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6205 "nd-flow-expr.cpp"
    break;

  case 373: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2440 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6217 "nd-flow-expr.cpp"
    break;

  case 374: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2447 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category != %s? %s\n",
        (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6230 "nd-flow-expr.cpp"
    break;

  case 375: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2458 "nd-flow-expr.ypp"
                                                {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_flow->category.domain
            )
        );

        _NDFP_debugf("Domain category ID == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6252 "nd-flow-expr.cpp"
    break;

  case 376: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2475 "nd-flow-expr.ypp"
                                                   {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_flow->category.domain
            )
        );

        _NDFP_debugf("Domain category ID != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6274 "nd-flow-expr.cpp"
    break;

  case 377: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2495 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6286 "nd-flow-expr.cpp"
    break;

  case 378: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2502 "nd-flow-expr.ypp"
                                                          {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6299 "nd-flow-expr.cpp"
    break;

  case 379: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2513 "nd-flow-expr.ypp"
                                                 {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_flow->category.lower_net
            )
        );

        if (! _NDFP_result) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.upper_net
                )
            );
        }

        _NDFP_debugf("Network category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6330 "nd-flow-expr.cpp"
    break;

  case 380: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2539 "nd-flow-expr.ypp"
                                                    {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_flow->category.lower_net
            )
        );

        if (! _NDFP_result) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) != _NDFP_flow->category.upper_net
                )
            );
        }

        _NDFP_debugf("Network category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6361 "nd-flow-expr.cpp"
    break;

  case 381: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2568 "nd-flow-expr.ypp"
                                                        {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.lower_net) == (yyvsp[0].ul_number)
        ));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.upper_net) == (yyvsp[0].ul_number)
            ));
        }

        _NDFP_debugf("Network category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6380 "nd-flow-expr.cpp"
    break;

  case 382: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2582 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.lower_net) != (yyvsp[0].ul_number)
        ));

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.upper_net) != (yyvsp[0].ul_number)
            ));
        }

        _NDFP_debugf("Network category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6399 "nd-flow-expr.cpp"
    break;

  case 383: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2599 "nd-flow-expr.ypp"
                                                       {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_local_net_cat
            )
        );

        _NDFP_debugf("Network (local) category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6421 "nd-flow-expr.cpp"
    break;

  case 384: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2616 "nd-flow-expr.ypp"
                                                          {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_local_net_cat
            )
        );

        _NDFP_debugf("Network (local) category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6443 "nd-flow-expr.cpp"
    break;

  case 385: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2636 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6456 "nd-flow-expr.cpp"
    break;

  case 386: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2644 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6469 "nd-flow-expr.cpp"
    break;

  case 387: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2655 "nd-flow-expr.ypp"
                                                       {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_other_net_cat
            )
        );

        _NDFP_debugf("Network (other) category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6491 "nd-flow-expr.cpp"
    break;

  case 388: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2672 "nd-flow-expr.ypp"
                                                          {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_other_net_cat
            )
        );

        _NDFP_debugf("Network (other) category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6513 "nd-flow-expr.cpp"
    break;

  case 389: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2692 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6526 "nd-flow-expr.cpp"
    break;

  case 390: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2700 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6539 "nd-flow-expr.cpp"
    break;

  case 391: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2711 "nd-flow-expr.ypp"
                                                     {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_src_net_cat
            )
        );

        _NDFP_debugf("Network (src) category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6561 "nd-flow-expr.cpp"
    break;

  case 392: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2728 "nd-flow-expr.ypp"
                                                        {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_src_net_cat
            )
        );

        _NDFP_debugf("Network (src) category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6583 "nd-flow-expr.cpp"
    break;

  case 393: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2748 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6596 "nd-flow-expr.cpp"
    break;

  case 394: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2756 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6609 "nd-flow-expr.cpp"
    break;

  case 395: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2767 "nd-flow-expr.ypp"
                                                     {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_dst_net_cat
            )
        );

        _NDFP_debugf("Network (dst) category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6631 "nd-flow-expr.cpp"
    break;

  case 396: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2784 "nd-flow-expr.ypp"
                                                        {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_dst_net_cat
            )
        );

        _NDFP_debugf("Network (dst) category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6653 "nd-flow-expr.cpp"
    break;

  case 397: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2804 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6666 "nd-flow-expr.cpp"
    break;

  case 398: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2812 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6679 "nd-flow-expr.cpp"
    break;

  case 399: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2823 "nd-flow-expr.ypp"
                                             {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_flow->category.overlay
            )
        );

        _NDFP_debugf("Overlay tag category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6701 "nd-flow-expr.cpp"
    break;

  case 400: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2840 "nd-flow-expr.ypp"
                                                {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_flow->category.overlay
            )
        );

        _NDFP_debugf("Overlay tag category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6723 "nd-flow-expr.cpp"
    break;

  case 401: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2860 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6736 "nd-flow-expr.cpp"
    break;

  case 402: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2868 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6749 "nd-flow-expr.cpp"
    break;

  case 403: /* expr_proto: FLOW_PROTOCOL  */
#line 2879 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol != ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6760 "nd-flow-expr.cpp"
    break;

  case 404: /* expr_proto: '!' FLOW_PROTOCOL  */
#line 2885 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol == ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6771 "nd-flow-expr.cpp"
    break;

  case 407: /* expr_proto_id: FLOW_PROTOCOL CMP_EQUAL VALUE_UNSIGNED  */
#line 2895 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6782 "nd-flow-expr.cpp"
    break;

  case 408: /* expr_proto_id: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2901 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6793 "nd-flow-expr.cpp"
    break;

  case 409: /* expr_proto_name: FLOW_PROTOCOL CMP_EQUAL VALUE_NAME  */
#line 2910 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->detected_protocol_name.empty()) {

            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            _NDFP_result = ((yyval.bool_result) = (strncasecmp(
                _NDFP_flow->detected_protocol_name.c_str(), search.c_str(), _NDFP_MAX_BUFLEN
            ) == 0));
        }

        _NDFP_debugf(
            "Protocol name == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 6818 "nd-flow-expr.cpp"
    break;

  case 410: /* expr_proto_name: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NAME  */
#line 2930 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = true);

        if (! _NDFP_flow->detected_protocol_name.empty()) {

            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            _NDFP_result = ((yyval.bool_result) = (strncasecmp(
                _NDFP_flow->detected_protocol_name.c_str(), search.c_str(), _NDFP_MAX_BUFLEN
            )));
        }
        _NDFP_debugf(
            "Protocol name != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 6842 "nd-flow-expr.cpp"
    break;

  case 411: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2952 "nd-flow-expr.ypp"
                                                  {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::PROTO, category) == _NDFP_flow->category.protocol
            )
        );

        _NDFP_debugf("Protocol category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6864 "nd-flow-expr.cpp"
    break;

  case 412: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2969 "nd-flow-expr.ypp"
                                                     {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::PROTO, category) != _NDFP_flow->category.protocol
            )
        );

        _NDFP_debugf("Protocol category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6886 "nd-flow-expr.cpp"
    break;

  case 413: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2989 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6899 "nd-flow-expr.cpp"
    break;

  case 414: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2997 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6912 "nd-flow-expr.cpp"
    break;

  case 415: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME  */
#line 3008 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == false
        ));
        _NDFP_debugf("Detected hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6924 "nd-flow-expr.cpp"
    break;

  case 416: /* expr_detected_hostname: '!' FLOW_DETECTED_HOSTNAME  */
#line 3015 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == true
        ));
        _NDFP_debugf("Detected hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6936 "nd-flow-expr.cpp"
    break;

  case 417: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 3022 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->host_server_name.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->host_server_name.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }

        _NDFP_debugf("Detected hostname == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6960 "nd-flow-expr.cpp"
    break;

  case 418: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 3041 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);

        if (! _NDFP_flow->host_server_name.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->host_server_name.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }

        _NDFP_debugf("Detected hostname != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6984 "nd-flow-expr.cpp"
    break;

  case 419: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 3060 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->host_server_name.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
                rx, _NDFP_flow->host_server_name
            ));
        }

        _NDFP_debugf("Detected hostname (RX) == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7003 "nd-flow-expr.cpp"
    break;

  case 420: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 3074 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Detected hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7014 "nd-flow-expr.cpp"
    break;

  case 421: /* expr_dns_hostname: FLOW_DNS_HOSTNAME  */
#line 3083 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == false
        ));
        _NDFP_debugf("DNS hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7026 "nd-flow-expr.cpp"
    break;

  case 422: /* expr_dns_hostname: '!' FLOW_DNS_HOSTNAME  */
#line 3090 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == true
        ));
        _NDFP_debugf("DNS hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7038 "nd-flow-expr.cpp"
    break;

  case 423: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 3097 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->dns_host_name.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->dns_host_name.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }

        _NDFP_debugf("DNS hostname == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7062 "nd-flow-expr.cpp"
    break;

  case 424: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 3116 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = true);

        if (! _NDFP_flow->dns_host_name.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->dns_host_name.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }

        _NDFP_debugf("DNS hostname != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7086 "nd-flow-expr.cpp"
    break;

  case 425: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 3135 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->dns_host_name.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
                rx, _NDFP_flow->dns_host_name
            ));
        }

        _NDFP_debugf("DNS hostname (RX) == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7105 "nd-flow-expr.cpp"
    break;

  case 426: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 3149 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("DNS hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7116 "nd-flow-expr.cpp"
    break;

  case 427: /* expr_risks: FLOW_RISKS  */
#line 3158 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() != 0));
        _NDFP_debugf("Risks detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7125 "nd-flow-expr.cpp"
    break;

  case 428: /* expr_risks: '!' FLOW_RISKS  */
#line 3162 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() == 0));
        _NDFP_debugf("Risks not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7134 "nd-flow-expr.cpp"
    break;

  case 429: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NAME  */
#line 3166 "nd-flow-expr.ypp"
                                      {
        size_t p;
        string risk((yyvsp[0].buffer));

        while ((p = risk.find_first_of("'\"")) != string::npos)
            risk.erase(p, 1);

        ndRisk::Id id = ndRisk::GetId(risk);

        _NDFP_result = false;
        for (auto &i : _NDFP_flow->risk.risks) {
            if (i != id) continue;
            _NDFP_result = true;
            break;
        }

        (yyval.bool_result) = _NDFP_result;
        _NDFP_debugf("Risks == %s %s\n", risk.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 7158 "nd-flow-expr.cpp"
    break;

  case 430: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NAME  */
#line 3185 "nd-flow-expr.ypp"
                                         {
        size_t p;
        string risk((yyvsp[0].buffer));

        while ((p = risk.find_first_of("'\"")) != string::npos)
            risk.erase(p, 1);

        ndRisk::Id id = ndRisk::GetId(risk);

        _NDFP_result = false;
        for (auto &i : _NDFP_flow->risk.risks) {
            if (i != id) continue;
            _NDFP_result = true;
            break;
        }

        _NDFP_result = ((yyval.bool_result) = (!_NDFP_result));
        _NDFP_debugf("Risks != %s %s\n", risk.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 7182 "nd-flow-expr.cpp"
    break;

  case 431: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_UNSIGNED  */
#line 3204 "nd-flow-expr.ypp"
                                          {
        unsigned long id = (yyvsp[0].ul_number);

        _NDFP_result = false;
        for (auto &i : _NDFP_flow->risk.risks) {
            if (static_cast<unsigned long>(i) != id) continue;
            _NDFP_result = true;
            break;
        }

        (yyval.bool_result) = _NDFP_result;
        _NDFP_debugf("Risks == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7200 "nd-flow-expr.cpp"
    break;

  case 432: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3217 "nd-flow-expr.ypp"
                                             {
        unsigned long id = (yyvsp[0].ul_number);

        _NDFP_result = false;
        for (auto &i : _NDFP_flow->risk.risks) {
            if (static_cast<unsigned long>(i) != id) continue;
            _NDFP_result = true;
            break;
        }

        _NDFP_result = ((yyval.bool_result) = (!_NDFP_result));
        _NDFP_debugf("Risks != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7218 "nd-flow-expr.cpp"
    break;

  case 433: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE  */
#line 3233 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != 0));
        _NDFP_debugf("nDPI risk score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7227 "nd-flow-expr.cpp"
    break;

  case 434: /* expr_risk_ndpi_score: '!' FLOW_NDPI_RISK_SCORE  */
#line 3237 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == 0));
        _NDFP_debugf("nDPI risk score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7236 "nd-flow-expr.cpp"
    break;

  case 435: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_EQUAL VALUE_UNSIGNED  */
#line 3241 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7245 "nd-flow-expr.cpp"
    break;

  case 436: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3245 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7254 "nd-flow-expr.cpp"
    break;

  case 437: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3249 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7263 "nd-flow-expr.cpp"
    break;

  case 438: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3253 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7272 "nd-flow-expr.cpp"
    break;

  case 439: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '>' VALUE_UNSIGNED  */
#line 3257 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7281 "nd-flow-expr.cpp"
    break;

  case 440: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '<' VALUE_UNSIGNED  */
#line 3261 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7290 "nd-flow-expr.cpp"
    break;

  case 441: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 3268 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != 0));
        _NDFP_debugf("nDPI risk client score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7299 "nd-flow-expr.cpp"
    break;

  case 442: /* expr_risk_ndpi_score_client: '!' FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 3272 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == 0));
        _NDFP_debugf("nDPI risk client score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7308 "nd-flow-expr.cpp"
    break;

  case 443: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_EQUAL VALUE_UNSIGNED  */
#line 3276 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7317 "nd-flow-expr.cpp"
    break;

  case 444: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3280 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7326 "nd-flow-expr.cpp"
    break;

  case 445: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3284 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7335 "nd-flow-expr.cpp"
    break;

  case 446: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3288 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7344 "nd-flow-expr.cpp"
    break;

  case 447: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '>' VALUE_UNSIGNED  */
#line 3292 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7353 "nd-flow-expr.cpp"
    break;

  case 448: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '<' VALUE_UNSIGNED  */
#line 3296 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7362 "nd-flow-expr.cpp"
    break;

  case 449: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER  */
#line 3303 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != 0));
        _NDFP_debugf("nDPI risk server score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7371 "nd-flow-expr.cpp"
    break;

  case 450: /* expr_risk_ndpi_score_server: '!' FLOW_NDPI_RISK_SCORE_SERVER  */
#line 3307 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == 0));
        _NDFP_debugf("nDPI risk server score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7380 "nd-flow-expr.cpp"
    break;

  case 451: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_EQUAL VALUE_UNSIGNED  */
#line 3311 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7389 "nd-flow-expr.cpp"
    break;

  case 452: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3315 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7398 "nd-flow-expr.cpp"
    break;

  case 453: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3319 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7407 "nd-flow-expr.cpp"
    break;

  case 454: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3323 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7416 "nd-flow-expr.cpp"
    break;

  case 455: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '>' VALUE_UNSIGNED  */
#line 3327 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7425 "nd-flow-expr.cpp"
    break;

  case 456: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '<' VALUE_UNSIGNED  */
#line 3331 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7434 "nd-flow-expr.cpp"
    break;

  case 457: /* expr_conntrack_id: FLOW_CONNTRACK_ID  */
#line 3338 "nd-flow-expr.ypp"
                        {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id != 0));
        _NDFP_debugf("CT ID set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7447 "nd-flow-expr.cpp"
    break;

  case 458: /* expr_conntrack_id: '!' FLOW_CONNTRACK_ID  */
#line 3346 "nd-flow-expr.ypp"
                            {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id == 0));
        _NDFP_debugf("CT ID not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7460 "nd-flow-expr.cpp"
    break;

  case 459: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 3354 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id == (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7473 "nd-flow-expr.cpp"
    break;

  case 460: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3362 "nd-flow-expr.ypp"
                                                    {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id != (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7486 "nd-flow-expr.cpp"
    break;

  case 461: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3370 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7499 "nd-flow-expr.cpp"
    break;

  case 462: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3378 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7512 "nd-flow-expr.cpp"
    break;

  case 463: /* expr_conntrack_id: FLOW_CONNTRACK_ID '>' VALUE_UNSIGNED  */
#line 3386 "nd-flow-expr.ypp"
                                           {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id > (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7525 "nd-flow-expr.cpp"
    break;

  case 464: /* expr_conntrack_id: FLOW_CONNTRACK_ID '<' VALUE_UNSIGNED  */
#line 3394 "nd-flow-expr.ypp"
                                           {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id < (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7538 "nd-flow-expr.cpp"
    break;

  case 465: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK  */
#line 3405 "nd-flow-expr.ypp"
                          {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != 0));
        _NDFP_debugf("CT Mark set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7551 "nd-flow-expr.cpp"
    break;

  case 466: /* expr_conntrack_mark: '!' FLOW_CONNTRACK_MARK  */
#line 3413 "nd-flow-expr.ypp"
                              {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == 0));
        _NDFP_debugf("CT Mark not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7564 "nd-flow-expr.cpp"
    break;

  case 467: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_EQUAL VALUE_UNSIGNED  */
#line 3421 "nd-flow-expr.ypp"
                                                   {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7577 "nd-flow-expr.cpp"
    break;

  case 468: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3429 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7590 "nd-flow-expr.cpp"
    break;

  case 469: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3437 "nd-flow-expr.ypp"
                                                        {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark >= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7603 "nd-flow-expr.cpp"
    break;

  case 470: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3445 "nd-flow-expr.ypp"
                                                        {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark <= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7616 "nd-flow-expr.cpp"
    break;

  case 471: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK '>' VALUE_UNSIGNED  */
#line 3453 "nd-flow-expr.ypp"
                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark > (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7629 "nd-flow-expr.cpp"
    break;

  case 472: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK '<' VALUE_UNSIGNED  */
#line 3461 "nd-flow-expr.ypp"
                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark < (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7642 "nd-flow-expr.cpp"
    break;

  case 473: /* expr_iface: FLOW_IFACE  */
#line 3472 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == false
        ));
        _NDFP_debugf("Interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7654 "nd-flow-expr.cpp"
    break;

  case 474: /* expr_iface: '!' FLOW_IFACE  */
#line 3479 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == true
        ));
        _NDFP_debugf("Interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7666 "nd-flow-expr.cpp"
    break;

  case 475: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_NAME  */
#line 3486 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->iface->ifname.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->iface->ifname.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }

        _NDFP_debugf("Interface == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7689 "nd-flow-expr.cpp"
    break;

  case 476: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_NAME  */
#line 3504 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->iface->ifname.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->iface->ifname.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }

        _NDFP_debugf("Interface != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7712 "nd-flow-expr.cpp"
    break;

  case 477: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_REGEX  */
#line 3522 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->iface->ifname.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
                rx, _NDFP_flow->iface->ifname
            ));
        }

        _NDFP_debugf("Interface (RX) == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7731 "nd-flow-expr.cpp"
    break;

  case 478: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_REGEX  */
#line 3536 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7742 "nd-flow-expr.cpp"
    break;

  case 479: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC  */
#line 3545 "nd-flow-expr.ypp"
                         {
#if defined(_ND_ENABLE_NFQUEUE)
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->nfq.src_iface.empty() == false
        ));
#else
        _NDFP_result = ((yyval.bool_result) = false);
#endif
        _NDFP_debugf("NFQ SRC interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7758 "nd-flow-expr.cpp"
    break;

  case 480: /* expr_iface_nfq_src: '!' FLOW_IFACE_NFQ_SRC  */
#line 3556 "nd-flow-expr.ypp"
                             {
#if defined(_ND_ENABLE_NFQUEUE)
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->nfq.src_iface.empty() == true
        ));
#else
        _NDFP_result = ((yyval.bool_result) = true);
#endif
        _NDFP_debugf("NFQ SRC interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7774 "nd-flow-expr.cpp"
    break;

  case 481: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_NAME  */
#line 3567 "nd-flow-expr.ypp"
                                              {
#if defined(_ND_ENABLE_NFQUEUE)
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->nfq.src_iface.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->nfq.src_iface.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
#else
        _NDFP_result = ((yyval.bool_result) = false);
#endif
        _NDFP_debugf("NFQ SRC interface == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7800 "nd-flow-expr.cpp"
    break;

  case 482: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_NAME  */
#line 3588 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.src_iface.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->nfq.src_iface.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
#endif
        _NDFP_debugf("NFQ SRC interface != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7824 "nd-flow-expr.cpp"
    break;

  case 483: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_REGEX  */
#line 3607 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.src_iface.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
                rx, _NDFP_flow->nfq.src_iface
            ));
        }
#endif
        _NDFP_debugf("NFQ SRC interface (RX) == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7843 "nd-flow-expr.cpp"
    break;

  case 484: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_REGEX  */
#line 3621 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.src_iface.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(
                rx, _NDFP_flow->nfq.src_iface
            ));
        }
#endif
        _NDFP_debugf("NFQ SRC interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7862 "nd-flow-expr.cpp"
    break;

  case 485: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST  */
#line 3638 "nd-flow-expr.ypp"
                         {
#if defined(_ND_ENABLE_NFQUEUE)
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->nfq.dst_iface.empty() == false
        ));
#else
        _NDFP_result = ((yyval.bool_result) = false);
#endif
        _NDFP_debugf("NFQ DST interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7878 "nd-flow-expr.cpp"
    break;

  case 486: /* expr_iface_nfq_dst: '!' FLOW_IFACE_NFQ_DST  */
#line 3649 "nd-flow-expr.ypp"
                             {
#if defined(_ND_ENABLE_NFQUEUE)
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->nfq.dst_iface.empty() == true
        ));
#else
        _NDFP_result = ((yyval.bool_result) = true);
#endif
        _NDFP_debugf("NFQ DST interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7894 "nd-flow-expr.cpp"
    break;

  case 487: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_NAME  */
#line 3660 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = false);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.dst_iface.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->nfq.dst_iface.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
#endif
        _NDFP_debugf("NFQ DST interface == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7918 "nd-flow-expr.cpp"
    break;

  case 488: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_NAME  */
#line 3679 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.dst_iface.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->nfq.dst_iface.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
#endif
        _NDFP_debugf("NFQ DST interface != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7942 "nd-flow-expr.cpp"
    break;

  case 489: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_REGEX  */
#line 3698 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.dst_iface.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
                rx, _NDFP_flow->nfq.dst_iface
            ));
        }

        _NDFP_debugf("NFQ DST interface (RX) == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
#endif
    }
#line 7962 "nd-flow-expr.cpp"
    break;

  case 490: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_REGEX  */
#line 3713 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
#if defined(_ND_ENABLE_NFQUEUE)
        if (! _NDFP_flow->nfq.dst_iface.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(
                rx, _NDFP_flow->nfq.dst_iface
            ));
        }
#endif
        _NDFP_debugf("NFQ DST interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7981 "nd-flow-expr.cpp"
    break;

  case 491: /* expr_intel: FLOW_INTEL  */
#line 3730 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[0].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {

            if (jvalue.is_boolean() && jvalue.get<bool>())
                _NDFP_result = ((yyval.bool_result) = true);
            else if (jvalue.is_number_float() && jvalue.get<float>() != 0.0f)
                _NDFP_result = ((yyval.bool_result) = true);
            else if (jvalue.is_number_integer() && jvalue.get<signed long>() != 0)
                _NDFP_result = ((yyval.bool_result) = true);
            else if (jvalue.is_number() && jvalue.get<unsigned long>() != 0)
                _NDFP_result = ((yyval.bool_result) = true);
            else if (jvalue.is_string() && ! jvalue.get<string>().empty())
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel set? %s: %s\n",
            key.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8007 "nd-flow-expr.cpp"
    break;

  case 492: /* expr_intel: '!' FLOW_INTEL  */
#line 3751 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[0].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_boolean() && jvalue.get<bool>())
                _NDFP_result = ((yyval.bool_result) = false);
            else if (jvalue.is_number_float() && jvalue.get<float>() != 0.0f)
                _NDFP_result = ((yyval.bool_result) = false);
            else if (jvalue.is_number_integer() && jvalue.get<signed long>() != 0)
                _NDFP_result = ((yyval.bool_result) = false);
            else if (jvalue.is_number() && jvalue.get<unsigned long>() != 0)
                _NDFP_result = ((yyval.bool_result) = false);
            else if (jvalue.is_string() && ! jvalue.get<string>().empty())
                _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel not set? %s: %s\n",
            key.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8032 "nd-flow-expr.cpp"
    break;

  case 493: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_NAME  */
#line 3773 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
            flow_intel(key, *_NDFP_params_intel, jvalue) && jvalue.is_string()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (! strncasecmp(search.c_str(),
              jvalue.get<string>().c_str(), _NDFP_MAX_BUFLEN))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s == %s? %s\n",
            key.c_str(), (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8057 "nd-flow-expr.cpp"
    break;

  case 494: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_REGEX  */
#line 3793 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)), search;
        json jvalue;
        if (_NDFP_params_intel &&
            flow_intel(key, *_NDFP_params_intel, jvalue) && jvalue.is_string()) {
            string rx((yyvsp[0].buffer));
            search = jvalue.get<string>();

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, search));
        }

        _NDFP_debugf("Flow intel %s (RX) == %s? %s\n",
            key.c_str(), search.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8078 "nd-flow-expr.cpp"
    break;

  case 495: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_UNSIGNED  */
#line 3809 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<unsigned long>() == (yyvsp[0].ul_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s == %lu? %s\n",
            key.c_str(), (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8095 "nd-flow-expr.cpp"
    break;

  case 496: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_SIGNED  */
#line 3821 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<signed long>() == (yyvsp[0].sl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s == %ld? %s\n",
            key.c_str(), (yyvsp[0].sl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8112 "nd-flow-expr.cpp"
    break;

  case 497: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FLOAT  */
#line 3833 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number_float() && jvalue.get<float>() == (yyvsp[0].fl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s == %.04f? %s\n",
            key.c_str(), (yyvsp[0].fl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8129 "nd-flow-expr.cpp"
    break;

  case 498: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_TRUE  */
#line 3845 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
          flow_intel(key, *_NDFP_params_intel, jvalue) &&
          jvalue.is_boolean() && jvalue.get<bool>()) {
            _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s == true? %s\n",
            key.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8147 "nd-flow-expr.cpp"
    break;

  case 499: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FALSE  */
#line 3858 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
          flow_intel(key, *_NDFP_params_intel, jvalue) &&
          jvalue.is_boolean() && ! jvalue.get<bool>()) {
            _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s == false? %s\n",
            key.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8165 "nd-flow-expr.cpp"
    break;

  case 500: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_NAME  */
#line 3873 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
            flow_intel(key, *_NDFP_params_intel, jvalue) && jvalue.is_string()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (! strncasecmp(search.c_str(),
              jvalue.get<string>().c_str(), _NDFP_MAX_BUFLEN))
                _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel %s != %s? %s\n",
            key.c_str(), (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8190 "nd-flow-expr.cpp"
    break;

  case 501: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_REGEX  */
#line 3893 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)), search;
        json jvalue;
        if (_NDFP_params_intel &&
            flow_intel(key, *_NDFP_params_intel, jvalue) && jvalue.is_string()) {
            string rx((yyvsp[0].buffer));
            search = jvalue.get<string>();

            _NDFP_result = ((yyval.bool_result) = ! _NDFP_regex_search(rx, search));
        }

        _NDFP_debugf("Flow intel %s (RX) != %s? %s\n",
            key.c_str(), search.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8211 "nd-flow-expr.cpp"
    break;

  case 502: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3909 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<unsigned long>() == (yyvsp[0].ul_number))
                _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel %s != %lu? %s\n",
            key.c_str(), (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8228 "nd-flow-expr.cpp"
    break;

  case 503: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_SIGNED  */
#line 3921 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<signed long>() == (yyvsp[0].sl_number))
                _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel %s != %ld? %s\n",
            key.c_str(), (yyvsp[0].sl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8245 "nd-flow-expr.cpp"
    break;

  case 504: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FLOAT  */
#line 3933 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number_float() && jvalue.get<float>() == (yyvsp[0].fl_number))
                _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel %s != %.04f? %s\n",
            key.c_str(), (yyvsp[0].fl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8262 "nd-flow-expr.cpp"
    break;

  case 505: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_TRUE  */
#line 3945 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
          flow_intel(key, *_NDFP_params_intel, jvalue) &&
          jvalue.is_boolean() && jvalue.get<bool>()) {
            _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel %s != true? %s\n",
            key.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8280 "nd-flow-expr.cpp"
    break;

  case 506: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FALSE  */
#line 3958 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
          flow_intel(key, *_NDFP_params_intel, jvalue) &&
          jvalue.is_boolean() && ! jvalue.get<bool>()) {
            _NDFP_result = ((yyval.bool_result) = false);
        }

        _NDFP_debugf("Flow intel %s != false? %s\n",
            key.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 8298 "nd-flow-expr.cpp"
    break;

  case 507: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3973 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<unsigned long>() >= (yyvsp[0].ul_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s >= %lu? %s\n",
            key.c_str(), (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8315 "nd-flow-expr.cpp"
    break;

  case 508: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_SIGNED  */
#line 3985 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<signed long>() >= (yyvsp[0].sl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s >= %ld? %s\n",
            key.c_str(), (yyvsp[0].sl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8332 "nd-flow-expr.cpp"
    break;

  case 509: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_FLOAT  */
#line 3997 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number_float() && jvalue.get<float>() >= (yyvsp[0].fl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s >= %.04f? %s\n",
            key.c_str(), (yyvsp[0].fl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8349 "nd-flow-expr.cpp"
    break;

  case 510: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4011 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<unsigned long>() <= (yyvsp[0].ul_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s <= %lu? %s\n",
            key.c_str(), (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8366 "nd-flow-expr.cpp"
    break;

  case 511: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_SIGNED  */
#line 4023 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<signed long>() <= (yyvsp[0].sl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s <= %ld? %s\n",
            key.c_str(), (yyvsp[0].sl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8383 "nd-flow-expr.cpp"
    break;

  case 512: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_FLOAT  */
#line 4035 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number_float() && jvalue.get<float>() <= (yyvsp[0].fl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s <= %.04f? %s\n",
            key.c_str(), (yyvsp[0].fl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8400 "nd-flow-expr.cpp"
    break;

  case 513: /* expr_intel: FLOW_INTEL '>' VALUE_UNSIGNED  */
#line 4049 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<unsigned long>() > (yyvsp[0].ul_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s > %lu? %s\n",
            key.c_str(), (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8417 "nd-flow-expr.cpp"
    break;

  case 514: /* expr_intel: FLOW_INTEL '>' VALUE_SIGNED  */
#line 4061 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<signed long>() > (yyvsp[0].sl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s > %ld? %s\n",
            key.c_str(), (yyvsp[0].sl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8434 "nd-flow-expr.cpp"
    break;

  case 515: /* expr_intel: FLOW_INTEL '>' VALUE_FLOAT  */
#line 4073 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number_float() && jvalue.get<float>() > (yyvsp[0].fl_number))
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s > %.04f? %s\n",
            key.c_str(), (yyvsp[0].fl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8451 "nd-flow-expr.cpp"
    break;

  case 516: /* expr_intel: FLOW_INTEL '<' VALUE_UNSIGNED  */
#line 4087 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<unsigned long>() < 0)
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s < %lu? %s\n",
            key.c_str(), (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8468 "nd-flow-expr.cpp"
    break;

  case 517: /* expr_intel: FLOW_INTEL '<' VALUE_SIGNED  */
#line 4099 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number() && jvalue.get<signed long>() < 0)
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s < %ld? %s\n",
            key.c_str(), (yyvsp[0].sl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8485 "nd-flow-expr.cpp"
    break;

  case 518: /* expr_intel: FLOW_INTEL '<' VALUE_FLOAT  */
#line 4111 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel && flow_intel(key, *_NDFP_params_intel, jvalue)) {
            if (jvalue.is_number_float() && jvalue.get<float>() < 0)
                _NDFP_result = ((yyval.bool_result) = true);
        }

        _NDFP_debugf("Flow intel %s < %.04f? %s\n",
            key.c_str(), (yyvsp[0].fl_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8502 "nd-flow-expr.cpp"
    break;

  case 519: /* expr_tls_version: FLOW_TLS_VERSION  */
#line 4126 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != 0));
        _NDFP_debugf("TLS version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8511 "nd-flow-expr.cpp"
    break;

  case 520: /* expr_tls_version: '!' FLOW_TLS_VERSION  */
#line 4130 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == 0));
        _NDFP_debugf("TLS version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8520 "nd-flow-expr.cpp"
    break;

  case 521: /* expr_tls_version: FLOW_TLS_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 4134 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8529 "nd-flow-expr.cpp"
    break;

  case 522: /* expr_tls_version: FLOW_TLS_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4138 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8538 "nd-flow-expr.cpp"
    break;

  case 523: /* expr_tls_version: FLOW_TLS_VERSION CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4142 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8547 "nd-flow-expr.cpp"
    break;

  case 524: /* expr_tls_version: FLOW_TLS_VERSION CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4146 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8556 "nd-flow-expr.cpp"
    break;

  case 525: /* expr_tls_version: FLOW_TLS_VERSION '>' VALUE_UNSIGNED  */
#line 4150 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8565 "nd-flow-expr.cpp"
    break;

  case 526: /* expr_tls_version: FLOW_TLS_VERSION '<' VALUE_UNSIGNED  */
#line 4154 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8574 "nd-flow-expr.cpp"
    break;

  case 527: /* expr_tls_cipher: FLOW_TLS_CIPHER  */
#line 4161 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != 0));
        _NDFP_debugf("TLS cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8583 "nd-flow-expr.cpp"
    break;

  case 528: /* expr_tls_cipher: '!' FLOW_TLS_CIPHER  */
#line 4165 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == 0));
        _NDFP_debugf("TLS cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8592 "nd-flow-expr.cpp"
    break;

  case 529: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_EQUAL VALUE_UNSIGNED  */
#line 4169 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8601 "nd-flow-expr.cpp"
    break;

  case 530: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4173 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8610 "nd-flow-expr.cpp"
    break;

  case 531: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4177 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8619 "nd-flow-expr.cpp"
    break;

  case 532: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4181 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8628 "nd-flow-expr.cpp"
    break;

  case 533: /* expr_tls_cipher: FLOW_TLS_CIPHER '>' VALUE_UNSIGNED  */
#line 4185 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8637 "nd-flow-expr.cpp"
    break;

  case 534: /* expr_tls_cipher: FLOW_TLS_CIPHER '<' VALUE_UNSIGNED  */
#line 4189 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8646 "nd-flow-expr.cpp"
    break;

  case 535: /* expr_tls_ech: FLOW_TLS_ECH  */
#line 4196 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != 0));
        _NDFP_debugf("TLS ECH version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8655 "nd-flow-expr.cpp"
    break;

  case 536: /* expr_tls_ech: '!' FLOW_TLS_ECH  */
#line 4200 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == 0));
        _NDFP_debugf("TLS ECH version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8664 "nd-flow-expr.cpp"
    break;

  case 537: /* expr_tls_ech: FLOW_TLS_ECH CMP_EQUAL VALUE_UNSIGNED  */
#line 4204 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8673 "nd-flow-expr.cpp"
    break;

  case 538: /* expr_tls_ech: FLOW_TLS_ECH CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4208 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8682 "nd-flow-expr.cpp"
    break;

  case 539: /* expr_tls_ech: FLOW_TLS_ECH CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4212 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8691 "nd-flow-expr.cpp"
    break;

  case 540: /* expr_tls_ech: FLOW_TLS_ECH CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4216 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8700 "nd-flow-expr.cpp"
    break;

  case 541: /* expr_tls_ech: FLOW_TLS_ECH '>' VALUE_UNSIGNED  */
#line 4220 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8709 "nd-flow-expr.cpp"
    break;

  case 542: /* expr_tls_ech: FLOW_TLS_ECH '<' VALUE_UNSIGNED  */
#line 4224 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8718 "nd-flow-expr.cpp"
    break;

  case 543: /* expr_tls_ja4: FLOW_TLS_JA4  */
#line 4231 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8730 "nd-flow-expr.cpp"
    break;

  case 544: /* expr_tls_ja4: '!' FLOW_TLS_JA4  */
#line 4238 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8742 "nd-flow-expr.cpp"
    break;

  case 545: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_NAME  */
#line 4245 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->tls.client_ja4.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->tls.client_ja4.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }

        _NDFP_debugf("TLS client JA4 == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8766 "nd-flow-expr.cpp"
    break;

  case 546: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_NAME  */
#line 4264 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = true);

        if (! _NDFP_flow->tls.client_ja4.empty()) {
            size_t p;
            string search((yyvsp[0].buffer));

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(search.c_str(),
                _NDFP_flow->tls.client_ja4.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }

        _NDFP_debugf("TLS client JA4 != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8790 "nd-flow-expr.cpp"
    break;

  case 547: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_REGEX  */
#line 4283 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = false);

        if (! _NDFP_flow->tls.client_ja4.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(
                rx, _NDFP_flow->tls.client_ja4
            ));
        }

        _NDFP_debugf("TLS client JA4 (RX) == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8809 "nd-flow-expr.cpp"
    break;

  case 548: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_REGEX  */
#line 4297 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = true);

        if (! _NDFP_flow->tls.client_ja4.empty()) {
            string rx((yyvsp[0].buffer));

            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(
                rx, _NDFP_flow->tls.client_ja4
            ));
        }

        _NDFP_debugf("TLS client JA4 (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8828 "nd-flow-expr.cpp"
    break;

  case 549: /* expr_origin: FLOW_ORIGIN  */
#line 4314 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8837 "nd-flow-expr.cpp"
    break;

  case 550: /* expr_origin: '!' FLOW_ORIGIN  */
#line 4318 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8846 "nd-flow-expr.cpp"
    break;

  case 551: /* expr_origin: FLOW_ORIGIN CMP_EQUAL value_origin_type  */
#line 4322 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8855 "nd-flow-expr.cpp"
    break;

  case 552: /* expr_origin: FLOW_ORIGIN CMP_NOTEQUAL value_origin_type  */
#line 4326 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8864 "nd-flow-expr.cpp"
    break;

  case 553: /* value_origin_type: FLOW_ORIGIN_LOCAL  */
#line 4333 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 8870 "nd-flow-expr.cpp"
    break;

  case 554: /* value_origin_type: FLOW_ORIGIN_OTHER  */
#line 4334 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 8876 "nd-flow-expr.cpp"
    break;

  case 555: /* value_origin_type: FLOW_ORIGIN_UNKNOWN  */
#line 4335 "nd-flow-expr.ypp"
                          { (yyval.us_number) = (yyvsp[0].us_number); }
#line 8882 "nd-flow-expr.cpp"
    break;

  case 556: /* expr_tag: FLOW_TAG  */
#line 4339 "nd-flow-expr.ypp"
               {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags not empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8894 "nd-flow-expr.cpp"
    break;

  case 557: /* expr_tag: '!' FLOW_TAG  */
#line 4346 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8906 "nd-flow-expr.cpp"
    break;

  case 558: /* expr_tag: FLOW_TAG CMP_EQUAL VALUE_NAME  */
#line 4353 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = false);

        size_t p;
        string search((yyvsp[0].buffer));

        while ((p = search.find_first_of("'\"")) != string::npos)
            search.erase(p, 1);

        for (auto &tag : _NDFP_flow->tags) {
            if (strncasecmp(search.c_str(),
                tag.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
                break;
            }

            if ((p = tag.find_first_of(".")) != string::npos) {
                if (strncasecmp(search.c_str(),
                    tag.substr(0, p).c_str(), _NDFP_MAX_BUFLEN) == 0) {
                    _NDFP_result = ((yyval.bool_result) = true);
                    break;
                }
            }
        }

        _NDFP_debugf("Overlay tag == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8939 "nd-flow-expr.cpp"
    break;

  case 559: /* expr_tag: FLOW_TAG CMP_NOTEQUAL VALUE_NAME  */
#line 4381 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = true);

        size_t p;
        string search((yyvsp[0].buffer));

        while ((p = search.find_first_of("'\"")) != string::npos)
            search.erase(p, 1);

        for (auto &tag : _NDFP_flow->tags) {
            if (strncasecmp(search.c_str(),
                tag.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
                break;
            }

            if ((p = tag.find_first_of(".")) != string::npos) {
                if (strncasecmp(search.c_str(),
                    tag.substr(0, p).c_str(), _NDFP_MAX_BUFLEN) == 0) {
                    _NDFP_result = ((yyval.bool_result) = false);
                    break;
                }
            }
        }

        _NDFP_debugf("Overlay tag != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 8972 "nd-flow-expr.cpp"
    break;

  case 560: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE  */
#line 4412 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == true));
    }
#line 8980 "nd-flow-expr.cpp"
    break;

  case 561: /* expr_app_ip_override: '!' FLOW_APP_IP_OVERRIDE  */
#line 4415 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == false));
    }
#line 8988 "nd-flow-expr.cpp"
    break;

  case 562: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_EQUAL VALUE_TRUE  */
#line 4418 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == true));
    }
#line 8996 "nd-flow-expr.cpp"
    break;

  case 563: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_EQUAL VALUE_FALSE  */
#line 4421 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == false));
    }
#line 9004 "nd-flow-expr.cpp"
    break;

  case 564: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_NOTEQUAL VALUE_TRUE  */
#line 4424 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() != true));
    }
#line 9012 "nd-flow-expr.cpp"
    break;

  case 565: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_NOTEQUAL VALUE_FALSE  */
#line 4427 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() != false));
    }
#line 9020 "nd-flow-expr.cpp"
    break;

  case 566: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS  */
#line 4433 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == true));
    }
#line 9028 "nd-flow-expr.cpp"
    break;

  case 567: /* expr_app_proto_twins: '!' FLOW_APP_PROTO_TWINS  */
#line 4436 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == false));
    }
#line 9036 "nd-flow-expr.cpp"
    break;

  case 568: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_EQUAL VALUE_TRUE  */
#line 4439 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == true));
    }
#line 9044 "nd-flow-expr.cpp"
    break;

  case 569: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_EQUAL VALUE_FALSE  */
#line 4442 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == false));
    }
#line 9052 "nd-flow-expr.cpp"
    break;

  case 570: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_NOTEQUAL VALUE_TRUE  */
#line 4445 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() != true));
    }
#line 9060 "nd-flow-expr.cpp"
    break;

  case 571: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_NOTEQUAL VALUE_FALSE  */
#line 4448 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() != false));
    }
#line 9068 "nd-flow-expr.cpp"
    break;

  case 572: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_EQUAL VALUE_UNSIGNED  */
#line 4454 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen == (yyvsp[0].ul_number)));
    }
#line 9076 "nd-flow-expr.cpp"
    break;

  case 573: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4457 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen != (yyvsp[0].ul_number)));
    }
#line 9084 "nd-flow-expr.cpp"
    break;

  case 574: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4460 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen >= (yyvsp[0].ul_number)));
    }
#line 9092 "nd-flow-expr.cpp"
    break;

  case 575: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4463 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen <= (yyvsp[0].ul_number)));
    }
#line 9100 "nd-flow-expr.cpp"
    break;

  case 576: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN '>' VALUE_UNSIGNED  */
#line 4466 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen > (yyvsp[0].ul_number)));
    }
#line 9108 "nd-flow-expr.cpp"
    break;

  case 577: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN '<' VALUE_UNSIGNED  */
#line 4469 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen < (yyvsp[0].ul_number)));
    }
#line 9116 "nd-flow-expr.cpp"
    break;

  case 578: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_EQUAL VALUE_UNSIGNED  */
#line 4475 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() == (yyvsp[0].ul_number)));
    }
#line 9124 "nd-flow-expr.cpp"
    break;

  case 579: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4478 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() != (yyvsp[0].ul_number)));
    }
#line 9132 "nd-flow-expr.cpp"
    break;

  case 580: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4481 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() >= (yyvsp[0].ul_number)));
    }
#line 9140 "nd-flow-expr.cpp"
    break;

  case 581: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4484 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() <= (yyvsp[0].ul_number)));
    }
#line 9148 "nd-flow-expr.cpp"
    break;

  case 582: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN '>' VALUE_UNSIGNED  */
#line 4487 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() > (yyvsp[0].ul_number)));
    }
#line 9156 "nd-flow-expr.cpp"
    break;

  case 583: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN '<' VALUE_UNSIGNED  */
#line 4490 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() < (yyvsp[0].ul_number)));
    }
#line 9164 "nd-flow-expr.cpp"
    break;

  case 584: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_EQUAL VALUE_UNSIGNED  */
#line 4496 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex == (yyvsp[0].ul_number)));
    }
#line 9172 "nd-flow-expr.cpp"
    break;

  case 585: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4499 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex != (yyvsp[0].ul_number)));
    }
#line 9180 "nd-flow-expr.cpp"
    break;

  case 586: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4502 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex >= (yyvsp[0].ul_number)));
    }
#line 9188 "nd-flow-expr.cpp"
    break;

  case 587: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4505 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex <= (yyvsp[0].ul_number)));
    }
#line 9196 "nd-flow-expr.cpp"
    break;

  case 588: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX '>' VALUE_UNSIGNED  */
#line 4508 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex > (yyvsp[0].ul_number)));
    }
#line 9204 "nd-flow-expr.cpp"
    break;

  case 589: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX '<' VALUE_UNSIGNED  */
#line 4511 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex < (yyvsp[0].ul_number)));
    }
#line 9212 "nd-flow-expr.cpp"
    break;

  case 590: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_EQUAL VALUE_UNSIGNED  */
#line 4517 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex == (yyvsp[0].ul_number)));
    }
#line 9220 "nd-flow-expr.cpp"
    break;

  case 591: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4520 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex != (yyvsp[0].ul_number)));
    }
#line 9228 "nd-flow-expr.cpp"
    break;

  case 592: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4523 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex >= (yyvsp[0].ul_number)));
    }
#line 9236 "nd-flow-expr.cpp"
    break;

  case 593: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4526 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex <= (yyvsp[0].ul_number)));
    }
#line 9244 "nd-flow-expr.cpp"
    break;

  case 594: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX '>' VALUE_UNSIGNED  */
#line 4529 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex > (yyvsp[0].ul_number)));
    }
#line 9252 "nd-flow-expr.cpp"
    break;

  case 595: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX '<' VALUE_UNSIGNED  */
#line 4532 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex < (yyvsp[0].ul_number)));
    }
#line 9260 "nd-flow-expr.cpp"
    break;

  case 596: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_EQUAL VALUE_UNSIGNED  */
#line 4538 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() == (yyvsp[0].ul_number)));
    }
#line 9268 "nd-flow-expr.cpp"
    break;

  case 597: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4541 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() != (yyvsp[0].ul_number)));
    }
#line 9276 "nd-flow-expr.cpp"
    break;

  case 598: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4544 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() >= (yyvsp[0].ul_number)));
    }
#line 9284 "nd-flow-expr.cpp"
    break;

  case 599: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4547 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() <= (yyvsp[0].ul_number)));
    }
#line 9292 "nd-flow-expr.cpp"
    break;

  case 600: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK '>' VALUE_UNSIGNED  */
#line 4550 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() > (yyvsp[0].ul_number)));
    }
#line 9300 "nd-flow-expr.cpp"
    break;

  case 601: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK '<' VALUE_UNSIGNED  */
#line 4553 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() < (yyvsp[0].ul_number)));
    }
#line 9308 "nd-flow-expr.cpp"
    break;

  case 602: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_EQUAL VALUE_UNSIGNED  */
#line 4559 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() == (yyvsp[0].ul_number)));
    }
#line 9316 "nd-flow-expr.cpp"
    break;

  case 603: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4562 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() != (yyvsp[0].ul_number)));
    }
#line 9324 "nd-flow-expr.cpp"
    break;

  case 604: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4565 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() >= (yyvsp[0].ul_number)));
    }
#line 9332 "nd-flow-expr.cpp"
    break;

  case 605: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4568 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() <= (yyvsp[0].ul_number)));
    }
#line 9340 "nd-flow-expr.cpp"
    break;

  case 606: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ '>' VALUE_UNSIGNED  */
#line 4571 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() > (yyvsp[0].ul_number)));
    }
#line 9348 "nd-flow-expr.cpp"
    break;

  case 607: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ '<' VALUE_UNSIGNED  */
#line 4574 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() < (yyvsp[0].ul_number)));
    }
#line 9356 "nd-flow-expr.cpp"
    break;

  case 608: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4580 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() == (yyvsp[0].ul_number)));
    }
#line 9364 "nd-flow-expr.cpp"
    break;

  case 609: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4583 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() != (yyvsp[0].ul_number)));
    }
#line 9372 "nd-flow-expr.cpp"
    break;

  case 610: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4586 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() >= (yyvsp[0].ul_number)));
    }
#line 9380 "nd-flow-expr.cpp"
    break;

  case 611: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4589 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() <= (yyvsp[0].ul_number)));
    }
#line 9388 "nd-flow-expr.cpp"
    break;

  case 612: /* expr_total_bytes: FLOW_TOTAL_BYTES '>' VALUE_UNSIGNED  */
#line 4592 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() > (yyvsp[0].ul_number)));
    }
#line 9396 "nd-flow-expr.cpp"
    break;

  case 613: /* expr_total_bytes: FLOW_TOTAL_BYTES '<' VALUE_UNSIGNED  */
#line 4595 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() < (yyvsp[0].ul_number)));
    }
#line 9404 "nd-flow-expr.cpp"
    break;

  case 614: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4601 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() == (yyvsp[0].ul_number)));
    }
#line 9412 "nd-flow-expr.cpp"
    break;

  case 615: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4604 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() != (yyvsp[0].ul_number)));
    }
#line 9420 "nd-flow-expr.cpp"
    break;

  case 616: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4607 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() >= (yyvsp[0].ul_number)));
    }
#line 9428 "nd-flow-expr.cpp"
    break;

  case 617: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4610 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() <= (yyvsp[0].ul_number)));
    }
#line 9436 "nd-flow-expr.cpp"
    break;

  case 618: /* expr_total_packets: FLOW_TOTAL_PACKETS '>' VALUE_UNSIGNED  */
#line 4613 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() > (yyvsp[0].ul_number)));
    }
#line 9444 "nd-flow-expr.cpp"
    break;

  case 619: /* expr_total_packets: FLOW_TOTAL_PACKETS '<' VALUE_UNSIGNED  */
#line 4616 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() < (yyvsp[0].ul_number)));
    }
#line 9452 "nd-flow-expr.cpp"
    break;

  case 620: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4622 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9461 "nd-flow-expr.cpp"
    break;

  case 621: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4626 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9470 "nd-flow-expr.cpp"
    break;

  case 622: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4630 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9479 "nd-flow-expr.cpp"
    break;

  case 623: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4634 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9488 "nd-flow-expr.cpp"
    break;

  case 624: /* expr_local_bytes: FLOW_LOCAL_BYTES '>' VALUE_UNSIGNED  */
#line 4638 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9497 "nd-flow-expr.cpp"
    break;

  case 625: /* expr_local_bytes: FLOW_LOCAL_BYTES '<' VALUE_UNSIGNED  */
#line 4642 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9506 "nd-flow-expr.cpp"
    break;

  case 626: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4649 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9515 "nd-flow-expr.cpp"
    break;

  case 627: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4653 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9524 "nd-flow-expr.cpp"
    break;

  case 628: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4657 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9533 "nd-flow-expr.cpp"
    break;

  case 629: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4661 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9542 "nd-flow-expr.cpp"
    break;

  case 630: /* expr_other_bytes: FLOW_OTHER_BYTES '>' VALUE_UNSIGNED  */
#line 4665 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9551 "nd-flow-expr.cpp"
    break;

  case 631: /* expr_other_bytes: FLOW_OTHER_BYTES '<' VALUE_UNSIGNED  */
#line 4669 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9560 "nd-flow-expr.cpp"
    break;

  case 632: /* expr_src_bytes: FLOW_SRC_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4676 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9569 "nd-flow-expr.cpp"
    break;

  case 633: /* expr_src_bytes: FLOW_SRC_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4680 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9578 "nd-flow-expr.cpp"
    break;

  case 634: /* expr_src_bytes: FLOW_SRC_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4684 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9587 "nd-flow-expr.cpp"
    break;

  case 635: /* expr_src_bytes: FLOW_SRC_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4688 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9596 "nd-flow-expr.cpp"
    break;

  case 636: /* expr_src_bytes: FLOW_SRC_BYTES '>' VALUE_UNSIGNED  */
#line 4692 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9605 "nd-flow-expr.cpp"
    break;

  case 637: /* expr_src_bytes: FLOW_SRC_BYTES '<' VALUE_UNSIGNED  */
#line 4696 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9614 "nd-flow-expr.cpp"
    break;

  case 638: /* expr_dst_bytes: FLOW_DST_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4703 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9623 "nd-flow-expr.cpp"
    break;

  case 639: /* expr_dst_bytes: FLOW_DST_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4707 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9632 "nd-flow-expr.cpp"
    break;

  case 640: /* expr_dst_bytes: FLOW_DST_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4711 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9641 "nd-flow-expr.cpp"
    break;

  case 641: /* expr_dst_bytes: FLOW_DST_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4715 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9650 "nd-flow-expr.cpp"
    break;

  case 642: /* expr_dst_bytes: FLOW_DST_BYTES '>' VALUE_UNSIGNED  */
#line 4719 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9659 "nd-flow-expr.cpp"
    break;

  case 643: /* expr_dst_bytes: FLOW_DST_BYTES '<' VALUE_UNSIGNED  */
#line 4723 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9668 "nd-flow-expr.cpp"
    break;

  case 644: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4730 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9677 "nd-flow-expr.cpp"
    break;

  case 645: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4734 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9686 "nd-flow-expr.cpp"
    break;

  case 646: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4738 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9695 "nd-flow-expr.cpp"
    break;

  case 647: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4742 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9704 "nd-flow-expr.cpp"
    break;

  case 648: /* expr_local_packets: FLOW_LOCAL_PACKETS '>' VALUE_UNSIGNED  */
#line 4746 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9713 "nd-flow-expr.cpp"
    break;

  case 649: /* expr_local_packets: FLOW_LOCAL_PACKETS '<' VALUE_UNSIGNED  */
#line 4750 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9722 "nd-flow-expr.cpp"
    break;

  case 650: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4757 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9731 "nd-flow-expr.cpp"
    break;

  case 651: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4761 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9740 "nd-flow-expr.cpp"
    break;

  case 652: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4765 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9749 "nd-flow-expr.cpp"
    break;

  case 653: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4769 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9758 "nd-flow-expr.cpp"
    break;

  case 654: /* expr_other_packets: FLOW_OTHER_PACKETS '>' VALUE_UNSIGNED  */
#line 4773 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9767 "nd-flow-expr.cpp"
    break;

  case 655: /* expr_other_packets: FLOW_OTHER_PACKETS '<' VALUE_UNSIGNED  */
#line 4777 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9776 "nd-flow-expr.cpp"
    break;

  case 656: /* expr_src_packets: FLOW_SRC_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4784 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9785 "nd-flow-expr.cpp"
    break;

  case 657: /* expr_src_packets: FLOW_SRC_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4788 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9794 "nd-flow-expr.cpp"
    break;

  case 658: /* expr_src_packets: FLOW_SRC_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4792 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9803 "nd-flow-expr.cpp"
    break;

  case 659: /* expr_src_packets: FLOW_SRC_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4796 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9812 "nd-flow-expr.cpp"
    break;

  case 660: /* expr_src_packets: FLOW_SRC_PACKETS '>' VALUE_UNSIGNED  */
#line 4800 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9821 "nd-flow-expr.cpp"
    break;

  case 661: /* expr_src_packets: FLOW_SRC_PACKETS '<' VALUE_UNSIGNED  */
#line 4804 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9830 "nd-flow-expr.cpp"
    break;

  case 662: /* expr_dst_packets: FLOW_DST_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4811 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9839 "nd-flow-expr.cpp"
    break;

  case 663: /* expr_dst_packets: FLOW_DST_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4815 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9848 "nd-flow-expr.cpp"
    break;

  case 664: /* expr_dst_packets: FLOW_DST_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4819 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9857 "nd-flow-expr.cpp"
    break;

  case 665: /* expr_dst_packets: FLOW_DST_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4823 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9866 "nd-flow-expr.cpp"
    break;

  case 666: /* expr_dst_packets: FLOW_DST_PACKETS '>' VALUE_UNSIGNED  */
#line 4827 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9875 "nd-flow-expr.cpp"
    break;

  case 667: /* expr_dst_packets: FLOW_DST_PACKETS '<' VALUE_UNSIGNED  */
#line 4831 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9884 "nd-flow-expr.cpp"
    break;

  case 668: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT  */
#line 4838 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.user_agent.empty() == false));
    }
#line 9892 "nd-flow-expr.cpp"
    break;

  case 669: /* expr_http_user_agent: '!' FLOW_HTTP_USER_AGENT  */
#line 4841 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.user_agent.empty() == true));
    }
#line 9900 "nd-flow-expr.cpp"
    break;

  case 670: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_EQUAL VALUE_NAME  */
#line 4844 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->http.user_agent.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->http.user_agent.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 9915 "nd-flow-expr.cpp"
    break;

  case 671: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_NOTEQUAL VALUE_NAME  */
#line 4854 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->http.user_agent.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->http.user_agent.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 9930 "nd-flow-expr.cpp"
    break;

  case 672: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_EQUAL VALUE_REGEX  */
#line 4864 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->http.user_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->http.user_agent));
        }
    }
#line 9942 "nd-flow-expr.cpp"
    break;

  case 673: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_NOTEQUAL VALUE_REGEX  */
#line 4871 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->http.user_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->http.user_agent));
        }
    }
#line 9954 "nd-flow-expr.cpp"
    break;

  case 674: /* expr_http_url: FLOW_HTTP_URL  */
#line 4881 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.url.empty() == false));
    }
#line 9962 "nd-flow-expr.cpp"
    break;

  case 675: /* expr_http_url: '!' FLOW_HTTP_URL  */
#line 4884 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.url.empty() == true));
    }
#line 9970 "nd-flow-expr.cpp"
    break;

  case 676: /* expr_http_url: FLOW_HTTP_URL CMP_EQUAL VALUE_NAME  */
#line 4887 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->http.url.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->http.url.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 9985 "nd-flow-expr.cpp"
    break;

  case 677: /* expr_http_url: FLOW_HTTP_URL CMP_NOTEQUAL VALUE_NAME  */
#line 4897 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->http.url.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->http.url.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10000 "nd-flow-expr.cpp"
    break;

  case 678: /* expr_http_url: FLOW_HTTP_URL CMP_EQUAL VALUE_REGEX  */
#line 4907 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->http.url.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->http.url));
        }
    }
#line 10012 "nd-flow-expr.cpp"
    break;

  case 679: /* expr_http_url: FLOW_HTTP_URL CMP_NOTEQUAL VALUE_REGEX  */
#line 4914 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->http.url.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->http.url));
        }
    }
#line 10024 "nd-flow-expr.cpp"
    break;

  case 680: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT  */
#line 4924 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.fingerprint.empty() == false));
    }
#line 10032 "nd-flow-expr.cpp"
    break;

  case 681: /* expr_dhcp_fingerprint: '!' FLOW_DHCP_FINGERPRINT  */
#line 4927 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.fingerprint.empty() == true));
    }
#line 10040 "nd-flow-expr.cpp"
    break;

  case 682: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_EQUAL VALUE_NAME  */
#line 4930 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->dhcp.fingerprint.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->dhcp.fingerprint.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10055 "nd-flow-expr.cpp"
    break;

  case 683: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_NOTEQUAL VALUE_NAME  */
#line 4940 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->dhcp.fingerprint.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->dhcp.fingerprint.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10070 "nd-flow-expr.cpp"
    break;

  case 684: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_EQUAL VALUE_REGEX  */
#line 4950 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->dhcp.fingerprint.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->dhcp.fingerprint));
        }
    }
#line 10082 "nd-flow-expr.cpp"
    break;

  case 685: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_NOTEQUAL VALUE_REGEX  */
#line 4957 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->dhcp.fingerprint.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->dhcp.fingerprint));
        }
    }
#line 10094 "nd-flow-expr.cpp"
    break;

  case 686: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT  */
#line 4967 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.class_ident.empty() == false));
    }
#line 10102 "nd-flow-expr.cpp"
    break;

  case 687: /* expr_dhcp_class_ident: '!' FLOW_DHCP_CLASS_IDENT  */
#line 4970 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.class_ident.empty() == true));
    }
#line 10110 "nd-flow-expr.cpp"
    break;

  case 688: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_EQUAL VALUE_NAME  */
#line 4973 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->dhcp.class_ident.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->dhcp.class_ident.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10125 "nd-flow-expr.cpp"
    break;

  case 689: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_NOTEQUAL VALUE_NAME  */
#line 4983 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->dhcp.class_ident.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->dhcp.class_ident.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10140 "nd-flow-expr.cpp"
    break;

  case 690: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_EQUAL VALUE_REGEX  */
#line 4993 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->dhcp.class_ident.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->dhcp.class_ident));
        }
    }
#line 10152 "nd-flow-expr.cpp"
    break;

  case 691: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_NOTEQUAL VALUE_REGEX  */
#line 5000 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->dhcp.class_ident.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->dhcp.class_ident));
        }
    }
#line 10164 "nd-flow-expr.cpp"
    break;

  case 692: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT  */
#line 5010 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.client_agent.empty() == false));
    }
#line 10172 "nd-flow-expr.cpp"
    break;

  case 693: /* expr_ssh_client_agent: '!' FLOW_SSH_CLIENT_AGENT  */
#line 5013 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.client_agent.empty() == true));
    }
#line 10180 "nd-flow-expr.cpp"
    break;

  case 694: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_EQUAL VALUE_NAME  */
#line 5016 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->ssh.client_agent.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->ssh.client_agent.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10195 "nd-flow-expr.cpp"
    break;

  case 695: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_NOTEQUAL VALUE_NAME  */
#line 5026 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->ssh.client_agent.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->ssh.client_agent.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10210 "nd-flow-expr.cpp"
    break;

  case 696: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_EQUAL VALUE_REGEX  */
#line 5036 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->ssh.client_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->ssh.client_agent));
        }
    }
#line 10222 "nd-flow-expr.cpp"
    break;

  case 697: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_NOTEQUAL VALUE_REGEX  */
#line 5043 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->ssh.client_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->ssh.client_agent));
        }
    }
#line 10234 "nd-flow-expr.cpp"
    break;

  case 698: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT  */
#line 5053 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.server_agent.empty() == false));
    }
#line 10242 "nd-flow-expr.cpp"
    break;

  case 699: /* expr_ssh_server_agent: '!' FLOW_SSH_SERVER_AGENT  */
#line 5056 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.server_agent.empty() == true));
    }
#line 10250 "nd-flow-expr.cpp"
    break;

  case 700: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_EQUAL VALUE_NAME  */
#line 5059 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->ssh.server_agent.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->ssh.server_agent.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10265 "nd-flow-expr.cpp"
    break;

  case 701: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_NOTEQUAL VALUE_NAME  */
#line 5069 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->ssh.server_agent.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->ssh.server_agent.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10280 "nd-flow-expr.cpp"
    break;

  case 702: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_EQUAL VALUE_REGEX  */
#line 5079 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->ssh.server_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->ssh.server_agent));
        }
    }
#line 10292 "nd-flow-expr.cpp"
    break;

  case 703: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_NOTEQUAL VALUE_REGEX  */
#line 5086 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->ssh.server_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->ssh.server_agent));
        }
    }
#line 10304 "nd-flow-expr.cpp"
    break;

  case 704: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN  */
#line 5096 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.subject_dn.empty() == false));
    }
#line 10312 "nd-flow-expr.cpp"
    break;

  case 705: /* expr_tls_subject_dn: '!' FLOW_TLS_SUBJECT_DN  */
#line 5099 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.subject_dn.empty() == true));
    }
#line 10320 "nd-flow-expr.cpp"
    break;

  case 706: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_EQUAL VALUE_NAME  */
#line 5102 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.subject_dn.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->tls.subject_dn.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10335 "nd-flow-expr.cpp"
    break;

  case 707: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_NOTEQUAL VALUE_NAME  */
#line 5112 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.subject_dn.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->tls.subject_dn.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10350 "nd-flow-expr.cpp"
    break;

  case 708: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_EQUAL VALUE_REGEX  */
#line 5122 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.subject_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->tls.subject_dn));
        }
    }
#line 10362 "nd-flow-expr.cpp"
    break;

  case 709: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_NOTEQUAL VALUE_REGEX  */
#line 5129 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.subject_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->tls.subject_dn));
        }
    }
#line 10374 "nd-flow-expr.cpp"
    break;

  case 710: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN  */
#line 5139 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.issuer_dn.empty() == false));
    }
#line 10382 "nd-flow-expr.cpp"
    break;

  case 711: /* expr_tls_issuer_dn: '!' FLOW_TLS_ISSUER_DN  */
#line 5142 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.issuer_dn.empty() == true));
    }
#line 10390 "nd-flow-expr.cpp"
    break;

  case 712: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_EQUAL VALUE_NAME  */
#line 5145 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.issuer_dn.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->tls.issuer_dn.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10405 "nd-flow-expr.cpp"
    break;

  case 713: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_NOTEQUAL VALUE_NAME  */
#line 5155 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.issuer_dn.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->tls.issuer_dn.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10420 "nd-flow-expr.cpp"
    break;

  case 714: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_EQUAL VALUE_REGEX  */
#line 5165 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.issuer_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->tls.issuer_dn));
        }
    }
#line 10432 "nd-flow-expr.cpp"
    break;

  case 715: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_NOTEQUAL VALUE_REGEX  */
#line 5172 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.issuer_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->tls.issuer_dn));
        }
    }
#line 10444 "nd-flow-expr.cpp"
    break;

  case 716: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN  */
#line 5182 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.server_cn.empty() == false));
    }
#line 10452 "nd-flow-expr.cpp"
    break;

  case 717: /* expr_tls_server_cn: '!' FLOW_TLS_SERVER_CN  */
#line 5185 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.server_cn.empty() == true));
    }
#line 10460 "nd-flow-expr.cpp"
    break;

  case 718: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_EQUAL VALUE_NAME  */
#line 5188 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.server_cn.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->tls.server_cn.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10475 "nd-flow-expr.cpp"
    break;

  case 719: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_NOTEQUAL VALUE_NAME  */
#line 5198 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.server_cn.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->tls.server_cn.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10490 "nd-flow-expr.cpp"
    break;

  case 720: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_EQUAL VALUE_REGEX  */
#line 5208 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.server_cn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->tls.server_cn));
        }
    }
#line 10502 "nd-flow-expr.cpp"
    break;

  case 721: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_NOTEQUAL VALUE_REGEX  */
#line 5215 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.server_cn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->tls.server_cn));
        }
    }
#line 10514 "nd-flow-expr.cpp"
    break;

  case 722: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME  */
#line 5225 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->mdns.domain_name.empty() == false));
    }
#line 10522 "nd-flow-expr.cpp"
    break;

  case 723: /* expr_mdns_domain_name: '!' FLOW_MDNS_DOMAIN_NAME  */
#line 5228 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->mdns.domain_name.empty() == true));
    }
#line 10530 "nd-flow-expr.cpp"
    break;

  case 724: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_EQUAL VALUE_NAME  */
#line 5231 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->mdns.domain_name.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->mdns.domain_name.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }
    }
#line 10545 "nd-flow-expr.cpp"
    break;

  case 725: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_NOTEQUAL VALUE_NAME  */
#line 5241 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->mdns.domain_name.empty()) {
            size_t p; std::string search((yyvsp[0].buffer));
            while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
            if (strncasecmp(search.c_str(), _NDFP_flow->mdns.domain_name.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }
    }
#line 10560 "nd-flow-expr.cpp"
    break;

  case 726: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_EQUAL VALUE_REGEX  */
#line 5251 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->mdns.domain_name.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->mdns.domain_name));
        }
    }
#line 10572 "nd-flow-expr.cpp"
    break;

  case 727: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_NOTEQUAL VALUE_REGEX  */
#line 5258 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->mdns.domain_name.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->mdns.domain_name));
        }
    }
#line 10584 "nd-flow-expr.cpp"
    break;

  case 728: /* expr_tls_alpn: FLOW_TLS_ALPN  */
#line 5268 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (! _NDFP_flow->tls.alpn.empty()));
    }
#line 10592 "nd-flow-expr.cpp"
    break;

  case 729: /* expr_tls_alpn: '!' FLOW_TLS_ALPN  */
#line 5271 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.alpn.empty()));
    }
#line 10600 "nd-flow-expr.cpp"
    break;

  case 730: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_EQUAL VALUE_NAME  */
#line 5274 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = false);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        for (auto &item : _NDFP_flow->tls.alpn) {
            if (strncasecmp(search.c_str(), item.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
                break;
            }
        }
    }
#line 10616 "nd-flow-expr.cpp"
    break;

  case 731: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_NOTEQUAL VALUE_NAME  */
#line 5285 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = true);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        for (auto &item : _NDFP_flow->tls.alpn) {
            if (strncasecmp(search.c_str(), item.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
                break;
            }
        }
    }
#line 10632 "nd-flow-expr.cpp"
    break;

  case 732: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_EQUAL VALUE_REGEX  */
#line 5296 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = false);
        std::string rx((yyvsp[0].buffer));
        for (auto &item : _NDFP_flow->tls.alpn) {
            if (_NDFP_regex_search(rx, item)) {
                _NDFP_result = ((yyval.bool_result) = true);
                break;
            }
        }
    }
#line 10647 "nd-flow-expr.cpp"
    break;

  case 733: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_NOTEQUAL VALUE_REGEX  */
#line 5306 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = true);
        std::string rx((yyvsp[0].buffer));
        for (auto &item : _NDFP_flow->tls.alpn) {
            if (_NDFP_regex_search(rx, item)) {
                _NDFP_result = ((yyval.bool_result) = false);
                break;
            }
        }
    }
#line 10662 "nd-flow-expr.cpp"
    break;

  case 734: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO  */
#line 5319 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == true));
    }
#line 10670 "nd-flow-expr.cpp"
    break;

  case 735: /* expr_tls_proc_hello: '!' FLOW_TLS_PROC_HELLO  */
#line 5322 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == false));
    }
#line 10678 "nd-flow-expr.cpp"
    break;

  case 736: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_EQUAL VALUE_TRUE  */
#line 5325 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == true));
    }
#line 10686 "nd-flow-expr.cpp"
    break;

  case 737: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_EQUAL VALUE_FALSE  */
#line 5328 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == false));
    }
#line 10694 "nd-flow-expr.cpp"
    break;

  case 738: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_NOTEQUAL VALUE_TRUE  */
#line 5331 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() != true));
    }
#line 10702 "nd-flow-expr.cpp"
    break;

  case 739: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_NOTEQUAL VALUE_FALSE  */
#line 5334 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() != false));
    }
#line 10710 "nd-flow-expr.cpp"
    break;

  case 740: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE  */
#line 5340 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == true));
    }
#line 10718 "nd-flow-expr.cpp"
    break;

  case 741: /* expr_tls_proc_certificate: '!' FLOW_TLS_PROC_CERTIFICATE  */
#line 5343 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == false));
    }
#line 10726 "nd-flow-expr.cpp"
    break;

  case 742: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_EQUAL VALUE_TRUE  */
#line 5346 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == true));
    }
#line 10734 "nd-flow-expr.cpp"
    break;

  case 743: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_EQUAL VALUE_FALSE  */
#line 5349 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == false));
    }
#line 10742 "nd-flow-expr.cpp"
    break;

  case 744: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_NOTEQUAL VALUE_TRUE  */
#line 5352 "nd-flow-expr.ypp"
                                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() != true));
    }
#line 10750 "nd-flow-expr.cpp"
    break;

  case 745: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_NOTEQUAL VALUE_FALSE  */
#line 5355 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() != false));
    }
#line 10758 "nd-flow-expr.cpp"
    break;

  case 746: /* expr_smtp_tls: FLOW_SMTP_TLS  */
#line 5361 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == true));
    }
#line 10766 "nd-flow-expr.cpp"
    break;

  case 747: /* expr_smtp_tls: '!' FLOW_SMTP_TLS  */
#line 5364 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == false));
    }
#line 10774 "nd-flow-expr.cpp"
    break;

  case 748: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_EQUAL VALUE_TRUE  */
#line 5367 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == true));
    }
#line 10782 "nd-flow-expr.cpp"
    break;

  case 749: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_EQUAL VALUE_FALSE  */
#line 5370 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == false));
    }
#line 10790 "nd-flow-expr.cpp"
    break;

  case 750: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_NOTEQUAL VALUE_TRUE  */
#line 5373 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls != true));
    }
#line 10798 "nd-flow-expr.cpp"
    break;

  case 751: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_NOTEQUAL VALUE_FALSE  */
#line 5376 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls != false));
    }
#line 10806 "nd-flow-expr.cpp"
    break;

  case 752: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5382 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10819 "nd-flow-expr.cpp"
    break;

  case 753: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5390 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10832 "nd-flow-expr.cpp"
    break;

  case 754: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5398 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10845 "nd-flow-expr.cpp"
    break;

  case 755: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5406 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10858 "nd-flow-expr.cpp"
    break;

  case 756: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES '>' VALUE_UNSIGNED  */
#line 5414 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10871 "nd-flow-expr.cpp"
    break;

  case 757: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES '<' VALUE_UNSIGNED  */
#line 5422 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10884 "nd-flow-expr.cpp"
    break;

  case 758: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5433 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10897 "nd-flow-expr.cpp"
    break;

  case 759: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5441 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10910 "nd-flow-expr.cpp"
    break;

  case 760: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5449 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10923 "nd-flow-expr.cpp"
    break;

  case 761: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5457 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10936 "nd-flow-expr.cpp"
    break;

  case 762: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES '>' VALUE_UNSIGNED  */
#line 5465 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10949 "nd-flow-expr.cpp"
    break;

  case 763: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES '<' VALUE_UNSIGNED  */
#line 5473 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10962 "nd-flow-expr.cpp"
    break;

  case 764: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5484 "nd-flow-expr.ypp"
                                                    {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10975 "nd-flow-expr.cpp"
    break;

  case 765: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5492 "nd-flow-expr.ypp"
                                                       {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10988 "nd-flow-expr.cpp"
    break;

  case 766: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5500 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11001 "nd-flow-expr.cpp"
    break;

  case 767: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5508 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11014 "nd-flow-expr.cpp"
    break;

  case 768: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES '>' VALUE_UNSIGNED  */
#line 5516 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11027 "nd-flow-expr.cpp"
    break;

  case 769: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES '<' VALUE_UNSIGNED  */
#line 5524 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11040 "nd-flow-expr.cpp"
    break;

  case 770: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5535 "nd-flow-expr.ypp"
                                                    {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11053 "nd-flow-expr.cpp"
    break;

  case 771: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5543 "nd-flow-expr.ypp"
                                                       {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11066 "nd-flow-expr.cpp"
    break;

  case 772: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5551 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11079 "nd-flow-expr.cpp"
    break;

  case 773: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5559 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11092 "nd-flow-expr.cpp"
    break;

  case 774: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES '>' VALUE_UNSIGNED  */
#line 5567 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11105 "nd-flow-expr.cpp"
    break;

  case 775: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES '<' VALUE_UNSIGNED  */
#line 5575 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11118 "nd-flow-expr.cpp"
    break;

  case 776: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5586 "nd-flow-expr.ypp"
                                                        {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11131 "nd-flow-expr.cpp"
    break;

  case 777: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5594 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11144 "nd-flow-expr.cpp"
    break;

  case 778: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5602 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11157 "nd-flow-expr.cpp"
    break;

  case 779: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5610 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11170 "nd-flow-expr.cpp"
    break;

  case 780: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS '>' VALUE_UNSIGNED  */
#line 5618 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11183 "nd-flow-expr.cpp"
    break;

  case 781: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS '<' VALUE_UNSIGNED  */
#line 5626 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11196 "nd-flow-expr.cpp"
    break;

  case 782: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5637 "nd-flow-expr.ypp"
                                                        {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11209 "nd-flow-expr.cpp"
    break;

  case 783: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5645 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11222 "nd-flow-expr.cpp"
    break;

  case 784: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5653 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11235 "nd-flow-expr.cpp"
    break;

  case 785: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5661 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11248 "nd-flow-expr.cpp"
    break;

  case 786: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS '>' VALUE_UNSIGNED  */
#line 5669 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11261 "nd-flow-expr.cpp"
    break;

  case 787: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS '<' VALUE_UNSIGNED  */
#line 5677 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11274 "nd-flow-expr.cpp"
    break;

  case 788: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5688 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11287 "nd-flow-expr.cpp"
    break;

  case 789: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5696 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11300 "nd-flow-expr.cpp"
    break;

  case 790: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5704 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11313 "nd-flow-expr.cpp"
    break;

  case 791: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5712 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11326 "nd-flow-expr.cpp"
    break;

  case 792: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS '>' VALUE_UNSIGNED  */
#line 5720 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11339 "nd-flow-expr.cpp"
    break;

  case 793: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS '<' VALUE_UNSIGNED  */
#line 5728 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11352 "nd-flow-expr.cpp"
    break;

  case 794: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5739 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11365 "nd-flow-expr.cpp"
    break;

  case 795: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5747 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11378 "nd-flow-expr.cpp"
    break;

  case 796: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5755 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11391 "nd-flow-expr.cpp"
    break;

  case 797: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5763 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11404 "nd-flow-expr.cpp"
    break;

  case 798: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS '>' VALUE_UNSIGNED  */
#line 5771 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11417 "nd-flow-expr.cpp"
    break;

  case 799: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS '<' VALUE_UNSIGNED  */
#line 5779 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11430 "nd-flow-expr.cpp"
    break;

  case 800: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5790 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11442 "nd-flow-expr.cpp"
    break;

  case 801: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5797 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11454 "nd-flow-expr.cpp"
    break;

  case 802: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5804 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11466 "nd-flow-expr.cpp"
    break;

  case 803: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5811 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11478 "nd-flow-expr.cpp"
    break;

  case 804: /* expr_detection_packets: FLOW_DETECTION_PACKETS '>' VALUE_UNSIGNED  */
#line 5818 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11490 "nd-flow-expr.cpp"
    break;

  case 805: /* expr_detection_packets: FLOW_DETECTION_PACKETS '<' VALUE_UNSIGNED  */
#line 5825 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11502 "nd-flow-expr.cpp"
    break;

  case 806: /* expr_local_rate: FLOW_LOCAL_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5835 "nd-flow-expr.ypp"
                                            {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11515 "nd-flow-expr.cpp"
    break;

  case 807: /* expr_local_rate: FLOW_LOCAL_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5843 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11528 "nd-flow-expr.cpp"
    break;

  case 808: /* expr_local_rate: FLOW_LOCAL_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5851 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11541 "nd-flow-expr.cpp"
    break;

  case 809: /* expr_local_rate: FLOW_LOCAL_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5859 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11554 "nd-flow-expr.cpp"
    break;

  case 810: /* expr_local_rate: FLOW_LOCAL_RATE '>' VALUE_FLOAT  */
#line 5867 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11567 "nd-flow-expr.cpp"
    break;

  case 811: /* expr_local_rate: FLOW_LOCAL_RATE '<' VALUE_FLOAT  */
#line 5875 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11580 "nd-flow-expr.cpp"
    break;

  case 812: /* expr_other_rate: FLOW_OTHER_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5886 "nd-flow-expr.ypp"
                                            {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11593 "nd-flow-expr.cpp"
    break;

  case 813: /* expr_other_rate: FLOW_OTHER_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5894 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11606 "nd-flow-expr.cpp"
    break;

  case 814: /* expr_other_rate: FLOW_OTHER_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5902 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11619 "nd-flow-expr.cpp"
    break;

  case 815: /* expr_other_rate: FLOW_OTHER_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5910 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11632 "nd-flow-expr.cpp"
    break;

  case 816: /* expr_other_rate: FLOW_OTHER_RATE '>' VALUE_FLOAT  */
#line 5918 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11645 "nd-flow-expr.cpp"
    break;

  case 817: /* expr_other_rate: FLOW_OTHER_RATE '<' VALUE_FLOAT  */
#line 5926 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11658 "nd-flow-expr.cpp"
    break;

  case 818: /* expr_src_rate: FLOW_SRC_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5937 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11671 "nd-flow-expr.cpp"
    break;

  case 819: /* expr_src_rate: FLOW_SRC_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5945 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11684 "nd-flow-expr.cpp"
    break;

  case 820: /* expr_src_rate: FLOW_SRC_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5953 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11697 "nd-flow-expr.cpp"
    break;

  case 821: /* expr_src_rate: FLOW_SRC_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5961 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11710 "nd-flow-expr.cpp"
    break;

  case 822: /* expr_src_rate: FLOW_SRC_RATE '>' VALUE_FLOAT  */
#line 5969 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11723 "nd-flow-expr.cpp"
    break;

  case 823: /* expr_src_rate: FLOW_SRC_RATE '<' VALUE_FLOAT  */
#line 5977 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11736 "nd-flow-expr.cpp"
    break;

  case 824: /* expr_dst_rate: FLOW_DST_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5988 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11749 "nd-flow-expr.cpp"
    break;

  case 825: /* expr_dst_rate: FLOW_DST_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5996 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11762 "nd-flow-expr.cpp"
    break;

  case 826: /* expr_dst_rate: FLOW_DST_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 6004 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11775 "nd-flow-expr.cpp"
    break;

  case 827: /* expr_dst_rate: FLOW_DST_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 6012 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11788 "nd-flow-expr.cpp"
    break;

  case 828: /* expr_dst_rate: FLOW_DST_RATE '>' VALUE_FLOAT  */
#line 6020 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11801 "nd-flow-expr.cpp"
    break;

  case 829: /* expr_dst_rate: FLOW_DST_RATE '<' VALUE_FLOAT  */
#line 6028 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11814 "nd-flow-expr.cpp"
    break;

  case 830: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_EQUAL VALUE_UNSIGNED  */
#line 6039 "nd-flow-expr.ypp"
                                                   {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11826 "nd-flow-expr.cpp"
    break;

  case 831: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 6046 "nd-flow-expr.ypp"
                                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11838 "nd-flow-expr.cpp"
    break;

  case 832: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 6053 "nd-flow-expr.ypp"
                                                        {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11850 "nd-flow-expr.cpp"
    break;

  case 833: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 6060 "nd-flow-expr.ypp"
                                                        {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11862 "nd-flow-expr.cpp"
    break;

  case 834: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS '>' VALUE_UNSIGNED  */
#line 6067 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11874 "nd-flow-expr.cpp"
    break;

  case 835: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS '<' VALUE_UNSIGNED  */
#line 6074 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11886 "nd-flow-expr.cpp"
    break;

  case 836: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_EQUAL VALUE_UNSIGNED  */
#line 6084 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11898 "nd-flow-expr.cpp"
    break;

  case 837: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 6091 "nd-flow-expr.ypp"
                                                  {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11910 "nd-flow-expr.cpp"
    break;

  case 838: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 6098 "nd-flow-expr.ypp"
                                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11922 "nd-flow-expr.cpp"
    break;

  case 839: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 6105 "nd-flow-expr.ypp"
                                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11934 "nd-flow-expr.cpp"
    break;

  case 840: /* expr_tcp_resets: FLOW_TCP_RESETS '>' VALUE_UNSIGNED  */
#line 6112 "nd-flow-expr.ypp"
                                         {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11946 "nd-flow-expr.cpp"
    break;

  case 841: /* expr_tcp_resets: FLOW_TCP_RESETS '<' VALUE_UNSIGNED  */
#line 6119 "nd-flow-expr.ypp"
                                         {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11958 "nd-flow-expr.cpp"
    break;

  case 842: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_EQUAL VALUE_UNSIGNED  */
#line 6129 "nd-flow-expr.ypp"
                                                {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11970 "nd-flow-expr.cpp"
    break;

  case 843: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 6136 "nd-flow-expr.ypp"
                                                   {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11982 "nd-flow-expr.cpp"
    break;

  case 844: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 6143 "nd-flow-expr.ypp"
                                                     {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11994 "nd-flow-expr.cpp"
    break;

  case 845: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 6150 "nd-flow-expr.ypp"
                                                     {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 12006 "nd-flow-expr.cpp"
    break;

  case 846: /* expr_tcp_retrans: FLOW_TCP_RETRANS '>' VALUE_UNSIGNED  */
#line 6157 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 12018 "nd-flow-expr.cpp"
    break;

  case 847: /* expr_tcp_retrans: FLOW_TCP_RETRANS '<' VALUE_UNSIGNED  */
#line 6164 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 12030 "nd-flow-expr.cpp"
    break;

  case 848: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER  */
#line 6174 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (! _NDFP_flow->tls.alpn_server.empty()));
    }
#line 12038 "nd-flow-expr.cpp"
    break;

  case 849: /* expr_tls_alpn_server: '!' FLOW_TLS_ALPN_SERVER  */
#line 6177 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.alpn_server.empty()));
    }
#line 12046 "nd-flow-expr.cpp"
    break;

  case 850: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_EQUAL VALUE_NAME  */
#line 6180 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        for (auto &item : _NDFP_flow->tls.alpn_server) {
            if (strncasecmp(search.c_str(), item.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
                break;
            }
        }
    }
#line 12062 "nd-flow-expr.cpp"
    break;

  case 851: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_NOTEQUAL VALUE_NAME  */
#line 6191 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        for (auto &item : _NDFP_flow->tls.alpn_server) {
            if (strncasecmp(search.c_str(), item.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
                break;
            }
        }
    }
#line 12078 "nd-flow-expr.cpp"
    break;

  case 852: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_EQUAL VALUE_REGEX  */
#line 6202 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        std::string rx((yyvsp[0].buffer));
        for (auto &item : _NDFP_flow->tls.alpn_server) {
            if (_NDFP_regex_search(rx, item)) {
                _NDFP_result = ((yyval.bool_result) = true);
                break;
            }
        }
    }
#line 12093 "nd-flow-expr.cpp"
    break;

  case 853: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_NOTEQUAL VALUE_REGEX  */
#line 6212 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        std::string rx((yyvsp[0].buffer));
        for (auto &item : _NDFP_flow->tls.alpn_server) {
            if (_NDFP_regex_search(rx, item)) {
                _NDFP_result = ((yyval.bool_result) = false);
                break;
            }
        }
    }
#line 12108 "nd-flow-expr.cpp"
    break;

  case 854: /* expr_tls_cert_fingerprint: FLOW_TLS_CERT_FINGERPRINT CMP_EQUAL VALUE_NAME  */
#line 6225 "nd-flow-expr.ypp"
                                                     {
        std::string digest;
        nd_sha1_to_string(_NDFP_flow->tls.cert_fingerprint, digest);
        _NDFP_result = ((yyval.bool_result) = false);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        if (strncasecmp(search.c_str(), digest.c_str(), _NDFP_MAX_BUFLEN) == 0) {
            _NDFP_result = ((yyval.bool_result) = true);
        }
    }
#line 12123 "nd-flow-expr.cpp"
    break;

  case 855: /* expr_tls_cert_fingerprint: FLOW_TLS_CERT_FINGERPRINT CMP_NOTEQUAL VALUE_NAME  */
#line 6235 "nd-flow-expr.ypp"
                                                        {
        std::string digest;
        nd_sha1_to_string(_NDFP_flow->tls.cert_fingerprint, digest);
        _NDFP_result = ((yyval.bool_result) = true);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        if (strncasecmp(search.c_str(), digest.c_str(), _NDFP_MAX_BUFLEN) == 0) {
            _NDFP_result = ((yyval.bool_result) = false);
        }
    }
#line 12138 "nd-flow-expr.cpp"
    break;

  case 856: /* expr_bt_info_hash: FLOW_BT_INFO_HASH CMP_EQUAL VALUE_NAME  */
#line 6248 "nd-flow-expr.ypp"
                                             {
        std::string digest;
        nd_sha1_to_string(_NDFP_flow->bt.info_hash, digest);
        _NDFP_result = ((yyval.bool_result) = false);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        if (strncasecmp(search.c_str(), digest.c_str(), _NDFP_MAX_BUFLEN) == 0) {
            _NDFP_result = ((yyval.bool_result) = true);
        }
    }
#line 12153 "nd-flow-expr.cpp"
    break;

  case 857: /* expr_bt_info_hash: FLOW_BT_INFO_HASH CMP_NOTEQUAL VALUE_NAME  */
#line 6258 "nd-flow-expr.ypp"
                                                {
        std::string digest;
        nd_sha1_to_string(_NDFP_flow->bt.info_hash, digest);
        _NDFP_result = ((yyval.bool_result) = true);
        size_t p; std::string search((yyvsp[0].buffer));
        while ((p = search.find_first_of("'\"")) != std::string::npos) search.erase(p, 1);
        if (strncasecmp(search.c_str(), digest.c_str(), _NDFP_MAX_BUFLEN) == 0) {
            _NDFP_result = ((yyval.bool_result) = false);
        }
    }
#line 12168 "nd-flow-expr.cpp"
    break;

  case 858: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_EQUAL value_addr_ip  */
#line 6271 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.mapped, (yyvsp[0].buffer)) == true));
    }
#line 12176 "nd-flow-expr.cpp"
    break;

  case 859: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_NOTEQUAL value_addr_ip  */
#line 6274 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.mapped, (yyvsp[0].buffer)) == false));
    }
#line 12184 "nd-flow-expr.cpp"
    break;

  case 860: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_EQUAL VALUE_ADDR_TAG  */
#line 6277 "nd-flow-expr.ypp"
                                                {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.mapped) == true));
    }
#line 12195 "nd-flow-expr.cpp"
    break;

  case 861: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6283 "nd-flow-expr.ypp"
                                                   {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.mapped) == false));
    }
#line 12206 "nd-flow-expr.cpp"
    break;

  case 862: /* expr_stun_peer: FLOW_STUN_PEER CMP_EQUAL value_addr_ip  */
#line 6292 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.peer, (yyvsp[0].buffer)) == true));
    }
#line 12214 "nd-flow-expr.cpp"
    break;

  case 863: /* expr_stun_peer: FLOW_STUN_PEER CMP_NOTEQUAL value_addr_ip  */
#line 6295 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.peer, (yyvsp[0].buffer)) == false));
    }
#line 12222 "nd-flow-expr.cpp"
    break;

  case 864: /* expr_stun_peer: FLOW_STUN_PEER CMP_EQUAL VALUE_ADDR_TAG  */
#line 6298 "nd-flow-expr.ypp"
                                              {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.peer) == true));
    }
#line 12233 "nd-flow-expr.cpp"
    break;

  case 865: /* expr_stun_peer: FLOW_STUN_PEER CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6304 "nd-flow-expr.ypp"
                                                 {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.peer) == false));
    }
#line 12244 "nd-flow-expr.cpp"
    break;

  case 866: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_EQUAL value_addr_ip  */
#line 6313 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.relayed, (yyvsp[0].buffer)) == true));
    }
#line 12252 "nd-flow-expr.cpp"
    break;

  case 867: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_NOTEQUAL value_addr_ip  */
#line 6316 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.relayed, (yyvsp[0].buffer)) == false));
    }
#line 12260 "nd-flow-expr.cpp"
    break;

  case 868: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_EQUAL VALUE_ADDR_TAG  */
#line 6319 "nd-flow-expr.ypp"
                                                 {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.relayed) == true));
    }
#line 12271 "nd-flow-expr.cpp"
    break;

  case 869: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6325 "nd-flow-expr.ypp"
                                                    {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.relayed) == false));
    }
#line 12282 "nd-flow-expr.cpp"
    break;

  case 870: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_EQUAL value_addr_ip  */
#line 6334 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.response, (yyvsp[0].buffer)) == true));
    }
#line 12290 "nd-flow-expr.cpp"
    break;

  case 871: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_NOTEQUAL value_addr_ip  */
#line 6337 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.response, (yyvsp[0].buffer)) == false));
    }
#line 12298 "nd-flow-expr.cpp"
    break;

  case 872: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_EQUAL VALUE_ADDR_TAG  */
#line 6340 "nd-flow-expr.ypp"
                                                  {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.response) == true));
    }
#line 12309 "nd-flow-expr.cpp"
    break;

  case 873: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6346 "nd-flow-expr.ypp"
                                                     {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.response) == false));
    }
#line 12320 "nd-flow-expr.cpp"
    break;

  case 874: /* expr_stun_other: FLOW_STUN_OTHER CMP_EQUAL value_addr_ip  */
#line 6355 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.other, (yyvsp[0].buffer)) == true));
    }
#line 12328 "nd-flow-expr.cpp"
    break;

  case 875: /* expr_stun_other: FLOW_STUN_OTHER CMP_NOTEQUAL value_addr_ip  */
#line 6358 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.other, (yyvsp[0].buffer)) == false));
    }
#line 12336 "nd-flow-expr.cpp"
    break;

  case 876: /* expr_stun_other: FLOW_STUN_OTHER CMP_EQUAL VALUE_ADDR_TAG  */
#line 6361 "nd-flow-expr.ypp"
                                               {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.other) == true));
    }
#line 12347 "nd-flow-expr.cpp"
    break;

  case 877: /* expr_stun_other: FLOW_STUN_OTHER CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6367 "nd-flow-expr.ypp"
                                                  {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.other) == false));
    }
#line 12358 "nd-flow-expr.cpp"
    break;


#line 12362 "nd-flow-expr.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (&yylloc, scanner, YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 6374 "nd-flow-expr.ypp"


ndFlowParser::ndFlowParser() {
    yyscan_t scanner;
    yylex_init_extra((void *)this, &scanner);

    if (scanner == nullptr)
        throw ndException("creating scanner context");

    this->scanner = static_cast<void *>(scanner);
}

ndFlowParser::~ndFlowParser()
{
    yylex_destroy(static_cast<yyscan_t>(scanner));
}

bool ndFlowParser::Parse(ndFlow::Ptr const& flow,
  const ndFlowParser::Params &params, const string &expr)
{
    this->flow = flow;
    expr_result = false;

    lock_guard<recursive_mutex> lg(flow->lock);

    switch (flow->lower_map) {
    case ndFlow::LowerMap::LOCAL:
        local_mac = &flow->lower_mac;
        other_mac = &flow->upper_mac;

        local_ip = &flow->lower_addr;
        other_ip = &flow->upper_addr;

        local_port = flow->lower_addr.GetPort();
        other_port = flow->upper_addr.GetPort();

        local_net_cat = flow->category.lower_net;
        other_net_cat = flow->category.upper_net;

        switch (flow->origin) {
        case ndFlow::Origin::LOWER:
            origin = _NDFP_ORIGIN_LOCAL;
            break;
        case ndFlow::Origin::UPPER:
            origin = _NDFP_ORIGIN_OTHER;
            break;
        default:
            origin = _NDFP_ORIGIN_UNKNOWN;
        }
        break;

    case ndFlow::LowerMap::OTHER:
        local_mac = &flow->upper_mac;
        other_mac = &flow->lower_mac;

        local_ip = &flow->upper_addr;
        other_ip = &flow->lower_addr;

        local_port = flow->upper_addr.GetPort();
        other_port = flow->lower_addr.GetPort();

        local_net_cat = flow->category.upper_net;
        other_net_cat = flow->category.lower_net;

        switch (flow->origin) {
        case ndFlow::Origin::LOWER:
            origin = _NDFP_ORIGIN_OTHER;
            break;
        case ndFlow::Origin::UPPER:
            origin = _NDFP_ORIGIN_LOCAL;
            break;
        default:
            origin = _NDFP_ORIGIN_UNKNOWN;
        }
        break;

    default:
        //nd_dprintf("Bad lower map: %u\n", flow->lower_map);
        this->flow.reset();
        return false;
    }

    switch (flow->origin) {
    case ndFlow::Origin::LOWER:
        src_mac = &flow->lower_mac;
        dst_mac = &flow->upper_mac;

        src_ip = &flow->lower_addr;
        dst_ip = &flow->upper_addr;

        src_port = flow->lower_addr.GetPort();
        dst_port = flow->upper_addr.GetPort();

        src_net_cat = flow->category.lower_net;
        dst_net_cat = flow->category.upper_net;

        break;

    case ndFlow::Origin::UPPER:
        src_mac = &flow->upper_mac;
        dst_mac = &flow->lower_mac;

        src_ip = &flow->upper_addr;
        dst_ip = &flow->lower_addr;

        src_port = flow->upper_addr.GetPort();
        dst_port = flow->lower_addr.GetPort();

        src_net_cat = flow->category.upper_net;
        dst_net_cat = flow->category.lower_net;
        break;

    default:
        //nd_dprintf("Unknown origin: %u\n", flow->origin);
        this->flow.reset();
        return false;
    }

#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
    ct_reply_src_ip = &flow->conntrack.reply_src_addr;
    ct_reply_dst_ip = &flow->conntrack.reply_dst_addr;
#endif

    auto ji = params.find("intel");
    params_intel = (ji != params.end()) ? &ji->second : nullptr;

    YY_BUFFER_STATE flow_expr_scan_buffer;
    flow_expr_scan_buffer = yy_scan_bytes(
        expr.c_str(), expr.size(), (yyscan_t)scanner
    );

    if (flow_expr_scan_buffer == nullptr)
        throw ndException("allocating flow expression scan buffer");

    yy_switch_to_buffer(flow_expr_scan_buffer, (yyscan_t)scanner);

    int rc = 0;

    try {
        rc = yyparse((yyscan_t)scanner);
    } catch (exception &e) {
        this->flow.reset();
        yy_delete_buffer(flow_expr_scan_buffer, scanner);
        nd_dprintf("flow parser exception: %s: \"%s\"\n",
            e.what(), expr.c_str());
        throw;
    } catch (...) {
        this->flow.reset();
        yy_delete_buffer(flow_expr_scan_buffer, scanner);
        nd_dprintf("flow parser exception: %s: \"%s\"\n",
            "unknown", expr.c_str());
        throw;
    }

    yy_delete_buffer(flow_expr_scan_buffer, scanner);

    this->flow.reset();

    return (rc == 0) ? expr_result : false;
}

bool ndFlowParser::RegExSearch(std::string &expr, const std::string &search) {
    size_t p;

    if ((p = expr.find_first_of("'\"")) != string::npos)
        expr.erase(p, 1);
    if ((p = expr.find_last_of("'\"")) != string::npos)
        expr.erase(p, 1);
    if ((p = expr.find_first_of(":")) != string::npos)
        expr.erase(0, p + 1);

    auto rx = rx_cache.find(expr);

    if (rx == rx_cache.end()) {
        try {
            unique_ptr<regex> up_rx(new regex(expr,
              regex::extended | regex::icase | regex::optimize));

            auto it = rx_cache.insert(make_pair(expr, std::move(up_rx)));

            rx = it.first;
        }
        catch (const regex_error &e) {
            string error;
            nd_regex_error(e, error);
            nd_printf(
              "WARNING: Error compiling flow expression regex: "
              "%s: %s [%d]\n",
              expr.c_str(), error.c_str(), e.code());

            return false;
        }
    }

    cmatch match;
    return regex_search(search.c_str(), match, *rx->second);
}

// vi: set ft=cpp ei=all modelines=1 :
