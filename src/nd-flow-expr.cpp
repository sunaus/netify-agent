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

#include "nd-flow-parser.hpp"
#include "nd-flow-expr.hpp"

using namespace std;

using json = nlohmann::json;

extern "C" {
    #include "nd-flow-criteria.h"

    void yyerror(YYLTYPE *yyllocp, yyscan_t scanner, const char *message);
}

void yyerror(YYLTYPE *yyllocp, yyscan_t scanner, const char *message) {
    throw string(message);
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


#line 191 "nd-flow-expr.cpp"




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
#line 128 "nd-flow-expr.ypp"

typedef void* yyscan_t;

#line 233 "nd-flow-expr.cpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FLOW_IP_PROTO = 258,           /* FLOW_IP_PROTO  */
    FLOW_IP_VERSION = 259,         /* FLOW_IP_VERSION  */
    FLOW_IP_DSCP = 260,            /* FLOW_IP_DSCP  */
    FLOW_VLAN_ID = 261,            /* FLOW_VLAN_ID  */
    FLOW_OTHER_TYPE = 262,         /* FLOW_OTHER_TYPE  */
    FLOW_LOCAL_MAC = 263,          /* FLOW_LOCAL_MAC  */
    FLOW_OTHER_MAC = 264,          /* FLOW_OTHER_MAC  */
    FLOW_LOCAL_IP = 265,           /* FLOW_LOCAL_IP  */
    FLOW_OTHER_IP = 266,           /* FLOW_OTHER_IP  */
    FLOW_LOCAL_PORT = 267,         /* FLOW_LOCAL_PORT  */
    FLOW_OTHER_PORT = 268,         /* FLOW_OTHER_PORT  */
    FLOW_TUNNEL_TYPE = 269,        /* FLOW_TUNNEL_TYPE  */
    FLOW_DETECTION_COMPLETE = 270, /* FLOW_DETECTION_COMPLETE  */
    FLOW_DETECTION_GUESSED = 271,  /* FLOW_DETECTION_GUESSED  */
    FLOW_DETECTION_INIT = 272,     /* FLOW_DETECTION_INIT  */
    FLOW_DETECTION_UPDATED = 273,  /* FLOW_DETECTION_UPDATED  */
    FLOW_DHC_HIT = 274,            /* FLOW_DHC_HIT  */
    FLOW_FHC_HIT = 275,            /* FLOW_FHC_HIT  */
    FLOW_IP_NAT = 276,             /* FLOW_IP_NAT  */
    FLOW_EXPIRING = 277,           /* FLOW_EXPIRING  */
    FLOW_EXPIRED = 278,            /* FLOW_EXPIRED  */
    FLOW_SOFT_DISSECTOR = 279,     /* FLOW_SOFT_DISSECTOR  */
    FLOW_RISKS = 280,              /* FLOW_RISKS  */
    FLOW_NDPI_RISK_SCORE = 281,    /* FLOW_NDPI_RISK_SCORE  */
    FLOW_NDPI_RISK_SCORE_CLIENT = 282, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
    FLOW_NDPI_RISK_SCORE_SERVER = 283, /* FLOW_NDPI_RISK_SCORE_SERVER  */
    FLOW_CATEGORY = 284,           /* FLOW_CATEGORY  */
    FLOW_CATEGORY_ID = 285,        /* FLOW_CATEGORY_ID  */
    FLOW_APPLICATION = 286,        /* FLOW_APPLICATION  */
    FLOW_APPLICATION_CATEGORY = 287, /* FLOW_APPLICATION_CATEGORY  */
    FLOW_APPLICATION_CATEGORY_ID = 288, /* FLOW_APPLICATION_CATEGORY_ID  */
    FLOW_DOMAIN_CATEGORY = 289,    /* FLOW_DOMAIN_CATEGORY  */
    FLOW_DOMAIN_CATEGORY_ID = 290, /* FLOW_DOMAIN_CATEGORY_ID  */
    FLOW_NETWORK_CATEGORY = 291,   /* FLOW_NETWORK_CATEGORY  */
    FLOW_NETWORK_CATEGORY_ID = 292, /* FLOW_NETWORK_CATEGORY_ID  */
    FLOW_PROTOCOL = 293,           /* FLOW_PROTOCOL  */
    FLOW_PROTOCOL_CATEGORY = 294,  /* FLOW_PROTOCOL_CATEGORY  */
    FLOW_PROTOCOL_CATEGORY_ID = 295, /* FLOW_PROTOCOL_CATEGORY_ID  */
    FLOW_DETECTED_HOSTNAME = 296,  /* FLOW_DETECTED_HOSTNAME  */
    FLOW_DNS_HOSTNAME = 297,       /* FLOW_DNS_HOSTNAME  */
    FLOW_ORIGIN = 298,             /* FLOW_ORIGIN  */
    FLOW_CT_MARK = 299,            /* FLOW_CT_MARK  */
    FLOW_TLS_VERSION = 300,        /* FLOW_TLS_VERSION  */
    FLOW_TLS_CIPHER = 301,         /* FLOW_TLS_CIPHER  */
    FLOW_TLS_ECH = 302,            /* FLOW_TLS_ECH  */
    FLOW_TLS_JA4 = 303,            /* FLOW_TLS_JA4  */
    FLOW_IFACE = 304,              /* FLOW_IFACE  */
    FLOW_IFACE_NFQ_SRC = 305,      /* FLOW_IFACE_NFQ_SRC  */
    FLOW_IFACE_NFQ_DST = 306,      /* FLOW_IFACE_NFQ_DST  */
    FLOW_OTHER_UNKNOWN = 307,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 308,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 309,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 310,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 311,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 312,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 313,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 314,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 315,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 316,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 317,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 318,         /* FLOW_TUNNEL_GTP  */
    FLOW_INTEL = 319,              /* FLOW_INTEL  */
    CMP_EQUAL = 320,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 321,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 322,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 323,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 324,                /* BOOL_AND  */
    BOOL_OR = 325,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 326,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 327,              /* VALUE_TRUE  */
    VALUE_FALSE = 328,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 329,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 330,              /* VALUE_NAME  */
    VALUE_REGEX = 331,             /* VALUE_REGEX  */
    VALUE_ADDR_IPV4 = 332,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 333,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 334,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 335,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 336,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 337,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 338              /* VALUE_FLOAT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define FLOW_IP_PROTO 258
#define FLOW_IP_VERSION 259
#define FLOW_IP_DSCP 260
#define FLOW_VLAN_ID 261
#define FLOW_OTHER_TYPE 262
#define FLOW_LOCAL_MAC 263
#define FLOW_OTHER_MAC 264
#define FLOW_LOCAL_IP 265
#define FLOW_OTHER_IP 266
#define FLOW_LOCAL_PORT 267
#define FLOW_OTHER_PORT 268
#define FLOW_TUNNEL_TYPE 269
#define FLOW_DETECTION_COMPLETE 270
#define FLOW_DETECTION_GUESSED 271
#define FLOW_DETECTION_INIT 272
#define FLOW_DETECTION_UPDATED 273
#define FLOW_DHC_HIT 274
#define FLOW_FHC_HIT 275
#define FLOW_IP_NAT 276
#define FLOW_EXPIRING 277
#define FLOW_EXPIRED 278
#define FLOW_SOFT_DISSECTOR 279
#define FLOW_RISKS 280
#define FLOW_NDPI_RISK_SCORE 281
#define FLOW_NDPI_RISK_SCORE_CLIENT 282
#define FLOW_NDPI_RISK_SCORE_SERVER 283
#define FLOW_CATEGORY 284
#define FLOW_CATEGORY_ID 285
#define FLOW_APPLICATION 286
#define FLOW_APPLICATION_CATEGORY 287
#define FLOW_APPLICATION_CATEGORY_ID 288
#define FLOW_DOMAIN_CATEGORY 289
#define FLOW_DOMAIN_CATEGORY_ID 290
#define FLOW_NETWORK_CATEGORY 291
#define FLOW_NETWORK_CATEGORY_ID 292
#define FLOW_PROTOCOL 293
#define FLOW_PROTOCOL_CATEGORY 294
#define FLOW_PROTOCOL_CATEGORY_ID 295
#define FLOW_DETECTED_HOSTNAME 296
#define FLOW_DNS_HOSTNAME 297
#define FLOW_ORIGIN 298
#define FLOW_CT_MARK 299
#define FLOW_TLS_VERSION 300
#define FLOW_TLS_CIPHER 301
#define FLOW_TLS_ECH 302
#define FLOW_TLS_JA4 303
#define FLOW_IFACE 304
#define FLOW_IFACE_NFQ_SRC 305
#define FLOW_IFACE_NFQ_DST 306
#define FLOW_OTHER_UNKNOWN 307
#define FLOW_OTHER_UNSUPPORTED 308
#define FLOW_OTHER_LOCAL 309
#define FLOW_OTHER_MULTICAST 310
#define FLOW_OTHER_BROADCAST 311
#define FLOW_OTHER_REMOTE 312
#define FLOW_OTHER_ERROR 313
#define FLOW_ORIGIN_LOCAL 314
#define FLOW_ORIGIN_OTHER 315
#define FLOW_ORIGIN_UNKNOWN 316
#define FLOW_TUNNEL_NONE 317
#define FLOW_TUNNEL_GTP 318
#define FLOW_INTEL 319
#define CMP_EQUAL 320
#define CMP_NOTEQUAL 321
#define CMP_GTHANEQUAL 322
#define CMP_LTHANEQUAL 323
#define BOOL_AND 324
#define BOOL_OR 325
#define VALUE_ADDR_IPMASK 326
#define VALUE_TRUE 327
#define VALUE_FALSE 328
#define VALUE_ADDR_MAC 329
#define VALUE_NAME 330
#define VALUE_REGEX 331
#define VALUE_ADDR_IPV4 332
#define VALUE_ADDR_IPV4_CIDR 333
#define VALUE_ADDR_IPV6 334
#define VALUE_ADDR_IPV6_CIDR 335
#define VALUE_SIGNED 336
#define VALUE_UNSIGNED 337
#define VALUE_FLOAT 338

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 135 "nd-flow-expr.ypp"

    char buffer[_NDFP_MAX_BUFLEN];

    bool bool_number;
    unsigned short us_number;
    long sl_number;
    unsigned long ul_number;
    float fl_number;

    bool bool_result;

#line 431 "nd-flow-expr.cpp"

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
  YYSYMBOL_FLOW_IP_PROTO = 3,              /* FLOW_IP_PROTO  */
  YYSYMBOL_FLOW_IP_VERSION = 4,            /* FLOW_IP_VERSION  */
  YYSYMBOL_FLOW_IP_DSCP = 5,               /* FLOW_IP_DSCP  */
  YYSYMBOL_FLOW_VLAN_ID = 6,               /* FLOW_VLAN_ID  */
  YYSYMBOL_FLOW_OTHER_TYPE = 7,            /* FLOW_OTHER_TYPE  */
  YYSYMBOL_FLOW_LOCAL_MAC = 8,             /* FLOW_LOCAL_MAC  */
  YYSYMBOL_FLOW_OTHER_MAC = 9,             /* FLOW_OTHER_MAC  */
  YYSYMBOL_FLOW_LOCAL_IP = 10,             /* FLOW_LOCAL_IP  */
  YYSYMBOL_FLOW_OTHER_IP = 11,             /* FLOW_OTHER_IP  */
  YYSYMBOL_FLOW_LOCAL_PORT = 12,           /* FLOW_LOCAL_PORT  */
  YYSYMBOL_FLOW_OTHER_PORT = 13,           /* FLOW_OTHER_PORT  */
  YYSYMBOL_FLOW_TUNNEL_TYPE = 14,          /* FLOW_TUNNEL_TYPE  */
  YYSYMBOL_FLOW_DETECTION_COMPLETE = 15,   /* FLOW_DETECTION_COMPLETE  */
  YYSYMBOL_FLOW_DETECTION_GUESSED = 16,    /* FLOW_DETECTION_GUESSED  */
  YYSYMBOL_FLOW_DETECTION_INIT = 17,       /* FLOW_DETECTION_INIT  */
  YYSYMBOL_FLOW_DETECTION_UPDATED = 18,    /* FLOW_DETECTION_UPDATED  */
  YYSYMBOL_FLOW_DHC_HIT = 19,              /* FLOW_DHC_HIT  */
  YYSYMBOL_FLOW_FHC_HIT = 20,              /* FLOW_FHC_HIT  */
  YYSYMBOL_FLOW_IP_NAT = 21,               /* FLOW_IP_NAT  */
  YYSYMBOL_FLOW_EXPIRING = 22,             /* FLOW_EXPIRING  */
  YYSYMBOL_FLOW_EXPIRED = 23,              /* FLOW_EXPIRED  */
  YYSYMBOL_FLOW_SOFT_DISSECTOR = 24,       /* FLOW_SOFT_DISSECTOR  */
  YYSYMBOL_FLOW_RISKS = 25,                /* FLOW_RISKS  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE = 26,      /* FLOW_NDPI_RISK_SCORE  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_CLIENT = 27, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_SERVER = 28, /* FLOW_NDPI_RISK_SCORE_SERVER  */
  YYSYMBOL_FLOW_CATEGORY = 29,             /* FLOW_CATEGORY  */
  YYSYMBOL_FLOW_CATEGORY_ID = 30,          /* FLOW_CATEGORY_ID  */
  YYSYMBOL_FLOW_APPLICATION = 31,          /* FLOW_APPLICATION  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY = 32, /* FLOW_APPLICATION_CATEGORY  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY_ID = 33, /* FLOW_APPLICATION_CATEGORY_ID  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY = 34,      /* FLOW_DOMAIN_CATEGORY  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY_ID = 35,   /* FLOW_DOMAIN_CATEGORY_ID  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY = 36,     /* FLOW_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY_ID = 37,  /* FLOW_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_PROTOCOL = 38,             /* FLOW_PROTOCOL  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY = 39,    /* FLOW_PROTOCOL_CATEGORY  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY_ID = 40, /* FLOW_PROTOCOL_CATEGORY_ID  */
  YYSYMBOL_FLOW_DETECTED_HOSTNAME = 41,    /* FLOW_DETECTED_HOSTNAME  */
  YYSYMBOL_FLOW_DNS_HOSTNAME = 42,         /* FLOW_DNS_HOSTNAME  */
  YYSYMBOL_FLOW_ORIGIN = 43,               /* FLOW_ORIGIN  */
  YYSYMBOL_FLOW_CT_MARK = 44,              /* FLOW_CT_MARK  */
  YYSYMBOL_FLOW_TLS_VERSION = 45,          /* FLOW_TLS_VERSION  */
  YYSYMBOL_FLOW_TLS_CIPHER = 46,           /* FLOW_TLS_CIPHER  */
  YYSYMBOL_FLOW_TLS_ECH = 47,              /* FLOW_TLS_ECH  */
  YYSYMBOL_FLOW_TLS_JA4 = 48,              /* FLOW_TLS_JA4  */
  YYSYMBOL_FLOW_IFACE = 49,                /* FLOW_IFACE  */
  YYSYMBOL_FLOW_IFACE_NFQ_SRC = 50,        /* FLOW_IFACE_NFQ_SRC  */
  YYSYMBOL_FLOW_IFACE_NFQ_DST = 51,        /* FLOW_IFACE_NFQ_DST  */
  YYSYMBOL_FLOW_OTHER_UNKNOWN = 52,        /* FLOW_OTHER_UNKNOWN  */
  YYSYMBOL_FLOW_OTHER_UNSUPPORTED = 53,    /* FLOW_OTHER_UNSUPPORTED  */
  YYSYMBOL_FLOW_OTHER_LOCAL = 54,          /* FLOW_OTHER_LOCAL  */
  YYSYMBOL_FLOW_OTHER_MULTICAST = 55,      /* FLOW_OTHER_MULTICAST  */
  YYSYMBOL_FLOW_OTHER_BROADCAST = 56,      /* FLOW_OTHER_BROADCAST  */
  YYSYMBOL_FLOW_OTHER_REMOTE = 57,         /* FLOW_OTHER_REMOTE  */
  YYSYMBOL_FLOW_OTHER_ERROR = 58,          /* FLOW_OTHER_ERROR  */
  YYSYMBOL_FLOW_ORIGIN_LOCAL = 59,         /* FLOW_ORIGIN_LOCAL  */
  YYSYMBOL_FLOW_ORIGIN_OTHER = 60,         /* FLOW_ORIGIN_OTHER  */
  YYSYMBOL_FLOW_ORIGIN_UNKNOWN = 61,       /* FLOW_ORIGIN_UNKNOWN  */
  YYSYMBOL_FLOW_TUNNEL_NONE = 62,          /* FLOW_TUNNEL_NONE  */
  YYSYMBOL_FLOW_TUNNEL_GTP = 63,           /* FLOW_TUNNEL_GTP  */
  YYSYMBOL_FLOW_INTEL = 64,                /* FLOW_INTEL  */
  YYSYMBOL_CMP_EQUAL = 65,                 /* CMP_EQUAL  */
  YYSYMBOL_CMP_NOTEQUAL = 66,              /* CMP_NOTEQUAL  */
  YYSYMBOL_CMP_GTHANEQUAL = 67,            /* CMP_GTHANEQUAL  */
  YYSYMBOL_CMP_LTHANEQUAL = 68,            /* CMP_LTHANEQUAL  */
  YYSYMBOL_BOOL_AND = 69,                  /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 70,                   /* BOOL_OR  */
  YYSYMBOL_VALUE_ADDR_IPMASK = 71,         /* VALUE_ADDR_IPMASK  */
  YYSYMBOL_VALUE_TRUE = 72,                /* VALUE_TRUE  */
  YYSYMBOL_VALUE_FALSE = 73,               /* VALUE_FALSE  */
  YYSYMBOL_VALUE_ADDR_MAC = 74,            /* VALUE_ADDR_MAC  */
  YYSYMBOL_VALUE_NAME = 75,                /* VALUE_NAME  */
  YYSYMBOL_VALUE_REGEX = 76,               /* VALUE_REGEX  */
  YYSYMBOL_VALUE_ADDR_IPV4 = 77,           /* VALUE_ADDR_IPV4  */
  YYSYMBOL_VALUE_ADDR_IPV4_CIDR = 78,      /* VALUE_ADDR_IPV4_CIDR  */
  YYSYMBOL_VALUE_ADDR_IPV6 = 79,           /* VALUE_ADDR_IPV6  */
  YYSYMBOL_VALUE_ADDR_IPV6_CIDR = 80,      /* VALUE_ADDR_IPV6_CIDR  */
  YYSYMBOL_VALUE_SIGNED = 81,              /* VALUE_SIGNED  */
  YYSYMBOL_VALUE_UNSIGNED = 82,            /* VALUE_UNSIGNED  */
  YYSYMBOL_VALUE_FLOAT = 83,               /* VALUE_FLOAT  */
  YYSYMBOL_84_ = 84,                       /* ';'  */
  YYSYMBOL_85_ = 85,                       /* '('  */
  YYSYMBOL_86_ = 86,                       /* ')'  */
  YYSYMBOL_87_ = 87,                       /* '!'  */
  YYSYMBOL_88_ = 88,                       /* '>'  */
  YYSYMBOL_89_ = 89,                       /* '<'  */
  YYSYMBOL_YYACCEPT = 90,                  /* $accept  */
  YYSYMBOL_exprs = 91,                     /* exprs  */
  YYSYMBOL_expr = 92,                      /* expr  */
  YYSYMBOL_expr_ip_proto = 93,             /* expr_ip_proto  */
  YYSYMBOL_expr_ip_dscp = 94,              /* expr_ip_dscp  */
  YYSYMBOL_expr_ip_version = 95,           /* expr_ip_version  */
  YYSYMBOL_expr_vlan_id = 96,              /* expr_vlan_id  */
  YYSYMBOL_expr_other_type = 97,           /* expr_other_type  */
  YYSYMBOL_value_other_type = 98,          /* value_other_type  */
  YYSYMBOL_expr_local_mac = 99,            /* expr_local_mac  */
  YYSYMBOL_expr_other_mac = 100,           /* expr_other_mac  */
  YYSYMBOL_expr_local_ip = 101,            /* expr_local_ip  */
  YYSYMBOL_expr_other_ip = 102,            /* expr_other_ip  */
  YYSYMBOL_value_addr_ip = 103,            /* value_addr_ip  */
  YYSYMBOL_expr_local_port = 104,          /* expr_local_port  */
  YYSYMBOL_expr_other_port = 105,          /* expr_other_port  */
  YYSYMBOL_expr_tunnel_type = 106,         /* expr_tunnel_type  */
  YYSYMBOL_value_tunnel_type = 107,        /* value_tunnel_type  */
  YYSYMBOL_expr_detection_complete = 108,  /* expr_detection_complete  */
  YYSYMBOL_expr_detection_guessed = 109,   /* expr_detection_guessed  */
  YYSYMBOL_expr_detection_init = 110,      /* expr_detection_init  */
  YYSYMBOL_expr_detection_updated = 111,   /* expr_detection_updated  */
  YYSYMBOL_expr_dhc_hit = 112,             /* expr_dhc_hit  */
  YYSYMBOL_expr_fhc_hit = 113,             /* expr_fhc_hit  */
  YYSYMBOL_expr_ip_nat = 114,              /* expr_ip_nat  */
  YYSYMBOL_expr_expiring = 115,            /* expr_expiring  */
  YYSYMBOL_expr_expired = 116,             /* expr_expired  */
  YYSYMBOL_expr_soft_dissector = 117,      /* expr_soft_dissector  */
  YYSYMBOL_expr_app = 118,                 /* expr_app  */
  YYSYMBOL_expr_app_id = 119,              /* expr_app_id  */
  YYSYMBOL_expr_app_name = 120,            /* expr_app_name  */
  YYSYMBOL_expr_category = 121,            /* expr_category  */
  YYSYMBOL_expr_category_id = 122,         /* expr_category_id  */
  YYSYMBOL_expr_app_category = 123,        /* expr_app_category  */
  YYSYMBOL_expr_app_category_id = 124,     /* expr_app_category_id  */
  YYSYMBOL_expr_domain_category = 125,     /* expr_domain_category  */
  YYSYMBOL_expr_domain_category_id = 126,  /* expr_domain_category_id  */
  YYSYMBOL_expr_network_category = 127,    /* expr_network_category  */
  YYSYMBOL_expr_network_category_id = 128, /* expr_network_category_id  */
  YYSYMBOL_expr_proto = 129,               /* expr_proto  */
  YYSYMBOL_expr_proto_id = 130,            /* expr_proto_id  */
  YYSYMBOL_expr_proto_name = 131,          /* expr_proto_name  */
  YYSYMBOL_expr_proto_category = 132,      /* expr_proto_category  */
  YYSYMBOL_expr_proto_category_id = 133,   /* expr_proto_category_id  */
  YYSYMBOL_expr_detected_hostname = 134,   /* expr_detected_hostname  */
  YYSYMBOL_expr_dns_hostname = 135,        /* expr_dns_hostname  */
  YYSYMBOL_expr_risks = 136,               /* expr_risks  */
  YYSYMBOL_expr_risk_ndpi_score = 137,     /* expr_risk_ndpi_score  */
  YYSYMBOL_expr_risk_ndpi_score_client = 138, /* expr_risk_ndpi_score_client  */
  YYSYMBOL_expr_risk_ndpi_score_server = 139, /* expr_risk_ndpi_score_server  */
  YYSYMBOL_expr_fwmark = 140,              /* expr_fwmark  */
  YYSYMBOL_expr_iface = 141,               /* expr_iface  */
  YYSYMBOL_expr_iface_nfq_src = 142,       /* expr_iface_nfq_src  */
  YYSYMBOL_expr_iface_nfq_dst = 143,       /* expr_iface_nfq_dst  */
  YYSYMBOL_expr_intel = 144,               /* expr_intel  */
  YYSYMBOL_expr_tls_version = 145,         /* expr_tls_version  */
  YYSYMBOL_expr_tls_cipher = 146,          /* expr_tls_cipher  */
  YYSYMBOL_expr_tls_ech = 147,             /* expr_tls_ech  */
  YYSYMBOL_expr_tls_ja4 = 148,             /* expr_tls_ja4  */
  YYSYMBOL_expr_origin = 149,              /* expr_origin  */
  YYSYMBOL_value_origin_type = 150         /* value_origin_type  */
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
#define YYLAST   520

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  90
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  350
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  520

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   338


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    87,     2,     2,     2,     2,     2,     2,
      85,    86,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    84,
      89,     2,    88,     2,     2,     2,     2,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    82,    83
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   209,   209,   211,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   269,   273,   277,   282,   286,
     290,   294,   298,   302,   306,   313,   317,   324,   328,   335,
     339,   343,   347,   351,   355,   359,   363,   370,   376,   382,
     426,   473,   474,   475,   476,   477,   478,   479,   483,   489,
     498,   504,   513,   519,   528,   534,   543,   544,   545,   546,
     550,   554,   558,   562,   566,   570,   574,   578,   585,   589,
     593,   597,   601,   605,   609,   613,   620,   626,   632,   651,
     673,   674,   677,   681,   687,   695,   703,   711,   722,   726,
     732,   740,   748,   756,   767,   771,   777,   785,   793,   801,
     812,   816,   822,   830,   838,   846,   857,   861,   867,   875,
     883,   891,   902,   906,   912,   920,   928,   936,   947,   951,
     955,   959,   963,   967,   974,   978,   982,   986,   990,   994,
    1001,  1005,  1009,  1013,  1017,  1021,  1028,  1032,  1036,  1040,
    1044,  1048,  1055,  1061,  1069,  1070,  1073,  1082,  1094,  1121,
    1151,  1189,  1226,  1248,  1273,  1289,  1308,  1315,  1325,  1341,
    1360,  1367,  1377,  1393,  1412,  1419,  1429,  1435,  1441,  1442,
    1445,  1451,  1460,  1480,  1502,  1519,  1539,  1547,  1558,  1565,
    1572,  1591,  1610,  1624,  1633,  1640,  1647,  1666,  1685,  1699,
    1708,  1712,  1716,  1735,  1754,  1767,  1783,  1787,  1791,  1795,
    1799,  1803,  1807,  1811,  1818,  1822,  1826,  1830,  1834,  1838,
    1842,  1846,  1853,  1857,  1861,  1865,  1869,  1873,  1877,  1881,
    1888,  1896,  1904,  1912,  1920,  1928,  1936,  1944,  1955,  1962,
    1969,  1987,  2005,  2019,  2028,  2039,  2050,  2071,  2090,  2104,
    2121,  2132,  2143,  2162,  2181,  2196,  2213,  2234,  2256,  2276,
    2291,  2303,  2315,  2327,  2340,  2355,  2375,  2390,  2402,  2414,
    2426,  2439,  2454,  2466,  2478,  2492,  2504,  2516,  2530,  2542,
    2554,  2568,  2580,  2592,  2607,  2611,  2615,  2619,  2623,  2627,
    2631,  2635,  2642,  2646,  2650,  2654,  2658,  2662,  2666,  2670,
    2677,  2681,  2685,  2689,  2693,  2697,  2701,  2705,  2712,  2719,
    2726,  2745,  2764,  2778,  2795,  2799,  2803,  2807,  2814,  2815,
    2816
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
  "\"end of file\"", "error", "\"invalid token\"", "FLOW_IP_PROTO",
  "FLOW_IP_VERSION", "FLOW_IP_DSCP", "FLOW_VLAN_ID", "FLOW_OTHER_TYPE",
  "FLOW_LOCAL_MAC", "FLOW_OTHER_MAC", "FLOW_LOCAL_IP", "FLOW_OTHER_IP",
  "FLOW_LOCAL_PORT", "FLOW_OTHER_PORT", "FLOW_TUNNEL_TYPE",
  "FLOW_DETECTION_COMPLETE", "FLOW_DETECTION_GUESSED",
  "FLOW_DETECTION_INIT", "FLOW_DETECTION_UPDATED", "FLOW_DHC_HIT",
  "FLOW_FHC_HIT", "FLOW_IP_NAT", "FLOW_EXPIRING", "FLOW_EXPIRED",
  "FLOW_SOFT_DISSECTOR", "FLOW_RISKS", "FLOW_NDPI_RISK_SCORE",
  "FLOW_NDPI_RISK_SCORE_CLIENT", "FLOW_NDPI_RISK_SCORE_SERVER",
  "FLOW_CATEGORY", "FLOW_CATEGORY_ID", "FLOW_APPLICATION",
  "FLOW_APPLICATION_CATEGORY", "FLOW_APPLICATION_CATEGORY_ID",
  "FLOW_DOMAIN_CATEGORY", "FLOW_DOMAIN_CATEGORY_ID",
  "FLOW_NETWORK_CATEGORY", "FLOW_NETWORK_CATEGORY_ID", "FLOW_PROTOCOL",
  "FLOW_PROTOCOL_CATEGORY", "FLOW_PROTOCOL_CATEGORY_ID",
  "FLOW_DETECTED_HOSTNAME", "FLOW_DNS_HOSTNAME", "FLOW_ORIGIN",
  "FLOW_CT_MARK", "FLOW_TLS_VERSION", "FLOW_TLS_CIPHER", "FLOW_TLS_ECH",
  "FLOW_TLS_JA4", "FLOW_IFACE", "FLOW_IFACE_NFQ_SRC", "FLOW_IFACE_NFQ_DST",
  "FLOW_OTHER_UNKNOWN", "FLOW_OTHER_UNSUPPORTED", "FLOW_OTHER_LOCAL",
  "FLOW_OTHER_MULTICAST", "FLOW_OTHER_BROADCAST", "FLOW_OTHER_REMOTE",
  "FLOW_OTHER_ERROR", "FLOW_ORIGIN_LOCAL", "FLOW_ORIGIN_OTHER",
  "FLOW_ORIGIN_UNKNOWN", "FLOW_TUNNEL_NONE", "FLOW_TUNNEL_GTP",
  "FLOW_INTEL", "CMP_EQUAL", "CMP_NOTEQUAL", "CMP_GTHANEQUAL",
  "CMP_LTHANEQUAL", "BOOL_AND", "BOOL_OR", "VALUE_ADDR_IPMASK",
  "VALUE_TRUE", "VALUE_FALSE", "VALUE_ADDR_MAC", "VALUE_NAME",
  "VALUE_REGEX", "VALUE_ADDR_IPV4", "VALUE_ADDR_IPV4_CIDR",
  "VALUE_ADDR_IPV6", "VALUE_ADDR_IPV6_CIDR", "VALUE_SIGNED",
  "VALUE_UNSIGNED", "VALUE_FLOAT", "';'", "'('", "')'", "'!'", "'>'",
  "'<'", "$accept", "exprs", "expr", "expr_ip_proto", "expr_ip_dscp",
  "expr_ip_version", "expr_vlan_id", "expr_other_type", "value_other_type",
  "expr_local_mac", "expr_other_mac", "expr_local_ip", "expr_other_ip",
  "value_addr_ip", "expr_local_port", "expr_other_port",
  "expr_tunnel_type", "value_tunnel_type", "expr_detection_complete",
  "expr_detection_guessed", "expr_detection_init",
  "expr_detection_updated", "expr_dhc_hit", "expr_fhc_hit", "expr_ip_nat",
  "expr_expiring", "expr_expired", "expr_soft_dissector", "expr_app",
  "expr_app_id", "expr_app_name", "expr_category", "expr_category_id",
  "expr_app_category", "expr_app_category_id", "expr_domain_category",
  "expr_domain_category_id", "expr_network_category",
  "expr_network_category_id", "expr_proto", "expr_proto_id",
  "expr_proto_name", "expr_proto_category", "expr_proto_category_id",
  "expr_detected_hostname", "expr_dns_hostname", "expr_risks",
  "expr_risk_ndpi_score", "expr_risk_ndpi_score_client",
  "expr_risk_ndpi_score_server", "expr_fwmark", "expr_iface",
  "expr_iface_nfq_src", "expr_iface_nfq_dst", "expr_intel",
  "expr_tls_version", "expr_tls_cipher", "expr_tls_ech", "expr_tls_ja4",
  "expr_origin", "value_origin_type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-64)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -64,     1,   -64,   -12,   -63,    13,    -8,    73,    78,   151,
     165,   180,    -4,   126,   201,   242,   244,   246,   248,   250,
     252,   254,   256,   258,   260,   262,   130,   134,   140,   264,
     266,   268,     9,   270,   272,   274,   276,   278,   280,   282,
     284,   286,   288,   290,   144,   159,   169,   176,   292,   294,
     296,   298,   184,    86,   139,     3,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
       0,    67,    92,    96,   122,   156,   174,   179,   189,   194,
     200,   215,   293,   352,   354,   356,   234,   234,   147,   361,
     365,   366,   216,   216,   216,   216,   359,   360,   362,   364,
     367,   368,   369,   370,   371,   372,   373,   374,   303,   303,
     295,   297,   299,   301,   304,   306,   308,   310,   312,   314,
     316,   318,   320,   322,   324,   326,   328,   330,   332,   334,
      -7,    65,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     400,   401,   395,   396,    66,    93,   404,   398,    94,   406,
     407,   402,   403,   408,   411,   405,   409,    97,   138,   413,
     414,   410,   412,   333,   335,   337,   339,    10,    10,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   341,   343,   345,   347,   349,   351,   353,
     355,   187,   202,   172,   198,   217,   220,    -3,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,    86,    86,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   363,   363
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,    57,     0,     0,    69,    77,     0,     0,
       0,     0,   100,   108,   116,   122,   128,   134,   140,   146,
     152,   158,   164,   170,   176,   230,   236,   244,   252,     0,
       0,   182,     0,     0,     0,     0,     0,     0,   206,     0,
       0,   218,   224,   344,   260,   314,   322,   330,   338,   268,
     274,   280,   286,     0,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    28,   184,
     185,    26,    27,    29,    30,    31,    32,    33,    34,    35,
     208,   209,    36,    37,    38,    39,    40,    41,    42,    43,
      49,    50,    51,    52,    53,    44,    45,    46,    47,    48,
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
       0,     0,     0,     0,     0,     0,     0,     0,    58,    70,
      78,   101,   109,   117,   123,   129,   135,   141,   147,   153,
     159,   165,   171,   177,   231,   237,   245,   253,   183,   207,
     219,   225,   345,   261,   315,   323,   331,   339,   269,   275,
     281,   287,     0,     0,     3,    59,    60,    61,    62,    63,
      64,    67,    68,    65,    66,    71,    72,    73,    74,    75,
      76,    81,    82,    83,    84,    85,    86,    87,    79,    80,
      88,    89,    90,    91,    96,    97,    98,    99,    92,    93,
      94,    95,   102,   103,   104,   105,   106,   107,   110,   111,
     112,   113,   114,   115,   120,   121,   118,   119,   124,   125,
     126,   127,   130,   131,   132,   133,   136,   137,   138,   139,
     142,   143,   144,   145,   148,   149,   150,   151,   154,   155,
     156,   157,   160,   161,   162,   163,   166,   167,   168,   169,
     172,   173,   174,   175,   178,   179,   180,   181,   232,   234,
     233,   235,   238,   239,   240,   241,   242,   243,   246,   247,
     248,   249,   250,   251,   254,   255,   256,   257,   258,   259,
     190,   191,   192,   193,   188,   186,   189,   187,   194,   196,
     195,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     212,   210,   213,   211,   214,   215,   216,   217,   220,   222,
     221,   223,   226,   228,   227,   229,   348,   349,   350,   346,
     347,   262,   263,   264,   265,   266,   267,   316,   317,   318,
     319,   320,   321,   324,   325,   326,   327,   328,   329,   332,
     333,   334,   335,   336,   337,   340,   342,   341,   343,   270,
     272,   271,   273,   276,   278,   277,   279,   282,   284,   283,
     285,   293,   294,   288,   289,   291,   290,   292,   300,   301,
     295,   296,   298,   297,   299,   303,   302,   304,   306,   305,
     307,   309,   308,   310,   312,   311,   313,    56,    55,    54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -64,   -64,   -53,   -64,   -64,   -64,   -64,   -64,   321,   -64,
     -64,   -64,   -64,   171,   -64,   -64,   -64,   344,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     219
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    55,    56,    57,    58,    59,    60,   318,    61,
      62,    63,    64,   328,    65,    66,    67,   346,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     449
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     257,     2,   116,   117,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,   110,   111,   112,   113,   120,   121,   122,
     123,   136,   137,   138,   139,    52,   292,   293,   388,   446,
     447,   448,   292,   293,   196,   389,   114,   115,   118,   119,
     124,   125,   295,   517,   140,   141,    53,   294,    54,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,   126,   127,
     390,   414,   258,   128,   129,   259,   260,   391,   415,   296,
      52,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   416,   420,
     278,    53,   430,    54,   297,   417,   421,   279,   298,   431,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   142,   143,   144,   145,   172,   173,   174,   175,   178,
     179,   180,   181,   291,   299,   184,   185,   186,   187,   219,
     220,   221,   222,   432,   146,   147,   130,   131,   176,   177,
     433,   320,   182,   183,   225,   226,   227,   228,   188,   189,
     132,   133,   223,   224,   231,   232,   233,   234,   300,   518,
     519,   237,   238,   239,   240,   134,   135,   229,   230,   251,
     252,   253,   254,   505,   506,   507,   301,   235,   236,   491,
     492,   302,   493,   494,   241,   242,   148,   149,   495,   496,
     497,   303,   255,   256,   498,   499,   304,   500,   501,   508,
     509,   510,   305,   502,   503,   504,   311,   312,   313,   314,
     315,   316,   317,   324,   325,   326,   327,   306,   511,   512,
     513,   514,   515,   516,   329,   330,   331,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   190,
     191,   192,   193,   194,   195,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   243,   244,   245,
     246,   247,   248,   249,   250,   344,   345,   348,   349,   350,
     351,   352,   353,   354,   355,   307,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   438,   439,
     440,   441,   442,   443,   444,   445,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   292,   293,   308,   321,   309,   450,   310,   322,
     323,   332,   333,     0,   334,     0,   335,     0,   319,   336,
     337,   338,   339,   340,   341,   342,   343,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   418,
     419,   422,   423,   426,   424,   425,   427,   428,   434,   435,
       0,   429,   436,   347,   437,     0,     0,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474
};

static const yytype_int16 yycheck[] =
{
      53,     0,    65,    66,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    65,    66,    67,    68,    65,    66,    67,
      68,    65,    66,    67,    68,    64,    69,    70,    75,    59,
      60,    61,    69,    70,    65,    82,    88,    89,    65,    66,
      88,    89,    82,    86,    88,    89,    85,    84,    87,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    65,    66,
      75,    75,     3,    65,    66,     6,     7,    82,    82,    82,
      64,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    75,    75,
      31,    85,    75,    87,    82,    82,    82,    38,    82,    82,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    65,    66,    67,    68,    65,    66,    67,    68,    65,
      66,    67,    68,    64,    82,    65,    66,    67,    68,    65,
      66,    67,    68,    75,    88,    89,    65,    66,    88,    89,
      82,    74,    88,    89,    65,    66,    67,    68,    88,    89,
      65,    66,    88,    89,    65,    66,    67,    68,    82,   292,
     293,    65,    66,    67,    68,    65,    66,    88,    89,    65,
      66,    67,    68,    81,    82,    83,    82,    88,    89,    72,
      73,    82,    75,    76,    88,    89,    65,    66,    81,    82,
      83,    82,    88,    89,    72,    73,    82,    75,    76,    81,
      82,    83,    82,    81,    82,    83,    52,    53,    54,    55,
      56,    57,    58,    77,    78,    79,    80,    82,    81,    82,
      83,    81,    82,    83,   133,   134,   135,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    65,    66,    62,    63,    72,    73,    72,
      73,    72,    73,    72,    73,    82,    72,    73,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    75,    76,
      75,    76,    75,    76,    75,    76,    75,    76,    75,    76,
      75,    76,    75,    76,    75,    76,    75,    76,    75,    76,
      75,    76,    69,    70,    82,    74,    82,   218,    82,    74,
      74,    82,    82,    -1,    82,    -1,    82,    -1,   127,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    75,    75,    82,    82,    75,
      82,    75,    75,    75,    82,    82,    75,    82,    75,    75,
      -1,    82,    82,   149,    82,    -1,    -1,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    91,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    64,    85,    87,    92,    93,    94,    95,    96,
      97,    99,   100,   101,   102,   104,   105,   106,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      65,    66,    67,    68,    88,    89,    65,    66,    65,    66,
      65,    66,    67,    68,    88,    89,    65,    66,    65,    66,
      65,    66,    65,    66,    65,    66,    65,    66,    67,    68,
      88,    89,    65,    66,    67,    68,    88,    89,    65,    66,
      65,    66,    65,    66,    65,    66,    65,    66,    65,    66,
      65,    66,    65,    66,    65,    66,    65,    66,    65,    66,
      65,    66,    65,    66,    67,    68,    88,    89,    65,    66,
      67,    68,    88,    89,    65,    66,    67,    68,    88,    89,
      65,    66,    65,    66,    65,    66,    65,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    65,    66,    65,    66,    65,    66,    65,
      66,    67,    68,    88,    89,    65,    66,    67,    68,    88,
      89,    65,    66,    67,    68,    88,    89,    65,    66,    67,
      68,    88,    89,    65,    66,    65,    66,    65,    66,    65,
      66,    65,    66,    67,    68,    88,    89,    92,     3,     6,
       7,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    31,    38,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    64,    69,    70,    84,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    52,    53,    54,    55,    56,    57,    58,    98,    98,
      74,    74,    74,    74,    77,    78,    79,    80,   103,   103,
     103,   103,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    62,    63,   107,   107,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    72,    73,
      72,    73,    72,    73,    72,    73,    72,    73,    75,    82,
      75,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      75,    75,    82,    82,    75,    82,    75,    82,    75,    82,
      75,    82,    75,    75,    82,    82,    75,    75,    82,    82,
      75,    82,    75,    82,    75,    75,    82,    82,    75,    76,
      75,    76,    75,    76,    75,    76,    59,    60,    61,   150,
     150,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    75,    76,    75,    76,    75,
      76,    75,    76,    75,    76,    75,    76,    75,    76,    75,
      76,    72,    73,    75,    76,    81,    82,    83,    72,    73,
      75,    76,    81,    82,    83,    81,    82,    83,    81,    82,
      83,    81,    82,    83,    81,    82,    83,    86,    92,    92
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    90,    91,    91,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    93,    93,
      93,    93,    93,    93,    93,    94,    94,    95,    95,    96,
      96,    96,    96,    96,    96,    96,    96,    97,    97,    97,
      97,    98,    98,    98,    98,    98,    98,    98,    99,    99,
     100,   100,   101,   101,   102,   102,   103,   103,   103,   103,
     104,   104,   104,   104,   104,   104,   104,   104,   105,   105,
     105,   105,   105,   105,   105,   105,   106,   106,   106,   106,
     107,   107,   108,   108,   108,   108,   108,   108,   109,   109,
     109,   109,   109,   109,   110,   110,   110,   110,   110,   110,
     111,   111,   111,   111,   111,   111,   112,   112,   112,   112,
     112,   112,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   114,   114,   115,   115,   115,   115,   115,   115,
     116,   116,   116,   116,   116,   116,   117,   117,   117,   117,
     117,   117,   118,   118,   118,   118,   119,   119,   120,   120,
     121,   121,   122,   122,   123,   123,   124,   124,   125,   125,
     126,   126,   127,   127,   128,   128,   129,   129,   129,   129,
     130,   130,   131,   131,   132,   132,   133,   133,   134,   134,
     134,   134,   134,   134,   135,   135,   135,   135,   135,   135,
     136,   136,   136,   136,   136,   136,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   138,   138,   138,   138,   138,
     138,   138,   139,   139,   139,   139,   139,   139,   139,   139,
     140,   140,   140,   140,   140,   140,   140,   140,   141,   141,
     141,   141,   141,   141,   142,   142,   142,   142,   142,   142,
     143,   143,   143,   143,   143,   143,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   145,   145,   145,   145,
     145,   145,   146,   146,   146,   146,   146,   146,   146,   146,
     147,   147,   147,   147,   147,   147,   147,   147,   148,   148,
     148,   148,   148,   148,   149,   149,   149,   149,   150,   150,
     150
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       1,     1,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     1,     1,
       1
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
  case 54: /* expr: expr BOOL_OR expr  */
#line 265 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) || (yyvsp[0].bool_result)));
        _NDFP_debugf("OR (%d || %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 2114 "nd-flow-expr.cpp"
    break;

  case 55: /* expr: expr BOOL_AND expr  */
#line 269 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) && (yyvsp[0].bool_result)));
        _NDFP_debugf("AND (%d && %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 2123 "nd-flow-expr.cpp"
    break;

  case 56: /* expr: '(' expr ')'  */
#line 273 "nd-flow-expr.ypp"
                   { _NDFP_result = ((yyval.bool_result) = (yyvsp[-1].bool_result)); }
#line 2129 "nd-flow-expr.cpp"
    break;

  case 57: /* expr_ip_proto: FLOW_IP_PROTO  */
#line 277 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != 0));
        _NDFP_debugf(
            "IP Protocol is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2139 "nd-flow-expr.cpp"
    break;

  case 58: /* expr_ip_proto: '!' FLOW_IP_PROTO  */
#line 282 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == 0));
        _NDFP_debugf("IP Protocol is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2148 "nd-flow-expr.cpp"
    break;

  case 59: /* expr_ip_proto: FLOW_IP_PROTO CMP_EQUAL VALUE_UNSIGNED  */
#line 286 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2157 "nd-flow-expr.cpp"
    break;

  case 60: /* expr_ip_proto: FLOW_IP_PROTO CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 290 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2166 "nd-flow-expr.cpp"
    break;

  case 61: /* expr_ip_proto: FLOW_IP_PROTO CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 294 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol >= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2175 "nd-flow-expr.cpp"
    break;

  case 62: /* expr_ip_proto: FLOW_IP_PROTO CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 298 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol <= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2184 "nd-flow-expr.cpp"
    break;

  case 63: /* expr_ip_proto: FLOW_IP_PROTO '>' VALUE_UNSIGNED  */
#line 302 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol > (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2193 "nd-flow-expr.cpp"
    break;

  case 64: /* expr_ip_proto: FLOW_IP_PROTO '<' VALUE_UNSIGNED  */
#line 306 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol < (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2202 "nd-flow-expr.cpp"
    break;

  case 65: /* expr_ip_dscp: FLOW_IP_DSCP CMP_EQUAL VALUE_UNSIGNED  */
#line 313 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2211 "nd-flow-expr.cpp"
    break;

  case 66: /* expr_ip_dscp: FLOW_IP_DSCP CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 317 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2220 "nd-flow-expr.cpp"
    break;

  case 67: /* expr_ip_version: FLOW_IP_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 324 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2229 "nd-flow-expr.cpp"
    break;

  case 68: /* expr_ip_version: FLOW_IP_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 328 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2238 "nd-flow-expr.cpp"
    break;

  case 69: /* expr_vlan_id: FLOW_VLAN_ID  */
#line 335 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0));
        _NDFP_debugf("VLAN ID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2247 "nd-flow-expr.cpp"
    break;

  case 70: /* expr_vlan_id: '!' FLOW_VLAN_ID  */
#line 339 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0));
        _NDFP_debugf("VLAN ID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2256 "nd-flow-expr.cpp"
    break;

  case 71: /* expr_vlan_id: FLOW_VLAN_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 343 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2265 "nd-flow-expr.cpp"
    break;

  case 72: /* expr_vlan_id: FLOW_VLAN_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 347 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2274 "nd-flow-expr.cpp"
    break;

  case 73: /* expr_vlan_id: FLOW_VLAN_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 351 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2283 "nd-flow-expr.cpp"
    break;

  case 74: /* expr_vlan_id: FLOW_VLAN_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 355 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2292 "nd-flow-expr.cpp"
    break;

  case 75: /* expr_vlan_id: FLOW_VLAN_ID '>' VALUE_UNSIGNED  */
#line 359 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2301 "nd-flow-expr.cpp"
    break;

  case 76: /* expr_vlan_id: FLOW_VLAN_ID '<' VALUE_UNSIGNED  */
#line 363 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2310 "nd-flow-expr.cpp"
    break;

  case 77: /* expr_other_type: FLOW_OTHER_TYPE  */
#line 370 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2321 "nd-flow-expr.cpp"
    break;

  case 78: /* expr_other_type: '!' FLOW_OTHER_TYPE  */
#line 376 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2332 "nd-flow-expr.cpp"
    break;

  case 79: /* expr_other_type: FLOW_OTHER_TYPE CMP_EQUAL value_other_type  */
#line 382 "nd-flow-expr.ypp"
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
#line 2381 "nd-flow-expr.cpp"
    break;

  case 80: /* expr_other_type: FLOW_OTHER_TYPE CMP_NOTEQUAL value_other_type  */
#line 426 "nd-flow-expr.ypp"
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
#line 2430 "nd-flow-expr.cpp"
    break;

  case 81: /* value_other_type: FLOW_OTHER_UNKNOWN  */
#line 473 "nd-flow-expr.ypp"
                         { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2436 "nd-flow-expr.cpp"
    break;

  case 82: /* value_other_type: FLOW_OTHER_UNSUPPORTED  */
#line 474 "nd-flow-expr.ypp"
                             { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2442 "nd-flow-expr.cpp"
    break;

  case 83: /* value_other_type: FLOW_OTHER_LOCAL  */
#line 475 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2448 "nd-flow-expr.cpp"
    break;

  case 84: /* value_other_type: FLOW_OTHER_MULTICAST  */
#line 476 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2454 "nd-flow-expr.cpp"
    break;

  case 85: /* value_other_type: FLOW_OTHER_BROADCAST  */
#line 477 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2460 "nd-flow-expr.cpp"
    break;

  case 86: /* value_other_type: FLOW_OTHER_REMOTE  */
#line 478 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2466 "nd-flow-expr.cpp"
    break;

  case 87: /* value_other_type: FLOW_OTHER_ERROR  */
#line 479 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2472 "nd-flow-expr.cpp"
    break;

  case 88: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 483 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2483 "nd-flow-expr.cpp"
    break;

  case 89: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 489 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2494 "nd-flow-expr.cpp"
    break;

  case 90: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 498 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_other_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2505 "nd-flow-expr.cpp"
    break;

  case 91: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 504 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_other_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2516 "nd-flow-expr.cpp"
    break;

  case 92: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL value_addr_ip  */
#line 513 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2527 "nd-flow-expr.cpp"
    break;

  case 93: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL value_addr_ip  */
#line 519 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2538 "nd-flow-expr.cpp"
    break;

  case 94: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL value_addr_ip  */
#line 528 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2549 "nd-flow-expr.cpp"
    break;

  case 95: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL value_addr_ip  */
#line 534 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2560 "nd-flow-expr.cpp"
    break;

  case 96: /* value_addr_ip: VALUE_ADDR_IPV4  */
#line 543 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2566 "nd-flow-expr.cpp"
    break;

  case 97: /* value_addr_ip: VALUE_ADDR_IPV4_CIDR  */
#line 544 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2572 "nd-flow-expr.cpp"
    break;

  case 98: /* value_addr_ip: VALUE_ADDR_IPV6  */
#line 545 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2578 "nd-flow-expr.cpp"
    break;

  case 99: /* value_addr_ip: VALUE_ADDR_IPV6_CIDR  */
#line 546 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2584 "nd-flow-expr.cpp"
    break;

  case 100: /* expr_local_port: FLOW_LOCAL_PORT  */
#line 550 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        _NDFP_debugf("Local port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2593 "nd-flow-expr.cpp"
    break;

  case 101: /* expr_local_port: '!' FLOW_LOCAL_PORT  */
#line 554 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        _NDFP_debugf("Local port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2602 "nd-flow-expr.cpp"
    break;

  case 102: /* expr_local_port: FLOW_LOCAL_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 558 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2611 "nd-flow-expr.cpp"
    break;

  case 103: /* expr_local_port: FLOW_LOCAL_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 562 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2620 "nd-flow-expr.cpp"
    break;

  case 104: /* expr_local_port: FLOW_LOCAL_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 566 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2629 "nd-flow-expr.cpp"
    break;

  case 105: /* expr_local_port: FLOW_LOCAL_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 570 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2638 "nd-flow-expr.cpp"
    break;

  case 106: /* expr_local_port: FLOW_LOCAL_PORT '>' VALUE_UNSIGNED  */
#line 574 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2647 "nd-flow-expr.cpp"
    break;

  case 107: /* expr_local_port: FLOW_LOCAL_PORT '<' VALUE_UNSIGNED  */
#line 578 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2656 "nd-flow-expr.cpp"
    break;

  case 108: /* expr_other_port: FLOW_OTHER_PORT  */
#line 585 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Other port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2665 "nd-flow-expr.cpp"
    break;

  case 109: /* expr_other_port: '!' FLOW_OTHER_PORT  */
#line 589 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Other port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2674 "nd-flow-expr.cpp"
    break;

  case 110: /* expr_other_port: FLOW_OTHER_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 593 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2683 "nd-flow-expr.cpp"
    break;

  case 111: /* expr_other_port: FLOW_OTHER_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 597 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2692 "nd-flow-expr.cpp"
    break;

  case 112: /* expr_other_port: FLOW_OTHER_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 601 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2701 "nd-flow-expr.cpp"
    break;

  case 113: /* expr_other_port: FLOW_OTHER_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 605 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2710 "nd-flow-expr.cpp"
    break;

  case 114: /* expr_other_port: FLOW_OTHER_PORT '>' VALUE_UNSIGNED  */
#line 609 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2719 "nd-flow-expr.cpp"
    break;

  case 115: /* expr_other_port: FLOW_OTHER_PORT '<' VALUE_UNSIGNED  */
#line 613 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2728 "nd-flow-expr.cpp"
    break;

  case 116: /* expr_tunnel_type: FLOW_TUNNEL_TYPE  */
#line 620 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2739 "nd-flow-expr.cpp"
    break;

  case 117: /* expr_tunnel_type: '!' FLOW_TUNNEL_TYPE  */
#line 626 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type is none? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2750 "nd-flow-expr.cpp"
    break;

  case 118: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_EQUAL value_tunnel_type  */
#line 632 "nd-flow-expr.ypp"
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
#line 2774 "nd-flow-expr.cpp"
    break;

  case 119: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_NOTEQUAL value_tunnel_type  */
#line 651 "nd-flow-expr.ypp"
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
#line 2798 "nd-flow-expr.cpp"
    break;

  case 120: /* value_tunnel_type: FLOW_TUNNEL_NONE  */
#line 673 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2804 "nd-flow-expr.cpp"
    break;

  case 121: /* value_tunnel_type: FLOW_TUNNEL_GTP  */
#line 674 "nd-flow-expr.ypp"
                      { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2810 "nd-flow-expr.cpp"
    break;

  case 122: /* expr_detection_complete: FLOW_DETECTION_COMPLETE  */
#line 677 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf("Detection was complete? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2819 "nd-flow-expr.cpp"
    break;

  case 123: /* expr_detection_complete: '!' FLOW_DETECTION_COMPLETE  */
#line 681 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf(
            "Detection was not complete? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2830 "nd-flow-expr.cpp"
    break;

  case 124: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_TRUE  */
#line 687 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == true
        ));
        _NDFP_debugf(
            "Detection complete == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2843 "nd-flow-expr.cpp"
    break;

  case 125: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_FALSE  */
#line 695 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == false
        ));
        _NDFP_debugf(
            "Detection complete == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2856 "nd-flow-expr.cpp"
    break;

  case 126: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_TRUE  */
#line 703 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != true
        ));
        _NDFP_debugf(
            "Detection complete != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2869 "nd-flow-expr.cpp"
    break;

  case 127: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_FALSE  */
#line 711 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != false
        ));
        _NDFP_debugf(
            "Detection complete != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2882 "nd-flow-expr.cpp"
    break;

  case 128: /* expr_detection_guessed: FLOW_DETECTION_GUESSED  */
#line 722 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf("Detection was guessed? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2891 "nd-flow-expr.cpp"
    break;

  case 129: /* expr_detection_guessed: '!' FLOW_DETECTION_GUESSED  */
#line 726 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf(
            "Detection was not guessed? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2902 "nd-flow-expr.cpp"
    break;

  case 130: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_TRUE  */
#line 732 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == true
        ));
        _NDFP_debugf(
            "Detection guessed == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2915 "nd-flow-expr.cpp"
    break;

  case 131: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_FALSE  */
#line 740 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == false
        ));
        _NDFP_debugf(
            "Detection guessed == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2928 "nd-flow-expr.cpp"
    break;

  case 132: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_TRUE  */
#line 748 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != true
        ));
        _NDFP_debugf(
            "Detection guessed != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2941 "nd-flow-expr.cpp"
    break;

  case 133: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_FALSE  */
#line 756 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != false
        ));
        _NDFP_debugf(
            "Detection guessed != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2954 "nd-flow-expr.cpp"
    break;

  case 134: /* expr_detection_init: FLOW_DETECTION_INIT  */
#line 767 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf("Detection was init? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2963 "nd-flow-expr.cpp"
    break;

  case 135: /* expr_detection_init: '!' FLOW_DETECTION_INIT  */
#line 771 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf(
            "Detection was not init? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2974 "nd-flow-expr.cpp"
    break;

  case 136: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_TRUE  */
#line 777 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == true
        ));
        _NDFP_debugf(
            "Detection init == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2987 "nd-flow-expr.cpp"
    break;

  case 137: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_FALSE  */
#line 785 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == false
        ));
        _NDFP_debugf(
            "Detection init == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3000 "nd-flow-expr.cpp"
    break;

  case 138: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_TRUE  */
#line 793 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != true
        ));
        _NDFP_debugf(
            "Detection init != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3013 "nd-flow-expr.cpp"
    break;

  case 139: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_FALSE  */
#line 801 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != false
        ));
        _NDFP_debugf(
            "Detection init != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3026 "nd-flow-expr.cpp"
    break;

  case 140: /* expr_detection_updated: FLOW_DETECTION_UPDATED  */
#line 812 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf("Detection was updated? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3035 "nd-flow-expr.cpp"
    break;

  case 141: /* expr_detection_updated: '!' FLOW_DETECTION_UPDATED  */
#line 816 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf(
            "Detection was not updated? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3046 "nd-flow-expr.cpp"
    break;

  case 142: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_TRUE  */
#line 822 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == true
        ));
        _NDFP_debugf(
            "Detection updated == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3059 "nd-flow-expr.cpp"
    break;

  case 143: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_FALSE  */
#line 830 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == false
        ));
        _NDFP_debugf(
            "Detection updated == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3072 "nd-flow-expr.cpp"
    break;

  case 144: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_TRUE  */
#line 838 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != true
        ));
        _NDFP_debugf(
            "Detection updated != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3085 "nd-flow-expr.cpp"
    break;

  case 145: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_FALSE  */
#line 846 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != false
        ));
        _NDFP_debugf(
            "Detection updated != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3098 "nd-flow-expr.cpp"
    break;

  case 146: /* expr_dhc_hit: FLOW_DHC_HIT  */
#line 857 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf("DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3107 "nd-flow-expr.cpp"
    break;

  case 147: /* expr_dhc_hit: '!' FLOW_DHC_HIT  */
#line 861 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf(
            "DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3118 "nd-flow-expr.cpp"
    break;

  case 148: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 867 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == true
        ));
        _NDFP_debugf(
            "DHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3131 "nd-flow-expr.cpp"
    break;

  case 149: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 875 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == false
        ));
        _NDFP_debugf(
            "DHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3144 "nd-flow-expr.cpp"
    break;

  case 150: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 883 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != true
        ));
        _NDFP_debugf(
            "DHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3157 "nd-flow-expr.cpp"
    break;

  case 151: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 891 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != false
        ));
        _NDFP_debugf(
            "DHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3170 "nd-flow-expr.cpp"
    break;

  case 152: /* expr_fhc_hit: FLOW_FHC_HIT  */
#line 902 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf("FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3179 "nd-flow-expr.cpp"
    break;

  case 153: /* expr_fhc_hit: '!' FLOW_FHC_HIT  */
#line 906 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf(
            "FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3190 "nd-flow-expr.cpp"
    break;

  case 154: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 912 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == true
        ));
        _NDFP_debugf(
            "FHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3203 "nd-flow-expr.cpp"
    break;

  case 155: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 920 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == false
        ));
        _NDFP_debugf(
            "FHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3216 "nd-flow-expr.cpp"
    break;

  case 156: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 928 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != true
        ));
        _NDFP_debugf(
            "FHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3229 "nd-flow-expr.cpp"
    break;

  case 157: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 936 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != false
        ));
        _NDFP_debugf(
            "FHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3242 "nd-flow-expr.cpp"
    break;

  case 158: /* expr_ip_nat: FLOW_IP_NAT  */
#line 947 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3251 "nd-flow-expr.cpp"
    break;

  case 159: /* expr_ip_nat: '!' FLOW_IP_NAT  */
#line 951 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3260 "nd-flow-expr.cpp"
    break;

  case 160: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_TRUE  */
#line 955 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3269 "nd-flow-expr.cpp"
    break;

  case 161: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_FALSE  */
#line 959 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3278 "nd-flow-expr.cpp"
    break;

  case 162: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_TRUE  */
#line 963 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != true));
        _NDFP_debugf("IP NAT != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3287 "nd-flow-expr.cpp"
    break;

  case 163: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_FALSE  */
#line 967 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != false));
        _NDFP_debugf("IP NAT != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3296 "nd-flow-expr.cpp"
    break;

  case 164: /* expr_expiring: FLOW_EXPIRING  */
#line 974 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3305 "nd-flow-expr.cpp"
    break;

  case 165: /* expr_expiring: '!' FLOW_EXPIRING  */
#line 978 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3314 "nd-flow-expr.cpp"
    break;

  case 166: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_TRUE  */
#line 982 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3323 "nd-flow-expr.cpp"
    break;

  case 167: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_FALSE  */
#line 986 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3332 "nd-flow-expr.cpp"
    break;

  case 168: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_TRUE  */
#line 990 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != true));
        _NDFP_debugf("Flow expiring != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3341 "nd-flow-expr.cpp"
    break;

  case 169: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_FALSE  */
#line 994 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != false));
        _NDFP_debugf("Flow expiring != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3350 "nd-flow-expr.cpp"
    break;

  case 170: /* expr_expired: FLOW_EXPIRED  */
#line 1001 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3359 "nd-flow-expr.cpp"
    break;

  case 171: /* expr_expired: '!' FLOW_EXPIRED  */
#line 1005 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3368 "nd-flow-expr.cpp"
    break;

  case 172: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_TRUE  */
#line 1009 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3377 "nd-flow-expr.cpp"
    break;

  case 173: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_FALSE  */
#line 1013 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3386 "nd-flow-expr.cpp"
    break;

  case 174: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_TRUE  */
#line 1017 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != true));
        _NDFP_debugf("Flow expired != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3395 "nd-flow-expr.cpp"
    break;

  case 175: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_FALSE  */
#line 1021 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != false));
        _NDFP_debugf("Flow expired != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3404 "nd-flow-expr.cpp"
    break;

  case 176: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR  */
#line 1028 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3413 "nd-flow-expr.cpp"
    break;

  case 177: /* expr_soft_dissector: '!' FLOW_SOFT_DISSECTOR  */
#line 1032 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3422 "nd-flow-expr.cpp"
    break;

  case 178: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_TRUE  */
#line 1036 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3431 "nd-flow-expr.cpp"
    break;

  case 179: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_FALSE  */
#line 1040 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3440 "nd-flow-expr.cpp"
    break;

  case 180: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_TRUE  */
#line 1044 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != true));
        _NDFP_debugf("Soft dissector matched != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3449 "nd-flow-expr.cpp"
    break;

  case 181: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_FALSE  */
#line 1048 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != false));
        _NDFP_debugf("Soft dissector matched != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3458 "nd-flow-expr.cpp"
    break;

  case 182: /* expr_app: FLOW_APPLICATION  */
#line 1055 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) != 0
        ));
        _NDFP_debugf("Application detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3469 "nd-flow-expr.cpp"
    break;

  case 183: /* expr_app: '!' FLOW_APPLICATION  */
#line 1061 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) == 0
        ));
        _NDFP_debugf(
            "Application not detected? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3482 "nd-flow-expr.cpp"
    break;

  case 186: /* expr_app_id: FLOW_APPLICATION CMP_EQUAL VALUE_UNSIGNED  */
#line 1073 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf(
            "Application ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3496 "nd-flow-expr.cpp"
    break;

  case 187: /* expr_app_id: FLOW_APPLICATION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1082 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = false);

        _NDFP_debugf(
            "Application ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3510 "nd-flow-expr.cpp"
    break;

  case 188: /* expr_app_name: FLOW_APPLICATION CMP_EQUAL VALUE_NAME  */
#line 1094 "nd-flow-expr.ypp"
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
#line 3542 "nd-flow-expr.cpp"
    break;

  case 189: /* expr_app_name: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NAME  */
#line 1121 "nd-flow-expr.ypp"
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
#line 3574 "nd-flow-expr.cpp"
    break;

  case 190: /* expr_category: FLOW_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1151 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = false);

        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        if (_NDFP_flow->category.application != ND_CAT_UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.application
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.domain != ND_CAT_UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.domain
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.network != ND_CAT_UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.network
                )
            );
        }

        _NDFP_debugf("App/domain/network category == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3617 "nd-flow-expr.cpp"
    break;

  case 191: /* expr_category: FLOW_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1189 "nd-flow-expr.ypp"
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
                        ndCategories::Type::APP, category) != _NDFP_flow->category.network
                )
            );
        }

        _NDFP_debugf("App/domain/network category != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3656 "nd-flow-expr.cpp"
    break;

  case 192: /* expr_category_id: FLOW_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1226 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("App category ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Domain category ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.network) == (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Network category ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        _NDFP_debugf("App/domain/network category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3683 "nd-flow-expr.cpp"
    break;

  case 193: /* expr_category_id: FLOW_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1248 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("App category ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Domain category ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        if (! _NDFP_result) {
            _NDFP_result = ((yyval.bool_result) = (
                static_cast<unsigned>(_NDFP_flow->category.network) != (yyvsp[0].ul_number)
            ));
            _NDFP_debugf("Network category ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
        }

        _NDFP_debugf("App/domain/network category ID != %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3710 "nd-flow-expr.cpp"
    break;

  case 194: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1273 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App category == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3731 "nd-flow-expr.cpp"
    break;

  case 195: /* expr_app_category: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_NAME  */
#line 1289 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App category != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3752 "nd-flow-expr.cpp"
    break;

  case 196: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1308 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3764 "nd-flow-expr.cpp"
    break;

  case 197: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1315 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category != %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3776 "nd-flow-expr.cpp"
    break;

  case 198: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1325 "nd-flow-expr.ypp"
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

        _NDFP_debugf("Domain category ID == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3797 "nd-flow-expr.cpp"
    break;

  case 199: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1341 "nd-flow-expr.ypp"
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

        _NDFP_debugf("Domain category ID != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3818 "nd-flow-expr.cpp"
    break;

  case 200: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1360 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3830 "nd-flow-expr.cpp"
    break;

  case 201: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1367 "nd-flow-expr.ypp"
                                                          {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID != %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3842 "nd-flow-expr.cpp"
    break;

  case 202: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1377 "nd-flow-expr.ypp"
                                                 {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) == _NDFP_flow->category.network
            )
        );

        _NDFP_debugf("Network category == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3863 "nd-flow-expr.cpp"
    break;

  case 203: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1393 "nd-flow-expr.ypp"
                                                    {
        size_t p;
        string category((yyvsp[0].buffer));

        while ((p = category.find_first_of("'\"")) != string::npos)
            category.erase(p, 1);

        _NDFP_result = (
            (yyval.bool_result) = (
                _NDFP_categories.LookupTag(
                    ndCategories::Type::APP, category) != _NDFP_flow->category.network
            )
        );

        _NDFP_debugf("Network category != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3884 "nd-flow-expr.cpp"
    break;

  case 204: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1412 "nd-flow-expr.ypp"
                                                        {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.network) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3896 "nd-flow-expr.cpp"
    break;

  case 205: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1419 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.network) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network category ID != %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3908 "nd-flow-expr.cpp"
    break;

  case 206: /* expr_proto: FLOW_PROTOCOL  */
#line 1429 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol != ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3919 "nd-flow-expr.cpp"
    break;

  case 207: /* expr_proto: '!' FLOW_PROTOCOL  */
#line 1435 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol == ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3930 "nd-flow-expr.cpp"
    break;

  case 210: /* expr_proto_id: FLOW_PROTOCOL CMP_EQUAL VALUE_UNSIGNED  */
#line 1445 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3941 "nd-flow-expr.cpp"
    break;

  case 211: /* expr_proto_id: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1451 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3952 "nd-flow-expr.cpp"
    break;

  case 212: /* expr_proto_name: FLOW_PROTOCOL CMP_EQUAL VALUE_NAME  */
#line 1460 "nd-flow-expr.ypp"
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
#line 3977 "nd-flow-expr.cpp"
    break;

  case 213: /* expr_proto_name: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NAME  */
#line 1480 "nd-flow-expr.ypp"
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
#line 4001 "nd-flow-expr.cpp"
    break;

  case 214: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1502 "nd-flow-expr.ypp"
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
#line 4023 "nd-flow-expr.cpp"
    break;

  case 215: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1519 "nd-flow-expr.ypp"
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
#line 4045 "nd-flow-expr.cpp"
    break;

  case 216: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1539 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4058 "nd-flow-expr.cpp"
    break;

  case 217: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1547 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4071 "nd-flow-expr.cpp"
    break;

  case 218: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME  */
#line 1558 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == false
        ));
        _NDFP_debugf("Detected hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4083 "nd-flow-expr.cpp"
    break;

  case 219: /* expr_detected_hostname: '!' FLOW_DETECTED_HOSTNAME  */
#line 1565 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == true
        ));
        _NDFP_debugf("Detected hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4095 "nd-flow-expr.cpp"
    break;

  case 220: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 1572 "nd-flow-expr.ypp"
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
#line 4119 "nd-flow-expr.cpp"
    break;

  case 221: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 1591 "nd-flow-expr.ypp"
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
#line 4143 "nd-flow-expr.cpp"
    break;

  case 222: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 1610 "nd-flow-expr.ypp"
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
#line 4162 "nd-flow-expr.cpp"
    break;

  case 223: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 1624 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Detected hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4173 "nd-flow-expr.cpp"
    break;

  case 224: /* expr_dns_hostname: FLOW_DNS_HOSTNAME  */
#line 1633 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == false
        ));
        _NDFP_debugf("DNS hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4185 "nd-flow-expr.cpp"
    break;

  case 225: /* expr_dns_hostname: '!' FLOW_DNS_HOSTNAME  */
#line 1640 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == true
        ));
        _NDFP_debugf("DNS hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4197 "nd-flow-expr.cpp"
    break;

  case 226: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 1647 "nd-flow-expr.ypp"
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
#line 4221 "nd-flow-expr.cpp"
    break;

  case 227: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 1666 "nd-flow-expr.ypp"
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
#line 4245 "nd-flow-expr.cpp"
    break;

  case 228: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 1685 "nd-flow-expr.ypp"
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
#line 4264 "nd-flow-expr.cpp"
    break;

  case 229: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 1699 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("DNS hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4275 "nd-flow-expr.cpp"
    break;

  case 230: /* expr_risks: FLOW_RISKS  */
#line 1708 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() != 0));
        _NDFP_debugf("Risks detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4284 "nd-flow-expr.cpp"
    break;

  case 231: /* expr_risks: '!' FLOW_RISKS  */
#line 1712 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() == 0));
        _NDFP_debugf("Risks not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4293 "nd-flow-expr.cpp"
    break;

  case 232: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NAME  */
#line 1716 "nd-flow-expr.ypp"
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
#line 4317 "nd-flow-expr.cpp"
    break;

  case 233: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NAME  */
#line 1735 "nd-flow-expr.ypp"
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
#line 4341 "nd-flow-expr.cpp"
    break;

  case 234: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_UNSIGNED  */
#line 1754 "nd-flow-expr.ypp"
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
#line 4359 "nd-flow-expr.cpp"
    break;

  case 235: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1767 "nd-flow-expr.ypp"
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
#line 4377 "nd-flow-expr.cpp"
    break;

  case 236: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE  */
#line 1783 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != 0));
        _NDFP_debugf("nDPI risk score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4386 "nd-flow-expr.cpp"
    break;

  case 237: /* expr_risk_ndpi_score: '!' FLOW_NDPI_RISK_SCORE  */
#line 1787 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == 0));
        _NDFP_debugf("nDPI risk score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4395 "nd-flow-expr.cpp"
    break;

  case 238: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_EQUAL VALUE_UNSIGNED  */
#line 1791 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4404 "nd-flow-expr.cpp"
    break;

  case 239: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1795 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4413 "nd-flow-expr.cpp"
    break;

  case 240: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1799 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4422 "nd-flow-expr.cpp"
    break;

  case 241: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1803 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4431 "nd-flow-expr.cpp"
    break;

  case 242: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '>' VALUE_UNSIGNED  */
#line 1807 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4440 "nd-flow-expr.cpp"
    break;

  case 243: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '<' VALUE_UNSIGNED  */
#line 1811 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4449 "nd-flow-expr.cpp"
    break;

  case 244: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 1818 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != 0));
        _NDFP_debugf("nDPI risk client score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4458 "nd-flow-expr.cpp"
    break;

  case 245: /* expr_risk_ndpi_score_client: '!' FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 1822 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == 0));
        _NDFP_debugf("nDPI risk client score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4467 "nd-flow-expr.cpp"
    break;

  case 246: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_EQUAL VALUE_UNSIGNED  */
#line 1826 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4476 "nd-flow-expr.cpp"
    break;

  case 247: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1830 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4485 "nd-flow-expr.cpp"
    break;

  case 248: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1834 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4494 "nd-flow-expr.cpp"
    break;

  case 249: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1838 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4503 "nd-flow-expr.cpp"
    break;

  case 250: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '>' VALUE_UNSIGNED  */
#line 1842 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4512 "nd-flow-expr.cpp"
    break;

  case 251: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '<' VALUE_UNSIGNED  */
#line 1846 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4521 "nd-flow-expr.cpp"
    break;

  case 252: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER  */
#line 1853 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != 0));
        _NDFP_debugf("nDPI risk server score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4530 "nd-flow-expr.cpp"
    break;

  case 253: /* expr_risk_ndpi_score_server: '!' FLOW_NDPI_RISK_SCORE_SERVER  */
#line 1857 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == 0));
        _NDFP_debugf("nDPI risk server score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4539 "nd-flow-expr.cpp"
    break;

  case 254: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_EQUAL VALUE_UNSIGNED  */
#line 1861 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4548 "nd-flow-expr.cpp"
    break;

  case 255: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1865 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4557 "nd-flow-expr.cpp"
    break;

  case 256: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1869 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4566 "nd-flow-expr.cpp"
    break;

  case 257: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1873 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4575 "nd-flow-expr.cpp"
    break;

  case 258: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '>' VALUE_UNSIGNED  */
#line 1877 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4584 "nd-flow-expr.cpp"
    break;

  case 259: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '<' VALUE_UNSIGNED  */
#line 1881 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4593 "nd-flow-expr.cpp"
    break;

  case 260: /* expr_fwmark: FLOW_CT_MARK  */
#line 1888 "nd-flow-expr.ypp"
                   {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != 0));
        _NDFP_debugf("FWMARK set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4606 "nd-flow-expr.cpp"
    break;

  case 261: /* expr_fwmark: '!' FLOW_CT_MARK  */
#line 1896 "nd-flow-expr.ypp"
                       {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == 0));
        _NDFP_debugf("FWMARK not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4619 "nd-flow-expr.cpp"
    break;

  case 262: /* expr_fwmark: FLOW_CT_MARK CMP_EQUAL VALUE_UNSIGNED  */
#line 1904 "nd-flow-expr.ypp"
                                            {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4632 "nd-flow-expr.cpp"
    break;

  case 263: /* expr_fwmark: FLOW_CT_MARK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1912 "nd-flow-expr.ypp"
                                               {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4645 "nd-flow-expr.cpp"
    break;

  case 264: /* expr_fwmark: FLOW_CT_MARK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1920 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark >= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4658 "nd-flow-expr.cpp"
    break;

  case 265: /* expr_fwmark: FLOW_CT_MARK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1928 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark <= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4671 "nd-flow-expr.cpp"
    break;

  case 266: /* expr_fwmark: FLOW_CT_MARK '>' VALUE_UNSIGNED  */
#line 1936 "nd-flow-expr.ypp"
                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark > (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4684 "nd-flow-expr.cpp"
    break;

  case 267: /* expr_fwmark: FLOW_CT_MARK '<' VALUE_UNSIGNED  */
#line 1944 "nd-flow-expr.ypp"
                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark < (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4697 "nd-flow-expr.cpp"
    break;

  case 268: /* expr_iface: FLOW_IFACE  */
#line 1955 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == false
        ));
        _NDFP_debugf("Interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4709 "nd-flow-expr.cpp"
    break;

  case 269: /* expr_iface: '!' FLOW_IFACE  */
#line 1962 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == true
        ));
        _NDFP_debugf("Interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4721 "nd-flow-expr.cpp"
    break;

  case 270: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_NAME  */
#line 1969 "nd-flow-expr.ypp"
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
#line 4744 "nd-flow-expr.cpp"
    break;

  case 271: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_NAME  */
#line 1987 "nd-flow-expr.ypp"
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
#line 4767 "nd-flow-expr.cpp"
    break;

  case 272: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_REGEX  */
#line 2005 "nd-flow-expr.ypp"
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
#line 4786 "nd-flow-expr.cpp"
    break;

  case 273: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_REGEX  */
#line 2019 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4797 "nd-flow-expr.cpp"
    break;

  case 274: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC  */
#line 2028 "nd-flow-expr.ypp"
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
#line 4813 "nd-flow-expr.cpp"
    break;

  case 275: /* expr_iface_nfq_src: '!' FLOW_IFACE_NFQ_SRC  */
#line 2039 "nd-flow-expr.ypp"
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
#line 4829 "nd-flow-expr.cpp"
    break;

  case 276: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_NAME  */
#line 2050 "nd-flow-expr.ypp"
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
#line 4855 "nd-flow-expr.cpp"
    break;

  case 277: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_NAME  */
#line 2071 "nd-flow-expr.ypp"
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
#line 4879 "nd-flow-expr.cpp"
    break;

  case 278: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_REGEX  */
#line 2090 "nd-flow-expr.ypp"
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
#line 4898 "nd-flow-expr.cpp"
    break;

  case 279: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_REGEX  */
#line 2104 "nd-flow-expr.ypp"
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
#line 4917 "nd-flow-expr.cpp"
    break;

  case 280: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST  */
#line 2121 "nd-flow-expr.ypp"
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
#line 4933 "nd-flow-expr.cpp"
    break;

  case 281: /* expr_iface_nfq_dst: '!' FLOW_IFACE_NFQ_DST  */
#line 2132 "nd-flow-expr.ypp"
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
#line 4949 "nd-flow-expr.cpp"
    break;

  case 282: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_NAME  */
#line 2143 "nd-flow-expr.ypp"
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
#line 4973 "nd-flow-expr.cpp"
    break;

  case 283: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_NAME  */
#line 2162 "nd-flow-expr.ypp"
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
#line 4997 "nd-flow-expr.cpp"
    break;

  case 284: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_REGEX  */
#line 2181 "nd-flow-expr.ypp"
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
#line 5017 "nd-flow-expr.cpp"
    break;

  case 285: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_REGEX  */
#line 2196 "nd-flow-expr.ypp"
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
#line 5036 "nd-flow-expr.cpp"
    break;

  case 286: /* expr_intel: FLOW_INTEL  */
#line 2213 "nd-flow-expr.ypp"
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
#line 5062 "nd-flow-expr.cpp"
    break;

  case 287: /* expr_intel: '!' FLOW_INTEL  */
#line 2234 "nd-flow-expr.ypp"
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
#line 5087 "nd-flow-expr.cpp"
    break;

  case 288: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_NAME  */
#line 2256 "nd-flow-expr.ypp"
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
#line 5112 "nd-flow-expr.cpp"
    break;

  case 289: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_REGEX  */
#line 2276 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = false);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
            flow_intel(key, *_NDFP_params_intel, jvalue) && jvalue.is_string()) {
            string rx((yyvsp[0].buffer));
            string search(jvalue.get<string>());

            _NDFP_result = ((yyval.bool_result) = _NDFP_regex_search(rx, search));
        }

        _NDFP_debugf("Flow intel %s (RX) == %s? %s\n",
            key.c_str(), search.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 5132 "nd-flow-expr.cpp"
    break;

  case 290: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_UNSIGNED  */
#line 2291 "nd-flow-expr.ypp"
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
#line 5149 "nd-flow-expr.cpp"
    break;

  case 291: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_SIGNED  */
#line 2303 "nd-flow-expr.ypp"
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
#line 5166 "nd-flow-expr.cpp"
    break;

  case 292: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FLOAT  */
#line 2315 "nd-flow-expr.ypp"
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
#line 5183 "nd-flow-expr.cpp"
    break;

  case 293: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_TRUE  */
#line 2327 "nd-flow-expr.ypp"
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
#line 5201 "nd-flow-expr.cpp"
    break;

  case 294: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FALSE  */
#line 2340 "nd-flow-expr.ypp"
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
#line 5219 "nd-flow-expr.cpp"
    break;

  case 295: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_NAME  */
#line 2355 "nd-flow-expr.ypp"
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
#line 5244 "nd-flow-expr.cpp"
    break;

  case 296: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_REGEX  */
#line 2375 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        string key((yyvsp[-2].buffer)); json jvalue;
        if (_NDFP_params_intel &&
            flow_intel(key, *_NDFP_params_intel, jvalue) && jvalue.is_string()) {
            string rx((yyvsp[0].buffer));
            string search(jvalue.get<string>());

            _NDFP_result = ((yyval.bool_result) = ! _NDFP_regex_search(rx, search));
        }

        _NDFP_debugf("Flow intel %s (RX) != %s? %s\n",
            key.c_str(), search.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 5264 "nd-flow-expr.cpp"
    break;

  case 297: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2390 "nd-flow-expr.ypp"
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
#line 5281 "nd-flow-expr.cpp"
    break;

  case 298: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_SIGNED  */
#line 2402 "nd-flow-expr.ypp"
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
#line 5298 "nd-flow-expr.cpp"
    break;

  case 299: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FLOAT  */
#line 2414 "nd-flow-expr.ypp"
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
#line 5315 "nd-flow-expr.cpp"
    break;

  case 300: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_TRUE  */
#line 2426 "nd-flow-expr.ypp"
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
#line 5333 "nd-flow-expr.cpp"
    break;

  case 301: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FALSE  */
#line 2439 "nd-flow-expr.ypp"
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
#line 5351 "nd-flow-expr.cpp"
    break;

  case 302: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2454 "nd-flow-expr.ypp"
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
#line 5368 "nd-flow-expr.cpp"
    break;

  case 303: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_SIGNED  */
#line 2466 "nd-flow-expr.ypp"
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
#line 5385 "nd-flow-expr.cpp"
    break;

  case 304: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_FLOAT  */
#line 2478 "nd-flow-expr.ypp"
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
#line 5402 "nd-flow-expr.cpp"
    break;

  case 305: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2492 "nd-flow-expr.ypp"
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
#line 5419 "nd-flow-expr.cpp"
    break;

  case 306: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_SIGNED  */
#line 2504 "nd-flow-expr.ypp"
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
#line 5436 "nd-flow-expr.cpp"
    break;

  case 307: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_FLOAT  */
#line 2516 "nd-flow-expr.ypp"
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
#line 5453 "nd-flow-expr.cpp"
    break;

  case 308: /* expr_intel: FLOW_INTEL '>' VALUE_UNSIGNED  */
#line 2530 "nd-flow-expr.ypp"
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
#line 5470 "nd-flow-expr.cpp"
    break;

  case 309: /* expr_intel: FLOW_INTEL '>' VALUE_SIGNED  */
#line 2542 "nd-flow-expr.ypp"
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
#line 5487 "nd-flow-expr.cpp"
    break;

  case 310: /* expr_intel: FLOW_INTEL '>' VALUE_FLOAT  */
#line 2554 "nd-flow-expr.ypp"
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
#line 5504 "nd-flow-expr.cpp"
    break;

  case 311: /* expr_intel: FLOW_INTEL '<' VALUE_UNSIGNED  */
#line 2568 "nd-flow-expr.ypp"
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
#line 5521 "nd-flow-expr.cpp"
    break;

  case 312: /* expr_intel: FLOW_INTEL '<' VALUE_SIGNED  */
#line 2580 "nd-flow-expr.ypp"
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
#line 5538 "nd-flow-expr.cpp"
    break;

  case 313: /* expr_intel: FLOW_INTEL '<' VALUE_FLOAT  */
#line 2592 "nd-flow-expr.ypp"
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
#line 5555 "nd-flow-expr.cpp"
    break;

  case 314: /* expr_tls_version: FLOW_TLS_VERSION  */
#line 2607 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != 0));
        _NDFP_debugf("TLS version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5564 "nd-flow-expr.cpp"
    break;

  case 315: /* expr_tls_version: '!' FLOW_TLS_VERSION  */
#line 2611 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == 0));
        _NDFP_debugf("TLS version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5573 "nd-flow-expr.cpp"
    break;

  case 316: /* expr_tls_version: FLOW_TLS_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 2615 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5582 "nd-flow-expr.cpp"
    break;

  case 317: /* expr_tls_version: FLOW_TLS_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2619 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5591 "nd-flow-expr.cpp"
    break;

  case 318: /* expr_tls_version: FLOW_TLS_VERSION CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2623 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5600 "nd-flow-expr.cpp"
    break;

  case 319: /* expr_tls_version: FLOW_TLS_VERSION CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2627 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5609 "nd-flow-expr.cpp"
    break;

  case 320: /* expr_tls_version: FLOW_TLS_VERSION '>' VALUE_UNSIGNED  */
#line 2631 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5618 "nd-flow-expr.cpp"
    break;

  case 321: /* expr_tls_version: FLOW_TLS_VERSION '<' VALUE_UNSIGNED  */
#line 2635 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5627 "nd-flow-expr.cpp"
    break;

  case 322: /* expr_tls_cipher: FLOW_TLS_CIPHER  */
#line 2642 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != 0));
        _NDFP_debugf("TLS cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5636 "nd-flow-expr.cpp"
    break;

  case 323: /* expr_tls_cipher: '!' FLOW_TLS_CIPHER  */
#line 2646 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == 0));
        _NDFP_debugf("TLS cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5645 "nd-flow-expr.cpp"
    break;

  case 324: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_EQUAL VALUE_UNSIGNED  */
#line 2650 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5654 "nd-flow-expr.cpp"
    break;

  case 325: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2654 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5663 "nd-flow-expr.cpp"
    break;

  case 326: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2658 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5672 "nd-flow-expr.cpp"
    break;

  case 327: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2662 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5681 "nd-flow-expr.cpp"
    break;

  case 328: /* expr_tls_cipher: FLOW_TLS_CIPHER '>' VALUE_UNSIGNED  */
#line 2666 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5690 "nd-flow-expr.cpp"
    break;

  case 329: /* expr_tls_cipher: FLOW_TLS_CIPHER '<' VALUE_UNSIGNED  */
#line 2670 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5699 "nd-flow-expr.cpp"
    break;

  case 330: /* expr_tls_ech: FLOW_TLS_ECH  */
#line 2677 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != 0));
        _NDFP_debugf("TLS ECH version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5708 "nd-flow-expr.cpp"
    break;

  case 331: /* expr_tls_ech: '!' FLOW_TLS_ECH  */
#line 2681 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == 0));
        _NDFP_debugf("TLS ECH version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5717 "nd-flow-expr.cpp"
    break;

  case 332: /* expr_tls_ech: FLOW_TLS_ECH CMP_EQUAL VALUE_UNSIGNED  */
#line 2685 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5726 "nd-flow-expr.cpp"
    break;

  case 333: /* expr_tls_ech: FLOW_TLS_ECH CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2689 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5735 "nd-flow-expr.cpp"
    break;

  case 334: /* expr_tls_ech: FLOW_TLS_ECH CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2693 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5744 "nd-flow-expr.cpp"
    break;

  case 335: /* expr_tls_ech: FLOW_TLS_ECH CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2697 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5753 "nd-flow-expr.cpp"
    break;

  case 336: /* expr_tls_ech: FLOW_TLS_ECH '>' VALUE_UNSIGNED  */
#line 2701 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5762 "nd-flow-expr.cpp"
    break;

  case 337: /* expr_tls_ech: FLOW_TLS_ECH '<' VALUE_UNSIGNED  */
#line 2705 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5771 "nd-flow-expr.cpp"
    break;

  case 338: /* expr_tls_ja4: FLOW_TLS_JA4  */
#line 2712 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5783 "nd-flow-expr.cpp"
    break;

  case 339: /* expr_tls_ja4: '!' FLOW_TLS_JA4  */
#line 2719 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5795 "nd-flow-expr.cpp"
    break;

  case 340: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_NAME  */
#line 2726 "nd-flow-expr.ypp"
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
#line 5819 "nd-flow-expr.cpp"
    break;

  case 341: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_NAME  */
#line 2745 "nd-flow-expr.ypp"
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
#line 5843 "nd-flow-expr.cpp"
    break;

  case 342: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_REGEX  */
#line 2764 "nd-flow-expr.ypp"
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
#line 5862 "nd-flow-expr.cpp"
    break;

  case 343: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_REGEX  */
#line 2778 "nd-flow-expr.ypp"
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
#line 5881 "nd-flow-expr.cpp"
    break;

  case 344: /* expr_origin: FLOW_ORIGIN  */
#line 2795 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5890 "nd-flow-expr.cpp"
    break;

  case 345: /* expr_origin: '!' FLOW_ORIGIN  */
#line 2799 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5899 "nd-flow-expr.cpp"
    break;

  case 346: /* expr_origin: FLOW_ORIGIN CMP_EQUAL value_origin_type  */
#line 2803 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5908 "nd-flow-expr.cpp"
    break;

  case 347: /* expr_origin: FLOW_ORIGIN CMP_NOTEQUAL value_origin_type  */
#line 2807 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5917 "nd-flow-expr.cpp"
    break;

  case 348: /* value_origin_type: FLOW_ORIGIN_LOCAL  */
#line 2814 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 5923 "nd-flow-expr.cpp"
    break;

  case 349: /* value_origin_type: FLOW_ORIGIN_OTHER  */
#line 2815 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 5929 "nd-flow-expr.cpp"
    break;

  case 350: /* value_origin_type: FLOW_ORIGIN_UNKNOWN  */
#line 2816 "nd-flow-expr.ypp"
                          { (yyval.us_number) = (yyvsp[0].us_number); }
#line 5935 "nd-flow-expr.cpp"
    break;


#line 5939 "nd-flow-expr.cpp"

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

#line 2818 "nd-flow-expr.ypp"


ndFlowParser::ndFlowParser()
    : flow(NULL), local_mac{}, other_mac{},
    local_ip(NULL), other_ip(NULL), local_port(0), other_port(0),
    origin(0), expr_result(false), scanner(NULL)
{
    yyscan_t scanner;
    yylex_init_extra((void *)this, &scanner);

    if (scanner == NULL)
        throw string("Error creating scanner context");

    this->scanner = (void *)scanner;
}

ndFlowParser::~ndFlowParser()
{
    yylex_destroy((yyscan_t)scanner);
}

bool ndFlowParser::Parse(ndFlow::Ptr const& flow,
  const ndFlowParser::Params &params, const string &expr)
{
    this->flow = flow;
    expr_result = false;

    lock_guard<recursive_mutex> lg(flow->lock);

    switch (flow->lower_map) {
    case ndFlow::LowerMap::LOCAL:
        local_mac = flow->lower_mac.GetString().c_str();
        other_mac = flow->upper_mac.GetString().c_str();

        local_ip = &flow->lower_addr;
        other_ip = &flow->upper_addr;

        local_port = flow->lower_addr.GetPort();
        other_port = flow->upper_addr.GetPort();

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
        local_mac = flow->upper_mac.GetString().c_str();
        other_mac = flow->lower_mac.GetString().c_str();

        local_ip = &flow->upper_addr;
        other_ip = &flow->lower_addr;

        local_port = flow->upper_addr.GetPort();
        other_port = flow->lower_addr.GetPort();

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

    auto ji = params.find("intel");
    params_intel = (ji != params.end()) ? &ji->second : nullptr;

    YY_BUFFER_STATE flow_expr_scan_buffer;
    flow_expr_scan_buffer = yy_scan_bytes(
        expr.c_str(), expr.size(), (yyscan_t)scanner
    );

    if (flow_expr_scan_buffer == NULL)
        throw string("Error allocating flow expression scan buffer");

    yy_switch_to_buffer(flow_expr_scan_buffer, (yyscan_t)scanner);

    int rc = 0;

    try {
        rc = yyparse((yyscan_t)scanner);
    } catch (...) {
        this->flow.reset();
        yy_delete_buffer(flow_expr_scan_buffer, scanner);
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
