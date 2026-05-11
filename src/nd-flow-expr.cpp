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
    FLOW_APP_IP_OVERRIDE = 330,    /* FLOW_APP_IP_OVERRIDE  */
    FLOW_APP_PROTO_TWINS = 331,    /* FLOW_APP_PROTO_TWINS  */
    FLOW_DHCP_CLASS_IDENT = 332,   /* FLOW_DHCP_CLASS_IDENT  */
    FLOW_DHCP_FINGERPRINT = 333,   /* FLOW_DHCP_FINGERPRINT  */
    FLOW_DST_BYTES = 334,          /* FLOW_DST_BYTES  */
    FLOW_DST_PACKETS = 335,        /* FLOW_DST_PACKETS  */
    FLOW_HTTP_URL = 336,           /* FLOW_HTTP_URL  */
    FLOW_HTTP_USER_AGENT = 337,    /* FLOW_HTTP_USER_AGENT  */
    FLOW_LOCAL_BYTES = 338,        /* FLOW_LOCAL_BYTES  */
    FLOW_LOCAL_PACKETS = 339,      /* FLOW_LOCAL_PACKETS  */
    FLOW_MDNS_DOMAIN_NAME = 340,   /* FLOW_MDNS_DOMAIN_NAME  */
    FLOW_NFQ_DST_IFINDEX = 341,    /* FLOW_NFQ_DST_IFINDEX  */
    FLOW_NFQ_SRC_IFINDEX = 342,    /* FLOW_NFQ_SRC_IFINDEX  */
    FLOW_OTHER_BYTES = 343,        /* FLOW_OTHER_BYTES  */
    FLOW_OTHER_PACKETS = 344,      /* FLOW_OTHER_PACKETS  */
    FLOW_SRC_BYTES = 345,          /* FLOW_SRC_BYTES  */
    FLOW_SRC_PACKETS = 346,        /* FLOW_SRC_PACKETS  */
    FLOW_SSH_CLIENT_AGENT = 347,   /* FLOW_SSH_CLIENT_AGENT  */
    FLOW_SSH_SERVER_AGENT = 348,   /* FLOW_SSH_SERVER_AGENT  */
    FLOW_TCP_FIN_ACK = 349,        /* FLOW_TCP_FIN_ACK  */
    FLOW_TCP_LAST_SEQ = 350,       /* FLOW_TCP_LAST_SEQ  */
    FLOW_TLS_ALPN = 351,           /* FLOW_TLS_ALPN  */
    FLOW_TLS_ISSUER_DN = 352,      /* FLOW_TLS_ISSUER_DN  */
    FLOW_TLS_SERVER_CN = 353,      /* FLOW_TLS_SERVER_CN  */
    FLOW_TLS_SUBJECT_DN = 354,     /* FLOW_TLS_SUBJECT_DN  */
    FLOW_TOTAL_BYTES = 355,        /* FLOW_TOTAL_BYTES  */
    FLOW_TOTAL_PACKETS = 356,      /* FLOW_TOTAL_PACKETS  */
    FLOW_TS_FIRST_SEEN = 357,      /* FLOW_TS_FIRST_SEEN  */
    FLOW_TS_LAST_SEEN = 358,       /* FLOW_TS_LAST_SEEN  */
    FLOW_TOTAL_LOCAL_BYTES = 359,  /* FLOW_TOTAL_LOCAL_BYTES  */
    FLOW_TOTAL_OTHER_BYTES = 360,  /* FLOW_TOTAL_OTHER_BYTES  */
    FLOW_TOTAL_SRC_BYTES = 361,    /* FLOW_TOTAL_SRC_BYTES  */
    FLOW_TOTAL_DST_BYTES = 362,    /* FLOW_TOTAL_DST_BYTES  */
    FLOW_TOTAL_LOCAL_PACKETS = 363, /* FLOW_TOTAL_LOCAL_PACKETS  */
    FLOW_TOTAL_OTHER_PACKETS = 364, /* FLOW_TOTAL_OTHER_PACKETS  */
    FLOW_TOTAL_SRC_PACKETS = 365,  /* FLOW_TOTAL_SRC_PACKETS  */
    FLOW_TOTAL_DST_PACKETS = 366,  /* FLOW_TOTAL_DST_PACKETS  */
    FLOW_DETECTION_PACKETS = 367,  /* FLOW_DETECTION_PACKETS  */
    FLOW_LOCAL_RATE = 368,         /* FLOW_LOCAL_RATE  */
    FLOW_OTHER_RATE = 369,         /* FLOW_OTHER_RATE  */
    FLOW_SRC_RATE = 370,           /* FLOW_SRC_RATE  */
    FLOW_DST_RATE = 371,           /* FLOW_DST_RATE  */
    FLOW_TCP_SEQ_ERRORS = 372,     /* FLOW_TCP_SEQ_ERRORS  */
    FLOW_TCP_RESETS = 373,         /* FLOW_TCP_RESETS  */
    FLOW_TCP_RETRANS = 374,        /* FLOW_TCP_RETRANS  */
    FLOW_TLS_CERT_FINGERPRINT = 375, /* FLOW_TLS_CERT_FINGERPRINT  */
    FLOW_TLS_ALPN_SERVER = 376,    /* FLOW_TLS_ALPN_SERVER  */
    FLOW_TLS_PROC_HELLO = 377,     /* FLOW_TLS_PROC_HELLO  */
    FLOW_TLS_PROC_CERTIFICATE = 378, /* FLOW_TLS_PROC_CERTIFICATE  */
    FLOW_SMTP_TLS = 379,           /* FLOW_SMTP_TLS  */
    FLOW_BT_INFO_HASH = 380,       /* FLOW_BT_INFO_HASH  */
    FLOW_STUN_MAPPED = 381,        /* FLOW_STUN_MAPPED  */
    FLOW_STUN_PEER = 382,          /* FLOW_STUN_PEER  */
    FLOW_STUN_RELAYED = 383,       /* FLOW_STUN_RELAYED  */
    FLOW_STUN_RESPONSE = 384,      /* FLOW_STUN_RESPONSE  */
    FLOW_STUN_OTHER = 385,         /* FLOW_STUN_OTHER  */
    FLOW_OTHER_UNKNOWN = 386,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 387,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 388,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 389,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 390,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 391,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 392,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 393,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 394,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 395,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 396,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 397,         /* FLOW_TUNNEL_GTP  */
    FLOW_INTEL = 398,              /* FLOW_INTEL  */
    CMP_EQUAL = 399,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 400,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 401,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 402,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 403,                /* BOOL_AND  */
    BOOL_OR = 404,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 405,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 406,              /* VALUE_TRUE  */
    VALUE_FALSE = 407,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 408,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 409,              /* VALUE_NAME  */
    VALUE_REGEX = 410,             /* VALUE_REGEX  */
    VALUE_ADDR_TAG = 411,          /* VALUE_ADDR_TAG  */
    VALUE_ADDR_IPV4 = 412,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 413,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 414,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 415,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 416,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 417,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 418              /* VALUE_FLOAT  */
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
#define FLOW_APP_IP_OVERRIDE 330
#define FLOW_APP_PROTO_TWINS 331
#define FLOW_DHCP_CLASS_IDENT 332
#define FLOW_DHCP_FINGERPRINT 333
#define FLOW_DST_BYTES 334
#define FLOW_DST_PACKETS 335
#define FLOW_HTTP_URL 336
#define FLOW_HTTP_USER_AGENT 337
#define FLOW_LOCAL_BYTES 338
#define FLOW_LOCAL_PACKETS 339
#define FLOW_MDNS_DOMAIN_NAME 340
#define FLOW_NFQ_DST_IFINDEX 341
#define FLOW_NFQ_SRC_IFINDEX 342
#define FLOW_OTHER_BYTES 343
#define FLOW_OTHER_PACKETS 344
#define FLOW_SRC_BYTES 345
#define FLOW_SRC_PACKETS 346
#define FLOW_SSH_CLIENT_AGENT 347
#define FLOW_SSH_SERVER_AGENT 348
#define FLOW_TCP_FIN_ACK 349
#define FLOW_TCP_LAST_SEQ 350
#define FLOW_TLS_ALPN 351
#define FLOW_TLS_ISSUER_DN 352
#define FLOW_TLS_SERVER_CN 353
#define FLOW_TLS_SUBJECT_DN 354
#define FLOW_TOTAL_BYTES 355
#define FLOW_TOTAL_PACKETS 356
#define FLOW_TS_FIRST_SEEN 357
#define FLOW_TS_LAST_SEEN 358
#define FLOW_TOTAL_LOCAL_BYTES 359
#define FLOW_TOTAL_OTHER_BYTES 360
#define FLOW_TOTAL_SRC_BYTES 361
#define FLOW_TOTAL_DST_BYTES 362
#define FLOW_TOTAL_LOCAL_PACKETS 363
#define FLOW_TOTAL_OTHER_PACKETS 364
#define FLOW_TOTAL_SRC_PACKETS 365
#define FLOW_TOTAL_DST_PACKETS 366
#define FLOW_DETECTION_PACKETS 367
#define FLOW_LOCAL_RATE 368
#define FLOW_OTHER_RATE 369
#define FLOW_SRC_RATE 370
#define FLOW_DST_RATE 371
#define FLOW_TCP_SEQ_ERRORS 372
#define FLOW_TCP_RESETS 373
#define FLOW_TCP_RETRANS 374
#define FLOW_TLS_CERT_FINGERPRINT 375
#define FLOW_TLS_ALPN_SERVER 376
#define FLOW_TLS_PROC_HELLO 377
#define FLOW_TLS_PROC_CERTIFICATE 378
#define FLOW_SMTP_TLS 379
#define FLOW_BT_INFO_HASH 380
#define FLOW_STUN_MAPPED 381
#define FLOW_STUN_PEER 382
#define FLOW_STUN_RELAYED 383
#define FLOW_STUN_RESPONSE 384
#define FLOW_STUN_OTHER 385
#define FLOW_OTHER_UNKNOWN 386
#define FLOW_OTHER_UNSUPPORTED 387
#define FLOW_OTHER_LOCAL 388
#define FLOW_OTHER_MULTICAST 389
#define FLOW_OTHER_BROADCAST 390
#define FLOW_OTHER_REMOTE 391
#define FLOW_OTHER_ERROR 392
#define FLOW_ORIGIN_LOCAL 393
#define FLOW_ORIGIN_OTHER 394
#define FLOW_ORIGIN_UNKNOWN 395
#define FLOW_TUNNEL_NONE 396
#define FLOW_TUNNEL_GTP 397
#define FLOW_INTEL 398
#define CMP_EQUAL 399
#define CMP_NOTEQUAL 400
#define CMP_GTHANEQUAL 401
#define CMP_LTHANEQUAL 402
#define BOOL_AND 403
#define BOOL_OR 404
#define VALUE_ADDR_IPMASK 405
#define VALUE_TRUE 406
#define VALUE_FALSE 407
#define VALUE_ADDR_MAC 408
#define VALUE_NAME 409
#define VALUE_REGEX 410
#define VALUE_ADDR_TAG 411
#define VALUE_ADDR_IPV4 412
#define VALUE_ADDR_IPV4_CIDR 413
#define VALUE_ADDR_IPV6 414
#define VALUE_ADDR_IPV6_CIDR 415
#define VALUE_SIGNED 416
#define VALUE_UNSIGNED 417
#define VALUE_FLOAT 418

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

#line 593 "nd-flow-expr.cpp"

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
  YYSYMBOL_FLOW_APP_IP_OVERRIDE = 75,      /* FLOW_APP_IP_OVERRIDE  */
  YYSYMBOL_FLOW_APP_PROTO_TWINS = 76,      /* FLOW_APP_PROTO_TWINS  */
  YYSYMBOL_FLOW_DHCP_CLASS_IDENT = 77,     /* FLOW_DHCP_CLASS_IDENT  */
  YYSYMBOL_FLOW_DHCP_FINGERPRINT = 78,     /* FLOW_DHCP_FINGERPRINT  */
  YYSYMBOL_FLOW_DST_BYTES = 79,            /* FLOW_DST_BYTES  */
  YYSYMBOL_FLOW_DST_PACKETS = 80,          /* FLOW_DST_PACKETS  */
  YYSYMBOL_FLOW_HTTP_URL = 81,             /* FLOW_HTTP_URL  */
  YYSYMBOL_FLOW_HTTP_USER_AGENT = 82,      /* FLOW_HTTP_USER_AGENT  */
  YYSYMBOL_FLOW_LOCAL_BYTES = 83,          /* FLOW_LOCAL_BYTES  */
  YYSYMBOL_FLOW_LOCAL_PACKETS = 84,        /* FLOW_LOCAL_PACKETS  */
  YYSYMBOL_FLOW_MDNS_DOMAIN_NAME = 85,     /* FLOW_MDNS_DOMAIN_NAME  */
  YYSYMBOL_FLOW_NFQ_DST_IFINDEX = 86,      /* FLOW_NFQ_DST_IFINDEX  */
  YYSYMBOL_FLOW_NFQ_SRC_IFINDEX = 87,      /* FLOW_NFQ_SRC_IFINDEX  */
  YYSYMBOL_FLOW_OTHER_BYTES = 88,          /* FLOW_OTHER_BYTES  */
  YYSYMBOL_FLOW_OTHER_PACKETS = 89,        /* FLOW_OTHER_PACKETS  */
  YYSYMBOL_FLOW_SRC_BYTES = 90,            /* FLOW_SRC_BYTES  */
  YYSYMBOL_FLOW_SRC_PACKETS = 91,          /* FLOW_SRC_PACKETS  */
  YYSYMBOL_FLOW_SSH_CLIENT_AGENT = 92,     /* FLOW_SSH_CLIENT_AGENT  */
  YYSYMBOL_FLOW_SSH_SERVER_AGENT = 93,     /* FLOW_SSH_SERVER_AGENT  */
  YYSYMBOL_FLOW_TCP_FIN_ACK = 94,          /* FLOW_TCP_FIN_ACK  */
  YYSYMBOL_FLOW_TCP_LAST_SEQ = 95,         /* FLOW_TCP_LAST_SEQ  */
  YYSYMBOL_FLOW_TLS_ALPN = 96,             /* FLOW_TLS_ALPN  */
  YYSYMBOL_FLOW_TLS_ISSUER_DN = 97,        /* FLOW_TLS_ISSUER_DN  */
  YYSYMBOL_FLOW_TLS_SERVER_CN = 98,        /* FLOW_TLS_SERVER_CN  */
  YYSYMBOL_FLOW_TLS_SUBJECT_DN = 99,       /* FLOW_TLS_SUBJECT_DN  */
  YYSYMBOL_FLOW_TOTAL_BYTES = 100,         /* FLOW_TOTAL_BYTES  */
  YYSYMBOL_FLOW_TOTAL_PACKETS = 101,       /* FLOW_TOTAL_PACKETS  */
  YYSYMBOL_FLOW_TS_FIRST_SEEN = 102,       /* FLOW_TS_FIRST_SEEN  */
  YYSYMBOL_FLOW_TS_LAST_SEEN = 103,        /* FLOW_TS_LAST_SEEN  */
  YYSYMBOL_FLOW_TOTAL_LOCAL_BYTES = 104,   /* FLOW_TOTAL_LOCAL_BYTES  */
  YYSYMBOL_FLOW_TOTAL_OTHER_BYTES = 105,   /* FLOW_TOTAL_OTHER_BYTES  */
  YYSYMBOL_FLOW_TOTAL_SRC_BYTES = 106,     /* FLOW_TOTAL_SRC_BYTES  */
  YYSYMBOL_FLOW_TOTAL_DST_BYTES = 107,     /* FLOW_TOTAL_DST_BYTES  */
  YYSYMBOL_FLOW_TOTAL_LOCAL_PACKETS = 108, /* FLOW_TOTAL_LOCAL_PACKETS  */
  YYSYMBOL_FLOW_TOTAL_OTHER_PACKETS = 109, /* FLOW_TOTAL_OTHER_PACKETS  */
  YYSYMBOL_FLOW_TOTAL_SRC_PACKETS = 110,   /* FLOW_TOTAL_SRC_PACKETS  */
  YYSYMBOL_FLOW_TOTAL_DST_PACKETS = 111,   /* FLOW_TOTAL_DST_PACKETS  */
  YYSYMBOL_FLOW_DETECTION_PACKETS = 112,   /* FLOW_DETECTION_PACKETS  */
  YYSYMBOL_FLOW_LOCAL_RATE = 113,          /* FLOW_LOCAL_RATE  */
  YYSYMBOL_FLOW_OTHER_RATE = 114,          /* FLOW_OTHER_RATE  */
  YYSYMBOL_FLOW_SRC_RATE = 115,            /* FLOW_SRC_RATE  */
  YYSYMBOL_FLOW_DST_RATE = 116,            /* FLOW_DST_RATE  */
  YYSYMBOL_FLOW_TCP_SEQ_ERRORS = 117,      /* FLOW_TCP_SEQ_ERRORS  */
  YYSYMBOL_FLOW_TCP_RESETS = 118,          /* FLOW_TCP_RESETS  */
  YYSYMBOL_FLOW_TCP_RETRANS = 119,         /* FLOW_TCP_RETRANS  */
  YYSYMBOL_FLOW_TLS_CERT_FINGERPRINT = 120, /* FLOW_TLS_CERT_FINGERPRINT  */
  YYSYMBOL_FLOW_TLS_ALPN_SERVER = 121,     /* FLOW_TLS_ALPN_SERVER  */
  YYSYMBOL_FLOW_TLS_PROC_HELLO = 122,      /* FLOW_TLS_PROC_HELLO  */
  YYSYMBOL_FLOW_TLS_PROC_CERTIFICATE = 123, /* FLOW_TLS_PROC_CERTIFICATE  */
  YYSYMBOL_FLOW_SMTP_TLS = 124,            /* FLOW_SMTP_TLS  */
  YYSYMBOL_FLOW_BT_INFO_HASH = 125,        /* FLOW_BT_INFO_HASH  */
  YYSYMBOL_FLOW_STUN_MAPPED = 126,         /* FLOW_STUN_MAPPED  */
  YYSYMBOL_FLOW_STUN_PEER = 127,           /* FLOW_STUN_PEER  */
  YYSYMBOL_FLOW_STUN_RELAYED = 128,        /* FLOW_STUN_RELAYED  */
  YYSYMBOL_FLOW_STUN_RESPONSE = 129,       /* FLOW_STUN_RESPONSE  */
  YYSYMBOL_FLOW_STUN_OTHER = 130,          /* FLOW_STUN_OTHER  */
  YYSYMBOL_FLOW_OTHER_UNKNOWN = 131,       /* FLOW_OTHER_UNKNOWN  */
  YYSYMBOL_FLOW_OTHER_UNSUPPORTED = 132,   /* FLOW_OTHER_UNSUPPORTED  */
  YYSYMBOL_FLOW_OTHER_LOCAL = 133,         /* FLOW_OTHER_LOCAL  */
  YYSYMBOL_FLOW_OTHER_MULTICAST = 134,     /* FLOW_OTHER_MULTICAST  */
  YYSYMBOL_FLOW_OTHER_BROADCAST = 135,     /* FLOW_OTHER_BROADCAST  */
  YYSYMBOL_FLOW_OTHER_REMOTE = 136,        /* FLOW_OTHER_REMOTE  */
  YYSYMBOL_FLOW_OTHER_ERROR = 137,         /* FLOW_OTHER_ERROR  */
  YYSYMBOL_FLOW_ORIGIN_LOCAL = 138,        /* FLOW_ORIGIN_LOCAL  */
  YYSYMBOL_FLOW_ORIGIN_OTHER = 139,        /* FLOW_ORIGIN_OTHER  */
  YYSYMBOL_FLOW_ORIGIN_UNKNOWN = 140,      /* FLOW_ORIGIN_UNKNOWN  */
  YYSYMBOL_FLOW_TUNNEL_NONE = 141,         /* FLOW_TUNNEL_NONE  */
  YYSYMBOL_FLOW_TUNNEL_GTP = 142,          /* FLOW_TUNNEL_GTP  */
  YYSYMBOL_FLOW_INTEL = 143,               /* FLOW_INTEL  */
  YYSYMBOL_CMP_EQUAL = 144,                /* CMP_EQUAL  */
  YYSYMBOL_CMP_NOTEQUAL = 145,             /* CMP_NOTEQUAL  */
  YYSYMBOL_CMP_GTHANEQUAL = 146,           /* CMP_GTHANEQUAL  */
  YYSYMBOL_CMP_LTHANEQUAL = 147,           /* CMP_LTHANEQUAL  */
  YYSYMBOL_BOOL_AND = 148,                 /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 149,                  /* BOOL_OR  */
  YYSYMBOL_VALUE_ADDR_IPMASK = 150,        /* VALUE_ADDR_IPMASK  */
  YYSYMBOL_VALUE_TRUE = 151,               /* VALUE_TRUE  */
  YYSYMBOL_VALUE_FALSE = 152,              /* VALUE_FALSE  */
  YYSYMBOL_VALUE_ADDR_MAC = 153,           /* VALUE_ADDR_MAC  */
  YYSYMBOL_VALUE_NAME = 154,               /* VALUE_NAME  */
  YYSYMBOL_VALUE_REGEX = 155,              /* VALUE_REGEX  */
  YYSYMBOL_VALUE_ADDR_TAG = 156,           /* VALUE_ADDR_TAG  */
  YYSYMBOL_VALUE_ADDR_IPV4 = 157,          /* VALUE_ADDR_IPV4  */
  YYSYMBOL_VALUE_ADDR_IPV4_CIDR = 158,     /* VALUE_ADDR_IPV4_CIDR  */
  YYSYMBOL_VALUE_ADDR_IPV6 = 159,          /* VALUE_ADDR_IPV6  */
  YYSYMBOL_VALUE_ADDR_IPV6_CIDR = 160,     /* VALUE_ADDR_IPV6_CIDR  */
  YYSYMBOL_VALUE_SIGNED = 161,             /* VALUE_SIGNED  */
  YYSYMBOL_VALUE_UNSIGNED = 162,           /* VALUE_UNSIGNED  */
  YYSYMBOL_VALUE_FLOAT = 163,              /* VALUE_FLOAT  */
  YYSYMBOL_164_ = 164,                     /* ';'  */
  YYSYMBOL_165_ = 165,                     /* '('  */
  YYSYMBOL_166_ = 166,                     /* ')'  */
  YYSYMBOL_167_ = 167,                     /* '!'  */
  YYSYMBOL_168_ = 168,                     /* '>'  */
  YYSYMBOL_169_ = 169,                     /* '<'  */
  YYSYMBOL_YYACCEPT = 170,                 /* $accept  */
  YYSYMBOL_exprs = 171,                    /* exprs  */
  YYSYMBOL_expr = 172,                     /* expr  */
  YYSYMBOL_expr_ip_proto = 173,            /* expr_ip_proto  */
  YYSYMBOL_expr_ip_dscp = 174,             /* expr_ip_dscp  */
  YYSYMBOL_expr_ip_version = 175,          /* expr_ip_version  */
  YYSYMBOL_expr_vlan_id = 176,             /* expr_vlan_id  */
  YYSYMBOL_expr_other_type = 177,          /* expr_other_type  */
  YYSYMBOL_value_other_type = 178,         /* value_other_type  */
  YYSYMBOL_expr_any_mac = 179,             /* expr_any_mac  */
  YYSYMBOL_expr_local_mac = 180,           /* expr_local_mac  */
  YYSYMBOL_expr_other_mac = 181,           /* expr_other_mac  */
  YYSYMBOL_expr_src_mac = 182,             /* expr_src_mac  */
  YYSYMBOL_expr_dst_mac = 183,             /* expr_dst_mac  */
  YYSYMBOL_expr_any_ip = 184,              /* expr_any_ip  */
  YYSYMBOL_expr_local_ip = 185,            /* expr_local_ip  */
  YYSYMBOL_expr_other_ip = 186,            /* expr_other_ip  */
  YYSYMBOL_expr_src_ip = 187,              /* expr_src_ip  */
  YYSYMBOL_expr_dst_ip = 188,              /* expr_dst_ip  */
  YYSYMBOL_expr_conntrack_reply_src_ip = 189, /* expr_conntrack_reply_src_ip  */
  YYSYMBOL_expr_conntrack_reply_dst_ip = 190, /* expr_conntrack_reply_dst_ip  */
  YYSYMBOL_value_addr_ip = 191,            /* value_addr_ip  */
  YYSYMBOL_expr_any_port = 192,            /* expr_any_port  */
  YYSYMBOL_expr_local_port = 193,          /* expr_local_port  */
  YYSYMBOL_expr_other_port = 194,          /* expr_other_port  */
  YYSYMBOL_expr_src_port = 195,            /* expr_src_port  */
  YYSYMBOL_expr_dst_port = 196,            /* expr_dst_port  */
  YYSYMBOL_expr_tunnel_type = 197,         /* expr_tunnel_type  */
  YYSYMBOL_value_tunnel_type = 198,        /* value_tunnel_type  */
  YYSYMBOL_expr_detection_complete = 199,  /* expr_detection_complete  */
  YYSYMBOL_expr_detection_guessed = 200,   /* expr_detection_guessed  */
  YYSYMBOL_expr_detection_init = 201,      /* expr_detection_init  */
  YYSYMBOL_expr_detection_updated = 202,   /* expr_detection_updated  */
  YYSYMBOL_expr_dhc_hit = 203,             /* expr_dhc_hit  */
  YYSYMBOL_expr_fhc_hit = 204,             /* expr_fhc_hit  */
  YYSYMBOL_expr_ip_nat = 205,              /* expr_ip_nat  */
  YYSYMBOL_expr_expiring = 206,            /* expr_expiring  */
  YYSYMBOL_expr_expired = 207,             /* expr_expired  */
  YYSYMBOL_expr_soft_dissector = 208,      /* expr_soft_dissector  */
  YYSYMBOL_expr_app = 209,                 /* expr_app  */
  YYSYMBOL_expr_app_id = 210,              /* expr_app_id  */
  YYSYMBOL_expr_app_name = 211,            /* expr_app_name  */
  YYSYMBOL_expr_category = 212,            /* expr_category  */
  YYSYMBOL_expr_category_id = 213,         /* expr_category_id  */
  YYSYMBOL_expr_app_category = 214,        /* expr_app_category  */
  YYSYMBOL_expr_app_category_id = 215,     /* expr_app_category_id  */
  YYSYMBOL_expr_domain_category = 216,     /* expr_domain_category  */
  YYSYMBOL_expr_domain_category_id = 217,  /* expr_domain_category_id  */
  YYSYMBOL_expr_network_category = 218,    /* expr_network_category  */
  YYSYMBOL_expr_network_category_id = 219, /* expr_network_category_id  */
  YYSYMBOL_expr_local_network_category = 220, /* expr_local_network_category  */
  YYSYMBOL_expr_local_network_category_id = 221, /* expr_local_network_category_id  */
  YYSYMBOL_expr_other_network_category = 222, /* expr_other_network_category  */
  YYSYMBOL_expr_other_network_category_id = 223, /* expr_other_network_category_id  */
  YYSYMBOL_expr_src_network_category = 224, /* expr_src_network_category  */
  YYSYMBOL_expr_src_network_category_id = 225, /* expr_src_network_category_id  */
  YYSYMBOL_expr_dst_network_category = 226, /* expr_dst_network_category  */
  YYSYMBOL_expr_dst_network_category_id = 227, /* expr_dst_network_category_id  */
  YYSYMBOL_expr_tag_category = 228,        /* expr_tag_category  */
  YYSYMBOL_expr_tag_category_id = 229,     /* expr_tag_category_id  */
  YYSYMBOL_expr_proto = 230,               /* expr_proto  */
  YYSYMBOL_expr_proto_id = 231,            /* expr_proto_id  */
  YYSYMBOL_expr_proto_name = 232,          /* expr_proto_name  */
  YYSYMBOL_expr_proto_category = 233,      /* expr_proto_category  */
  YYSYMBOL_expr_proto_category_id = 234,   /* expr_proto_category_id  */
  YYSYMBOL_expr_detected_hostname = 235,   /* expr_detected_hostname  */
  YYSYMBOL_expr_dns_hostname = 236,        /* expr_dns_hostname  */
  YYSYMBOL_expr_risks = 237,               /* expr_risks  */
  YYSYMBOL_expr_risk_ndpi_score = 238,     /* expr_risk_ndpi_score  */
  YYSYMBOL_expr_risk_ndpi_score_client = 239, /* expr_risk_ndpi_score_client  */
  YYSYMBOL_expr_risk_ndpi_score_server = 240, /* expr_risk_ndpi_score_server  */
  YYSYMBOL_expr_conntrack_id = 241,        /* expr_conntrack_id  */
  YYSYMBOL_expr_conntrack_mark = 242,      /* expr_conntrack_mark  */
  YYSYMBOL_expr_iface = 243,               /* expr_iface  */
  YYSYMBOL_expr_iface_nfq_src = 244,       /* expr_iface_nfq_src  */
  YYSYMBOL_expr_iface_nfq_dst = 245,       /* expr_iface_nfq_dst  */
  YYSYMBOL_expr_intel = 246,               /* expr_intel  */
  YYSYMBOL_expr_tls_version = 247,         /* expr_tls_version  */
  YYSYMBOL_expr_tls_cipher = 248,          /* expr_tls_cipher  */
  YYSYMBOL_expr_tls_ech = 249,             /* expr_tls_ech  */
  YYSYMBOL_expr_tls_ja4 = 250,             /* expr_tls_ja4  */
  YYSYMBOL_expr_origin = 251,              /* expr_origin  */
  YYSYMBOL_value_origin_type = 252,        /* value_origin_type  */
  YYSYMBOL_expr_tag = 253,                 /* expr_tag  */
  YYSYMBOL_expr_app_ip_override = 254,     /* expr_app_ip_override  */
  YYSYMBOL_expr_app_proto_twins = 255,     /* expr_app_proto_twins  */
  YYSYMBOL_expr_ts_first_seen = 256,       /* expr_ts_first_seen  */
  YYSYMBOL_expr_ts_last_seen = 257,        /* expr_ts_last_seen  */
  YYSYMBOL_expr_nfq_src_ifindex = 258,     /* expr_nfq_src_ifindex  */
  YYSYMBOL_expr_nfq_dst_ifindex = 259,     /* expr_nfq_dst_ifindex  */
  YYSYMBOL_expr_tcp_fin_ack = 260,         /* expr_tcp_fin_ack  */
  YYSYMBOL_expr_tcp_last_seq = 261,        /* expr_tcp_last_seq  */
  YYSYMBOL_expr_total_bytes = 262,         /* expr_total_bytes  */
  YYSYMBOL_expr_total_packets = 263,       /* expr_total_packets  */
  YYSYMBOL_expr_local_bytes = 264,         /* expr_local_bytes  */
  YYSYMBOL_expr_other_bytes = 265,         /* expr_other_bytes  */
  YYSYMBOL_expr_src_bytes = 266,           /* expr_src_bytes  */
  YYSYMBOL_expr_dst_bytes = 267,           /* expr_dst_bytes  */
  YYSYMBOL_expr_local_packets = 268,       /* expr_local_packets  */
  YYSYMBOL_expr_other_packets = 269,       /* expr_other_packets  */
  YYSYMBOL_expr_src_packets = 270,         /* expr_src_packets  */
  YYSYMBOL_expr_dst_packets = 271,         /* expr_dst_packets  */
  YYSYMBOL_expr_http_user_agent = 272,     /* expr_http_user_agent  */
  YYSYMBOL_expr_http_url = 273,            /* expr_http_url  */
  YYSYMBOL_expr_dhcp_fingerprint = 274,    /* expr_dhcp_fingerprint  */
  YYSYMBOL_expr_dhcp_class_ident = 275,    /* expr_dhcp_class_ident  */
  YYSYMBOL_expr_ssh_client_agent = 276,    /* expr_ssh_client_agent  */
  YYSYMBOL_expr_ssh_server_agent = 277,    /* expr_ssh_server_agent  */
  YYSYMBOL_expr_tls_subject_dn = 278,      /* expr_tls_subject_dn  */
  YYSYMBOL_expr_tls_issuer_dn = 279,       /* expr_tls_issuer_dn  */
  YYSYMBOL_expr_tls_server_cn = 280,       /* expr_tls_server_cn  */
  YYSYMBOL_expr_mdns_domain_name = 281,    /* expr_mdns_domain_name  */
  YYSYMBOL_expr_tls_alpn = 282,            /* expr_tls_alpn  */
  YYSYMBOL_expr_tls_proc_hello = 283,      /* expr_tls_proc_hello  */
  YYSYMBOL_expr_tls_proc_certificate = 284, /* expr_tls_proc_certificate  */
  YYSYMBOL_expr_smtp_tls = 285,            /* expr_smtp_tls  */
  YYSYMBOL_expr_total_local_bytes = 286,   /* expr_total_local_bytes  */
  YYSYMBOL_expr_total_other_bytes = 287,   /* expr_total_other_bytes  */
  YYSYMBOL_expr_total_src_bytes = 288,     /* expr_total_src_bytes  */
  YYSYMBOL_expr_total_dst_bytes = 289,     /* expr_total_dst_bytes  */
  YYSYMBOL_expr_total_local_packets = 290, /* expr_total_local_packets  */
  YYSYMBOL_expr_total_other_packets = 291, /* expr_total_other_packets  */
  YYSYMBOL_expr_total_src_packets = 292,   /* expr_total_src_packets  */
  YYSYMBOL_expr_total_dst_packets = 293,   /* expr_total_dst_packets  */
  YYSYMBOL_expr_detection_packets = 294,   /* expr_detection_packets  */
  YYSYMBOL_expr_local_rate = 295,          /* expr_local_rate  */
  YYSYMBOL_expr_other_rate = 296,          /* expr_other_rate  */
  YYSYMBOL_expr_src_rate = 297,            /* expr_src_rate  */
  YYSYMBOL_expr_dst_rate = 298,            /* expr_dst_rate  */
  YYSYMBOL_expr_tcp_seq_errors = 299,      /* expr_tcp_seq_errors  */
  YYSYMBOL_expr_tcp_resets = 300,          /* expr_tcp_resets  */
  YYSYMBOL_expr_tcp_retrans = 301,         /* expr_tcp_retrans  */
  YYSYMBOL_expr_tls_alpn_server = 302,     /* expr_tls_alpn_server  */
  YYSYMBOL_expr_tls_cert_fingerprint = 303, /* expr_tls_cert_fingerprint  */
  YYSYMBOL_expr_bt_info_hash = 304,        /* expr_bt_info_hash  */
  YYSYMBOL_expr_stun_mapped = 305,         /* expr_stun_mapped  */
  YYSYMBOL_expr_stun_peer = 306,           /* expr_stun_peer  */
  YYSYMBOL_expr_stun_relayed = 307,        /* expr_stun_relayed  */
  YYSYMBOL_expr_stun_response = 308,       /* expr_stun_response  */
  YYSYMBOL_expr_stun_other = 309           /* expr_stun_other  */
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
#define YYLAST   1470

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  170
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  140
/* YYNRULES -- Number of rules.  */
#define YYNRULES  843
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1373

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   418


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
       2,     2,     2,   167,     2,     2,     2,     2,     2,     2,
     165,   166,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   164,
     169,     2,   168,     2,     2,     2,     2,     2,     2,     2,
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
     155,   156,   157,   158,   159,   160,   161,   162,   163
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   279,   279,   281,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   418,   422,   426,   431,   435,   439,
     443,   447,   451,   455,   462,   466,   473,   477,   484,   488,
     492,   496,   500,   504,   508,   512,   519,   525,   531,   575,
     622,   623,   624,   625,   626,   627,   628,   632,   647,   660,
     676,   694,   701,   707,   716,   728,   735,   742,   751,   763,
     770,   777,   786,   798,   805,   812,   821,   833,   846,   859,
     874,   892,   898,   904,   913,   925,   931,   937,   946,   958,
     965,   972,   982,   995,  1002,  1009,  1019,  1032,  1043,  1054,
    1068,  1085,  1096,  1107,  1121,  1138,  1139,  1140,  1141,  1145,
    1151,  1157,  1163,  1169,  1175,  1181,  1187,  1196,  1200,  1204,
    1208,  1212,  1216,  1220,  1224,  1231,  1235,  1239,  1243,  1247,
    1251,  1255,  1259,  1266,  1270,  1274,  1278,  1282,  1286,  1290,
    1294,  1301,  1305,  1309,  1313,  1317,  1321,  1325,  1329,  1336,
    1342,  1348,  1367,  1389,  1390,  1393,  1397,  1403,  1411,  1419,
    1427,  1438,  1442,  1448,  1456,  1464,  1472,  1483,  1487,  1493,
    1501,  1509,  1517,  1528,  1532,  1538,  1546,  1554,  1562,  1573,
    1577,  1583,  1591,  1599,  1607,  1618,  1622,  1628,  1636,  1644,
    1652,  1663,  1667,  1671,  1675,  1679,  1683,  1690,  1694,  1698,
    1702,  1706,  1710,  1717,  1721,  1725,  1729,  1733,  1737,  1744,
    1748,  1752,  1756,  1760,  1764,  1771,  1777,  1785,  1786,  1789,
    1798,  1810,  1837,  1867,  1935,  2002,  2055,  2111,  2128,  2148,
    2155,  2166,  2183,  2203,  2210,  2221,  2247,  2276,  2290,  2307,
    2324,  2344,  2352,  2363,  2380,  2400,  2408,  2419,  2436,  2456,
    2464,  2475,  2492,  2512,  2520,  2531,  2548,  2568,  2576,  2587,
    2593,  2599,  2600,  2603,  2609,  2618,  2638,  2660,  2677,  2697,
    2705,  2716,  2723,  2730,  2749,  2768,  2782,  2791,  2798,  2805,
    2824,  2843,  2857,  2866,  2870,  2874,  2893,  2912,  2925,  2941,
    2945,  2949,  2953,  2957,  2961,  2965,  2969,  2976,  2980,  2984,
    2988,  2992,  2996,  3000,  3004,  3011,  3015,  3019,  3023,  3027,
    3031,  3035,  3039,  3046,  3054,  3062,  3070,  3078,  3086,  3094,
    3102,  3113,  3121,  3129,  3137,  3145,  3153,  3161,  3169,  3180,
    3187,  3194,  3212,  3230,  3244,  3253,  3264,  3275,  3296,  3315,
    3329,  3346,  3357,  3368,  3387,  3406,  3421,  3438,  3459,  3481,
    3501,  3517,  3529,  3541,  3553,  3566,  3581,  3601,  3617,  3629,
    3641,  3653,  3666,  3681,  3693,  3705,  3719,  3731,  3743,  3757,
    3769,  3781,  3795,  3807,  3819,  3834,  3838,  3842,  3846,  3850,
    3854,  3858,  3862,  3869,  3873,  3877,  3881,  3885,  3889,  3893,
    3897,  3904,  3908,  3912,  3916,  3920,  3924,  3928,  3932,  3939,
    3946,  3953,  3972,  3991,  4005,  4022,  4026,  4030,  4034,  4041,
    4042,  4043,  4047,  4054,  4061,  4089,  4120,  4123,  4126,  4129,
    4132,  4135,  4141,  4144,  4147,  4150,  4153,  4156,  4162,  4165,
    4168,  4171,  4174,  4177,  4183,  4186,  4189,  4192,  4195,  4198,
    4204,  4207,  4210,  4213,  4216,  4219,  4225,  4228,  4231,  4234,
    4237,  4240,  4246,  4249,  4252,  4255,  4258,  4261,  4267,  4270,
    4273,  4276,  4279,  4282,  4288,  4291,  4294,  4297,  4300,  4303,
    4309,  4312,  4315,  4318,  4321,  4324,  4330,  4334,  4338,  4342,
    4346,  4350,  4357,  4361,  4365,  4369,  4373,  4377,  4384,  4388,
    4392,  4396,  4400,  4404,  4411,  4415,  4419,  4423,  4427,  4431,
    4438,  4442,  4446,  4450,  4454,  4458,  4465,  4469,  4473,  4477,
    4481,  4485,  4492,  4496,  4500,  4504,  4508,  4512,  4519,  4523,
    4527,  4531,  4535,  4539,  4546,  4549,  4552,  4562,  4572,  4579,
    4589,  4592,  4595,  4605,  4615,  4622,  4632,  4635,  4638,  4648,
    4658,  4665,  4675,  4678,  4681,  4691,  4701,  4708,  4718,  4721,
    4724,  4734,  4744,  4751,  4761,  4764,  4767,  4777,  4787,  4794,
    4804,  4807,  4810,  4820,  4830,  4837,  4847,  4850,  4853,  4863,
    4873,  4880,  4890,  4893,  4896,  4906,  4916,  4923,  4933,  4936,
    4939,  4949,  4959,  4966,  4976,  4979,  4982,  4993,  5004,  5014,
    5027,  5030,  5033,  5036,  5039,  5042,  5048,  5051,  5054,  5057,
    5060,  5063,  5069,  5072,  5075,  5078,  5081,  5084,  5090,  5098,
    5106,  5114,  5122,  5130,  5141,  5149,  5157,  5165,  5173,  5181,
    5192,  5200,  5208,  5216,  5224,  5232,  5243,  5251,  5259,  5267,
    5275,  5283,  5294,  5302,  5310,  5318,  5326,  5334,  5345,  5353,
    5361,  5369,  5377,  5385,  5396,  5404,  5412,  5420,  5428,  5436,
    5447,  5455,  5463,  5471,  5479,  5487,  5498,  5505,  5512,  5519,
    5526,  5533,  5543,  5551,  5559,  5567,  5575,  5583,  5594,  5602,
    5610,  5618,  5626,  5634,  5645,  5653,  5661,  5669,  5677,  5685,
    5696,  5704,  5712,  5720,  5728,  5736,  5747,  5754,  5761,  5768,
    5775,  5782,  5792,  5799,  5806,  5813,  5820,  5827,  5837,  5844,
    5851,  5858,  5865,  5872,  5882,  5885,  5888,  5899,  5910,  5920,
    5933,  5943,  5956,  5966,  5979,  5982,  5985,  5991,  6000,  6003,
    6006,  6012,  6021,  6024,  6027,  6033,  6042,  6045,  6048,  6054,
    6063,  6066,  6069,  6075
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
  "FLOW_TUNNEL_TYPE", "FLOW_VLAN_ID", "FLOW_APP_IP_OVERRIDE",
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

#define YYPACT_NINF (-269)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -269,   155,  -269,  -105,   -49,  -142,   -24,   164,   168,   172,
     228,  -138,  -131,   258,   262,   266,   270,   274,   433,   484,
     494,   504,   516,   520,   527,   531,   564,   575,  -127,   590,
     608,   616,   620,   624,   654,   672,   776,  -123,   778,   780,
     782,   784,   786,   -97,   -88,   -84,   -80,   788,   790,   792,
     794,   796,   798,   800,   -71,   802,   804,   806,   808,   810,
     812,   814,   816,   818,   820,   -45,   822,   824,   826,   -41,
     -36,   828,   -32,   830,   -28,   832,   834,   836,   838,   142,
     146,   840,   842,   150,   156,   844,   160,   186,   190,   194,
     198,   202,   846,   848,   206,   232,   850,   852,   854,   856,
     236,   240,   244,   248,   252,   435,   523,   538,   542,   549,
     553,   568,   582,   586,   594,   598,   602,   612,   628,   632,
     858,   860,   862,   864,   866,   868,   870,   872,   874,   876,
     878,   638,   441,   581,   -96,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,   273,   416,
     -62,   -31,  -152,  -118,   -92,   -79,   -55,   -23,  -126,  -111,
    -129,   -20,   -13,   137,   -10,   167,   165,   207,   421,   423,
     429,   443,   445,   454,   472,   480,   483,   519,   528,   539,
     463,   630,   635,   665,   440,   871,   877,   879,   881,   883,
     885,   887,   889,   891,   893,   895,   894,   896,   562,   569,
     563,   643,   670,   675,   -27,   -18,   656,   762,   865,   998,
    1000,  1002,  1004,  1006,  1007,  1008,   901,   903,   905,   907,
     909,   911,   910,   912,   914,   916,   918,   920,  1009,  1011,
     925,   927,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,
     680,   685,    -6,    -5,  1028,  1029,  1022,  1023,  1024,  1025,
    1026,  1027,  1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,
    1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,
    1048,  1049,  1058,  1059,  1052,  1053,   450,   450,   690,   695,
     170,   204,  1062,  1063,  1056,  1057,  1060,  1061,  1064,  1065,
    1066,  1067,   291,   291,   -85,   -76,  1070,  1071,  1068,  1069,
     -75,   -72,   929,   931,   700,   705,   208,   212,  1078,  1079,
    1072,  1073,  1074,  1075,  1076,  1077,  1080,  1081,  1086,  1087,
    1090,  1091,  1084,  1085,  1088,  1089,  1092,  1093,  1094,  1095,
    1096,  1097,  1098,  1099,  1100,  1101,   930,   932,  1102,  1103,
    1104,  1105,  1106,  1107,   947,   947,  1108,  1109,  1110,  1111,
    1112,  1113,   939,   941,   943,   945,   944,   946,   948,   950,
    1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,
    1124,  1125,   952,   954,   956,   958,  1126,  1127,  1128,  1129,
    1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,   960,   962,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,
    1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1171,  1172,  1173,   964,   966,   968,   970,
    1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,
    1184,  1185,   972,   974,   976,   978,   980,   982,   984,   986,
    1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,
    1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,   193,   861,
    1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,
    1284,  1285,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,
    1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,
    1313,  1314,   988,   990,   995,   997,   999,  1001,  1003,  1005,
    1315,  1316,   710,   715,   720,   725,   730,   735,   740,   745,
     750,   755,    -9,   657,   470,   641,   652,   756,  -137,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,   441,   441,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  1010,  1010
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,     0,     0,   219,   325,     0,     0,     0,
       0,   423,   431,     0,     0,   381,   265,   271,   277,   283,
     289,   387,     0,     0,     0,     0,     0,     0,   251,   313,
     307,   295,   439,   451,   445,     0,   301,   136,     0,     0,
       0,     0,     0,   227,   399,   407,   415,     0,     0,   515,
       0,     0,     0,     0,   235,   156,   369,     0,     0,   393,
     319,     0,     0,     0,     0,   243,   522,     0,     0,   493,
     501,   509,   485,   259,   148,   526,   532,   652,   646,     0,
       0,   640,   634,     0,     0,   688,     0,     0,     0,     0,
       0,     0,   658,   664,     0,     0,   694,   676,   682,   670,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   814,   700,   706,   712,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     4,     5,     6,     7,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    95,   327,
     328,    93,    94,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     371,   372,   113,   114,   115,   116,   117,   118,   119,   120,
     126,   127,   128,   129,   130,   131,   121,   122,   123,   124,
     125,   132,     8,     9,    35,    36,    20,    19,    27,    28,
      33,    34,    16,    21,    23,    12,    17,    22,    24,    13,
      15,    14,    11,    10,    25,    26,    32,    30,    31,    18,
      29,    55,    56,    57,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      54,    53,    58,    59,    60,    61,    62,    63,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,   220,
     326,   424,   432,   382,   266,   272,   278,   284,   290,   388,
     252,   314,   308,   296,   440,   452,   446,   302,   137,   228,
     400,   408,   416,   516,   236,   157,   370,   394,   320,   244,
     523,   494,   502,   510,   486,   260,   149,   527,   533,   653,
     647,   641,   635,   689,   659,   665,   695,   677,   683,   671,
     815,   701,   707,   713,   458,     0,     0,     3,   189,   215,
     216,   217,   218,   187,   190,   188,   167,   169,   168,   170,
     221,   222,   223,   224,   225,   226,   331,   329,   332,   330,
     337,   338,   339,   340,   333,   334,   335,   336,   425,   426,
     427,   428,   429,   430,   433,   434,   435,   436,   437,   438,
     213,   211,   214,   212,   209,   207,   210,   208,   383,   385,
     384,   386,   267,   268,   269,   270,   273,   274,   275,   276,
     279,   280,   281,   282,   285,   286,   287,   288,   291,   292,
     293,   294,   389,   391,   390,   392,   341,   342,   343,   344,
     205,   203,   206,   204,   183,   185,   184,   186,   361,   362,
     363,   364,   253,   254,   255,   256,   257,   258,   315,   316,
     317,   318,   309,   310,   311,   312,   297,   298,   299,   300,
     441,   443,   442,   444,   453,   455,   454,   456,   447,   449,
     448,   450,   144,   145,   303,   304,   305,   306,   138,   139,
     140,   141,   142,   143,   146,   147,   193,   191,   194,   192,
     171,   173,   172,   174,   349,   350,   351,   352,   229,   230,
     231,   232,   233,   234,   401,   402,   403,   404,   405,   406,
     409,   410,   411,   412,   413,   414,   417,   418,   419,   420,
     421,   422,   345,   346,   347,   348,   519,   520,   521,   517,
     518,   197,   195,   198,   196,   175,   177,   176,   178,   353,
     354,   355,   356,   237,   238,   239,   240,   241,   242,   160,
     161,   162,   163,   164,   165,   166,   158,   159,   375,   373,
     376,   374,   377,   378,   379,   380,   395,   397,   396,   398,
     321,   322,   323,   324,   201,   199,   202,   200,   179,   181,
     180,   182,   357,   358,   359,   360,   245,   246,   247,   248,
     249,   250,   524,   525,   365,   366,   367,   368,   495,   496,
     497,   498,   499,   500,   503,   504,   505,   506,   507,   508,
     511,   513,   512,   514,   487,   488,   489,   490,   491,   492,
     263,   264,   261,   262,   150,   151,   152,   153,   154,   155,
     528,   529,   530,   531,   534,   535,   536,   537,   654,   656,
     655,   657,   648,   650,   649,   651,   604,   605,   606,   607,
     608,   609,   628,   629,   630,   631,   632,   633,   642,   644,
     643,   645,   636,   638,   637,   639,   586,   587,   588,   589,
     590,   591,   610,   611,   612,   613,   614,   615,   690,   692,
     691,   693,   556,   557,   558,   559,   560,   561,   550,   551,
     552,   553,   554,   555,   592,   593,   594,   595,   596,   597,
     616,   617,   618,   619,   620,   621,   598,   599,   600,   601,
     602,   603,   622,   623,   624,   625,   626,   627,   660,   662,
     661,   663,   666,   668,   667,   669,   562,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   696,   698,
     697,   699,   678,   680,   679,   681,   684,   686,   685,   687,
     672,   674,   673,   675,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   718,   719,
     720,   721,   722,   723,   724,   725,   726,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   820,   821,   816,   818,   817,   819,
     702,   703,   704,   705,   708,   709,   710,   711,   714,   715,
     716,   717,   822,   823,   826,   824,   827,   825,   830,   828,
     831,   829,   834,   832,   835,   833,   838,   836,   839,   837,
     842,   840,   843,   841,   464,   465,   459,   460,   462,   461,
     463,   471,   472,   466,   467,   469,   468,   470,   474,   473,
     475,   477,   476,   478,   480,   479,   481,   483,   482,   484,
     135,   134,   133
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -269,  -269,  -132,  -269,  -269,  -269,  -269,  -269,   748,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -268,  -269,  -269,  -269,  -269,  -269,  -269,   698,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,   768,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   134,   135,   136,   137,   138,   139,   986,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   783,   152,   153,   154,   155,   156,   157,  1052,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   959,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     718,   785,   272,   273,   274,   275,   288,   289,   290,   291,
     790,   775,   776,   294,   295,   296,   297,   330,   331,   332,
     333,   352,   353,   354,   355,   800,   276,   277,   796,  1370,
     292,   293,   821,   823,   825,   827,   797,   298,   299,   268,
     269,   334,   335,   798,   791,   356,   357,   368,   369,   370,
     371,   799,   775,   776,   861,   863,   374,   375,   376,   377,
     380,   381,   382,   383,   386,   387,   388,   389,   777,   988,
     792,   372,   373,   406,   407,   408,   409,   989,   990,   996,
     378,   379,   998,   793,   384,   385,   991,   997,   390,   391,
     999,   786,   917,   919,   787,   270,   271,   410,   411,   432,
     433,   434,   435,   444,   445,   446,   447,   794,   450,   451,
     452,   453,   458,   459,   460,   461,   466,   467,   468,   469,
     278,   279,   788,   436,   437,   789,   864,   448,   449,   865,
     962,   964,   454,   455,   801,   866,   462,   463,   867,   795,
     470,   471,  1344,  1345,   804,  1346,  1347,   920,   922,   802,
     921,   923,  1348,  1349,  1350,     2,  1005,  1007,     3,     4,
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
     125,   126,   127,   128,   129,   130,   480,   481,   482,   483,
     486,   487,   488,   489,   496,   497,   498,   499,   131,   803,
     502,   503,   504,   505,   510,   511,   512,   513,   280,   281,
     484,   485,   282,   283,   490,   491,   284,   285,   500,   501,
     132,   805,   133,   965,   506,   507,   966,   806,   514,   515,
     516,   517,   518,   519,   522,   523,   524,   525,   528,   529,
     530,   531,   534,   535,   536,   537,   540,   541,   542,   543,
     550,   551,   552,   553,   520,   521,  1262,   967,   526,   527,
     968,  1008,   532,   533,  1009,  1010,   538,   539,  1011,   807,
     544,   545,   286,   287,   554,   555,   556,   557,   558,   559,
     570,   571,   572,   573,   576,   577,   578,   579,   582,   583,
     584,   585,   588,   589,   590,   591,   594,   595,   596,   597,
     560,   561,   300,   301,   574,   575,   302,   303,   580,   581,
     304,   305,   586,   587,   306,   307,   592,   593,   308,   309,
     598,   599,   979,   980,   981,   982,   983,   984,   985,   778,
     779,   780,   781,   782,  1325,  1327,  1329,  1331,  1333,  1335,
    1337,  1339,  1341,  1343,     3,     4,     5,     6,     7,     8,
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
     129,   130,   784,   779,   780,   781,   782,   310,   311,   600,
     601,   602,   603,   808,   131,   809,   719,   720,   956,   957,
     958,   810,   721,   722,   828,   829,   723,   724,   725,   726,
     727,   728,   729,   604,   605,   811,   132,   812,   133,   730,
     731,   732,   733,   734,   735,   736,   813,   737,   738,   820,
     779,   780,   781,   782,   739,   740,   741,   742,   312,   313,
     743,  1358,  1359,  1360,   814,   744,   745,   746,   314,   315,
     747,   748,   815,  1371,  1372,   816,   749,   750,   316,   317,
     751,   752,   753,   754,   755,   756,   757,   758,   759,   760,
     318,   319,   761,   762,   320,   321,   763,   606,   607,   608,
     609,   322,   323,   764,   765,   324,   325,   766,   767,   768,
     769,   817,   612,   613,   614,   615,   618,   619,   620,   621,
     818,   610,   611,   624,   625,   626,   627,   630,   631,   632,
     633,   819,   770,   771,   772,   773,   616,   617,   326,   327,
     622,   623,   636,   637,   638,   639,   856,   628,   629,   328,
     329,   634,   635,   857,   774,   858,   642,   643,   644,   645,
     648,   649,   650,   651,   336,   337,   640,   641,   654,   655,
     656,   657,   660,   661,   662,   663,   666,   667,   668,   669,
     646,   647,   338,   339,   652,   653,   672,   673,   674,   675,
     340,   341,   658,   659,   342,   343,   664,   665,   344,   345,
     670,   671,   678,   679,   680,   681,   684,   685,   686,   687,
     676,   677,   712,   713,   714,   715,   822,   779,   780,   781,
     782,   824,   779,   780,   781,   782,   682,   683,   346,   347,
     688,   689,  1361,  1362,  1363,   859,   716,   717,  1351,  1352,
     868,  1353,  1354,  1364,  1365,  1366,   348,   349,  1355,  1356,
    1357,   826,   779,   780,   781,   782,   860,   779,   780,   781,
     782,   862,   779,   780,   781,   782,   916,   779,   780,   781,
     782,   918,   779,   780,   781,   782,   961,   779,   780,   781,
     782,   963,   779,   780,   781,   782,  1004,   779,   780,   781,
     782,  1006,   779,   780,   781,   782,  1324,   779,   780,   781,
     782,  1326,   779,   780,   781,   782,  1328,   779,   780,   781,
     782,  1330,   779,   780,   781,   782,  1332,   779,   780,   781,
     782,  1334,   779,   780,   781,   782,  1336,   779,   780,   781,
     782,  1338,   779,   780,   781,   782,  1340,   779,   780,   781,
     782,  1342,   779,   780,   781,   782,   869,  1367,  1368,  1369,
     350,   351,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   438,   439,   440,   441,
     442,   443,   456,   457,   464,   465,   472,   473,   474,   475,
     476,   477,   478,   479,   492,   493,   494,   495,   508,   509,
     546,   547,   548,   549,   562,   563,   564,   565,   566,   567,
     568,   569,   690,   691,   692,   693,   694,   695,   696,   697,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,  1263,   830,   831,   870,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   846,   847,   848,   849,   850,   851,   852,   853,
     854,   855,   878,   879,   880,   881,   882,   883,   884,   885,
     886,   887,   888,   889,   890,   891,   892,   893,   894,   895,
     896,   897,   898,   899,   900,   901,   904,   905,   906,   907,
    1000,  1001,  1002,  1003,  1040,  1041,  1042,  1043,  1050,  1051,
    1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,
    1070,  1071,  1072,  1073,  1074,  1075,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1108,  1109,  1110,  1111,  1148,  1149,
    1150,  1151,  1152,  1153,  1154,  1155,  1168,  1169,  1170,  1171,
    1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  1183,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,
    1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,   775,   776,
     871,   987,   872,  1053,   873,   960,   874,     0,   875,   876,
     877,   902,     0,   903,   908,   909,   910,   911,   912,   913,
     914,   915,   924,   925,   926,   927,   928,   929,   930,   931,
       0,     0,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   969,   970,   971,   972,
       0,     0,   973,   974,   992,   993,   975,   976,   977,   978,
     994,   995,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,
    1022,  1023,  1020,  1021,  1024,  1025,  1026,  1027,     0,     0,
    1028,  1029,     0,     0,  1030,  1031,  1032,  1033,  1034,  1035,
    1036,  1037,  1038,  1039,  1044,  1045,  1046,  1047,  1048,  1049,
    1054,  1055,  1056,  1057,  1058,  1059,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1096,  1097,
    1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,
    1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1142,  1143,  1144,  1145,  1146,  1147,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1184,  1185,
    1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,
    1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,     0,  1264,  1265,  1266,
    1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,
    1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,  1296,
    1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,  1322,
    1323
};

static const yytype_int16 yycheck[] =
{
     132,   269,   144,   145,   146,   147,   144,   145,   146,   147,
     162,   148,   149,   144,   145,   146,   147,   144,   145,   146,
     147,   144,   145,   146,   147,   154,   168,   169,   154,   166,
     168,   169,   300,   301,   302,   303,   162,   168,   169,   144,
     145,   168,   169,   154,   162,   168,   169,   144,   145,   146,
     147,   162,   148,   149,   322,   323,   144,   145,   146,   147,
     144,   145,   146,   147,   144,   145,   146,   147,   164,   154,
     162,   168,   169,   144,   145,   146,   147,   162,   154,   154,
     168,   169,   154,   162,   168,   169,   162,   162,   168,   169,
     162,   153,   360,   361,   156,   144,   145,   168,   169,   144,
     145,   146,   147,   144,   145,   146,   147,   162,   144,   145,
     146,   147,   144,   145,   146,   147,   144,   145,   146,   147,
     144,   145,   153,   168,   169,   156,   153,   168,   169,   156,
     398,   399,   168,   169,   154,   153,   168,   169,   156,   162,
     168,   169,   151,   152,   154,   154,   155,   153,   153,   162,
     156,   156,   161,   162,   163,     0,   424,   425,     3,     4,
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
     125,   126,   127,   128,   129,   130,   144,   145,   146,   147,
     144,   145,   146,   147,   144,   145,   146,   147,   143,   162,
     144,   145,   146,   147,   144,   145,   146,   147,   144,   145,
     168,   169,   144,   145,   168,   169,   144,   145,   168,   169,
     165,   154,   167,   153,   168,   169,   156,   162,   168,   169,
     144,   145,   146,   147,   144,   145,   146,   147,   144,   145,
     146,   147,   144,   145,   146,   147,   144,   145,   146,   147,
     144,   145,   146,   147,   168,   169,   163,   153,   168,   169,
     156,   153,   168,   169,   156,   153,   168,   169,   156,   162,
     168,   169,   144,   145,   168,   169,   144,   145,   146,   147,
     144,   145,   146,   147,   144,   145,   146,   147,   144,   145,
     146,   147,   144,   145,   146,   147,   144,   145,   146,   147,
     168,   169,   144,   145,   168,   169,   144,   145,   168,   169,
     144,   145,   168,   169,   144,   145,   168,   169,   144,   145,
     168,   169,   131,   132,   133,   134,   135,   136,   137,   156,
     157,   158,   159,   160,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,     3,     4,     5,     6,     7,     8,
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
     129,   130,   156,   157,   158,   159,   160,   144,   145,   144,
     145,   146,   147,   162,   143,   162,     5,     6,   138,   139,
     140,   162,    11,    12,   154,   155,    15,    16,    17,    18,
      19,    20,    21,   168,   169,   162,   165,   162,   167,    28,
      29,    30,    31,    32,    33,    34,   162,    36,    37,   156,
     157,   158,   159,   160,    43,    44,    45,    46,   144,   145,
      49,   161,   162,   163,   162,    54,    55,    56,   144,   145,
      59,    60,   162,   775,   776,   162,    65,    66,   144,   145,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
     144,   145,    81,    82,   144,   145,    85,   144,   145,   146,
     147,   144,   145,    92,    93,   144,   145,    96,    97,    98,
      99,   162,   144,   145,   146,   147,   144,   145,   146,   147,
     162,   168,   169,   144,   145,   146,   147,   144,   145,   146,
     147,   162,   121,   122,   123,   124,   168,   169,   144,   145,
     168,   169,   144,   145,   146,   147,   154,   168,   169,   144,
     145,   168,   169,   154,   143,   162,   144,   145,   146,   147,
     144,   145,   146,   147,   144,   145,   168,   169,   144,   145,
     146,   147,   144,   145,   146,   147,   144,   145,   146,   147,
     168,   169,   144,   145,   168,   169,   144,   145,   146,   147,
     144,   145,   168,   169,   144,   145,   168,   169,   144,   145,
     168,   169,   144,   145,   146,   147,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   168,   169,   144,   145,
     168,   169,   161,   162,   163,   162,   168,   169,   151,   152,
     154,   154,   155,   161,   162,   163,   144,   145,   161,   162,
     163,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   156,   157,   158,   159,
     160,   156,   157,   158,   159,   160,   154,   161,   162,   163,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   163,   154,   155,   162,   151,   152,
     151,   152,   151,   152,   151,   152,   151,   152,   151,   152,
     151,   152,   151,   152,   151,   152,   151,   152,   154,   155,
     154,   155,   151,   152,   151,   152,   151,   152,   151,   152,
     151,   152,   151,   152,   154,   155,   154,   155,   154,   155,
     154,   155,   154,   155,   154,   155,   151,   152,   151,   152,
     151,   152,   151,   152,   154,   155,   154,   155,   141,   142,
     151,   152,   151,   152,   151,   152,   151,   152,   154,   155,
     154,   155,   154,   155,   154,   155,   154,   155,   154,   155,
     154,   155,   154,   155,   154,   155,   154,   155,   154,   155,
     154,   155,   154,   155,   154,   155,   154,   155,   154,   155,
     154,   155,   154,   155,   154,   155,   154,   155,   154,   155,
     154,   155,   154,   155,   154,   155,   151,   152,   151,   152,
     151,   152,   151,   152,   151,   152,   151,   152,   148,   149,
     162,   413,   162,   465,   162,   397,   162,    -1,   162,   162,
     162,   162,    -1,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   154,   154,   162,   162,   162,   162,   162,   162,
      -1,    -1,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   154,   154,   162,   162,   154,   154,   162,   162,
      -1,    -1,   162,   162,   154,   154,   162,   162,   162,   162,
     162,   162,   154,   154,   162,   162,   162,   162,   162,   162,
     154,   154,   162,   162,   154,   154,   162,   162,    -1,    -1,
     162,   162,    -1,    -1,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,    -1,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   154,   154,   154,
     154
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   171,     0,     3,     4,     5,     6,     7,     8,     9,
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
     130,   143,   165,   167,   172,   173,   174,   175,   176,   177,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   192,   193,   194,   195,   196,   197,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   144,   145,
     144,   145,   144,   145,   146,   147,   168,   169,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   146,   147,   168,   169,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   146,   147,   168,   169,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   146,   147,
     168,   169,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   146,   147,   168,   169,   144,   145,
     144,   145,   144,   145,   144,   145,   146,   147,   168,   169,
     144,   145,   146,   147,   168,   169,   144,   145,   144,   145,
     146,   147,   168,   169,   144,   145,   144,   145,   146,   147,
     168,   169,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   144,   145,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   168,   169,   144,   145,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   168,   169,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   146,   147,   168,   169,   144,   145,   144,   145,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   168,   169,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   168,   169,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   168,   169,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   146,   147,   168,   169,   144,   145,   146,   147,
     168,   169,   144,   145,   146,   147,   168,   169,   144,   145,
     146,   147,   168,   169,   144,   145,   146,   147,   168,   169,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,   144,   145,   146,   147,   168,   169,   172,     5,
       6,    11,    12,    15,    16,    17,    18,    19,    20,    21,
      28,    29,    30,    31,    32,    33,    34,    36,    37,    43,
      44,    45,    46,    49,    54,    55,    56,    59,    60,    65,
      66,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    81,    82,    85,    92,    93,    96,    97,    98,    99,
     121,   122,   123,   124,   143,   148,   149,   164,   156,   157,
     158,   159,   160,   191,   156,   191,   153,   156,   153,   156,
     162,   162,   162,   162,   162,   162,   154,   162,   154,   162,
     154,   154,   162,   162,   154,   154,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     156,   191,   156,   191,   156,   191,   156,   191,   154,   155,
     154,   155,   151,   152,   151,   152,   151,   152,   151,   152,
     151,   152,   151,   152,   151,   152,   151,   152,   151,   152,
     151,   152,   154,   155,   154,   155,   154,   154,   162,   162,
     156,   191,   156,   191,   153,   156,   153,   156,   154,   154,
     162,   162,   162,   162,   162,   162,   162,   162,   151,   152,
     151,   152,   151,   152,   151,   152,   151,   152,   151,   152,
     154,   155,   154,   155,   154,   155,   154,   155,   154,   155,
     154,   155,   162,   162,   151,   152,   151,   152,   162,   162,
     162,   162,   162,   162,   162,   162,   156,   191,   156,   191,
     153,   156,   153,   156,   154,   154,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   154,   154,   162,   162,   138,   139,   140,   252,
     252,   156,   191,   156,   191,   153,   156,   153,   156,   154,
     154,   162,   162,   162,   162,   162,   162,   162,   162,   131,
     132,   133,   134,   135,   136,   137,   178,   178,   154,   162,
     154,   162,   154,   154,   162,   162,   154,   162,   154,   162,
     151,   152,   151,   152,   156,   191,   156,   191,   153,   156,
     153,   156,   154,   154,   162,   162,   162,   162,   162,   162,
     162,   162,   154,   154,   154,   154,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     154,   155,   154,   155,   162,   162,   162,   162,   162,   162,
     141,   142,   198,   198,   162,   162,   162,   162,   162,   162,
     151,   152,   151,   152,   151,   152,   151,   152,   154,   155,
     154,   155,   154,   155,   154,   155,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   154,   155,
     154,   155,   154,   155,   154,   155,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   154,   155,
     154,   155,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   154,   155,
     154,   155,   154,   155,   154,   155,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   154,   155,
     154,   155,   154,   155,   154,   155,   154,   155,   154,   155,
     154,   155,   154,   155,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   154,   154,   154,   155,   154,   155,
     151,   152,   151,   152,   151,   152,   151,   152,   151,   152,
     151,   152,   154,   154,   156,   191,   156,   191,   156,   191,
     156,   191,   156,   191,   156,   191,   156,   191,   156,   191,
     156,   191,   156,   191,   151,   152,   154,   155,   161,   162,
     163,   151,   152,   154,   155,   161,   162,   163,   161,   162,
     163,   161,   162,   163,   161,   162,   163,   161,   162,   163,
     166,   172,   172
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   170,   171,   171,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   174,   175,   175,   176,   176,
     176,   176,   176,   176,   176,   176,   177,   177,   177,   177,
     178,   178,   178,   178,   178,   178,   178,   179,   179,   179,
     179,   180,   180,   180,   180,   181,   181,   181,   181,   182,
     182,   182,   182,   183,   183,   183,   183,   184,   184,   184,
     184,   185,   185,   185,   185,   186,   186,   186,   186,   187,
     187,   187,   187,   188,   188,   188,   188,   189,   189,   189,
     189,   190,   190,   190,   190,   191,   191,   191,   191,   192,
     192,   192,   192,   192,   192,   192,   192,   193,   193,   193,
     193,   193,   193,   193,   193,   194,   194,   194,   194,   194,
     194,   194,   194,   195,   195,   195,   195,   195,   195,   195,
     195,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     197,   197,   197,   198,   198,   199,   199,   199,   199,   199,
     199,   200,   200,   200,   200,   200,   200,   201,   201,   201,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   203,   203,   204,   204,   204,   204,   204,
     204,   205,   205,   205,   205,   205,   205,   206,   206,   206,
     206,   206,   206,   207,   207,   207,   207,   207,   207,   208,
     208,   208,   208,   208,   208,   209,   209,   209,   209,   210,
     210,   211,   211,   212,   212,   213,   213,   214,   214,   215,
     215,   216,   216,   217,   217,   218,   218,   219,   219,   220,
     220,   221,   221,   222,   222,   223,   223,   224,   224,   225,
     225,   226,   226,   227,   227,   228,   228,   229,   229,   230,
     230,   230,   230,   231,   231,   232,   232,   233,   233,   234,
     234,   235,   235,   235,   235,   235,   235,   236,   236,   236,
     236,   236,   236,   237,   237,   237,   237,   237,   237,   238,
     238,   238,   238,   238,   238,   238,   238,   239,   239,   239,
     239,   239,   239,   239,   239,   240,   240,   240,   240,   240,
     240,   240,   240,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   242,   242,   242,   242,   242,   242,   242,   243,
     243,   243,   243,   243,   243,   244,   244,   244,   244,   244,
     244,   245,   245,   245,   245,   245,   245,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   247,   247,   247,   247,   247,
     247,   247,   247,   248,   248,   248,   248,   248,   248,   248,
     248,   249,   249,   249,   249,   249,   249,   249,   249,   250,
     250,   250,   250,   250,   250,   251,   251,   251,   251,   252,
     252,   252,   253,   253,   253,   253,   254,   254,   254,   254,
     254,   254,   255,   255,   255,   255,   255,   255,   256,   256,
     256,   256,   256,   256,   257,   257,   257,   257,   257,   257,
     258,   258,   258,   258,   258,   258,   259,   259,   259,   259,
     259,   259,   260,   260,   260,   260,   260,   260,   261,   261,
     261,   261,   261,   261,   262,   262,   262,   262,   262,   262,
     263,   263,   263,   263,   263,   263,   264,   264,   264,   264,
     264,   264,   265,   265,   265,   265,   265,   265,   266,   266,
     266,   266,   266,   266,   267,   267,   267,   267,   267,   267,
     268,   268,   268,   268,   268,   268,   269,   269,   269,   269,
     269,   269,   270,   270,   270,   270,   270,   270,   271,   271,
     271,   271,   271,   271,   272,   272,   272,   272,   272,   272,
     273,   273,   273,   273,   273,   273,   274,   274,   274,   274,
     274,   274,   275,   275,   275,   275,   275,   275,   276,   276,
     276,   276,   276,   276,   277,   277,   277,   277,   277,   277,
     278,   278,   278,   278,   278,   278,   279,   279,   279,   279,
     279,   279,   280,   280,   280,   280,   280,   280,   281,   281,
     281,   281,   281,   281,   282,   282,   282,   282,   282,   282,
     283,   283,   283,   283,   283,   283,   284,   284,   284,   284,
     284,   284,   285,   285,   285,   285,   285,   285,   286,   286,
     286,   286,   286,   286,   287,   287,   287,   287,   287,   287,
     288,   288,   288,   288,   288,   288,   289,   289,   289,   289,
     289,   289,   290,   290,   290,   290,   290,   290,   291,   291,
     291,   291,   291,   291,   292,   292,   292,   292,   292,   292,
     293,   293,   293,   293,   293,   293,   294,   294,   294,   294,
     294,   294,   295,   295,   295,   295,   295,   295,   296,   296,
     296,   296,   296,   296,   297,   297,   297,   297,   297,   297,
     298,   298,   298,   298,   298,   298,   299,   299,   299,   299,
     299,   299,   300,   300,   300,   300,   300,   300,   301,   301,
     301,   301,   301,   301,   302,   302,   302,   302,   302,   302,
     303,   303,   304,   304,   305,   305,   305,   305,   306,   306,
     306,   306,   307,   307,   307,   307,   308,   308,   308,   308,
     309,   309,   309,   309
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
       1,     1,     1,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     1,     1,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     2,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     2,     3,     3,     1,
       1,     1,     1,     2,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3
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
  case 133: /* expr: expr BOOL_OR expr  */
#line 414 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) || (yyvsp[0].bool_result)));
        _NDFP_debugf("OR (%d || %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 3110 "nd-flow-expr.cpp"
    break;

  case 134: /* expr: expr BOOL_AND expr  */
#line 418 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) && (yyvsp[0].bool_result)));
        _NDFP_debugf("AND (%d && %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 3119 "nd-flow-expr.cpp"
    break;

  case 135: /* expr: '(' expr ')'  */
#line 422 "nd-flow-expr.ypp"
                   { _NDFP_result = ((yyval.bool_result) = (yyvsp[-1].bool_result)); }
#line 3125 "nd-flow-expr.cpp"
    break;

  case 136: /* expr_ip_proto: FLOW_IP_PROTO  */
#line 426 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != 0));
        _NDFP_debugf(
            "IP Protocol is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3135 "nd-flow-expr.cpp"
    break;

  case 137: /* expr_ip_proto: '!' FLOW_IP_PROTO  */
#line 431 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == 0));
        _NDFP_debugf("IP Protocol is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3144 "nd-flow-expr.cpp"
    break;

  case 138: /* expr_ip_proto: FLOW_IP_PROTO CMP_EQUAL VALUE_UNSIGNED  */
#line 435 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3153 "nd-flow-expr.cpp"
    break;

  case 139: /* expr_ip_proto: FLOW_IP_PROTO CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 439 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3162 "nd-flow-expr.cpp"
    break;

  case 140: /* expr_ip_proto: FLOW_IP_PROTO CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 443 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol >= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3171 "nd-flow-expr.cpp"
    break;

  case 141: /* expr_ip_proto: FLOW_IP_PROTO CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 447 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol <= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3180 "nd-flow-expr.cpp"
    break;

  case 142: /* expr_ip_proto: FLOW_IP_PROTO '>' VALUE_UNSIGNED  */
#line 451 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol > (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3189 "nd-flow-expr.cpp"
    break;

  case 143: /* expr_ip_proto: FLOW_IP_PROTO '<' VALUE_UNSIGNED  */
#line 455 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol < (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3198 "nd-flow-expr.cpp"
    break;

  case 144: /* expr_ip_dscp: FLOW_IP_DSCP CMP_EQUAL VALUE_UNSIGNED  */
#line 462 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3207 "nd-flow-expr.cpp"
    break;

  case 145: /* expr_ip_dscp: FLOW_IP_DSCP CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 466 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3216 "nd-flow-expr.cpp"
    break;

  case 146: /* expr_ip_version: FLOW_IP_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 473 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3225 "nd-flow-expr.cpp"
    break;

  case 147: /* expr_ip_version: FLOW_IP_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 477 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3234 "nd-flow-expr.cpp"
    break;

  case 148: /* expr_vlan_id: FLOW_VLAN_ID  */
#line 484 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0));
        _NDFP_debugf("VLAN ID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3243 "nd-flow-expr.cpp"
    break;

  case 149: /* expr_vlan_id: '!' FLOW_VLAN_ID  */
#line 488 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0));
        _NDFP_debugf("VLAN ID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3252 "nd-flow-expr.cpp"
    break;

  case 150: /* expr_vlan_id: FLOW_VLAN_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 492 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3261 "nd-flow-expr.cpp"
    break;

  case 151: /* expr_vlan_id: FLOW_VLAN_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 496 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3270 "nd-flow-expr.cpp"
    break;

  case 152: /* expr_vlan_id: FLOW_VLAN_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 500 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3279 "nd-flow-expr.cpp"
    break;

  case 153: /* expr_vlan_id: FLOW_VLAN_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 504 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3288 "nd-flow-expr.cpp"
    break;

  case 154: /* expr_vlan_id: FLOW_VLAN_ID '>' VALUE_UNSIGNED  */
#line 508 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3297 "nd-flow-expr.cpp"
    break;

  case 155: /* expr_vlan_id: FLOW_VLAN_ID '<' VALUE_UNSIGNED  */
#line 512 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3306 "nd-flow-expr.cpp"
    break;

  case 156: /* expr_other_type: FLOW_OTHER_TYPE  */
#line 519 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3317 "nd-flow-expr.cpp"
    break;

  case 157: /* expr_other_type: '!' FLOW_OTHER_TYPE  */
#line 525 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3328 "nd-flow-expr.cpp"
    break;

  case 158: /* expr_other_type: FLOW_OTHER_TYPE CMP_EQUAL value_other_type  */
#line 531 "nd-flow-expr.ypp"
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
#line 3377 "nd-flow-expr.cpp"
    break;

  case 159: /* expr_other_type: FLOW_OTHER_TYPE CMP_NOTEQUAL value_other_type  */
#line 575 "nd-flow-expr.ypp"
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
#line 3426 "nd-flow-expr.cpp"
    break;

  case 160: /* value_other_type: FLOW_OTHER_UNKNOWN  */
#line 622 "nd-flow-expr.ypp"
                         { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3432 "nd-flow-expr.cpp"
    break;

  case 161: /* value_other_type: FLOW_OTHER_UNSUPPORTED  */
#line 623 "nd-flow-expr.ypp"
                             { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3438 "nd-flow-expr.cpp"
    break;

  case 162: /* value_other_type: FLOW_OTHER_LOCAL  */
#line 624 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3444 "nd-flow-expr.cpp"
    break;

  case 163: /* value_other_type: FLOW_OTHER_MULTICAST  */
#line 625 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3450 "nd-flow-expr.cpp"
    break;

  case 164: /* value_other_type: FLOW_OTHER_BROADCAST  */
#line 626 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3456 "nd-flow-expr.cpp"
    break;

  case 165: /* value_other_type: FLOW_OTHER_REMOTE  */
#line 627 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3462 "nd-flow-expr.cpp"
    break;

  case 166: /* value_other_type: FLOW_OTHER_ERROR  */
#line 628 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3468 "nd-flow-expr.cpp"
    break;

  case 167: /* expr_any_mac: FLOW_ANY_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 632 "nd-flow-expr.ypp"
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
#line 3488 "nd-flow-expr.cpp"
    break;

  case 168: /* expr_any_mac: FLOW_ANY_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 647 "nd-flow-expr.ypp"
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
#line 3506 "nd-flow-expr.cpp"
    break;

  case 169: /* expr_any_mac: FLOW_ANY_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 660 "nd-flow-expr.ypp"
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
#line 3527 "nd-flow-expr.cpp"
    break;

  case 170: /* expr_any_mac: FLOW_ANY_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 676 "nd-flow-expr.ypp"
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
#line 3547 "nd-flow-expr.cpp"
    break;

  case 171: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 694 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3559 "nd-flow-expr.cpp"
    break;

  case 172: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 701 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3570 "nd-flow-expr.cpp"
    break;

  case 173: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 707 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == true));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3584 "nd-flow-expr.cpp"
    break;

  case 174: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 716 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == false));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3598 "nd-flow-expr.cpp"
    break;

  case 175: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 728 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3610 "nd-flow-expr.cpp"
    break;

  case 176: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 735 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3622 "nd-flow-expr.cpp"
    break;

  case 177: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 742 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == true));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3636 "nd-flow-expr.cpp"
    break;

  case 178: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 751 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == false));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3650 "nd-flow-expr.cpp"
    break;

  case 179: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 763 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3662 "nd-flow-expr.cpp"
    break;

  case 180: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 770 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3674 "nd-flow-expr.cpp"
    break;

  case 181: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 777 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == true));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3688 "nd-flow-expr.cpp"
    break;

  case 182: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 786 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == false));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3702 "nd-flow-expr.cpp"
    break;

  case 183: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 798 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3714 "nd-flow-expr.cpp"
    break;

  case 184: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 805 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3726 "nd-flow-expr.cpp"
    break;

  case 185: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 812 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == true));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3740 "nd-flow-expr.cpp"
    break;

  case 186: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 821 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == false));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3754 "nd-flow-expr.cpp"
    break;

  case 187: /* expr_any_ip: FLOW_ANY_IP CMP_EQUAL value_addr_ip  */
#line 833 "nd-flow-expr.ypp"
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
#line 3772 "nd-flow-expr.cpp"
    break;

  case 188: /* expr_any_ip: FLOW_ANY_IP CMP_NOTEQUAL value_addr_ip  */
#line 846 "nd-flow-expr.ypp"
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
#line 3790 "nd-flow-expr.cpp"
    break;

  case 189: /* expr_any_ip: FLOW_ANY_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 859 "nd-flow-expr.ypp"
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
#line 3810 "nd-flow-expr.cpp"
    break;

  case 190: /* expr_any_ip: FLOW_ANY_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 874 "nd-flow-expr.ypp"
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
#line 3830 "nd-flow-expr.cpp"
    break;

  case 191: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL value_addr_ip  */
#line 892 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3841 "nd-flow-expr.cpp"
    break;

  case 192: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL value_addr_ip  */
#line 898 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3852 "nd-flow-expr.cpp"
    break;

  case 193: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 904 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == true));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3866 "nd-flow-expr.cpp"
    break;

  case 194: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 913 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == false));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3880 "nd-flow-expr.cpp"
    break;

  case 195: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL value_addr_ip  */
#line 925 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3891 "nd-flow-expr.cpp"
    break;

  case 196: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL value_addr_ip  */
#line 931 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3902 "nd-flow-expr.cpp"
    break;

  case 197: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 937 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == true));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3916 "nd-flow-expr.cpp"
    break;

  case 198: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 946 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == false));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3930 "nd-flow-expr.cpp"
    break;

  case 199: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL value_addr_ip  */
#line 958 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Source IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3942 "nd-flow-expr.cpp"
    break;

  case 200: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL value_addr_ip  */
#line 965 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Source IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3954 "nd-flow-expr.cpp"
    break;

  case 201: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 972 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == true));
        _NDFP_debugf("Source IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3969 "nd-flow-expr.cpp"
    break;

  case 202: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 982 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == false));
        _NDFP_debugf("Source IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3984 "nd-flow-expr.cpp"
    break;

  case 203: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL value_addr_ip  */
#line 995 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Destination IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3996 "nd-flow-expr.cpp"
    break;

  case 204: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL value_addr_ip  */
#line 1002 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Destination IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4008 "nd-flow-expr.cpp"
    break;

  case 205: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1009 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == true));
        _NDFP_debugf("Destination IP == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4023 "nd-flow-expr.cpp"
    break;

  case 206: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1019 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == false));
        _NDFP_debugf("Destination IP != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4038 "nd-flow-expr.cpp"
    break;

  case 207: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_EQUAL value_addr_ip  */
#line 1032 "nd-flow-expr.ypp"
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
#line 4054 "nd-flow-expr.cpp"
    break;

  case 208: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_NOTEQUAL value_addr_ip  */
#line 1043 "nd-flow-expr.ypp"
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
#line 4070 "nd-flow-expr.cpp"
    break;

  case 209: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1054 "nd-flow-expr.ypp"
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
#line 4089 "nd-flow-expr.cpp"
    break;

  case 210: /* expr_conntrack_reply_src_ip: FLOW_CONNTRACK_REPLY_SRC_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1068 "nd-flow-expr.ypp"
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
#line 4108 "nd-flow-expr.cpp"
    break;

  case 211: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_EQUAL value_addr_ip  */
#line 1085 "nd-flow-expr.ypp"
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
#line 4124 "nd-flow-expr.cpp"
    break;

  case 212: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_NOTEQUAL value_addr_ip  */
#line 1096 "nd-flow-expr.ypp"
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
#line 4140 "nd-flow-expr.cpp"
    break;

  case 213: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 1107 "nd-flow-expr.ypp"
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
#line 4159 "nd-flow-expr.cpp"
    break;

  case 214: /* expr_conntrack_reply_dst_ip: FLOW_CONNTRACK_REPLY_DST_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 1121 "nd-flow-expr.ypp"
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
#line 4178 "nd-flow-expr.cpp"
    break;

  case 215: /* value_addr_ip: VALUE_ADDR_IPV4  */
#line 1138 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4184 "nd-flow-expr.cpp"
    break;

  case 216: /* value_addr_ip: VALUE_ADDR_IPV4_CIDR  */
#line 1139 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4190 "nd-flow-expr.cpp"
    break;

  case 217: /* value_addr_ip: VALUE_ADDR_IPV6  */
#line 1140 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4196 "nd-flow-expr.cpp"
    break;

  case 218: /* value_addr_ip: VALUE_ADDR_IPV6_CIDR  */
#line 1141 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 4202 "nd-flow-expr.cpp"
    break;

  case 219: /* expr_any_port: FLOW_ANY_PORT  */
#line 1145 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Any port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4213 "nd-flow-expr.cpp"
    break;

  case 220: /* expr_any_port: '!' FLOW_ANY_PORT  */
#line 1151 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Any port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4224 "nd-flow-expr.cpp"
    break;

  case 221: /* expr_any_port: FLOW_ANY_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1157 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4235 "nd-flow-expr.cpp"
    break;

  case 222: /* expr_any_port: FLOW_ANY_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1163 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4246 "nd-flow-expr.cpp"
    break;

  case 223: /* expr_any_port: FLOW_ANY_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1169 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4257 "nd-flow-expr.cpp"
    break;

  case 224: /* expr_any_port: FLOW_ANY_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1175 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4268 "nd-flow-expr.cpp"
    break;

  case 225: /* expr_any_port: FLOW_ANY_PORT '>' VALUE_UNSIGNED  */
#line 1181 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4279 "nd-flow-expr.cpp"
    break;

  case 226: /* expr_any_port: FLOW_ANY_PORT '<' VALUE_UNSIGNED  */
#line 1187 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4290 "nd-flow-expr.cpp"
    break;

  case 227: /* expr_local_port: FLOW_LOCAL_PORT  */
#line 1196 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        _NDFP_debugf("Local port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4299 "nd-flow-expr.cpp"
    break;

  case 228: /* expr_local_port: '!' FLOW_LOCAL_PORT  */
#line 1200 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        _NDFP_debugf("Local port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4308 "nd-flow-expr.cpp"
    break;

  case 229: /* expr_local_port: FLOW_LOCAL_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1204 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4317 "nd-flow-expr.cpp"
    break;

  case 230: /* expr_local_port: FLOW_LOCAL_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1208 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4326 "nd-flow-expr.cpp"
    break;

  case 231: /* expr_local_port: FLOW_LOCAL_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1212 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4335 "nd-flow-expr.cpp"
    break;

  case 232: /* expr_local_port: FLOW_LOCAL_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1216 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4344 "nd-flow-expr.cpp"
    break;

  case 233: /* expr_local_port: FLOW_LOCAL_PORT '>' VALUE_UNSIGNED  */
#line 1220 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4353 "nd-flow-expr.cpp"
    break;

  case 234: /* expr_local_port: FLOW_LOCAL_PORT '<' VALUE_UNSIGNED  */
#line 1224 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4362 "nd-flow-expr.cpp"
    break;

  case 235: /* expr_other_port: FLOW_OTHER_PORT  */
#line 1231 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Other port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4371 "nd-flow-expr.cpp"
    break;

  case 236: /* expr_other_port: '!' FLOW_OTHER_PORT  */
#line 1235 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Other port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4380 "nd-flow-expr.cpp"
    break;

  case 237: /* expr_other_port: FLOW_OTHER_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1239 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4389 "nd-flow-expr.cpp"
    break;

  case 238: /* expr_other_port: FLOW_OTHER_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1243 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4398 "nd-flow-expr.cpp"
    break;

  case 239: /* expr_other_port: FLOW_OTHER_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1247 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4407 "nd-flow-expr.cpp"
    break;

  case 240: /* expr_other_port: FLOW_OTHER_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1251 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4416 "nd-flow-expr.cpp"
    break;

  case 241: /* expr_other_port: FLOW_OTHER_PORT '>' VALUE_UNSIGNED  */
#line 1255 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4425 "nd-flow-expr.cpp"
    break;

  case 242: /* expr_other_port: FLOW_OTHER_PORT '<' VALUE_UNSIGNED  */
#line 1259 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4434 "nd-flow-expr.cpp"
    break;

  case 243: /* expr_src_port: FLOW_SRC_PORT  */
#line 1266 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != 0));
        _NDFP_debugf("Source port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4443 "nd-flow-expr.cpp"
    break;

  case 244: /* expr_src_port: '!' FLOW_SRC_PORT  */
#line 1270 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == 0));
        _NDFP_debugf("Source port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4452 "nd-flow-expr.cpp"
    break;

  case 245: /* expr_src_port: FLOW_SRC_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1274 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4461 "nd-flow-expr.cpp"
    break;

  case 246: /* expr_src_port: FLOW_SRC_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1278 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4470 "nd-flow-expr.cpp"
    break;

  case 247: /* expr_src_port: FLOW_SRC_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1282 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4479 "nd-flow-expr.cpp"
    break;

  case 248: /* expr_src_port: FLOW_SRC_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1286 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4488 "nd-flow-expr.cpp"
    break;

  case 249: /* expr_src_port: FLOW_SRC_PORT '>' VALUE_UNSIGNED  */
#line 1290 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4497 "nd-flow-expr.cpp"
    break;

  case 250: /* expr_src_port: FLOW_SRC_PORT '<' VALUE_UNSIGNED  */
#line 1294 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4506 "nd-flow-expr.cpp"
    break;

  case 251: /* expr_dst_port: FLOW_DST_PORT  */
#line 1301 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != 0));
        _NDFP_debugf("Destination port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4515 "nd-flow-expr.cpp"
    break;

  case 252: /* expr_dst_port: '!' FLOW_DST_PORT  */
#line 1305 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == 0));
        _NDFP_debugf("Destination port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4524 "nd-flow-expr.cpp"
    break;

  case 253: /* expr_dst_port: FLOW_DST_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1309 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4533 "nd-flow-expr.cpp"
    break;

  case 254: /* expr_dst_port: FLOW_DST_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1313 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4542 "nd-flow-expr.cpp"
    break;

  case 255: /* expr_dst_port: FLOW_DST_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1317 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4551 "nd-flow-expr.cpp"
    break;

  case 256: /* expr_dst_port: FLOW_DST_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1321 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4560 "nd-flow-expr.cpp"
    break;

  case 257: /* expr_dst_port: FLOW_DST_PORT '>' VALUE_UNSIGNED  */
#line 1325 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4569 "nd-flow-expr.cpp"
    break;

  case 258: /* expr_dst_port: FLOW_DST_PORT '<' VALUE_UNSIGNED  */
#line 1329 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4578 "nd-flow-expr.cpp"
    break;

  case 259: /* expr_tunnel_type: FLOW_TUNNEL_TYPE  */
#line 1336 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4589 "nd-flow-expr.cpp"
    break;

  case 260: /* expr_tunnel_type: '!' FLOW_TUNNEL_TYPE  */
#line 1342 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type is none? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4600 "nd-flow-expr.cpp"
    break;

  case 261: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_EQUAL value_tunnel_type  */
#line 1348 "nd-flow-expr.ypp"
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
#line 4624 "nd-flow-expr.cpp"
    break;

  case 262: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_NOTEQUAL value_tunnel_type  */
#line 1367 "nd-flow-expr.ypp"
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
#line 4648 "nd-flow-expr.cpp"
    break;

  case 263: /* value_tunnel_type: FLOW_TUNNEL_NONE  */
#line 1389 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 4654 "nd-flow-expr.cpp"
    break;

  case 264: /* value_tunnel_type: FLOW_TUNNEL_GTP  */
#line 1390 "nd-flow-expr.ypp"
                      { (yyval.us_number) = (yyvsp[0].us_number); }
#line 4660 "nd-flow-expr.cpp"
    break;

  case 265: /* expr_detection_complete: FLOW_DETECTION_COMPLETE  */
#line 1393 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf("Detection was complete? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4669 "nd-flow-expr.cpp"
    break;

  case 266: /* expr_detection_complete: '!' FLOW_DETECTION_COMPLETE  */
#line 1397 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf(
            "Detection was not complete? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4680 "nd-flow-expr.cpp"
    break;

  case 267: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_TRUE  */
#line 1403 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == true
        ));
        _NDFP_debugf(
            "Detection complete == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4693 "nd-flow-expr.cpp"
    break;

  case 268: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_FALSE  */
#line 1411 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == false
        ));
        _NDFP_debugf(
            "Detection complete == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4706 "nd-flow-expr.cpp"
    break;

  case 269: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_TRUE  */
#line 1419 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != true
        ));
        _NDFP_debugf(
            "Detection complete != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4719 "nd-flow-expr.cpp"
    break;

  case 270: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_FALSE  */
#line 1427 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != false
        ));
        _NDFP_debugf(
            "Detection complete != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4732 "nd-flow-expr.cpp"
    break;

  case 271: /* expr_detection_guessed: FLOW_DETECTION_GUESSED  */
#line 1438 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf("Detection was guessed? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4741 "nd-flow-expr.cpp"
    break;

  case 272: /* expr_detection_guessed: '!' FLOW_DETECTION_GUESSED  */
#line 1442 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf(
            "Detection was not guessed? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4752 "nd-flow-expr.cpp"
    break;

  case 273: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_TRUE  */
#line 1448 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == true
        ));
        _NDFP_debugf(
            "Detection guessed == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4765 "nd-flow-expr.cpp"
    break;

  case 274: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_FALSE  */
#line 1456 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == false
        ));
        _NDFP_debugf(
            "Detection guessed == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4778 "nd-flow-expr.cpp"
    break;

  case 275: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_TRUE  */
#line 1464 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != true
        ));
        _NDFP_debugf(
            "Detection guessed != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4791 "nd-flow-expr.cpp"
    break;

  case 276: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_FALSE  */
#line 1472 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != false
        ));
        _NDFP_debugf(
            "Detection guessed != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4804 "nd-flow-expr.cpp"
    break;

  case 277: /* expr_detection_init: FLOW_DETECTION_INIT  */
#line 1483 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf("Detection was init? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4813 "nd-flow-expr.cpp"
    break;

  case 278: /* expr_detection_init: '!' FLOW_DETECTION_INIT  */
#line 1487 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf(
            "Detection was not init? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4824 "nd-flow-expr.cpp"
    break;

  case 279: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_TRUE  */
#line 1493 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == true
        ));
        _NDFP_debugf(
            "Detection init == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4837 "nd-flow-expr.cpp"
    break;

  case 280: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_FALSE  */
#line 1501 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == false
        ));
        _NDFP_debugf(
            "Detection init == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4850 "nd-flow-expr.cpp"
    break;

  case 281: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1509 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != true
        ));
        _NDFP_debugf(
            "Detection init != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4863 "nd-flow-expr.cpp"
    break;

  case 282: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1517 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != false
        ));
        _NDFP_debugf(
            "Detection init != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4876 "nd-flow-expr.cpp"
    break;

  case 283: /* expr_detection_updated: FLOW_DETECTION_UPDATED  */
#line 1528 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf("Detection was updated? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4885 "nd-flow-expr.cpp"
    break;

  case 284: /* expr_detection_updated: '!' FLOW_DETECTION_UPDATED  */
#line 1532 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf(
            "Detection was not updated? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4896 "nd-flow-expr.cpp"
    break;

  case 285: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_TRUE  */
#line 1538 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == true
        ));
        _NDFP_debugf(
            "Detection updated == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4909 "nd-flow-expr.cpp"
    break;

  case 286: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_FALSE  */
#line 1546 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == false
        ));
        _NDFP_debugf(
            "Detection updated == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4922 "nd-flow-expr.cpp"
    break;

  case 287: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_TRUE  */
#line 1554 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != true
        ));
        _NDFP_debugf(
            "Detection updated != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4935 "nd-flow-expr.cpp"
    break;

  case 288: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_FALSE  */
#line 1562 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != false
        ));
        _NDFP_debugf(
            "Detection updated != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4948 "nd-flow-expr.cpp"
    break;

  case 289: /* expr_dhc_hit: FLOW_DHC_HIT  */
#line 1573 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf("DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4957 "nd-flow-expr.cpp"
    break;

  case 290: /* expr_dhc_hit: '!' FLOW_DHC_HIT  */
#line 1577 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf(
            "DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4968 "nd-flow-expr.cpp"
    break;

  case 291: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1583 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == true
        ));
        _NDFP_debugf(
            "DHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4981 "nd-flow-expr.cpp"
    break;

  case 292: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1591 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == false
        ));
        _NDFP_debugf(
            "DHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4994 "nd-flow-expr.cpp"
    break;

  case 293: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1599 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != true
        ));
        _NDFP_debugf(
            "DHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5007 "nd-flow-expr.cpp"
    break;

  case 294: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1607 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != false
        ));
        _NDFP_debugf(
            "DHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5020 "nd-flow-expr.cpp"
    break;

  case 295: /* expr_fhc_hit: FLOW_FHC_HIT  */
#line 1618 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf("FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5029 "nd-flow-expr.cpp"
    break;

  case 296: /* expr_fhc_hit: '!' FLOW_FHC_HIT  */
#line 1622 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf(
            "FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5040 "nd-flow-expr.cpp"
    break;

  case 297: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1628 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == true
        ));
        _NDFP_debugf(
            "FHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5053 "nd-flow-expr.cpp"
    break;

  case 298: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1636 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == false
        ));
        _NDFP_debugf(
            "FHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5066 "nd-flow-expr.cpp"
    break;

  case 299: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1644 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != true
        ));
        _NDFP_debugf(
            "FHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5079 "nd-flow-expr.cpp"
    break;

  case 300: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1652 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != false
        ));
        _NDFP_debugf(
            "FHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5092 "nd-flow-expr.cpp"
    break;

  case 301: /* expr_ip_nat: FLOW_IP_NAT  */
#line 1663 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5101 "nd-flow-expr.cpp"
    break;

  case 302: /* expr_ip_nat: '!' FLOW_IP_NAT  */
#line 1667 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5110 "nd-flow-expr.cpp"
    break;

  case 303: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_TRUE  */
#line 1671 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5119 "nd-flow-expr.cpp"
    break;

  case 304: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_FALSE  */
#line 1675 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5128 "nd-flow-expr.cpp"
    break;

  case 305: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_TRUE  */
#line 1679 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != true));
        _NDFP_debugf("IP NAT != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5137 "nd-flow-expr.cpp"
    break;

  case 306: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_FALSE  */
#line 1683 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != false));
        _NDFP_debugf("IP NAT != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5146 "nd-flow-expr.cpp"
    break;

  case 307: /* expr_expiring: FLOW_EXPIRING  */
#line 1690 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5155 "nd-flow-expr.cpp"
    break;

  case 308: /* expr_expiring: '!' FLOW_EXPIRING  */
#line 1694 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5164 "nd-flow-expr.cpp"
    break;

  case 309: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_TRUE  */
#line 1698 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5173 "nd-flow-expr.cpp"
    break;

  case 310: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_FALSE  */
#line 1702 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5182 "nd-flow-expr.cpp"
    break;

  case 311: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_TRUE  */
#line 1706 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != true));
        _NDFP_debugf("Flow expiring != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5191 "nd-flow-expr.cpp"
    break;

  case 312: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_FALSE  */
#line 1710 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != false));
        _NDFP_debugf("Flow expiring != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5200 "nd-flow-expr.cpp"
    break;

  case 313: /* expr_expired: FLOW_EXPIRED  */
#line 1717 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5209 "nd-flow-expr.cpp"
    break;

  case 314: /* expr_expired: '!' FLOW_EXPIRED  */
#line 1721 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5218 "nd-flow-expr.cpp"
    break;

  case 315: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_TRUE  */
#line 1725 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5227 "nd-flow-expr.cpp"
    break;

  case 316: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_FALSE  */
#line 1729 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5236 "nd-flow-expr.cpp"
    break;

  case 317: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_TRUE  */
#line 1733 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != true));
        _NDFP_debugf("Flow expired != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5245 "nd-flow-expr.cpp"
    break;

  case 318: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_FALSE  */
#line 1737 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != false));
        _NDFP_debugf("Flow expired != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5254 "nd-flow-expr.cpp"
    break;

  case 319: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR  */
#line 1744 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5263 "nd-flow-expr.cpp"
    break;

  case 320: /* expr_soft_dissector: '!' FLOW_SOFT_DISSECTOR  */
#line 1748 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5272 "nd-flow-expr.cpp"
    break;

  case 321: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_TRUE  */
#line 1752 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5281 "nd-flow-expr.cpp"
    break;

  case 322: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_FALSE  */
#line 1756 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5290 "nd-flow-expr.cpp"
    break;

  case 323: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_TRUE  */
#line 1760 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != true));
        _NDFP_debugf("Soft dissector matched != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5299 "nd-flow-expr.cpp"
    break;

  case 324: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_FALSE  */
#line 1764 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != false));
        _NDFP_debugf("Soft dissector matched != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5308 "nd-flow-expr.cpp"
    break;

  case 325: /* expr_app: FLOW_APPLICATION  */
#line 1771 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) != 0
        ));
        _NDFP_debugf("Application detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5319 "nd-flow-expr.cpp"
    break;

  case 326: /* expr_app: '!' FLOW_APPLICATION  */
#line 1777 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) == 0
        ));
        _NDFP_debugf(
            "Application not detected? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5332 "nd-flow-expr.cpp"
    break;

  case 329: /* expr_app_id: FLOW_APPLICATION CMP_EQUAL VALUE_UNSIGNED  */
#line 1789 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf(
            "Application ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5346 "nd-flow-expr.cpp"
    break;

  case 330: /* expr_app_id: FLOW_APPLICATION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1798 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = false);

        _NDFP_debugf(
            "Application ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 5360 "nd-flow-expr.cpp"
    break;

  case 331: /* expr_app_name: FLOW_APPLICATION CMP_EQUAL VALUE_NAME  */
#line 1810 "nd-flow-expr.ypp"
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
#line 5392 "nd-flow-expr.cpp"
    break;

  case 332: /* expr_app_name: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NAME  */
#line 1837 "nd-flow-expr.ypp"
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
#line 5424 "nd-flow-expr.cpp"
    break;

  case 333: /* expr_category: FLOW_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1867 "nd-flow-expr.ypp"
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
#line 5497 "nd-flow-expr.cpp"
    break;

  case 334: /* expr_category: FLOW_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1935 "nd-flow-expr.ypp"
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
#line 5566 "nd-flow-expr.cpp"
    break;

  case 335: /* expr_category_id: FLOW_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2002 "nd-flow-expr.ypp"
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
#line 5624 "nd-flow-expr.cpp"
    break;

  case 336: /* expr_category_id: FLOW_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2055 "nd-flow-expr.ypp"
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
#line 5682 "nd-flow-expr.cpp"
    break;

  case 337: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2111 "nd-flow-expr.ypp"
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
#line 5704 "nd-flow-expr.cpp"
    break;

  case 338: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2128 "nd-flow-expr.ypp"
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
#line 5726 "nd-flow-expr.cpp"
    break;

  case 339: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2148 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5738 "nd-flow-expr.cpp"
    break;

  case 340: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2155 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category != %s? %s\n",
        (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5751 "nd-flow-expr.cpp"
    break;

  case 341: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2166 "nd-flow-expr.ypp"
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
#line 5773 "nd-flow-expr.cpp"
    break;

  case 342: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2183 "nd-flow-expr.ypp"
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
#line 5795 "nd-flow-expr.cpp"
    break;

  case 343: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2203 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5807 "nd-flow-expr.cpp"
    break;

  case 344: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2210 "nd-flow-expr.ypp"
                                                          {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5820 "nd-flow-expr.cpp"
    break;

  case 345: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2221 "nd-flow-expr.ypp"
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
#line 5851 "nd-flow-expr.cpp"
    break;

  case 346: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2247 "nd-flow-expr.ypp"
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
#line 5882 "nd-flow-expr.cpp"
    break;

  case 347: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2276 "nd-flow-expr.ypp"
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
#line 5901 "nd-flow-expr.cpp"
    break;

  case 348: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2290 "nd-flow-expr.ypp"
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
#line 5920 "nd-flow-expr.cpp"
    break;

  case 349: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2307 "nd-flow-expr.ypp"
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
#line 5942 "nd-flow-expr.cpp"
    break;

  case 350: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2324 "nd-flow-expr.ypp"
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
#line 5964 "nd-flow-expr.cpp"
    break;

  case 351: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2344 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5977 "nd-flow-expr.cpp"
    break;

  case 352: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2352 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5990 "nd-flow-expr.cpp"
    break;

  case 353: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2363 "nd-flow-expr.ypp"
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
#line 6012 "nd-flow-expr.cpp"
    break;

  case 354: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2380 "nd-flow-expr.ypp"
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
#line 6034 "nd-flow-expr.cpp"
    break;

  case 355: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2400 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6047 "nd-flow-expr.cpp"
    break;

  case 356: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2408 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6060 "nd-flow-expr.cpp"
    break;

  case 357: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2419 "nd-flow-expr.ypp"
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
#line 6082 "nd-flow-expr.cpp"
    break;

  case 358: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2436 "nd-flow-expr.ypp"
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
#line 6104 "nd-flow-expr.cpp"
    break;

  case 359: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2456 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6117 "nd-flow-expr.cpp"
    break;

  case 360: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2464 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6130 "nd-flow-expr.cpp"
    break;

  case 361: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2475 "nd-flow-expr.ypp"
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
#line 6152 "nd-flow-expr.cpp"
    break;

  case 362: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2492 "nd-flow-expr.ypp"
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
#line 6174 "nd-flow-expr.cpp"
    break;

  case 363: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2512 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6187 "nd-flow-expr.cpp"
    break;

  case 364: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2520 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6200 "nd-flow-expr.cpp"
    break;

  case 365: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2531 "nd-flow-expr.ypp"
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
#line 6222 "nd-flow-expr.cpp"
    break;

  case 366: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2548 "nd-flow-expr.ypp"
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
#line 6244 "nd-flow-expr.cpp"
    break;

  case 367: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2568 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6257 "nd-flow-expr.cpp"
    break;

  case 368: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2576 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6270 "nd-flow-expr.cpp"
    break;

  case 369: /* expr_proto: FLOW_PROTOCOL  */
#line 2587 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol != ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6281 "nd-flow-expr.cpp"
    break;

  case 370: /* expr_proto: '!' FLOW_PROTOCOL  */
#line 2593 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol == ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6292 "nd-flow-expr.cpp"
    break;

  case 373: /* expr_proto_id: FLOW_PROTOCOL CMP_EQUAL VALUE_UNSIGNED  */
#line 2603 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6303 "nd-flow-expr.cpp"
    break;

  case 374: /* expr_proto_id: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2609 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6314 "nd-flow-expr.cpp"
    break;

  case 375: /* expr_proto_name: FLOW_PROTOCOL CMP_EQUAL VALUE_NAME  */
#line 2618 "nd-flow-expr.ypp"
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
#line 6339 "nd-flow-expr.cpp"
    break;

  case 376: /* expr_proto_name: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NAME  */
#line 2638 "nd-flow-expr.ypp"
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
#line 6363 "nd-flow-expr.cpp"
    break;

  case 377: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2660 "nd-flow-expr.ypp"
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
#line 6385 "nd-flow-expr.cpp"
    break;

  case 378: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2677 "nd-flow-expr.ypp"
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
#line 6407 "nd-flow-expr.cpp"
    break;

  case 379: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2697 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6420 "nd-flow-expr.cpp"
    break;

  case 380: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2705 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6433 "nd-flow-expr.cpp"
    break;

  case 381: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME  */
#line 2716 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == false
        ));
        _NDFP_debugf("Detected hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6445 "nd-flow-expr.cpp"
    break;

  case 382: /* expr_detected_hostname: '!' FLOW_DETECTED_HOSTNAME  */
#line 2723 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == true
        ));
        _NDFP_debugf("Detected hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6457 "nd-flow-expr.cpp"
    break;

  case 383: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 2730 "nd-flow-expr.ypp"
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
#line 6481 "nd-flow-expr.cpp"
    break;

  case 384: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 2749 "nd-flow-expr.ypp"
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
#line 6505 "nd-flow-expr.cpp"
    break;

  case 385: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 2768 "nd-flow-expr.ypp"
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
#line 6524 "nd-flow-expr.cpp"
    break;

  case 386: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 2782 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Detected hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6535 "nd-flow-expr.cpp"
    break;

  case 387: /* expr_dns_hostname: FLOW_DNS_HOSTNAME  */
#line 2791 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == false
        ));
        _NDFP_debugf("DNS hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6547 "nd-flow-expr.cpp"
    break;

  case 388: /* expr_dns_hostname: '!' FLOW_DNS_HOSTNAME  */
#line 2798 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == true
        ));
        _NDFP_debugf("DNS hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6559 "nd-flow-expr.cpp"
    break;

  case 389: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 2805 "nd-flow-expr.ypp"
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
#line 6583 "nd-flow-expr.cpp"
    break;

  case 390: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 2824 "nd-flow-expr.ypp"
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
#line 6607 "nd-flow-expr.cpp"
    break;

  case 391: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 2843 "nd-flow-expr.ypp"
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
#line 6626 "nd-flow-expr.cpp"
    break;

  case 392: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 2857 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("DNS hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6637 "nd-flow-expr.cpp"
    break;

  case 393: /* expr_risks: FLOW_RISKS  */
#line 2866 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() != 0));
        _NDFP_debugf("Risks detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6646 "nd-flow-expr.cpp"
    break;

  case 394: /* expr_risks: '!' FLOW_RISKS  */
#line 2870 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() == 0));
        _NDFP_debugf("Risks not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6655 "nd-flow-expr.cpp"
    break;

  case 395: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NAME  */
#line 2874 "nd-flow-expr.ypp"
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
#line 6679 "nd-flow-expr.cpp"
    break;

  case 396: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NAME  */
#line 2893 "nd-flow-expr.ypp"
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
#line 6703 "nd-flow-expr.cpp"
    break;

  case 397: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_UNSIGNED  */
#line 2912 "nd-flow-expr.ypp"
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
#line 6721 "nd-flow-expr.cpp"
    break;

  case 398: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2925 "nd-flow-expr.ypp"
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
#line 6739 "nd-flow-expr.cpp"
    break;

  case 399: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE  */
#line 2941 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != 0));
        _NDFP_debugf("nDPI risk score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6748 "nd-flow-expr.cpp"
    break;

  case 400: /* expr_risk_ndpi_score: '!' FLOW_NDPI_RISK_SCORE  */
#line 2945 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == 0));
        _NDFP_debugf("nDPI risk score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6757 "nd-flow-expr.cpp"
    break;

  case 401: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_EQUAL VALUE_UNSIGNED  */
#line 2949 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6766 "nd-flow-expr.cpp"
    break;

  case 402: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2953 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6775 "nd-flow-expr.cpp"
    break;

  case 403: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2957 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6784 "nd-flow-expr.cpp"
    break;

  case 404: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2961 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6793 "nd-flow-expr.cpp"
    break;

  case 405: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '>' VALUE_UNSIGNED  */
#line 2965 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6802 "nd-flow-expr.cpp"
    break;

  case 406: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '<' VALUE_UNSIGNED  */
#line 2969 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6811 "nd-flow-expr.cpp"
    break;

  case 407: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 2976 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != 0));
        _NDFP_debugf("nDPI risk client score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6820 "nd-flow-expr.cpp"
    break;

  case 408: /* expr_risk_ndpi_score_client: '!' FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 2980 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == 0));
        _NDFP_debugf("nDPI risk client score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6829 "nd-flow-expr.cpp"
    break;

  case 409: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_EQUAL VALUE_UNSIGNED  */
#line 2984 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6838 "nd-flow-expr.cpp"
    break;

  case 410: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2988 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6847 "nd-flow-expr.cpp"
    break;

  case 411: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2992 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6856 "nd-flow-expr.cpp"
    break;

  case 412: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2996 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6865 "nd-flow-expr.cpp"
    break;

  case 413: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '>' VALUE_UNSIGNED  */
#line 3000 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6874 "nd-flow-expr.cpp"
    break;

  case 414: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '<' VALUE_UNSIGNED  */
#line 3004 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6883 "nd-flow-expr.cpp"
    break;

  case 415: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER  */
#line 3011 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != 0));
        _NDFP_debugf("nDPI risk server score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6892 "nd-flow-expr.cpp"
    break;

  case 416: /* expr_risk_ndpi_score_server: '!' FLOW_NDPI_RISK_SCORE_SERVER  */
#line 3015 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == 0));
        _NDFP_debugf("nDPI risk server score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6901 "nd-flow-expr.cpp"
    break;

  case 417: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_EQUAL VALUE_UNSIGNED  */
#line 3019 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6910 "nd-flow-expr.cpp"
    break;

  case 418: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3023 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6919 "nd-flow-expr.cpp"
    break;

  case 419: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3027 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6928 "nd-flow-expr.cpp"
    break;

  case 420: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3031 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6937 "nd-flow-expr.cpp"
    break;

  case 421: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '>' VALUE_UNSIGNED  */
#line 3035 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6946 "nd-flow-expr.cpp"
    break;

  case 422: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '<' VALUE_UNSIGNED  */
#line 3039 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6955 "nd-flow-expr.cpp"
    break;

  case 423: /* expr_conntrack_id: FLOW_CONNTRACK_ID  */
#line 3046 "nd-flow-expr.ypp"
                        {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id != 0));
        _NDFP_debugf("CT ID set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6968 "nd-flow-expr.cpp"
    break;

  case 424: /* expr_conntrack_id: '!' FLOW_CONNTRACK_ID  */
#line 3054 "nd-flow-expr.ypp"
                            {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id == 0));
        _NDFP_debugf("CT ID not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6981 "nd-flow-expr.cpp"
    break;

  case 425: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 3062 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id == (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6994 "nd-flow-expr.cpp"
    break;

  case 426: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3070 "nd-flow-expr.ypp"
                                                    {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id != (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7007 "nd-flow-expr.cpp"
    break;

  case 427: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3078 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7020 "nd-flow-expr.cpp"
    break;

  case 428: /* expr_conntrack_id: FLOW_CONNTRACK_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3086 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7033 "nd-flow-expr.cpp"
    break;

  case 429: /* expr_conntrack_id: FLOW_CONNTRACK_ID '>' VALUE_UNSIGNED  */
#line 3094 "nd-flow-expr.ypp"
                                           {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id > (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7046 "nd-flow-expr.cpp"
    break;

  case 430: /* expr_conntrack_id: FLOW_CONNTRACK_ID '<' VALUE_UNSIGNED  */
#line 3102 "nd-flow-expr.ypp"
                                           {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.id < (yyvsp[0].ul_number)));
        _NDFP_debugf("CT ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7059 "nd-flow-expr.cpp"
    break;

  case 431: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK  */
#line 3113 "nd-flow-expr.ypp"
                          {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != 0));
        _NDFP_debugf("CT Mark set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7072 "nd-flow-expr.cpp"
    break;

  case 432: /* expr_conntrack_mark: '!' FLOW_CONNTRACK_MARK  */
#line 3121 "nd-flow-expr.ypp"
                              {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == 0));
        _NDFP_debugf("CT Mark not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7085 "nd-flow-expr.cpp"
    break;

  case 433: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_EQUAL VALUE_UNSIGNED  */
#line 3129 "nd-flow-expr.ypp"
                                                   {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7098 "nd-flow-expr.cpp"
    break;

  case 434: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3137 "nd-flow-expr.ypp"
                                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7111 "nd-flow-expr.cpp"
    break;

  case 435: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3145 "nd-flow-expr.ypp"
                                                        {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark >= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7124 "nd-flow-expr.cpp"
    break;

  case 436: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3153 "nd-flow-expr.ypp"
                                                        {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark <= (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7137 "nd-flow-expr.cpp"
    break;

  case 437: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK '>' VALUE_UNSIGNED  */
#line 3161 "nd-flow-expr.ypp"
                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark > (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7150 "nd-flow-expr.cpp"
    break;

  case 438: /* expr_conntrack_mark: FLOW_CONNTRACK_MARK '<' VALUE_UNSIGNED  */
#line 3169 "nd-flow-expr.ypp"
                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark < (yyvsp[0].ul_number)));
        _NDFP_debugf("CT Mark < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 7163 "nd-flow-expr.cpp"
    break;

  case 439: /* expr_iface: FLOW_IFACE  */
#line 3180 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == false
        ));
        _NDFP_debugf("Interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7175 "nd-flow-expr.cpp"
    break;

  case 440: /* expr_iface: '!' FLOW_IFACE  */
#line 3187 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == true
        ));
        _NDFP_debugf("Interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7187 "nd-flow-expr.cpp"
    break;

  case 441: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_NAME  */
#line 3194 "nd-flow-expr.ypp"
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
#line 7210 "nd-flow-expr.cpp"
    break;

  case 442: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_NAME  */
#line 3212 "nd-flow-expr.ypp"
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
#line 7233 "nd-flow-expr.cpp"
    break;

  case 443: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_REGEX  */
#line 3230 "nd-flow-expr.ypp"
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
#line 7252 "nd-flow-expr.cpp"
    break;

  case 444: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_REGEX  */
#line 3244 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7263 "nd-flow-expr.cpp"
    break;

  case 445: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC  */
#line 3253 "nd-flow-expr.ypp"
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
#line 7279 "nd-flow-expr.cpp"
    break;

  case 446: /* expr_iface_nfq_src: '!' FLOW_IFACE_NFQ_SRC  */
#line 3264 "nd-flow-expr.ypp"
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
#line 7295 "nd-flow-expr.cpp"
    break;

  case 447: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_NAME  */
#line 3275 "nd-flow-expr.ypp"
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
#line 7321 "nd-flow-expr.cpp"
    break;

  case 448: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_NAME  */
#line 3296 "nd-flow-expr.ypp"
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
#line 7345 "nd-flow-expr.cpp"
    break;

  case 449: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_REGEX  */
#line 3315 "nd-flow-expr.ypp"
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
#line 7364 "nd-flow-expr.cpp"
    break;

  case 450: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_REGEX  */
#line 3329 "nd-flow-expr.ypp"
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
#line 7383 "nd-flow-expr.cpp"
    break;

  case 451: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST  */
#line 3346 "nd-flow-expr.ypp"
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
#line 7399 "nd-flow-expr.cpp"
    break;

  case 452: /* expr_iface_nfq_dst: '!' FLOW_IFACE_NFQ_DST  */
#line 3357 "nd-flow-expr.ypp"
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
#line 7415 "nd-flow-expr.cpp"
    break;

  case 453: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_NAME  */
#line 3368 "nd-flow-expr.ypp"
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
#line 7439 "nd-flow-expr.cpp"
    break;

  case 454: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_NAME  */
#line 3387 "nd-flow-expr.ypp"
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
#line 7463 "nd-flow-expr.cpp"
    break;

  case 455: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_REGEX  */
#line 3406 "nd-flow-expr.ypp"
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
#line 7483 "nd-flow-expr.cpp"
    break;

  case 456: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_REGEX  */
#line 3421 "nd-flow-expr.ypp"
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
#line 7502 "nd-flow-expr.cpp"
    break;

  case 457: /* expr_intel: FLOW_INTEL  */
#line 3438 "nd-flow-expr.ypp"
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
#line 7528 "nd-flow-expr.cpp"
    break;

  case 458: /* expr_intel: '!' FLOW_INTEL  */
#line 3459 "nd-flow-expr.ypp"
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
#line 7553 "nd-flow-expr.cpp"
    break;

  case 459: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_NAME  */
#line 3481 "nd-flow-expr.ypp"
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
#line 7578 "nd-flow-expr.cpp"
    break;

  case 460: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_REGEX  */
#line 3501 "nd-flow-expr.ypp"
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
#line 7599 "nd-flow-expr.cpp"
    break;

  case 461: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_UNSIGNED  */
#line 3517 "nd-flow-expr.ypp"
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
#line 7616 "nd-flow-expr.cpp"
    break;

  case 462: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_SIGNED  */
#line 3529 "nd-flow-expr.ypp"
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
#line 7633 "nd-flow-expr.cpp"
    break;

  case 463: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FLOAT  */
#line 3541 "nd-flow-expr.ypp"
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
#line 7650 "nd-flow-expr.cpp"
    break;

  case 464: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_TRUE  */
#line 3553 "nd-flow-expr.ypp"
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
#line 7668 "nd-flow-expr.cpp"
    break;

  case 465: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FALSE  */
#line 3566 "nd-flow-expr.ypp"
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
#line 7686 "nd-flow-expr.cpp"
    break;

  case 466: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_NAME  */
#line 3581 "nd-flow-expr.ypp"
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
#line 7711 "nd-flow-expr.cpp"
    break;

  case 467: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_REGEX  */
#line 3601 "nd-flow-expr.ypp"
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
#line 7732 "nd-flow-expr.cpp"
    break;

  case 468: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3617 "nd-flow-expr.ypp"
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
#line 7749 "nd-flow-expr.cpp"
    break;

  case 469: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_SIGNED  */
#line 3629 "nd-flow-expr.ypp"
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
#line 7766 "nd-flow-expr.cpp"
    break;

  case 470: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FLOAT  */
#line 3641 "nd-flow-expr.ypp"
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
#line 7783 "nd-flow-expr.cpp"
    break;

  case 471: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_TRUE  */
#line 3653 "nd-flow-expr.ypp"
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
#line 7801 "nd-flow-expr.cpp"
    break;

  case 472: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FALSE  */
#line 3666 "nd-flow-expr.ypp"
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
#line 7819 "nd-flow-expr.cpp"
    break;

  case 473: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3681 "nd-flow-expr.ypp"
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
#line 7836 "nd-flow-expr.cpp"
    break;

  case 474: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_SIGNED  */
#line 3693 "nd-flow-expr.ypp"
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
#line 7853 "nd-flow-expr.cpp"
    break;

  case 475: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_FLOAT  */
#line 3705 "nd-flow-expr.ypp"
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
#line 7870 "nd-flow-expr.cpp"
    break;

  case 476: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3719 "nd-flow-expr.ypp"
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
#line 7887 "nd-flow-expr.cpp"
    break;

  case 477: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_SIGNED  */
#line 3731 "nd-flow-expr.ypp"
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
#line 7904 "nd-flow-expr.cpp"
    break;

  case 478: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_FLOAT  */
#line 3743 "nd-flow-expr.ypp"
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
#line 7921 "nd-flow-expr.cpp"
    break;

  case 479: /* expr_intel: FLOW_INTEL '>' VALUE_UNSIGNED  */
#line 3757 "nd-flow-expr.ypp"
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
#line 7938 "nd-flow-expr.cpp"
    break;

  case 480: /* expr_intel: FLOW_INTEL '>' VALUE_SIGNED  */
#line 3769 "nd-flow-expr.ypp"
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
#line 7955 "nd-flow-expr.cpp"
    break;

  case 481: /* expr_intel: FLOW_INTEL '>' VALUE_FLOAT  */
#line 3781 "nd-flow-expr.ypp"
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
#line 7972 "nd-flow-expr.cpp"
    break;

  case 482: /* expr_intel: FLOW_INTEL '<' VALUE_UNSIGNED  */
#line 3795 "nd-flow-expr.ypp"
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
#line 7989 "nd-flow-expr.cpp"
    break;

  case 483: /* expr_intel: FLOW_INTEL '<' VALUE_SIGNED  */
#line 3807 "nd-flow-expr.ypp"
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
#line 8006 "nd-flow-expr.cpp"
    break;

  case 484: /* expr_intel: FLOW_INTEL '<' VALUE_FLOAT  */
#line 3819 "nd-flow-expr.ypp"
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
#line 8023 "nd-flow-expr.cpp"
    break;

  case 485: /* expr_tls_version: FLOW_TLS_VERSION  */
#line 3834 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != 0));
        _NDFP_debugf("TLS version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8032 "nd-flow-expr.cpp"
    break;

  case 486: /* expr_tls_version: '!' FLOW_TLS_VERSION  */
#line 3838 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == 0));
        _NDFP_debugf("TLS version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8041 "nd-flow-expr.cpp"
    break;

  case 487: /* expr_tls_version: FLOW_TLS_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 3842 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8050 "nd-flow-expr.cpp"
    break;

  case 488: /* expr_tls_version: FLOW_TLS_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3846 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8059 "nd-flow-expr.cpp"
    break;

  case 489: /* expr_tls_version: FLOW_TLS_VERSION CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3850 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8068 "nd-flow-expr.cpp"
    break;

  case 490: /* expr_tls_version: FLOW_TLS_VERSION CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3854 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8077 "nd-flow-expr.cpp"
    break;

  case 491: /* expr_tls_version: FLOW_TLS_VERSION '>' VALUE_UNSIGNED  */
#line 3858 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8086 "nd-flow-expr.cpp"
    break;

  case 492: /* expr_tls_version: FLOW_TLS_VERSION '<' VALUE_UNSIGNED  */
#line 3862 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8095 "nd-flow-expr.cpp"
    break;

  case 493: /* expr_tls_cipher: FLOW_TLS_CIPHER  */
#line 3869 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != 0));
        _NDFP_debugf("TLS cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8104 "nd-flow-expr.cpp"
    break;

  case 494: /* expr_tls_cipher: '!' FLOW_TLS_CIPHER  */
#line 3873 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == 0));
        _NDFP_debugf("TLS cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8113 "nd-flow-expr.cpp"
    break;

  case 495: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_EQUAL VALUE_UNSIGNED  */
#line 3877 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8122 "nd-flow-expr.cpp"
    break;

  case 496: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3881 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8131 "nd-flow-expr.cpp"
    break;

  case 497: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3885 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8140 "nd-flow-expr.cpp"
    break;

  case 498: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3889 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8149 "nd-flow-expr.cpp"
    break;

  case 499: /* expr_tls_cipher: FLOW_TLS_CIPHER '>' VALUE_UNSIGNED  */
#line 3893 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8158 "nd-flow-expr.cpp"
    break;

  case 500: /* expr_tls_cipher: FLOW_TLS_CIPHER '<' VALUE_UNSIGNED  */
#line 3897 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8167 "nd-flow-expr.cpp"
    break;

  case 501: /* expr_tls_ech: FLOW_TLS_ECH  */
#line 3904 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != 0));
        _NDFP_debugf("TLS ECH version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8176 "nd-flow-expr.cpp"
    break;

  case 502: /* expr_tls_ech: '!' FLOW_TLS_ECH  */
#line 3908 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == 0));
        _NDFP_debugf("TLS ECH version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8185 "nd-flow-expr.cpp"
    break;

  case 503: /* expr_tls_ech: FLOW_TLS_ECH CMP_EQUAL VALUE_UNSIGNED  */
#line 3912 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8194 "nd-flow-expr.cpp"
    break;

  case 504: /* expr_tls_ech: FLOW_TLS_ECH CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3916 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8203 "nd-flow-expr.cpp"
    break;

  case 505: /* expr_tls_ech: FLOW_TLS_ECH CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3920 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8212 "nd-flow-expr.cpp"
    break;

  case 506: /* expr_tls_ech: FLOW_TLS_ECH CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3924 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8221 "nd-flow-expr.cpp"
    break;

  case 507: /* expr_tls_ech: FLOW_TLS_ECH '>' VALUE_UNSIGNED  */
#line 3928 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8230 "nd-flow-expr.cpp"
    break;

  case 508: /* expr_tls_ech: FLOW_TLS_ECH '<' VALUE_UNSIGNED  */
#line 3932 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8239 "nd-flow-expr.cpp"
    break;

  case 509: /* expr_tls_ja4: FLOW_TLS_JA4  */
#line 3939 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8251 "nd-flow-expr.cpp"
    break;

  case 510: /* expr_tls_ja4: '!' FLOW_TLS_JA4  */
#line 3946 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8263 "nd-flow-expr.cpp"
    break;

  case 511: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_NAME  */
#line 3953 "nd-flow-expr.ypp"
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
#line 8287 "nd-flow-expr.cpp"
    break;

  case 512: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_NAME  */
#line 3972 "nd-flow-expr.ypp"
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
#line 8311 "nd-flow-expr.cpp"
    break;

  case 513: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_REGEX  */
#line 3991 "nd-flow-expr.ypp"
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
#line 8330 "nd-flow-expr.cpp"
    break;

  case 514: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_REGEX  */
#line 4005 "nd-flow-expr.ypp"
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
#line 8349 "nd-flow-expr.cpp"
    break;

  case 515: /* expr_origin: FLOW_ORIGIN  */
#line 4022 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8358 "nd-flow-expr.cpp"
    break;

  case 516: /* expr_origin: '!' FLOW_ORIGIN  */
#line 4026 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 8367 "nd-flow-expr.cpp"
    break;

  case 517: /* expr_origin: FLOW_ORIGIN CMP_EQUAL value_origin_type  */
#line 4030 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8376 "nd-flow-expr.cpp"
    break;

  case 518: /* expr_origin: FLOW_ORIGIN CMP_NOTEQUAL value_origin_type  */
#line 4034 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 8385 "nd-flow-expr.cpp"
    break;

  case 519: /* value_origin_type: FLOW_ORIGIN_LOCAL  */
#line 4041 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 8391 "nd-flow-expr.cpp"
    break;

  case 520: /* value_origin_type: FLOW_ORIGIN_OTHER  */
#line 4042 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 8397 "nd-flow-expr.cpp"
    break;

  case 521: /* value_origin_type: FLOW_ORIGIN_UNKNOWN  */
#line 4043 "nd-flow-expr.ypp"
                          { (yyval.us_number) = (yyvsp[0].us_number); }
#line 8403 "nd-flow-expr.cpp"
    break;

  case 522: /* expr_tag: FLOW_TAG  */
#line 4047 "nd-flow-expr.ypp"
               {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags not empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8415 "nd-flow-expr.cpp"
    break;

  case 523: /* expr_tag: '!' FLOW_TAG  */
#line 4054 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 8427 "nd-flow-expr.cpp"
    break;

  case 524: /* expr_tag: FLOW_TAG CMP_EQUAL VALUE_NAME  */
#line 4061 "nd-flow-expr.ypp"
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
#line 8460 "nd-flow-expr.cpp"
    break;

  case 525: /* expr_tag: FLOW_TAG CMP_NOTEQUAL VALUE_NAME  */
#line 4089 "nd-flow-expr.ypp"
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
#line 8493 "nd-flow-expr.cpp"
    break;

  case 526: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE  */
#line 4120 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == true));
    }
#line 8501 "nd-flow-expr.cpp"
    break;

  case 527: /* expr_app_ip_override: '!' FLOW_APP_IP_OVERRIDE  */
#line 4123 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == false));
    }
#line 8509 "nd-flow-expr.cpp"
    break;

  case 528: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_EQUAL VALUE_TRUE  */
#line 4126 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == true));
    }
#line 8517 "nd-flow-expr.cpp"
    break;

  case 529: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_EQUAL VALUE_FALSE  */
#line 4129 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() == false));
    }
#line 8525 "nd-flow-expr.cpp"
    break;

  case 530: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_NOTEQUAL VALUE_TRUE  */
#line 4132 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() != true));
    }
#line 8533 "nd-flow-expr.cpp"
    break;

  case 531: /* expr_app_ip_override: FLOW_APP_IP_OVERRIDE CMP_NOTEQUAL VALUE_FALSE  */
#line 4135 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_ip_override.load() != false));
    }
#line 8541 "nd-flow-expr.cpp"
    break;

  case 532: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS  */
#line 4141 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == true));
    }
#line 8549 "nd-flow-expr.cpp"
    break;

  case 533: /* expr_app_proto_twins: '!' FLOW_APP_PROTO_TWINS  */
#line 4144 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == false));
    }
#line 8557 "nd-flow-expr.cpp"
    break;

  case 534: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_EQUAL VALUE_TRUE  */
#line 4147 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == true));
    }
#line 8565 "nd-flow-expr.cpp"
    break;

  case 535: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_EQUAL VALUE_FALSE  */
#line 4150 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() == false));
    }
#line 8573 "nd-flow-expr.cpp"
    break;

  case 536: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_NOTEQUAL VALUE_TRUE  */
#line 4153 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() != true));
    }
#line 8581 "nd-flow-expr.cpp"
    break;

  case 537: /* expr_app_proto_twins: FLOW_APP_PROTO_TWINS CMP_NOTEQUAL VALUE_FALSE  */
#line 4156 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.app_proto_twins.load() != false));
    }
#line 8589 "nd-flow-expr.cpp"
    break;

  case 538: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_EQUAL VALUE_UNSIGNED  */
#line 4162 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen == (yyvsp[0].ul_number)));
    }
#line 8597 "nd-flow-expr.cpp"
    break;

  case 539: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4165 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen != (yyvsp[0].ul_number)));
    }
#line 8605 "nd-flow-expr.cpp"
    break;

  case 540: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4168 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen >= (yyvsp[0].ul_number)));
    }
#line 8613 "nd-flow-expr.cpp"
    break;

  case 541: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4171 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen <= (yyvsp[0].ul_number)));
    }
#line 8621 "nd-flow-expr.cpp"
    break;

  case 542: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN '>' VALUE_UNSIGNED  */
#line 4174 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen > (yyvsp[0].ul_number)));
    }
#line 8629 "nd-flow-expr.cpp"
    break;

  case 543: /* expr_ts_first_seen: FLOW_TS_FIRST_SEEN '<' VALUE_UNSIGNED  */
#line 4177 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_first_seen < (yyvsp[0].ul_number)));
    }
#line 8637 "nd-flow-expr.cpp"
    break;

  case 544: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_EQUAL VALUE_UNSIGNED  */
#line 4183 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() == (yyvsp[0].ul_number)));
    }
#line 8645 "nd-flow-expr.cpp"
    break;

  case 545: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4186 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() != (yyvsp[0].ul_number)));
    }
#line 8653 "nd-flow-expr.cpp"
    break;

  case 546: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4189 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() >= (yyvsp[0].ul_number)));
    }
#line 8661 "nd-flow-expr.cpp"
    break;

  case 547: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4192 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() <= (yyvsp[0].ul_number)));
    }
#line 8669 "nd-flow-expr.cpp"
    break;

  case 548: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN '>' VALUE_UNSIGNED  */
#line 4195 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() > (yyvsp[0].ul_number)));
    }
#line 8677 "nd-flow-expr.cpp"
    break;

  case 549: /* expr_ts_last_seen: FLOW_TS_LAST_SEEN '<' VALUE_UNSIGNED  */
#line 4198 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ts_last_seen.load() < (yyvsp[0].ul_number)));
    }
#line 8685 "nd-flow-expr.cpp"
    break;

  case 550: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_EQUAL VALUE_UNSIGNED  */
#line 4204 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex == (yyvsp[0].ul_number)));
    }
#line 8693 "nd-flow-expr.cpp"
    break;

  case 551: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4207 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex != (yyvsp[0].ul_number)));
    }
#line 8701 "nd-flow-expr.cpp"
    break;

  case 552: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4210 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex >= (yyvsp[0].ul_number)));
    }
#line 8709 "nd-flow-expr.cpp"
    break;

  case 553: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4213 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex <= (yyvsp[0].ul_number)));
    }
#line 8717 "nd-flow-expr.cpp"
    break;

  case 554: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX '>' VALUE_UNSIGNED  */
#line 4216 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex > (yyvsp[0].ul_number)));
    }
#line 8725 "nd-flow-expr.cpp"
    break;

  case 555: /* expr_nfq_src_ifindex: FLOW_NFQ_SRC_IFINDEX '<' VALUE_UNSIGNED  */
#line 4219 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.src_ifindex < (yyvsp[0].ul_number)));
    }
#line 8733 "nd-flow-expr.cpp"
    break;

  case 556: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_EQUAL VALUE_UNSIGNED  */
#line 4225 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex == (yyvsp[0].ul_number)));
    }
#line 8741 "nd-flow-expr.cpp"
    break;

  case 557: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4228 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex != (yyvsp[0].ul_number)));
    }
#line 8749 "nd-flow-expr.cpp"
    break;

  case 558: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4231 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex >= (yyvsp[0].ul_number)));
    }
#line 8757 "nd-flow-expr.cpp"
    break;

  case 559: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4234 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex <= (yyvsp[0].ul_number)));
    }
#line 8765 "nd-flow-expr.cpp"
    break;

  case 560: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX '>' VALUE_UNSIGNED  */
#line 4237 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex > (yyvsp[0].ul_number)));
    }
#line 8773 "nd-flow-expr.cpp"
    break;

  case 561: /* expr_nfq_dst_ifindex: FLOW_NFQ_DST_IFINDEX '<' VALUE_UNSIGNED  */
#line 4240 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->nfq.dst_ifindex < (yyvsp[0].ul_number)));
    }
#line 8781 "nd-flow-expr.cpp"
    break;

  case 562: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_EQUAL VALUE_UNSIGNED  */
#line 4246 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() == (yyvsp[0].ul_number)));
    }
#line 8789 "nd-flow-expr.cpp"
    break;

  case 563: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4249 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() != (yyvsp[0].ul_number)));
    }
#line 8797 "nd-flow-expr.cpp"
    break;

  case 564: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4252 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() >= (yyvsp[0].ul_number)));
    }
#line 8805 "nd-flow-expr.cpp"
    break;

  case 565: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4255 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() <= (yyvsp[0].ul_number)));
    }
#line 8813 "nd-flow-expr.cpp"
    break;

  case 566: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK '>' VALUE_UNSIGNED  */
#line 4258 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() > (yyvsp[0].ul_number)));
    }
#line 8821 "nd-flow-expr.cpp"
    break;

  case 567: /* expr_tcp_fin_ack: FLOW_TCP_FIN_ACK '<' VALUE_UNSIGNED  */
#line 4261 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.fin_ack.load() < (yyvsp[0].ul_number)));
    }
#line 8829 "nd-flow-expr.cpp"
    break;

  case 568: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_EQUAL VALUE_UNSIGNED  */
#line 4267 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() == (yyvsp[0].ul_number)));
    }
#line 8837 "nd-flow-expr.cpp"
    break;

  case 569: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4270 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() != (yyvsp[0].ul_number)));
    }
#line 8845 "nd-flow-expr.cpp"
    break;

  case 570: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4273 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() >= (yyvsp[0].ul_number)));
    }
#line 8853 "nd-flow-expr.cpp"
    break;

  case 571: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4276 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() <= (yyvsp[0].ul_number)));
    }
#line 8861 "nd-flow-expr.cpp"
    break;

  case 572: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ '>' VALUE_UNSIGNED  */
#line 4279 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() > (yyvsp[0].ul_number)));
    }
#line 8869 "nd-flow-expr.cpp"
    break;

  case 573: /* expr_tcp_last_seq: FLOW_TCP_LAST_SEQ '<' VALUE_UNSIGNED  */
#line 4282 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tcp.last_seq.load() < (yyvsp[0].ul_number)));
    }
#line 8877 "nd-flow-expr.cpp"
    break;

  case 574: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4288 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() == (yyvsp[0].ul_number)));
    }
#line 8885 "nd-flow-expr.cpp"
    break;

  case 575: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4291 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() != (yyvsp[0].ul_number)));
    }
#line 8893 "nd-flow-expr.cpp"
    break;

  case 576: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4294 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() >= (yyvsp[0].ul_number)));
    }
#line 8901 "nd-flow-expr.cpp"
    break;

  case 577: /* expr_total_bytes: FLOW_TOTAL_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4297 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() <= (yyvsp[0].ul_number)));
    }
#line 8909 "nd-flow-expr.cpp"
    break;

  case 578: /* expr_total_bytes: FLOW_TOTAL_BYTES '>' VALUE_UNSIGNED  */
#line 4300 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() > (yyvsp[0].ul_number)));
    }
#line 8917 "nd-flow-expr.cpp"
    break;

  case 579: /* expr_total_bytes: FLOW_TOTAL_BYTES '<' VALUE_UNSIGNED  */
#line 4303 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_bytes.load() < (yyvsp[0].ul_number)));
    }
#line 8925 "nd-flow-expr.cpp"
    break;

  case 580: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4309 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() == (yyvsp[0].ul_number)));
    }
#line 8933 "nd-flow-expr.cpp"
    break;

  case 581: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4312 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() != (yyvsp[0].ul_number)));
    }
#line 8941 "nd-flow-expr.cpp"
    break;

  case 582: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4315 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() >= (yyvsp[0].ul_number)));
    }
#line 8949 "nd-flow-expr.cpp"
    break;

  case 583: /* expr_total_packets: FLOW_TOTAL_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4318 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() <= (yyvsp[0].ul_number)));
    }
#line 8957 "nd-flow-expr.cpp"
    break;

  case 584: /* expr_total_packets: FLOW_TOTAL_PACKETS '>' VALUE_UNSIGNED  */
#line 4321 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() > (yyvsp[0].ul_number)));
    }
#line 8965 "nd-flow-expr.cpp"
    break;

  case 585: /* expr_total_packets: FLOW_TOTAL_PACKETS '<' VALUE_UNSIGNED  */
#line 4324 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.total_packets.load() < (yyvsp[0].ul_number)));
    }
#line 8973 "nd-flow-expr.cpp"
    break;

  case 586: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4330 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 8982 "nd-flow-expr.cpp"
    break;

  case 587: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4334 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 8991 "nd-flow-expr.cpp"
    break;

  case 588: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4338 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9000 "nd-flow-expr.cpp"
    break;

  case 589: /* expr_local_bytes: FLOW_LOCAL_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4342 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9009 "nd-flow-expr.cpp"
    break;

  case 590: /* expr_local_bytes: FLOW_LOCAL_BYTES '>' VALUE_UNSIGNED  */
#line 4346 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9018 "nd-flow-expr.cpp"
    break;

  case 591: /* expr_local_bytes: FLOW_LOCAL_BYTES '<' VALUE_UNSIGNED  */
#line 4350 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9027 "nd-flow-expr.cpp"
    break;

  case 592: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4357 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9036 "nd-flow-expr.cpp"
    break;

  case 593: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4361 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9045 "nd-flow-expr.cpp"
    break;

  case 594: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4365 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9054 "nd-flow-expr.cpp"
    break;

  case 595: /* expr_other_bytes: FLOW_OTHER_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4369 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9063 "nd-flow-expr.cpp"
    break;

  case 596: /* expr_other_bytes: FLOW_OTHER_BYTES '>' VALUE_UNSIGNED  */
#line 4373 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9072 "nd-flow-expr.cpp"
    break;

  case 597: /* expr_other_bytes: FLOW_OTHER_BYTES '<' VALUE_UNSIGNED  */
#line 4377 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9081 "nd-flow-expr.cpp"
    break;

  case 598: /* expr_src_bytes: FLOW_SRC_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4384 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9090 "nd-flow-expr.cpp"
    break;

  case 599: /* expr_src_bytes: FLOW_SRC_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4388 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9099 "nd-flow-expr.cpp"
    break;

  case 600: /* expr_src_bytes: FLOW_SRC_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4392 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9108 "nd-flow-expr.cpp"
    break;

  case 601: /* expr_src_bytes: FLOW_SRC_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4396 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9117 "nd-flow-expr.cpp"
    break;

  case 602: /* expr_src_bytes: FLOW_SRC_BYTES '>' VALUE_UNSIGNED  */
#line 4400 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9126 "nd-flow-expr.cpp"
    break;

  case 603: /* expr_src_bytes: FLOW_SRC_BYTES '<' VALUE_UNSIGNED  */
#line 4404 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_bytes.load() : _NDFP_flow->stats.upper_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9135 "nd-flow-expr.cpp"
    break;

  case 604: /* expr_dst_bytes: FLOW_DST_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 4411 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) == (yyvsp[0].ul_number)));
    }
#line 9144 "nd-flow-expr.cpp"
    break;

  case 605: /* expr_dst_bytes: FLOW_DST_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4415 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) != (yyvsp[0].ul_number)));
    }
#line 9153 "nd-flow-expr.cpp"
    break;

  case 606: /* expr_dst_bytes: FLOW_DST_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4419 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) >= (yyvsp[0].ul_number)));
    }
#line 9162 "nd-flow-expr.cpp"
    break;

  case 607: /* expr_dst_bytes: FLOW_DST_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4423 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) <= (yyvsp[0].ul_number)));
    }
#line 9171 "nd-flow-expr.cpp"
    break;

  case 608: /* expr_dst_bytes: FLOW_DST_BYTES '>' VALUE_UNSIGNED  */
#line 4427 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) > (yyvsp[0].ul_number)));
    }
#line 9180 "nd-flow-expr.cpp"
    break;

  case 609: /* expr_dst_bytes: FLOW_DST_BYTES '<' VALUE_UNSIGNED  */
#line 4431 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_bytes.load() : _NDFP_flow->stats.lower_bytes.load()) < (yyvsp[0].ul_number)));
    }
#line 9189 "nd-flow-expr.cpp"
    break;

  case 610: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4438 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9198 "nd-flow-expr.cpp"
    break;

  case 611: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4442 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9207 "nd-flow-expr.cpp"
    break;

  case 612: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4446 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9216 "nd-flow-expr.cpp"
    break;

  case 613: /* expr_local_packets: FLOW_LOCAL_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4450 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9225 "nd-flow-expr.cpp"
    break;

  case 614: /* expr_local_packets: FLOW_LOCAL_PACKETS '>' VALUE_UNSIGNED  */
#line 4454 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9234 "nd-flow-expr.cpp"
    break;

  case 615: /* expr_local_packets: FLOW_LOCAL_PACKETS '<' VALUE_UNSIGNED  */
#line 4458 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9243 "nd-flow-expr.cpp"
    break;

  case 616: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4465 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9252 "nd-flow-expr.cpp"
    break;

  case 617: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4469 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9261 "nd-flow-expr.cpp"
    break;

  case 618: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4473 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9270 "nd-flow-expr.cpp"
    break;

  case 619: /* expr_other_packets: FLOW_OTHER_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4477 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9279 "nd-flow-expr.cpp"
    break;

  case 620: /* expr_other_packets: FLOW_OTHER_PACKETS '>' VALUE_UNSIGNED  */
#line 4481 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9288 "nd-flow-expr.cpp"
    break;

  case 621: /* expr_other_packets: FLOW_OTHER_PACKETS '<' VALUE_UNSIGNED  */
#line 4485 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9297 "nd-flow-expr.cpp"
    break;

  case 622: /* expr_src_packets: FLOW_SRC_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4492 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9306 "nd-flow-expr.cpp"
    break;

  case 623: /* expr_src_packets: FLOW_SRC_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4496 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9315 "nd-flow-expr.cpp"
    break;

  case 624: /* expr_src_packets: FLOW_SRC_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4500 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9324 "nd-flow-expr.cpp"
    break;

  case 625: /* expr_src_packets: FLOW_SRC_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4504 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9333 "nd-flow-expr.cpp"
    break;

  case 626: /* expr_src_packets: FLOW_SRC_PACKETS '>' VALUE_UNSIGNED  */
#line 4508 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9342 "nd-flow-expr.cpp"
    break;

  case 627: /* expr_src_packets: FLOW_SRC_PACKETS '<' VALUE_UNSIGNED  */
#line 4512 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_packets.load() : _NDFP_flow->stats.upper_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9351 "nd-flow-expr.cpp"
    break;

  case 628: /* expr_dst_packets: FLOW_DST_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 4519 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) == (yyvsp[0].ul_number)));
    }
#line 9360 "nd-flow-expr.cpp"
    break;

  case 629: /* expr_dst_packets: FLOW_DST_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 4523 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) != (yyvsp[0].ul_number)));
    }
#line 9369 "nd-flow-expr.cpp"
    break;

  case 630: /* expr_dst_packets: FLOW_DST_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 4527 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) >= (yyvsp[0].ul_number)));
    }
#line 9378 "nd-flow-expr.cpp"
    break;

  case 631: /* expr_dst_packets: FLOW_DST_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 4531 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) <= (yyvsp[0].ul_number)));
    }
#line 9387 "nd-flow-expr.cpp"
    break;

  case 632: /* expr_dst_packets: FLOW_DST_PACKETS '>' VALUE_UNSIGNED  */
#line 4535 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) > (yyvsp[0].ul_number)));
    }
#line 9396 "nd-flow-expr.cpp"
    break;

  case 633: /* expr_dst_packets: FLOW_DST_PACKETS '<' VALUE_UNSIGNED  */
#line 4539 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_packets.load() : _NDFP_flow->stats.lower_packets.load()) < (yyvsp[0].ul_number)));
    }
#line 9405 "nd-flow-expr.cpp"
    break;

  case 634: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT  */
#line 4546 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.user_agent.empty() == false));
    }
#line 9413 "nd-flow-expr.cpp"
    break;

  case 635: /* expr_http_user_agent: '!' FLOW_HTTP_USER_AGENT  */
#line 4549 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.user_agent.empty() == true));
    }
#line 9421 "nd-flow-expr.cpp"
    break;

  case 636: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_EQUAL VALUE_NAME  */
#line 4552 "nd-flow-expr.ypp"
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
#line 9436 "nd-flow-expr.cpp"
    break;

  case 637: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_NOTEQUAL VALUE_NAME  */
#line 4562 "nd-flow-expr.ypp"
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
#line 9451 "nd-flow-expr.cpp"
    break;

  case 638: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_EQUAL VALUE_REGEX  */
#line 4572 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->http.user_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->http.user_agent));
        }
    }
#line 9463 "nd-flow-expr.cpp"
    break;

  case 639: /* expr_http_user_agent: FLOW_HTTP_USER_AGENT CMP_NOTEQUAL VALUE_REGEX  */
#line 4579 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->http.user_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->http.user_agent));
        }
    }
#line 9475 "nd-flow-expr.cpp"
    break;

  case 640: /* expr_http_url: FLOW_HTTP_URL  */
#line 4589 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.url.empty() == false));
    }
#line 9483 "nd-flow-expr.cpp"
    break;

  case 641: /* expr_http_url: '!' FLOW_HTTP_URL  */
#line 4592 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->http.url.empty() == true));
    }
#line 9491 "nd-flow-expr.cpp"
    break;

  case 642: /* expr_http_url: FLOW_HTTP_URL CMP_EQUAL VALUE_NAME  */
#line 4595 "nd-flow-expr.ypp"
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
#line 9506 "nd-flow-expr.cpp"
    break;

  case 643: /* expr_http_url: FLOW_HTTP_URL CMP_NOTEQUAL VALUE_NAME  */
#line 4605 "nd-flow-expr.ypp"
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
#line 9521 "nd-flow-expr.cpp"
    break;

  case 644: /* expr_http_url: FLOW_HTTP_URL CMP_EQUAL VALUE_REGEX  */
#line 4615 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->http.url.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->http.url));
        }
    }
#line 9533 "nd-flow-expr.cpp"
    break;

  case 645: /* expr_http_url: FLOW_HTTP_URL CMP_NOTEQUAL VALUE_REGEX  */
#line 4622 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->http.url.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->http.url));
        }
    }
#line 9545 "nd-flow-expr.cpp"
    break;

  case 646: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT  */
#line 4632 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.fingerprint.empty() == false));
    }
#line 9553 "nd-flow-expr.cpp"
    break;

  case 647: /* expr_dhcp_fingerprint: '!' FLOW_DHCP_FINGERPRINT  */
#line 4635 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.fingerprint.empty() == true));
    }
#line 9561 "nd-flow-expr.cpp"
    break;

  case 648: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_EQUAL VALUE_NAME  */
#line 4638 "nd-flow-expr.ypp"
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
#line 9576 "nd-flow-expr.cpp"
    break;

  case 649: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_NOTEQUAL VALUE_NAME  */
#line 4648 "nd-flow-expr.ypp"
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
#line 9591 "nd-flow-expr.cpp"
    break;

  case 650: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_EQUAL VALUE_REGEX  */
#line 4658 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->dhcp.fingerprint.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->dhcp.fingerprint));
        }
    }
#line 9603 "nd-flow-expr.cpp"
    break;

  case 651: /* expr_dhcp_fingerprint: FLOW_DHCP_FINGERPRINT CMP_NOTEQUAL VALUE_REGEX  */
#line 4665 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->dhcp.fingerprint.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->dhcp.fingerprint));
        }
    }
#line 9615 "nd-flow-expr.cpp"
    break;

  case 652: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT  */
#line 4675 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.class_ident.empty() == false));
    }
#line 9623 "nd-flow-expr.cpp"
    break;

  case 653: /* expr_dhcp_class_ident: '!' FLOW_DHCP_CLASS_IDENT  */
#line 4678 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->dhcp.class_ident.empty() == true));
    }
#line 9631 "nd-flow-expr.cpp"
    break;

  case 654: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_EQUAL VALUE_NAME  */
#line 4681 "nd-flow-expr.ypp"
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
#line 9646 "nd-flow-expr.cpp"
    break;

  case 655: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_NOTEQUAL VALUE_NAME  */
#line 4691 "nd-flow-expr.ypp"
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
#line 9661 "nd-flow-expr.cpp"
    break;

  case 656: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_EQUAL VALUE_REGEX  */
#line 4701 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->dhcp.class_ident.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->dhcp.class_ident));
        }
    }
#line 9673 "nd-flow-expr.cpp"
    break;

  case 657: /* expr_dhcp_class_ident: FLOW_DHCP_CLASS_IDENT CMP_NOTEQUAL VALUE_REGEX  */
#line 4708 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->dhcp.class_ident.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->dhcp.class_ident));
        }
    }
#line 9685 "nd-flow-expr.cpp"
    break;

  case 658: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT  */
#line 4718 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.client_agent.empty() == false));
    }
#line 9693 "nd-flow-expr.cpp"
    break;

  case 659: /* expr_ssh_client_agent: '!' FLOW_SSH_CLIENT_AGENT  */
#line 4721 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.client_agent.empty() == true));
    }
#line 9701 "nd-flow-expr.cpp"
    break;

  case 660: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_EQUAL VALUE_NAME  */
#line 4724 "nd-flow-expr.ypp"
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
#line 9716 "nd-flow-expr.cpp"
    break;

  case 661: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_NOTEQUAL VALUE_NAME  */
#line 4734 "nd-flow-expr.ypp"
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
#line 9731 "nd-flow-expr.cpp"
    break;

  case 662: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_EQUAL VALUE_REGEX  */
#line 4744 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->ssh.client_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->ssh.client_agent));
        }
    }
#line 9743 "nd-flow-expr.cpp"
    break;

  case 663: /* expr_ssh_client_agent: FLOW_SSH_CLIENT_AGENT CMP_NOTEQUAL VALUE_REGEX  */
#line 4751 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->ssh.client_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->ssh.client_agent));
        }
    }
#line 9755 "nd-flow-expr.cpp"
    break;

  case 664: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT  */
#line 4761 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.server_agent.empty() == false));
    }
#line 9763 "nd-flow-expr.cpp"
    break;

  case 665: /* expr_ssh_server_agent: '!' FLOW_SSH_SERVER_AGENT  */
#line 4764 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ssh.server_agent.empty() == true));
    }
#line 9771 "nd-flow-expr.cpp"
    break;

  case 666: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_EQUAL VALUE_NAME  */
#line 4767 "nd-flow-expr.ypp"
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
#line 9786 "nd-flow-expr.cpp"
    break;

  case 667: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_NOTEQUAL VALUE_NAME  */
#line 4777 "nd-flow-expr.ypp"
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
#line 9801 "nd-flow-expr.cpp"
    break;

  case 668: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_EQUAL VALUE_REGEX  */
#line 4787 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->ssh.server_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->ssh.server_agent));
        }
    }
#line 9813 "nd-flow-expr.cpp"
    break;

  case 669: /* expr_ssh_server_agent: FLOW_SSH_SERVER_AGENT CMP_NOTEQUAL VALUE_REGEX  */
#line 4794 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->ssh.server_agent.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->ssh.server_agent));
        }
    }
#line 9825 "nd-flow-expr.cpp"
    break;

  case 670: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN  */
#line 4804 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.subject_dn.empty() == false));
    }
#line 9833 "nd-flow-expr.cpp"
    break;

  case 671: /* expr_tls_subject_dn: '!' FLOW_TLS_SUBJECT_DN  */
#line 4807 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.subject_dn.empty() == true));
    }
#line 9841 "nd-flow-expr.cpp"
    break;

  case 672: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_EQUAL VALUE_NAME  */
#line 4810 "nd-flow-expr.ypp"
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
#line 9856 "nd-flow-expr.cpp"
    break;

  case 673: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_NOTEQUAL VALUE_NAME  */
#line 4820 "nd-flow-expr.ypp"
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
#line 9871 "nd-flow-expr.cpp"
    break;

  case 674: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_EQUAL VALUE_REGEX  */
#line 4830 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.subject_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->tls.subject_dn));
        }
    }
#line 9883 "nd-flow-expr.cpp"
    break;

  case 675: /* expr_tls_subject_dn: FLOW_TLS_SUBJECT_DN CMP_NOTEQUAL VALUE_REGEX  */
#line 4837 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.subject_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->tls.subject_dn));
        }
    }
#line 9895 "nd-flow-expr.cpp"
    break;

  case 676: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN  */
#line 4847 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.issuer_dn.empty() == false));
    }
#line 9903 "nd-flow-expr.cpp"
    break;

  case 677: /* expr_tls_issuer_dn: '!' FLOW_TLS_ISSUER_DN  */
#line 4850 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.issuer_dn.empty() == true));
    }
#line 9911 "nd-flow-expr.cpp"
    break;

  case 678: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_EQUAL VALUE_NAME  */
#line 4853 "nd-flow-expr.ypp"
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
#line 9926 "nd-flow-expr.cpp"
    break;

  case 679: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_NOTEQUAL VALUE_NAME  */
#line 4863 "nd-flow-expr.ypp"
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
#line 9941 "nd-flow-expr.cpp"
    break;

  case 680: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_EQUAL VALUE_REGEX  */
#line 4873 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.issuer_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->tls.issuer_dn));
        }
    }
#line 9953 "nd-flow-expr.cpp"
    break;

  case 681: /* expr_tls_issuer_dn: FLOW_TLS_ISSUER_DN CMP_NOTEQUAL VALUE_REGEX  */
#line 4880 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.issuer_dn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->tls.issuer_dn));
        }
    }
#line 9965 "nd-flow-expr.cpp"
    break;

  case 682: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN  */
#line 4890 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.server_cn.empty() == false));
    }
#line 9973 "nd-flow-expr.cpp"
    break;

  case 683: /* expr_tls_server_cn: '!' FLOW_TLS_SERVER_CN  */
#line 4893 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.server_cn.empty() == true));
    }
#line 9981 "nd-flow-expr.cpp"
    break;

  case 684: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_EQUAL VALUE_NAME  */
#line 4896 "nd-flow-expr.ypp"
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
#line 9996 "nd-flow-expr.cpp"
    break;

  case 685: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_NOTEQUAL VALUE_NAME  */
#line 4906 "nd-flow-expr.ypp"
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
#line 10011 "nd-flow-expr.cpp"
    break;

  case 686: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_EQUAL VALUE_REGEX  */
#line 4916 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->tls.server_cn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->tls.server_cn));
        }
    }
#line 10023 "nd-flow-expr.cpp"
    break;

  case 687: /* expr_tls_server_cn: FLOW_TLS_SERVER_CN CMP_NOTEQUAL VALUE_REGEX  */
#line 4923 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->tls.server_cn.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->tls.server_cn));
        }
    }
#line 10035 "nd-flow-expr.cpp"
    break;

  case 688: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME  */
#line 4933 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->mdns.domain_name.empty() == false));
    }
#line 10043 "nd-flow-expr.cpp"
    break;

  case 689: /* expr_mdns_domain_name: '!' FLOW_MDNS_DOMAIN_NAME  */
#line 4936 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->mdns.domain_name.empty() == true));
    }
#line 10051 "nd-flow-expr.cpp"
    break;

  case 690: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_EQUAL VALUE_NAME  */
#line 4939 "nd-flow-expr.ypp"
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
#line 10066 "nd-flow-expr.cpp"
    break;

  case 691: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_NOTEQUAL VALUE_NAME  */
#line 4949 "nd-flow-expr.ypp"
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
#line 10081 "nd-flow-expr.cpp"
    break;

  case 692: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_EQUAL VALUE_REGEX  */
#line 4959 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->mdns.domain_name.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, _NDFP_flow->mdns.domain_name));
        }
    }
#line 10093 "nd-flow-expr.cpp"
    break;

  case 693: /* expr_mdns_domain_name: FLOW_MDNS_DOMAIN_NAME CMP_NOTEQUAL VALUE_REGEX  */
#line 4966 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->mdns.domain_name.empty()) {
            std::string rx((yyvsp[0].buffer));
            _NDFP_result = ((yyval.bool_result) = !_NDFP_regex_search(rx, _NDFP_flow->mdns.domain_name));
        }
    }
#line 10105 "nd-flow-expr.cpp"
    break;

  case 694: /* expr_tls_alpn: FLOW_TLS_ALPN  */
#line 4976 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (! _NDFP_flow->tls.alpn.empty()));
    }
#line 10113 "nd-flow-expr.cpp"
    break;

  case 695: /* expr_tls_alpn: '!' FLOW_TLS_ALPN  */
#line 4979 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.alpn.empty()));
    }
#line 10121 "nd-flow-expr.cpp"
    break;

  case 696: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_EQUAL VALUE_NAME  */
#line 4982 "nd-flow-expr.ypp"
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
#line 10137 "nd-flow-expr.cpp"
    break;

  case 697: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_NOTEQUAL VALUE_NAME  */
#line 4993 "nd-flow-expr.ypp"
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
#line 10153 "nd-flow-expr.cpp"
    break;

  case 698: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_EQUAL VALUE_REGEX  */
#line 5004 "nd-flow-expr.ypp"
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
#line 10168 "nd-flow-expr.cpp"
    break;

  case 699: /* expr_tls_alpn: FLOW_TLS_ALPN CMP_NOTEQUAL VALUE_REGEX  */
#line 5014 "nd-flow-expr.ypp"
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
#line 10183 "nd-flow-expr.cpp"
    break;

  case 700: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO  */
#line 5027 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == true));
    }
#line 10191 "nd-flow-expr.cpp"
    break;

  case 701: /* expr_tls_proc_hello: '!' FLOW_TLS_PROC_HELLO  */
#line 5030 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == false));
    }
#line 10199 "nd-flow-expr.cpp"
    break;

  case 702: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_EQUAL VALUE_TRUE  */
#line 5033 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == true));
    }
#line 10207 "nd-flow-expr.cpp"
    break;

  case 703: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_EQUAL VALUE_FALSE  */
#line 5036 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() == false));
    }
#line 10215 "nd-flow-expr.cpp"
    break;

  case 704: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_NOTEQUAL VALUE_TRUE  */
#line 5039 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() != true));
    }
#line 10223 "nd-flow-expr.cpp"
    break;

  case 705: /* expr_tls_proc_hello: FLOW_TLS_PROC_HELLO CMP_NOTEQUAL VALUE_FALSE  */
#line 5042 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_hello.load() != false));
    }
#line 10231 "nd-flow-expr.cpp"
    break;

  case 706: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE  */
#line 5048 "nd-flow-expr.ypp"
                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == true));
    }
#line 10239 "nd-flow-expr.cpp"
    break;

  case 707: /* expr_tls_proc_certificate: '!' FLOW_TLS_PROC_CERTIFICATE  */
#line 5051 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == false));
    }
#line 10247 "nd-flow-expr.cpp"
    break;

  case 708: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_EQUAL VALUE_TRUE  */
#line 5054 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == true));
    }
#line 10255 "nd-flow-expr.cpp"
    break;

  case 709: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_EQUAL VALUE_FALSE  */
#line 5057 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() == false));
    }
#line 10263 "nd-flow-expr.cpp"
    break;

  case 710: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_NOTEQUAL VALUE_TRUE  */
#line 5060 "nd-flow-expr.ypp"
                                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() != true));
    }
#line 10271 "nd-flow-expr.cpp"
    break;

  case 711: /* expr_tls_proc_certificate: FLOW_TLS_PROC_CERTIFICATE CMP_NOTEQUAL VALUE_FALSE  */
#line 5063 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.proc_certificate.load() != false));
    }
#line 10279 "nd-flow-expr.cpp"
    break;

  case 712: /* expr_smtp_tls: FLOW_SMTP_TLS  */
#line 5069 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == true));
    }
#line 10287 "nd-flow-expr.cpp"
    break;

  case 713: /* expr_smtp_tls: '!' FLOW_SMTP_TLS  */
#line 5072 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == false));
    }
#line 10295 "nd-flow-expr.cpp"
    break;

  case 714: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_EQUAL VALUE_TRUE  */
#line 5075 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == true));
    }
#line 10303 "nd-flow-expr.cpp"
    break;

  case 715: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_EQUAL VALUE_FALSE  */
#line 5078 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls == false));
    }
#line 10311 "nd-flow-expr.cpp"
    break;

  case 716: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_NOTEQUAL VALUE_TRUE  */
#line 5081 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls != true));
    }
#line 10319 "nd-flow-expr.cpp"
    break;

  case 717: /* expr_smtp_tls: FLOW_SMTP_TLS CMP_NOTEQUAL VALUE_FALSE  */
#line 5084 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->smtp.tls != false));
    }
#line 10327 "nd-flow-expr.cpp"
    break;

  case 718: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5090 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10340 "nd-flow-expr.cpp"
    break;

  case 719: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5098 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10353 "nd-flow-expr.cpp"
    break;

  case 720: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5106 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10366 "nd-flow-expr.cpp"
    break;

  case 721: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5114 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10379 "nd-flow-expr.cpp"
    break;

  case 722: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES '>' VALUE_UNSIGNED  */
#line 5122 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10392 "nd-flow-expr.cpp"
    break;

  case 723: /* expr_total_local_bytes: FLOW_TOTAL_LOCAL_BYTES '<' VALUE_UNSIGNED  */
#line 5130 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10405 "nd-flow-expr.cpp"
    break;

  case 724: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5141 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10418 "nd-flow-expr.cpp"
    break;

  case 725: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5149 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10431 "nd-flow-expr.cpp"
    break;

  case 726: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5157 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10444 "nd-flow-expr.cpp"
    break;

  case 727: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5165 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10457 "nd-flow-expr.cpp"
    break;

  case 728: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES '>' VALUE_UNSIGNED  */
#line 5173 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10470 "nd-flow-expr.cpp"
    break;

  case 729: /* expr_total_other_bytes: FLOW_TOTAL_OTHER_BYTES '<' VALUE_UNSIGNED  */
#line 5181 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10483 "nd-flow-expr.cpp"
    break;

  case 730: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5192 "nd-flow-expr.ypp"
                                                    {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10496 "nd-flow-expr.cpp"
    break;

  case 731: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5200 "nd-flow-expr.ypp"
                                                       {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10509 "nd-flow-expr.cpp"
    break;

  case 732: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5208 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10522 "nd-flow-expr.cpp"
    break;

  case 733: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5216 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10535 "nd-flow-expr.cpp"
    break;

  case 734: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES '>' VALUE_UNSIGNED  */
#line 5224 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10548 "nd-flow-expr.cpp"
    break;

  case 735: /* expr_total_src_bytes: FLOW_TOTAL_SRC_BYTES '<' VALUE_UNSIGNED  */
#line 5232 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_bytes.load() : _NDFP_flow->stats.total_upper_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10561 "nd-flow-expr.cpp"
    break;

  case 736: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_EQUAL VALUE_UNSIGNED  */
#line 5243 "nd-flow-expr.ypp"
                                                    {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10574 "nd-flow-expr.cpp"
    break;

  case 737: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5251 "nd-flow-expr.ypp"
                                                       {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10587 "nd-flow-expr.cpp"
    break;

  case 738: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5259 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10600 "nd-flow-expr.cpp"
    break;

  case 739: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5267 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10613 "nd-flow-expr.cpp"
    break;

  case 740: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES '>' VALUE_UNSIGNED  */
#line 5275 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10626 "nd-flow-expr.cpp"
    break;

  case 741: /* expr_total_dst_bytes: FLOW_TOTAL_DST_BYTES '<' VALUE_UNSIGNED  */
#line 5283 "nd-flow-expr.ypp"
                                              {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_bytes.load() : _NDFP_flow->stats.total_lower_bytes.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10639 "nd-flow-expr.cpp"
    break;

  case 742: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5294 "nd-flow-expr.ypp"
                                                        {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10652 "nd-flow-expr.cpp"
    break;

  case 743: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5302 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10665 "nd-flow-expr.cpp"
    break;

  case 744: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5310 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10678 "nd-flow-expr.cpp"
    break;

  case 745: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5318 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10691 "nd-flow-expr.cpp"
    break;

  case 746: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS '>' VALUE_UNSIGNED  */
#line 5326 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10704 "nd-flow-expr.cpp"
    break;

  case 747: /* expr_total_local_packets: FLOW_TOTAL_LOCAL_PACKETS '<' VALUE_UNSIGNED  */
#line 5334 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10717 "nd-flow-expr.cpp"
    break;

  case 748: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5345 "nd-flow-expr.ypp"
                                                        {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10730 "nd-flow-expr.cpp"
    break;

  case 749: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5353 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10743 "nd-flow-expr.cpp"
    break;

  case 750: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5361 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10756 "nd-flow-expr.cpp"
    break;

  case 751: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5369 "nd-flow-expr.ypp"
                                                             {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10769 "nd-flow-expr.cpp"
    break;

  case 752: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS '>' VALUE_UNSIGNED  */
#line 5377 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10782 "nd-flow-expr.cpp"
    break;

  case 753: /* expr_total_other_packets: FLOW_TOTAL_OTHER_PACKETS '<' VALUE_UNSIGNED  */
#line 5385 "nd-flow-expr.ypp"
                                                  {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10795 "nd-flow-expr.cpp"
    break;

  case 754: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5396 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10808 "nd-flow-expr.cpp"
    break;

  case 755: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5404 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10821 "nd-flow-expr.cpp"
    break;

  case 756: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5412 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10834 "nd-flow-expr.cpp"
    break;

  case 757: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5420 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10847 "nd-flow-expr.cpp"
    break;

  case 758: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS '>' VALUE_UNSIGNED  */
#line 5428 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10860 "nd-flow-expr.cpp"
    break;

  case 759: /* expr_total_src_packets: FLOW_TOTAL_SRC_PACKETS '<' VALUE_UNSIGNED  */
#line 5436 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_lower_packets.load() : _NDFP_flow->stats.total_upper_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10873 "nd-flow-expr.cpp"
    break;

  case 760: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5447 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10886 "nd-flow-expr.cpp"
    break;

  case 761: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5455 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10899 "nd-flow-expr.cpp"
    break;

  case 762: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5463 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10912 "nd-flow-expr.cpp"
    break;

  case 763: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5471 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10925 "nd-flow-expr.cpp"
    break;

  case 764: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS '>' VALUE_UNSIGNED  */
#line 5479 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 10938 "nd-flow-expr.cpp"
    break;

  case 765: /* expr_total_dst_packets: FLOW_TOTAL_DST_PACKETS '<' VALUE_UNSIGNED  */
#line 5487 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.total_upper_packets.load() : _NDFP_flow->stats.total_lower_packets.load()) < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 10951 "nd-flow-expr.cpp"
    break;

  case 766: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5498 "nd-flow-expr.ypp"
                                                      {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 10963 "nd-flow-expr.cpp"
    break;

  case 767: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5505 "nd-flow-expr.ypp"
                                                         {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 10975 "nd-flow-expr.cpp"
    break;

  case 768: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5512 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 10987 "nd-flow-expr.cpp"
    break;

  case 769: /* expr_detection_packets: FLOW_DETECTION_PACKETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5519 "nd-flow-expr.ypp"
                                                           {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 10999 "nd-flow-expr.cpp"
    break;

  case 770: /* expr_detection_packets: FLOW_DETECTION_PACKETS '>' VALUE_UNSIGNED  */
#line 5526 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11011 "nd-flow-expr.cpp"
    break;

  case 771: /* expr_detection_packets: FLOW_DETECTION_PACKETS '<' VALUE_UNSIGNED  */
#line 5533 "nd-flow-expr.ypp"
                                                {
#ifndef _ND_FLOW_EXPR_DUMMY_MACRO
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.detection_packets.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11023 "nd-flow-expr.cpp"
    break;

  case 772: /* expr_local_rate: FLOW_LOCAL_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5543 "nd-flow-expr.ypp"
                                            {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11036 "nd-flow-expr.cpp"
    break;

  case 773: /* expr_local_rate: FLOW_LOCAL_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5551 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11049 "nd-flow-expr.cpp"
    break;

  case 774: /* expr_local_rate: FLOW_LOCAL_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5559 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11062 "nd-flow-expr.cpp"
    break;

  case 775: /* expr_local_rate: FLOW_LOCAL_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5567 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11075 "nd-flow-expr.cpp"
    break;

  case 776: /* expr_local_rate: FLOW_LOCAL_RATE '>' VALUE_FLOAT  */
#line 5575 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11088 "nd-flow-expr.cpp"
    break;

  case 777: /* expr_local_rate: FLOW_LOCAL_RATE '<' VALUE_FLOAT  */
#line 5583 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11101 "nd-flow-expr.cpp"
    break;

  case 778: /* expr_other_rate: FLOW_OTHER_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5594 "nd-flow-expr.ypp"
                                            {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11114 "nd-flow-expr.cpp"
    break;

  case 779: /* expr_other_rate: FLOW_OTHER_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5602 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11127 "nd-flow-expr.cpp"
    break;

  case 780: /* expr_other_rate: FLOW_OTHER_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5610 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11140 "nd-flow-expr.cpp"
    break;

  case 781: /* expr_other_rate: FLOW_OTHER_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5618 "nd-flow-expr.ypp"
                                                 {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11153 "nd-flow-expr.cpp"
    break;

  case 782: /* expr_other_rate: FLOW_OTHER_RATE '>' VALUE_FLOAT  */
#line 5626 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11166 "nd-flow-expr.cpp"
    break;

  case 783: /* expr_other_rate: FLOW_OTHER_RATE '<' VALUE_FLOAT  */
#line 5634 "nd-flow-expr.ypp"
                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->lower_map == ndFlow::LowerMap::LOCAL ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11179 "nd-flow-expr.cpp"
    break;

  case 784: /* expr_src_rate: FLOW_SRC_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5645 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11192 "nd-flow-expr.cpp"
    break;

  case 785: /* expr_src_rate: FLOW_SRC_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5653 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11205 "nd-flow-expr.cpp"
    break;

  case 786: /* expr_src_rate: FLOW_SRC_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5661 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11218 "nd-flow-expr.cpp"
    break;

  case 787: /* expr_src_rate: FLOW_SRC_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5669 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11231 "nd-flow-expr.cpp"
    break;

  case 788: /* expr_src_rate: FLOW_SRC_RATE '>' VALUE_FLOAT  */
#line 5677 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11244 "nd-flow-expr.cpp"
    break;

  case 789: /* expr_src_rate: FLOW_SRC_RATE '<' VALUE_FLOAT  */
#line 5685 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.lower_rate.load() : _NDFP_flow->stats.upper_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11257 "nd-flow-expr.cpp"
    break;

  case 790: /* expr_dst_rate: FLOW_DST_RATE CMP_EQUAL VALUE_FLOAT  */
#line 5696 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) == (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].fl_number)));
#endif
    }
#line 11270 "nd-flow-expr.cpp"
    break;

  case 791: /* expr_dst_rate: FLOW_DST_RATE CMP_NOTEQUAL VALUE_FLOAT  */
#line 5704 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) != (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].fl_number)));
#endif
    }
#line 11283 "nd-flow-expr.cpp"
    break;

  case 792: /* expr_dst_rate: FLOW_DST_RATE CMP_GTHANEQUAL VALUE_FLOAT  */
#line 5712 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) >= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].fl_number)));
#endif
    }
#line 11296 "nd-flow-expr.cpp"
    break;

  case 793: /* expr_dst_rate: FLOW_DST_RATE CMP_LTHANEQUAL VALUE_FLOAT  */
#line 5720 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) <= (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].fl_number)));
#endif
    }
#line 11309 "nd-flow-expr.cpp"
    break;

  case 794: /* expr_dst_rate: FLOW_DST_RATE '>' VALUE_FLOAT  */
#line 5728 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) > (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].fl_number)));
#endif
    }
#line 11322 "nd-flow-expr.cpp"
    break;

  case 795: /* expr_dst_rate: FLOW_DST_RATE '<' VALUE_FLOAT  */
#line 5736 "nd-flow-expr.ypp"
                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = ((_NDFP_flow->origin == ndFlow::Origin::LOWER ?
          _NDFP_flow->stats.upper_rate.load() : _NDFP_flow->stats.lower_rate.load()) < (yyvsp[0].fl_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].fl_number)));
#endif
    }
#line 11335 "nd-flow-expr.cpp"
    break;

  case 796: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_EQUAL VALUE_UNSIGNED  */
#line 5747 "nd-flow-expr.ypp"
                                                   {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11347 "nd-flow-expr.cpp"
    break;

  case 797: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5754 "nd-flow-expr.ypp"
                                                      {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11359 "nd-flow-expr.cpp"
    break;

  case 798: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5761 "nd-flow-expr.ypp"
                                                        {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11371 "nd-flow-expr.cpp"
    break;

  case 799: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5768 "nd-flow-expr.ypp"
                                                        {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11383 "nd-flow-expr.cpp"
    break;

  case 800: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS '>' VALUE_UNSIGNED  */
#line 5775 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11395 "nd-flow-expr.cpp"
    break;

  case 801: /* expr_tcp_seq_errors: FLOW_TCP_SEQ_ERRORS '<' VALUE_UNSIGNED  */
#line 5782 "nd-flow-expr.ypp"
                                             {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_seq_errors.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11407 "nd-flow-expr.cpp"
    break;

  case 802: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_EQUAL VALUE_UNSIGNED  */
#line 5792 "nd-flow-expr.ypp"
                                               {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11419 "nd-flow-expr.cpp"
    break;

  case 803: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5799 "nd-flow-expr.ypp"
                                                  {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11431 "nd-flow-expr.cpp"
    break;

  case 804: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5806 "nd-flow-expr.ypp"
                                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11443 "nd-flow-expr.cpp"
    break;

  case 805: /* expr_tcp_resets: FLOW_TCP_RESETS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5813 "nd-flow-expr.ypp"
                                                    {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11455 "nd-flow-expr.cpp"
    break;

  case 806: /* expr_tcp_resets: FLOW_TCP_RESETS '>' VALUE_UNSIGNED  */
#line 5820 "nd-flow-expr.ypp"
                                         {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11467 "nd-flow-expr.cpp"
    break;

  case 807: /* expr_tcp_resets: FLOW_TCP_RESETS '<' VALUE_UNSIGNED  */
#line 5827 "nd-flow-expr.ypp"
                                         {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_resets.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11479 "nd-flow-expr.cpp"
    break;

  case 808: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_EQUAL VALUE_UNSIGNED  */
#line 5837 "nd-flow-expr.ypp"
                                                {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() == (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 == (yyvsp[0].ul_number)));
#endif
    }
#line 11491 "nd-flow-expr.cpp"
    break;

  case 809: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 5844 "nd-flow-expr.ypp"
                                                   {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() != (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 != (yyvsp[0].ul_number)));
#endif
    }
#line 11503 "nd-flow-expr.cpp"
    break;

  case 810: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 5851 "nd-flow-expr.ypp"
                                                     {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() >= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 >= (yyvsp[0].ul_number)));
#endif
    }
#line 11515 "nd-flow-expr.cpp"
    break;

  case 811: /* expr_tcp_retrans: FLOW_TCP_RETRANS CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 5858 "nd-flow-expr.ypp"
                                                     {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() <= (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 <= (yyvsp[0].ul_number)));
#endif
    }
#line 11527 "nd-flow-expr.cpp"
    break;

  case 812: /* expr_tcp_retrans: FLOW_TCP_RETRANS '>' VALUE_UNSIGNED  */
#line 5865 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() > (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 > (yyvsp[0].ul_number)));
#endif
    }
#line 11539 "nd-flow-expr.cpp"
    break;

  case 813: /* expr_tcp_retrans: FLOW_TCP_RETRANS '<' VALUE_UNSIGNED  */
#line 5872 "nd-flow-expr.ypp"
                                          {
#ifdef _ND_ENABLE_EXTENDED_STATS
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->stats.tcp_retrans.load() < (yyvsp[0].ul_number)));
#else
        _NDFP_result = ((yyval.bool_result) = (0 < (yyvsp[0].ul_number)));
#endif
    }
#line 11551 "nd-flow-expr.cpp"
    break;

  case 814: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER  */
#line 5882 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (! _NDFP_flow->tls.alpn_server.empty()));
    }
#line 11559 "nd-flow-expr.cpp"
    break;

  case 815: /* expr_tls_alpn_server: '!' FLOW_TLS_ALPN_SERVER  */
#line 5885 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.alpn_server.empty()));
    }
#line 11567 "nd-flow-expr.cpp"
    break;

  case 816: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_EQUAL VALUE_NAME  */
#line 5888 "nd-flow-expr.ypp"
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
#line 11583 "nd-flow-expr.cpp"
    break;

  case 817: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_NOTEQUAL VALUE_NAME  */
#line 5899 "nd-flow-expr.ypp"
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
#line 11599 "nd-flow-expr.cpp"
    break;

  case 818: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_EQUAL VALUE_REGEX  */
#line 5910 "nd-flow-expr.ypp"
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
#line 11614 "nd-flow-expr.cpp"
    break;

  case 819: /* expr_tls_alpn_server: FLOW_TLS_ALPN_SERVER CMP_NOTEQUAL VALUE_REGEX  */
#line 5920 "nd-flow-expr.ypp"
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
#line 11629 "nd-flow-expr.cpp"
    break;

  case 820: /* expr_tls_cert_fingerprint: FLOW_TLS_CERT_FINGERPRINT CMP_EQUAL VALUE_NAME  */
#line 5933 "nd-flow-expr.ypp"
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
#line 11644 "nd-flow-expr.cpp"
    break;

  case 821: /* expr_tls_cert_fingerprint: FLOW_TLS_CERT_FINGERPRINT CMP_NOTEQUAL VALUE_NAME  */
#line 5943 "nd-flow-expr.ypp"
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
#line 11659 "nd-flow-expr.cpp"
    break;

  case 822: /* expr_bt_info_hash: FLOW_BT_INFO_HASH CMP_EQUAL VALUE_NAME  */
#line 5956 "nd-flow-expr.ypp"
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
#line 11674 "nd-flow-expr.cpp"
    break;

  case 823: /* expr_bt_info_hash: FLOW_BT_INFO_HASH CMP_NOTEQUAL VALUE_NAME  */
#line 5966 "nd-flow-expr.ypp"
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
#line 11689 "nd-flow-expr.cpp"
    break;

  case 824: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_EQUAL value_addr_ip  */
#line 5979 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.mapped, (yyvsp[0].buffer)) == true));
    }
#line 11697 "nd-flow-expr.cpp"
    break;

  case 825: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_NOTEQUAL value_addr_ip  */
#line 5982 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.mapped, (yyvsp[0].buffer)) == false));
    }
#line 11705 "nd-flow-expr.cpp"
    break;

  case 826: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_EQUAL VALUE_ADDR_TAG  */
#line 5985 "nd-flow-expr.ypp"
                                                {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.mapped) == true));
    }
#line 11716 "nd-flow-expr.cpp"
    break;

  case 827: /* expr_stun_mapped: FLOW_STUN_MAPPED CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 5991 "nd-flow-expr.ypp"
                                                   {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.mapped) == false));
    }
#line 11727 "nd-flow-expr.cpp"
    break;

  case 828: /* expr_stun_peer: FLOW_STUN_PEER CMP_EQUAL value_addr_ip  */
#line 6000 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.peer, (yyvsp[0].buffer)) == true));
    }
#line 11735 "nd-flow-expr.cpp"
    break;

  case 829: /* expr_stun_peer: FLOW_STUN_PEER CMP_NOTEQUAL value_addr_ip  */
#line 6003 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.peer, (yyvsp[0].buffer)) == false));
    }
#line 11743 "nd-flow-expr.cpp"
    break;

  case 830: /* expr_stun_peer: FLOW_STUN_PEER CMP_EQUAL VALUE_ADDR_TAG  */
#line 6006 "nd-flow-expr.ypp"
                                              {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.peer) == true));
    }
#line 11754 "nd-flow-expr.cpp"
    break;

  case 831: /* expr_stun_peer: FLOW_STUN_PEER CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6012 "nd-flow-expr.ypp"
                                                 {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.peer) == false));
    }
#line 11765 "nd-flow-expr.cpp"
    break;

  case 832: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_EQUAL value_addr_ip  */
#line 6021 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.relayed, (yyvsp[0].buffer)) == true));
    }
#line 11773 "nd-flow-expr.cpp"
    break;

  case 833: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_NOTEQUAL value_addr_ip  */
#line 6024 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.relayed, (yyvsp[0].buffer)) == false));
    }
#line 11781 "nd-flow-expr.cpp"
    break;

  case 834: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_EQUAL VALUE_ADDR_TAG  */
#line 6027 "nd-flow-expr.ypp"
                                                 {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.relayed) == true));
    }
#line 11792 "nd-flow-expr.cpp"
    break;

  case 835: /* expr_stun_relayed: FLOW_STUN_RELAYED CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6033 "nd-flow-expr.ypp"
                                                    {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.relayed) == false));
    }
#line 11803 "nd-flow-expr.cpp"
    break;

  case 836: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_EQUAL value_addr_ip  */
#line 6042 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.response, (yyvsp[0].buffer)) == true));
    }
#line 11811 "nd-flow-expr.cpp"
    break;

  case 837: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_NOTEQUAL value_addr_ip  */
#line 6045 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.response, (yyvsp[0].buffer)) == false));
    }
#line 11819 "nd-flow-expr.cpp"
    break;

  case 838: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_EQUAL VALUE_ADDR_TAG  */
#line 6048 "nd-flow-expr.ypp"
                                                  {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.response) == true));
    }
#line 11830 "nd-flow-expr.cpp"
    break;

  case 839: /* expr_stun_response: FLOW_STUN_RESPONSE CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6054 "nd-flow-expr.ypp"
                                                     {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.response) == false));
    }
#line 11841 "nd-flow-expr.cpp"
    break;

  case 840: /* expr_stun_other: FLOW_STUN_OTHER CMP_EQUAL value_addr_ip  */
#line 6063 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.other, (yyvsp[0].buffer)) == true));
    }
#line 11849 "nd-flow-expr.cpp"
    break;

  case 841: /* expr_stun_other: FLOW_STUN_OTHER CMP_NOTEQUAL value_addr_ip  */
#line 6066 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (is_addr_equal(&_NDFP_flow->stun.other, (yyvsp[0].buffer)) == false));
    }
#line 11857 "nd-flow-expr.cpp"
    break;

  case 842: /* expr_stun_other: FLOW_STUN_OTHER CMP_EQUAL VALUE_ADDR_TAG  */
#line 6069 "nd-flow-expr.ypp"
                                               {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.other) == true));
    }
#line 11868 "nd-flow-expr.cpp"
    break;

  case 843: /* expr_stun_other: FLOW_STUN_OTHER CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 6075 "nd-flow-expr.ypp"
                                                  {
        std::string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != std::string::npos) tag.erase(p, 1);
        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup.LookupGroupAddress(tag, _NDFP_flow->stun.other) == false));
    }
#line 11879 "nd-flow-expr.cpp"
    break;


#line 11883 "nd-flow-expr.cpp"

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

#line 6082 "nd-flow-expr.ypp"


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
