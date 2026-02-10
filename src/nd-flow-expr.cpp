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
    FLOW_ANY_MAC = 263,            /* FLOW_ANY_MAC  */
    FLOW_LOCAL_MAC = 264,          /* FLOW_LOCAL_MAC  */
    FLOW_OTHER_MAC = 265,          /* FLOW_OTHER_MAC  */
    FLOW_SRC_MAC = 266,            /* FLOW_SRC_MAC  */
    FLOW_DST_MAC = 267,            /* FLOW_DST_MAC  */
    FLOW_ANY_IP = 268,             /* FLOW_ANY_IP  */
    FLOW_LOCAL_IP = 269,           /* FLOW_LOCAL_IP  */
    FLOW_OTHER_IP = 270,           /* FLOW_OTHER_IP  */
    FLOW_SRC_IP = 271,             /* FLOW_SRC_IP  */
    FLOW_DST_IP = 272,             /* FLOW_DST_IP  */
    FLOW_ANY_PORT = 273,           /* FLOW_ANY_PORT  */
    FLOW_LOCAL_PORT = 274,         /* FLOW_LOCAL_PORT  */
    FLOW_OTHER_PORT = 275,         /* FLOW_OTHER_PORT  */
    FLOW_SRC_PORT = 276,           /* FLOW_SRC_PORT  */
    FLOW_DST_PORT = 277,           /* FLOW_DST_PORT  */
    FLOW_TUNNEL_TYPE = 278,        /* FLOW_TUNNEL_TYPE  */
    FLOW_DETECTION_COMPLETE = 279, /* FLOW_DETECTION_COMPLETE  */
    FLOW_DETECTION_GUESSED = 280,  /* FLOW_DETECTION_GUESSED  */
    FLOW_DETECTION_INIT = 281,     /* FLOW_DETECTION_INIT  */
    FLOW_DETECTION_UPDATED = 282,  /* FLOW_DETECTION_UPDATED  */
    FLOW_DHC_HIT = 283,            /* FLOW_DHC_HIT  */
    FLOW_FHC_HIT = 284,            /* FLOW_FHC_HIT  */
    FLOW_IP_NAT = 285,             /* FLOW_IP_NAT  */
    FLOW_EXPIRING = 286,           /* FLOW_EXPIRING  */
    FLOW_EXPIRED = 287,            /* FLOW_EXPIRED  */
    FLOW_SOFT_DISSECTOR = 288,     /* FLOW_SOFT_DISSECTOR  */
    FLOW_RISKS = 289,              /* FLOW_RISKS  */
    FLOW_NDPI_RISK_SCORE = 290,    /* FLOW_NDPI_RISK_SCORE  */
    FLOW_NDPI_RISK_SCORE_CLIENT = 291, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
    FLOW_NDPI_RISK_SCORE_SERVER = 292, /* FLOW_NDPI_RISK_SCORE_SERVER  */
    FLOW_CATEGORY = 293,           /* FLOW_CATEGORY  */
    FLOW_CATEGORY_ID = 294,        /* FLOW_CATEGORY_ID  */
    FLOW_APPLICATION = 295,        /* FLOW_APPLICATION  */
    FLOW_APPLICATION_CATEGORY = 296, /* FLOW_APPLICATION_CATEGORY  */
    FLOW_APPLICATION_CATEGORY_ID = 297, /* FLOW_APPLICATION_CATEGORY_ID  */
    FLOW_DOMAIN_CATEGORY = 298,    /* FLOW_DOMAIN_CATEGORY  */
    FLOW_DOMAIN_CATEGORY_ID = 299, /* FLOW_DOMAIN_CATEGORY_ID  */
    FLOW_NETWORK_CATEGORY = 300,   /* FLOW_NETWORK_CATEGORY  */
    FLOW_NETWORK_CATEGORY_ID = 301, /* FLOW_NETWORK_CATEGORY_ID  */
    FLOW_LOCAL_NETWORK_CATEGORY = 302, /* FLOW_LOCAL_NETWORK_CATEGORY  */
    FLOW_LOCAL_NETWORK_CATEGORY_ID = 303, /* FLOW_LOCAL_NETWORK_CATEGORY_ID  */
    FLOW_OTHER_NETWORK_CATEGORY = 304, /* FLOW_OTHER_NETWORK_CATEGORY  */
    FLOW_OTHER_NETWORK_CATEGORY_ID = 305, /* FLOW_OTHER_NETWORK_CATEGORY_ID  */
    FLOW_SRC_NETWORK_CATEGORY = 306, /* FLOW_SRC_NETWORK_CATEGORY  */
    FLOW_SRC_NETWORK_CATEGORY_ID = 307, /* FLOW_SRC_NETWORK_CATEGORY_ID  */
    FLOW_DST_NETWORK_CATEGORY = 308, /* FLOW_DST_NETWORK_CATEGORY  */
    FLOW_DST_NETWORK_CATEGORY_ID = 309, /* FLOW_DST_NETWORK_CATEGORY_ID  */
    FLOW_TAG = 310,                /* FLOW_TAG  */
    FLOW_TAG_CATEGORY = 311,       /* FLOW_TAG_CATEGORY  */
    FLOW_TAG_CATEGORY_ID = 312,    /* FLOW_TAG_CATEGORY_ID  */
    FLOW_PROTOCOL = 313,           /* FLOW_PROTOCOL  */
    FLOW_PROTOCOL_CATEGORY = 314,  /* FLOW_PROTOCOL_CATEGORY  */
    FLOW_PROTOCOL_CATEGORY_ID = 315, /* FLOW_PROTOCOL_CATEGORY_ID  */
    FLOW_DETECTED_HOSTNAME = 316,  /* FLOW_DETECTED_HOSTNAME  */
    FLOW_DNS_HOSTNAME = 317,       /* FLOW_DNS_HOSTNAME  */
    FLOW_ORIGIN = 318,             /* FLOW_ORIGIN  */
    FLOW_CT_MARK = 319,            /* FLOW_CT_MARK  */
    FLOW_TLS_VERSION = 320,        /* FLOW_TLS_VERSION  */
    FLOW_TLS_CIPHER = 321,         /* FLOW_TLS_CIPHER  */
    FLOW_TLS_ECH = 322,            /* FLOW_TLS_ECH  */
    FLOW_TLS_JA4 = 323,            /* FLOW_TLS_JA4  */
    FLOW_IFACE = 324,              /* FLOW_IFACE  */
    FLOW_IFACE_NFQ_SRC = 325,      /* FLOW_IFACE_NFQ_SRC  */
    FLOW_IFACE_NFQ_DST = 326,      /* FLOW_IFACE_NFQ_DST  */
    FLOW_OTHER_UNKNOWN = 327,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 328,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 329,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 330,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 331,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 332,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 333,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 334,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 335,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 336,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 337,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 338,         /* FLOW_TUNNEL_GTP  */
    FLOW_INTEL = 339,              /* FLOW_INTEL  */
    CMP_EQUAL = 340,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 341,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 342,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 343,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 344,                /* BOOL_AND  */
    BOOL_OR = 345,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 346,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 347,              /* VALUE_TRUE  */
    VALUE_FALSE = 348,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 349,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 350,              /* VALUE_NAME  */
    VALUE_REGEX = 351,             /* VALUE_REGEX  */
    VALUE_ADDR_TAG = 352,          /* VALUE_ADDR_TAG  */
    VALUE_ADDR_IPV4 = 353,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 354,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 355,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 356,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 357,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 358,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 359              /* VALUE_FLOAT  */
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
#define FLOW_ANY_MAC 263
#define FLOW_LOCAL_MAC 264
#define FLOW_OTHER_MAC 265
#define FLOW_SRC_MAC 266
#define FLOW_DST_MAC 267
#define FLOW_ANY_IP 268
#define FLOW_LOCAL_IP 269
#define FLOW_OTHER_IP 270
#define FLOW_SRC_IP 271
#define FLOW_DST_IP 272
#define FLOW_ANY_PORT 273
#define FLOW_LOCAL_PORT 274
#define FLOW_OTHER_PORT 275
#define FLOW_SRC_PORT 276
#define FLOW_DST_PORT 277
#define FLOW_TUNNEL_TYPE 278
#define FLOW_DETECTION_COMPLETE 279
#define FLOW_DETECTION_GUESSED 280
#define FLOW_DETECTION_INIT 281
#define FLOW_DETECTION_UPDATED 282
#define FLOW_DHC_HIT 283
#define FLOW_FHC_HIT 284
#define FLOW_IP_NAT 285
#define FLOW_EXPIRING 286
#define FLOW_EXPIRED 287
#define FLOW_SOFT_DISSECTOR 288
#define FLOW_RISKS 289
#define FLOW_NDPI_RISK_SCORE 290
#define FLOW_NDPI_RISK_SCORE_CLIENT 291
#define FLOW_NDPI_RISK_SCORE_SERVER 292
#define FLOW_CATEGORY 293
#define FLOW_CATEGORY_ID 294
#define FLOW_APPLICATION 295
#define FLOW_APPLICATION_CATEGORY 296
#define FLOW_APPLICATION_CATEGORY_ID 297
#define FLOW_DOMAIN_CATEGORY 298
#define FLOW_DOMAIN_CATEGORY_ID 299
#define FLOW_NETWORK_CATEGORY 300
#define FLOW_NETWORK_CATEGORY_ID 301
#define FLOW_LOCAL_NETWORK_CATEGORY 302
#define FLOW_LOCAL_NETWORK_CATEGORY_ID 303
#define FLOW_OTHER_NETWORK_CATEGORY 304
#define FLOW_OTHER_NETWORK_CATEGORY_ID 305
#define FLOW_SRC_NETWORK_CATEGORY 306
#define FLOW_SRC_NETWORK_CATEGORY_ID 307
#define FLOW_DST_NETWORK_CATEGORY 308
#define FLOW_DST_NETWORK_CATEGORY_ID 309
#define FLOW_TAG 310
#define FLOW_TAG_CATEGORY 311
#define FLOW_TAG_CATEGORY_ID 312
#define FLOW_PROTOCOL 313
#define FLOW_PROTOCOL_CATEGORY 314
#define FLOW_PROTOCOL_CATEGORY_ID 315
#define FLOW_DETECTED_HOSTNAME 316
#define FLOW_DNS_HOSTNAME 317
#define FLOW_ORIGIN 318
#define FLOW_CT_MARK 319
#define FLOW_TLS_VERSION 320
#define FLOW_TLS_CIPHER 321
#define FLOW_TLS_ECH 322
#define FLOW_TLS_JA4 323
#define FLOW_IFACE 324
#define FLOW_IFACE_NFQ_SRC 325
#define FLOW_IFACE_NFQ_DST 326
#define FLOW_OTHER_UNKNOWN 327
#define FLOW_OTHER_UNSUPPORTED 328
#define FLOW_OTHER_LOCAL 329
#define FLOW_OTHER_MULTICAST 330
#define FLOW_OTHER_BROADCAST 331
#define FLOW_OTHER_REMOTE 332
#define FLOW_OTHER_ERROR 333
#define FLOW_ORIGIN_LOCAL 334
#define FLOW_ORIGIN_OTHER 335
#define FLOW_ORIGIN_UNKNOWN 336
#define FLOW_TUNNEL_NONE 337
#define FLOW_TUNNEL_GTP 338
#define FLOW_INTEL 339
#define CMP_EQUAL 340
#define CMP_NOTEQUAL 341
#define CMP_GTHANEQUAL 342
#define CMP_LTHANEQUAL 343
#define BOOL_AND 344
#define BOOL_OR 345
#define VALUE_ADDR_IPMASK 346
#define VALUE_TRUE 347
#define VALUE_FALSE 348
#define VALUE_ADDR_MAC 349
#define VALUE_NAME 350
#define VALUE_REGEX 351
#define VALUE_ADDR_TAG 352
#define VALUE_ADDR_IPV4 353
#define VALUE_ADDR_IPV4_CIDR 354
#define VALUE_ADDR_IPV6 355
#define VALUE_ADDR_IPV6_CIDR 356
#define VALUE_SIGNED 357
#define VALUE_UNSIGNED 358
#define VALUE_FLOAT 359

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

#line 473 "nd-flow-expr.cpp"

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
  YYSYMBOL_FLOW_ANY_MAC = 8,               /* FLOW_ANY_MAC  */
  YYSYMBOL_FLOW_LOCAL_MAC = 9,             /* FLOW_LOCAL_MAC  */
  YYSYMBOL_FLOW_OTHER_MAC = 10,            /* FLOW_OTHER_MAC  */
  YYSYMBOL_FLOW_SRC_MAC = 11,              /* FLOW_SRC_MAC  */
  YYSYMBOL_FLOW_DST_MAC = 12,              /* FLOW_DST_MAC  */
  YYSYMBOL_FLOW_ANY_IP = 13,               /* FLOW_ANY_IP  */
  YYSYMBOL_FLOW_LOCAL_IP = 14,             /* FLOW_LOCAL_IP  */
  YYSYMBOL_FLOW_OTHER_IP = 15,             /* FLOW_OTHER_IP  */
  YYSYMBOL_FLOW_SRC_IP = 16,               /* FLOW_SRC_IP  */
  YYSYMBOL_FLOW_DST_IP = 17,               /* FLOW_DST_IP  */
  YYSYMBOL_FLOW_ANY_PORT = 18,             /* FLOW_ANY_PORT  */
  YYSYMBOL_FLOW_LOCAL_PORT = 19,           /* FLOW_LOCAL_PORT  */
  YYSYMBOL_FLOW_OTHER_PORT = 20,           /* FLOW_OTHER_PORT  */
  YYSYMBOL_FLOW_SRC_PORT = 21,             /* FLOW_SRC_PORT  */
  YYSYMBOL_FLOW_DST_PORT = 22,             /* FLOW_DST_PORT  */
  YYSYMBOL_FLOW_TUNNEL_TYPE = 23,          /* FLOW_TUNNEL_TYPE  */
  YYSYMBOL_FLOW_DETECTION_COMPLETE = 24,   /* FLOW_DETECTION_COMPLETE  */
  YYSYMBOL_FLOW_DETECTION_GUESSED = 25,    /* FLOW_DETECTION_GUESSED  */
  YYSYMBOL_FLOW_DETECTION_INIT = 26,       /* FLOW_DETECTION_INIT  */
  YYSYMBOL_FLOW_DETECTION_UPDATED = 27,    /* FLOW_DETECTION_UPDATED  */
  YYSYMBOL_FLOW_DHC_HIT = 28,              /* FLOW_DHC_HIT  */
  YYSYMBOL_FLOW_FHC_HIT = 29,              /* FLOW_FHC_HIT  */
  YYSYMBOL_FLOW_IP_NAT = 30,               /* FLOW_IP_NAT  */
  YYSYMBOL_FLOW_EXPIRING = 31,             /* FLOW_EXPIRING  */
  YYSYMBOL_FLOW_EXPIRED = 32,              /* FLOW_EXPIRED  */
  YYSYMBOL_FLOW_SOFT_DISSECTOR = 33,       /* FLOW_SOFT_DISSECTOR  */
  YYSYMBOL_FLOW_RISKS = 34,                /* FLOW_RISKS  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE = 35,      /* FLOW_NDPI_RISK_SCORE  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_CLIENT = 36, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_SERVER = 37, /* FLOW_NDPI_RISK_SCORE_SERVER  */
  YYSYMBOL_FLOW_CATEGORY = 38,             /* FLOW_CATEGORY  */
  YYSYMBOL_FLOW_CATEGORY_ID = 39,          /* FLOW_CATEGORY_ID  */
  YYSYMBOL_FLOW_APPLICATION = 40,          /* FLOW_APPLICATION  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY = 41, /* FLOW_APPLICATION_CATEGORY  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY_ID = 42, /* FLOW_APPLICATION_CATEGORY_ID  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY = 43,      /* FLOW_DOMAIN_CATEGORY  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY_ID = 44,   /* FLOW_DOMAIN_CATEGORY_ID  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY = 45,     /* FLOW_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY_ID = 46,  /* FLOW_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_LOCAL_NETWORK_CATEGORY = 47, /* FLOW_LOCAL_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_LOCAL_NETWORK_CATEGORY_ID = 48, /* FLOW_LOCAL_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_OTHER_NETWORK_CATEGORY = 49, /* FLOW_OTHER_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_OTHER_NETWORK_CATEGORY_ID = 50, /* FLOW_OTHER_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_SRC_NETWORK_CATEGORY = 51, /* FLOW_SRC_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_SRC_NETWORK_CATEGORY_ID = 52, /* FLOW_SRC_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_DST_NETWORK_CATEGORY = 53, /* FLOW_DST_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_DST_NETWORK_CATEGORY_ID = 54, /* FLOW_DST_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_TAG = 55,                  /* FLOW_TAG  */
  YYSYMBOL_FLOW_TAG_CATEGORY = 56,         /* FLOW_TAG_CATEGORY  */
  YYSYMBOL_FLOW_TAG_CATEGORY_ID = 57,      /* FLOW_TAG_CATEGORY_ID  */
  YYSYMBOL_FLOW_PROTOCOL = 58,             /* FLOW_PROTOCOL  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY = 59,    /* FLOW_PROTOCOL_CATEGORY  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY_ID = 60, /* FLOW_PROTOCOL_CATEGORY_ID  */
  YYSYMBOL_FLOW_DETECTED_HOSTNAME = 61,    /* FLOW_DETECTED_HOSTNAME  */
  YYSYMBOL_FLOW_DNS_HOSTNAME = 62,         /* FLOW_DNS_HOSTNAME  */
  YYSYMBOL_FLOW_ORIGIN = 63,               /* FLOW_ORIGIN  */
  YYSYMBOL_FLOW_CT_MARK = 64,              /* FLOW_CT_MARK  */
  YYSYMBOL_FLOW_TLS_VERSION = 65,          /* FLOW_TLS_VERSION  */
  YYSYMBOL_FLOW_TLS_CIPHER = 66,           /* FLOW_TLS_CIPHER  */
  YYSYMBOL_FLOW_TLS_ECH = 67,              /* FLOW_TLS_ECH  */
  YYSYMBOL_FLOW_TLS_JA4 = 68,              /* FLOW_TLS_JA4  */
  YYSYMBOL_FLOW_IFACE = 69,                /* FLOW_IFACE  */
  YYSYMBOL_FLOW_IFACE_NFQ_SRC = 70,        /* FLOW_IFACE_NFQ_SRC  */
  YYSYMBOL_FLOW_IFACE_NFQ_DST = 71,        /* FLOW_IFACE_NFQ_DST  */
  YYSYMBOL_FLOW_OTHER_UNKNOWN = 72,        /* FLOW_OTHER_UNKNOWN  */
  YYSYMBOL_FLOW_OTHER_UNSUPPORTED = 73,    /* FLOW_OTHER_UNSUPPORTED  */
  YYSYMBOL_FLOW_OTHER_LOCAL = 74,          /* FLOW_OTHER_LOCAL  */
  YYSYMBOL_FLOW_OTHER_MULTICAST = 75,      /* FLOW_OTHER_MULTICAST  */
  YYSYMBOL_FLOW_OTHER_BROADCAST = 76,      /* FLOW_OTHER_BROADCAST  */
  YYSYMBOL_FLOW_OTHER_REMOTE = 77,         /* FLOW_OTHER_REMOTE  */
  YYSYMBOL_FLOW_OTHER_ERROR = 78,          /* FLOW_OTHER_ERROR  */
  YYSYMBOL_FLOW_ORIGIN_LOCAL = 79,         /* FLOW_ORIGIN_LOCAL  */
  YYSYMBOL_FLOW_ORIGIN_OTHER = 80,         /* FLOW_ORIGIN_OTHER  */
  YYSYMBOL_FLOW_ORIGIN_UNKNOWN = 81,       /* FLOW_ORIGIN_UNKNOWN  */
  YYSYMBOL_FLOW_TUNNEL_NONE = 82,          /* FLOW_TUNNEL_NONE  */
  YYSYMBOL_FLOW_TUNNEL_GTP = 83,           /* FLOW_TUNNEL_GTP  */
  YYSYMBOL_FLOW_INTEL = 84,                /* FLOW_INTEL  */
  YYSYMBOL_CMP_EQUAL = 85,                 /* CMP_EQUAL  */
  YYSYMBOL_CMP_NOTEQUAL = 86,              /* CMP_NOTEQUAL  */
  YYSYMBOL_CMP_GTHANEQUAL = 87,            /* CMP_GTHANEQUAL  */
  YYSYMBOL_CMP_LTHANEQUAL = 88,            /* CMP_LTHANEQUAL  */
  YYSYMBOL_BOOL_AND = 89,                  /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 90,                   /* BOOL_OR  */
  YYSYMBOL_VALUE_ADDR_IPMASK = 91,         /* VALUE_ADDR_IPMASK  */
  YYSYMBOL_VALUE_TRUE = 92,                /* VALUE_TRUE  */
  YYSYMBOL_VALUE_FALSE = 93,               /* VALUE_FALSE  */
  YYSYMBOL_VALUE_ADDR_MAC = 94,            /* VALUE_ADDR_MAC  */
  YYSYMBOL_VALUE_NAME = 95,                /* VALUE_NAME  */
  YYSYMBOL_VALUE_REGEX = 96,               /* VALUE_REGEX  */
  YYSYMBOL_VALUE_ADDR_TAG = 97,            /* VALUE_ADDR_TAG  */
  YYSYMBOL_VALUE_ADDR_IPV4 = 98,           /* VALUE_ADDR_IPV4  */
  YYSYMBOL_VALUE_ADDR_IPV4_CIDR = 99,      /* VALUE_ADDR_IPV4_CIDR  */
  YYSYMBOL_VALUE_ADDR_IPV6 = 100,          /* VALUE_ADDR_IPV6  */
  YYSYMBOL_VALUE_ADDR_IPV6_CIDR = 101,     /* VALUE_ADDR_IPV6_CIDR  */
  YYSYMBOL_VALUE_SIGNED = 102,             /* VALUE_SIGNED  */
  YYSYMBOL_VALUE_UNSIGNED = 103,           /* VALUE_UNSIGNED  */
  YYSYMBOL_VALUE_FLOAT = 104,              /* VALUE_FLOAT  */
  YYSYMBOL_105_ = 105,                     /* ';'  */
  YYSYMBOL_106_ = 106,                     /* '('  */
  YYSYMBOL_107_ = 107,                     /* ')'  */
  YYSYMBOL_108_ = 108,                     /* '!'  */
  YYSYMBOL_109_ = 109,                     /* '>'  */
  YYSYMBOL_110_ = 110,                     /* '<'  */
  YYSYMBOL_YYACCEPT = 111,                 /* $accept  */
  YYSYMBOL_exprs = 112,                    /* exprs  */
  YYSYMBOL_expr = 113,                     /* expr  */
  YYSYMBOL_expr_ip_proto = 114,            /* expr_ip_proto  */
  YYSYMBOL_expr_ip_dscp = 115,             /* expr_ip_dscp  */
  YYSYMBOL_expr_ip_version = 116,          /* expr_ip_version  */
  YYSYMBOL_expr_vlan_id = 117,             /* expr_vlan_id  */
  YYSYMBOL_expr_other_type = 118,          /* expr_other_type  */
  YYSYMBOL_value_other_type = 119,         /* value_other_type  */
  YYSYMBOL_expr_any_mac = 120,             /* expr_any_mac  */
  YYSYMBOL_expr_local_mac = 121,           /* expr_local_mac  */
  YYSYMBOL_expr_other_mac = 122,           /* expr_other_mac  */
  YYSYMBOL_expr_src_mac = 123,             /* expr_src_mac  */
  YYSYMBOL_expr_dst_mac = 124,             /* expr_dst_mac  */
  YYSYMBOL_expr_any_ip = 125,              /* expr_any_ip  */
  YYSYMBOL_expr_local_ip = 126,            /* expr_local_ip  */
  YYSYMBOL_expr_other_ip = 127,            /* expr_other_ip  */
  YYSYMBOL_expr_src_ip = 128,              /* expr_src_ip  */
  YYSYMBOL_expr_dst_ip = 129,              /* expr_dst_ip  */
  YYSYMBOL_value_addr_ip = 130,            /* value_addr_ip  */
  YYSYMBOL_expr_any_port = 131,            /* expr_any_port  */
  YYSYMBOL_expr_local_port = 132,          /* expr_local_port  */
  YYSYMBOL_expr_other_port = 133,          /* expr_other_port  */
  YYSYMBOL_expr_src_port = 134,            /* expr_src_port  */
  YYSYMBOL_expr_dst_port = 135,            /* expr_dst_port  */
  YYSYMBOL_expr_tunnel_type = 136,         /* expr_tunnel_type  */
  YYSYMBOL_value_tunnel_type = 137,        /* value_tunnel_type  */
  YYSYMBOL_expr_detection_complete = 138,  /* expr_detection_complete  */
  YYSYMBOL_expr_detection_guessed = 139,   /* expr_detection_guessed  */
  YYSYMBOL_expr_detection_init = 140,      /* expr_detection_init  */
  YYSYMBOL_expr_detection_updated = 141,   /* expr_detection_updated  */
  YYSYMBOL_expr_dhc_hit = 142,             /* expr_dhc_hit  */
  YYSYMBOL_expr_fhc_hit = 143,             /* expr_fhc_hit  */
  YYSYMBOL_expr_ip_nat = 144,              /* expr_ip_nat  */
  YYSYMBOL_expr_expiring = 145,            /* expr_expiring  */
  YYSYMBOL_expr_expired = 146,             /* expr_expired  */
  YYSYMBOL_expr_soft_dissector = 147,      /* expr_soft_dissector  */
  YYSYMBOL_expr_app = 148,                 /* expr_app  */
  YYSYMBOL_expr_app_id = 149,              /* expr_app_id  */
  YYSYMBOL_expr_app_name = 150,            /* expr_app_name  */
  YYSYMBOL_expr_category = 151,            /* expr_category  */
  YYSYMBOL_expr_category_id = 152,         /* expr_category_id  */
  YYSYMBOL_expr_app_category = 153,        /* expr_app_category  */
  YYSYMBOL_expr_app_category_id = 154,     /* expr_app_category_id  */
  YYSYMBOL_expr_domain_category = 155,     /* expr_domain_category  */
  YYSYMBOL_expr_domain_category_id = 156,  /* expr_domain_category_id  */
  YYSYMBOL_expr_network_category = 157,    /* expr_network_category  */
  YYSYMBOL_expr_network_category_id = 158, /* expr_network_category_id  */
  YYSYMBOL_expr_local_network_category = 159, /* expr_local_network_category  */
  YYSYMBOL_expr_local_network_category_id = 160, /* expr_local_network_category_id  */
  YYSYMBOL_expr_other_network_category = 161, /* expr_other_network_category  */
  YYSYMBOL_expr_other_network_category_id = 162, /* expr_other_network_category_id  */
  YYSYMBOL_expr_src_network_category = 163, /* expr_src_network_category  */
  YYSYMBOL_expr_src_network_category_id = 164, /* expr_src_network_category_id  */
  YYSYMBOL_expr_dst_network_category = 165, /* expr_dst_network_category  */
  YYSYMBOL_expr_dst_network_category_id = 166, /* expr_dst_network_category_id  */
  YYSYMBOL_expr_tag_category = 167,        /* expr_tag_category  */
  YYSYMBOL_expr_tag_category_id = 168,     /* expr_tag_category_id  */
  YYSYMBOL_expr_proto = 169,               /* expr_proto  */
  YYSYMBOL_expr_proto_id = 170,            /* expr_proto_id  */
  YYSYMBOL_expr_proto_name = 171,          /* expr_proto_name  */
  YYSYMBOL_expr_proto_category = 172,      /* expr_proto_category  */
  YYSYMBOL_expr_proto_category_id = 173,   /* expr_proto_category_id  */
  YYSYMBOL_expr_detected_hostname = 174,   /* expr_detected_hostname  */
  YYSYMBOL_expr_dns_hostname = 175,        /* expr_dns_hostname  */
  YYSYMBOL_expr_risks = 176,               /* expr_risks  */
  YYSYMBOL_expr_risk_ndpi_score = 177,     /* expr_risk_ndpi_score  */
  YYSYMBOL_expr_risk_ndpi_score_client = 178, /* expr_risk_ndpi_score_client  */
  YYSYMBOL_expr_risk_ndpi_score_server = 179, /* expr_risk_ndpi_score_server  */
  YYSYMBOL_expr_fwmark = 180,              /* expr_fwmark  */
  YYSYMBOL_expr_iface = 181,               /* expr_iface  */
  YYSYMBOL_expr_iface_nfq_src = 182,       /* expr_iface_nfq_src  */
  YYSYMBOL_expr_iface_nfq_dst = 183,       /* expr_iface_nfq_dst  */
  YYSYMBOL_expr_intel = 184,               /* expr_intel  */
  YYSYMBOL_expr_tls_version = 185,         /* expr_tls_version  */
  YYSYMBOL_expr_tls_cipher = 186,          /* expr_tls_cipher  */
  YYSYMBOL_expr_tls_ech = 187,             /* expr_tls_ech  */
  YYSYMBOL_expr_tls_ja4 = 188,             /* expr_tls_ja4  */
  YYSYMBOL_expr_origin = 189,              /* expr_origin  */
  YYSYMBOL_value_origin_type = 190,        /* value_origin_type  */
  YYSYMBOL_expr_tag = 191                  /* expr_tag  */
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
#define YYLAST   772

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  111
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  81
/* YYNRULES -- Number of rules.  */
#define YYNRULES  450
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  688

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   359


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
       2,     2,     2,   108,     2,     2,     2,     2,     2,     2,
     106,   107,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   105,
     110,     2,   109,     2,     2,     2,     2,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   223,   223,   225,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   303,   307,   311,   316,   320,
     324,   328,   332,   336,   340,   347,   351,   358,   362,   369,
     373,   377,   381,   385,   389,   393,   397,   404,   410,   416,
     460,   507,   508,   509,   510,   511,   512,   513,   517,   532,
     545,   561,   579,   586,   592,   601,   613,   620,   627,   636,
     648,   655,   662,   671,   683,   690,   697,   706,   718,   731,
     744,   759,   777,   783,   789,   798,   810,   816,   822,   831,
     843,   849,   855,   864,   876,   882,   888,   897,   909,   910,
     911,   912,   916,   922,   928,   934,   940,   946,   952,   958,
     967,   971,   975,   979,   983,   987,   991,   995,  1002,  1006,
    1010,  1014,  1018,  1022,  1026,  1030,  1037,  1041,  1045,  1049,
    1053,  1057,  1061,  1065,  1072,  1076,  1080,  1084,  1088,  1092,
    1096,  1100,  1107,  1113,  1119,  1138,  1160,  1161,  1164,  1168,
    1174,  1182,  1190,  1198,  1209,  1213,  1219,  1227,  1235,  1243,
    1254,  1258,  1264,  1272,  1280,  1288,  1299,  1303,  1309,  1317,
    1325,  1333,  1344,  1348,  1354,  1362,  1370,  1378,  1389,  1393,
    1399,  1407,  1415,  1423,  1434,  1438,  1442,  1446,  1450,  1454,
    1461,  1465,  1469,  1473,  1477,  1481,  1488,  1492,  1496,  1500,
    1504,  1508,  1515,  1519,  1523,  1527,  1531,  1535,  1542,  1548,
    1556,  1557,  1560,  1569,  1581,  1608,  1638,  1695,  1751,  1793,
    1838,  1855,  1875,  1882,  1893,  1910,  1930,  1937,  1948,  1974,
    2003,  2017,  2034,  2051,  2071,  2079,  2090,  2107,  2127,  2135,
    2146,  2163,  2183,  2191,  2202,  2219,  2239,  2247,  2258,  2275,
    2295,  2303,  2314,  2320,  2326,  2327,  2330,  2336,  2345,  2365,
    2387,  2404,  2424,  2432,  2443,  2450,  2457,  2476,  2495,  2509,
    2518,  2525,  2532,  2551,  2570,  2584,  2593,  2597,  2601,  2620,
    2639,  2652,  2668,  2672,  2676,  2680,  2684,  2688,  2692,  2696,
    2703,  2707,  2711,  2715,  2719,  2723,  2727,  2731,  2738,  2742,
    2746,  2750,  2754,  2758,  2762,  2766,  2773,  2781,  2789,  2797,
    2805,  2813,  2821,  2829,  2840,  2847,  2854,  2872,  2890,  2904,
    2913,  2924,  2935,  2956,  2975,  2989,  3006,  3017,  3028,  3047,
    3066,  3081,  3098,  3119,  3141,  3161,  3177,  3189,  3201,  3213,
    3226,  3241,  3261,  3277,  3289,  3301,  3313,  3326,  3341,  3353,
    3365,  3379,  3391,  3403,  3417,  3429,  3441,  3455,  3467,  3479,
    3494,  3498,  3502,  3506,  3510,  3514,  3518,  3522,  3529,  3533,
    3537,  3541,  3545,  3549,  3553,  3557,  3564,  3568,  3572,  3576,
    3580,  3584,  3588,  3592,  3599,  3606,  3613,  3632,  3651,  3665,
    3682,  3686,  3690,  3694,  3701,  3702,  3703,  3707,  3714,  3721,
    3749
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
  "FLOW_ANY_MAC", "FLOW_LOCAL_MAC", "FLOW_OTHER_MAC", "FLOW_SRC_MAC",
  "FLOW_DST_MAC", "FLOW_ANY_IP", "FLOW_LOCAL_IP", "FLOW_OTHER_IP",
  "FLOW_SRC_IP", "FLOW_DST_IP", "FLOW_ANY_PORT", "FLOW_LOCAL_PORT",
  "FLOW_OTHER_PORT", "FLOW_SRC_PORT", "FLOW_DST_PORT", "FLOW_TUNNEL_TYPE",
  "FLOW_DETECTION_COMPLETE", "FLOW_DETECTION_GUESSED",
  "FLOW_DETECTION_INIT", "FLOW_DETECTION_UPDATED", "FLOW_DHC_HIT",
  "FLOW_FHC_HIT", "FLOW_IP_NAT", "FLOW_EXPIRING", "FLOW_EXPIRED",
  "FLOW_SOFT_DISSECTOR", "FLOW_RISKS", "FLOW_NDPI_RISK_SCORE",
  "FLOW_NDPI_RISK_SCORE_CLIENT", "FLOW_NDPI_RISK_SCORE_SERVER",
  "FLOW_CATEGORY", "FLOW_CATEGORY_ID", "FLOW_APPLICATION",
  "FLOW_APPLICATION_CATEGORY", "FLOW_APPLICATION_CATEGORY_ID",
  "FLOW_DOMAIN_CATEGORY", "FLOW_DOMAIN_CATEGORY_ID",
  "FLOW_NETWORK_CATEGORY", "FLOW_NETWORK_CATEGORY_ID",
  "FLOW_LOCAL_NETWORK_CATEGORY", "FLOW_LOCAL_NETWORK_CATEGORY_ID",
  "FLOW_OTHER_NETWORK_CATEGORY", "FLOW_OTHER_NETWORK_CATEGORY_ID",
  "FLOW_SRC_NETWORK_CATEGORY", "FLOW_SRC_NETWORK_CATEGORY_ID",
  "FLOW_DST_NETWORK_CATEGORY", "FLOW_DST_NETWORK_CATEGORY_ID", "FLOW_TAG",
  "FLOW_TAG_CATEGORY", "FLOW_TAG_CATEGORY_ID", "FLOW_PROTOCOL",
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
  "VALUE_REGEX", "VALUE_ADDR_TAG", "VALUE_ADDR_IPV4",
  "VALUE_ADDR_IPV4_CIDR", "VALUE_ADDR_IPV6", "VALUE_ADDR_IPV6_CIDR",
  "VALUE_SIGNED", "VALUE_UNSIGNED", "VALUE_FLOAT", "';'", "'('", "')'",
  "'!'", "'>'", "'<'", "$accept", "exprs", "expr", "expr_ip_proto",
  "expr_ip_dscp", "expr_ip_version", "expr_vlan_id", "expr_other_type",
  "value_other_type", "expr_any_mac", "expr_local_mac", "expr_other_mac",
  "expr_src_mac", "expr_dst_mac", "expr_any_ip", "expr_local_ip",
  "expr_other_ip", "expr_src_ip", "expr_dst_ip", "value_addr_ip",
  "expr_any_port", "expr_local_port", "expr_other_port", "expr_src_port",
  "expr_dst_port", "expr_tunnel_type", "value_tunnel_type",
  "expr_detection_complete", "expr_detection_guessed",
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
  "expr_risk_ndpi_score_server", "expr_fwmark", "expr_iface",
  "expr_iface_nfq_src", "expr_iface_nfq_dst", "expr_intel",
  "expr_tls_version", "expr_tls_cipher", "expr_tls_ech", "expr_tls_ja4",
  "expr_origin", "value_origin_type", "expr_tag", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-84)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -84,     1,   -84,   -12,   -83,    14,    -8,   117,   225,   252,
     264,   344,   353,   355,   357,   359,   361,   363,    -4,    97,
     101,   161,   187,   365,   367,   369,   371,   373,   375,   377,
     379,   381,   383,   385,   387,   191,   199,   203,   389,   391,
     393,     9,   395,   397,   399,   401,   403,   405,   407,   409,
     411,   413,   415,   417,   419,   421,   423,   425,   427,   429,
     431,   433,   435,   437,   217,   232,   236,   243,   439,   441,
     443,   445,   247,   107,   198,     3,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
      87,   105,   111,   133,   152,   155,   178,   222,   257,   262,
     496,   498,   500,   501,   502,   503,   299,   299,   115,   145,
     160,   186,   201,   246,   254,   269,   270,   334,   281,   286,
     291,   296,   301,   306,   311,   316,   321,   326,   504,   505,
     506,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   272,   272,
     440,   442,   444,   446,   448,   450,   452,   454,   456,   458,
     460,   462,   464,   466,   468,   470,   472,   474,   476,   478,
      -7,     0,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     561,   562,   555,   556,   142,   204,   565,   558,   211,   567,
     568,   563,   564,   569,   570,   566,   571,   573,   575,   572,
     574,   576,   577,   578,   579,   581,   583,   580,   582,   584,
     585,   586,   587,   589,   591,   592,   593,   588,   590,   241,
     244,   597,   599,   594,   595,   477,   479,   481,   483,    10,
      10,   596,   598,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   615,   616,
     617,   618,   619,   620,   621,   485,   487,   489,   491,   493,
     495,   497,   499,   148,   266,    77,   190,   330,   333,    -3,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   107,   107,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   507,   507
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,    77,     0,     0,    89,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,   160,
     168,   176,   184,   192,   198,   204,   210,   216,   222,   228,
     234,   240,   246,   252,   326,   332,   340,   348,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   447,     0,     0,   302,     0,
       0,   314,   320,   440,   356,   410,   418,   426,   434,   364,
     370,   376,   382,     0,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    37,   260,   261,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,   304,
     305,    55,    56,    57,    58,    59,    60,    61,    62,    68,
      69,    70,    71,    72,    63,    64,    65,    66,    67,    73,
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
      78,    90,    98,   153,   161,   169,   177,   185,   193,   199,
     205,   211,   217,   223,   229,   235,   241,   247,   253,   327,
     333,   341,   349,   259,   448,   303,   315,   321,   441,   357,
     411,   419,   427,   435,   365,   371,   377,   383,     0,     0,
       3,    79,    80,    81,    82,    83,    84,    87,    88,    85,
      86,    91,    92,    93,    94,    95,    96,   101,   102,   103,
     104,   105,   106,   107,    99,   100,   108,   110,   109,   111,
     112,   114,   113,   115,   116,   118,   117,   119,   120,   122,
     121,   123,   124,   126,   125,   127,   130,   148,   149,   150,
     151,   128,   131,   129,   134,   132,   135,   133,   138,   136,
     139,   137,   142,   140,   143,   141,   146,   144,   147,   145,
     154,   155,   156,   157,   158,   159,   162,   163,   164,   165,
     166,   167,   170,   171,   172,   173,   174,   175,   178,   179,
     180,   181,   182,   183,   186,   187,   188,   189,   190,   191,
     196,   197,   194,   195,   200,   201,   202,   203,   206,   207,
     208,   209,   212,   213,   214,   215,   218,   219,   220,   221,
     224,   225,   226,   227,   230,   231,   232,   233,   236,   237,
     238,   239,   242,   243,   244,   245,   248,   249,   250,   251,
     254,   255,   256,   257,   328,   330,   329,   331,   334,   335,
     336,   337,   338,   339,   342,   343,   344,   345,   346,   347,
     350,   351,   352,   353,   354,   355,   266,   267,   268,   269,
     264,   262,   265,   263,   270,   272,   271,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   449,   450,   298,   299,   300,   301,   308,   306,
     309,   307,   310,   311,   312,   313,   316,   318,   317,   319,
     322,   324,   323,   325,   444,   445,   446,   442,   443,   358,
     359,   360,   361,   362,   363,   412,   413,   414,   415,   416,
     417,   420,   421,   422,   423,   424,   425,   428,   429,   430,
     431,   432,   433,   436,   438,   437,   439,   366,   368,   367,
     369,   372,   374,   373,   375,   378,   380,   379,   381,   389,
     390,   384,   385,   387,   386,   388,   396,   397,   391,   392,
     394,   393,   395,   399,   398,   400,   402,   401,   403,   405,
     404,   406,   408,   407,   409,    76,    75,    74
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -84,   -84,   -73,   -84,   -84,   -84,   -84,   -84,   559,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,    13,
     -84,   -84,   -84,   -84,   -84,   -84,   553,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   288,
     -84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    75,    76,    77,    78,    79,    80,   414,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,   441,
      91,    92,    93,    94,    95,    96,   492,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   617,
     149
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     349,     2,   156,   157,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,   150,   151,   152,   153,   160,   161,   162,
     163,   188,   189,   190,   191,    72,   388,   389,   534,   614,
     615,   616,   388,   389,   266,   536,   535,   154,   155,   158,
     159,   164,   165,   537,   685,   192,   193,    73,   390,    74,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,   673,
     674,   675,   194,   195,   196,   197,   200,   201,   202,   203,
     391,    72,   443,   445,   447,   449,   451,   453,   455,   457,
     459,   350,   166,   167,   351,   352,   198,   199,   392,   416,
     204,   205,   417,    73,   393,    74,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   394,   560,   373,   418,
     659,   660,   419,   661,   662,   561,   206,   207,   208,   209,
     663,   664,   665,   374,   420,   395,   375,   421,   396,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     210,   211,   212,   213,   214,   215,   242,   243,   244,   245,
     422,   397,   387,   423,   248,   249,   250,   251,   254,   255,
     256,   257,   676,   677,   678,   424,   216,   217,   425,   562,
     246,   247,   311,   312,   313,   314,   566,   563,   252,   253,
     168,   169,   258,   259,   567,   686,   687,   317,   318,   319,
     320,   323,   324,   325,   326,   398,   315,   316,   329,   330,
     331,   332,   343,   344,   345,   346,   598,   170,   171,   600,
     426,   321,   322,   427,   599,   327,   328,   601,   428,   172,
     173,   429,   333,   334,   490,   491,   347,   348,   666,   667,
     399,   668,   669,   430,   432,   400,   431,   433,   670,   671,
     672,   407,   408,   409,   410,   411,   412,   413,   436,   437,
     438,   439,   440,   442,   437,   438,   439,   440,   444,   437,
     438,   439,   440,   446,   437,   438,   439,   440,   448,   437,
     438,   439,   440,   450,   437,   438,   439,   440,   452,   437,
     438,   439,   440,   454,   437,   438,   439,   440,   456,   437,
     438,   439,   440,   458,   437,   438,   439,   440,   434,   174,
     175,   435,   679,   680,   681,   682,   683,   684,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   260,   261,   262,   263,   264,   265,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   335,   336,   337,   338,   339,   340,
     341,   342,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   606,   607,   608,   609,   610,   611,   612,   613,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   388,   389,   618,   401,
       0,   402,     0,   403,   404,   405,   406,   460,   461,   462,
       0,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   538,   539,
     540,   541,   542,   543,   544,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     564,   565,   568,   569,   572,   573,   570,   571,   576,   574,
     577,   580,   581,     0,   575,   578,   584,   579,   585,   588,
     589,   582,   583,   586,   592,   587,   593,   594,   595,   590,
     591,   596,   602,   597,   603,     0,     0,   604,   605,   619,
       0,   620,     0,   621,   622,   623,   624,   625,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,     0,   415,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   493
};

static const yytype_int16 yycheck[] =
{
      73,     0,    85,    86,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    85,    86,    87,    88,    85,    86,    87,
      88,    85,    86,    87,    88,    84,    89,    90,    95,    79,
      80,    81,    89,    90,    85,    95,   103,   109,   110,    85,
      86,   109,   110,   103,   107,   109,   110,   106,   105,   108,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,   102,
     103,   104,    85,    86,    87,    88,    85,    86,    87,    88,
     103,    84,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     3,    85,    86,     6,     7,   109,   110,   103,    94,
     109,   110,    97,   106,   103,   108,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,   103,    95,    40,    94,
      92,    93,    97,    95,    96,   103,    85,    86,    87,    88,
     102,   103,   104,    55,    94,   103,    58,    97,   103,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
     109,   110,    85,    86,    87,    88,    85,    86,    87,    88,
      94,   103,    84,    97,    85,    86,    87,    88,    85,    86,
      87,    88,   102,   103,   104,    94,   109,   110,    97,    95,
     109,   110,    85,    86,    87,    88,    95,   103,   109,   110,
      85,    86,   109,   110,   103,   388,   389,    85,    86,    87,
      88,    85,    86,    87,    88,   103,   109,   110,    85,    86,
      87,    88,    85,    86,    87,    88,    95,    85,    86,    95,
      94,   109,   110,    97,   103,   109,   110,   103,    94,    85,
      86,    97,   109,   110,    82,    83,   109,   110,    92,    93,
     103,    95,    96,    94,    94,   103,    97,    97,   102,   103,
     104,    72,    73,    74,    75,    76,    77,    78,    97,    98,
      99,   100,   101,    97,    98,    99,   100,   101,    97,    98,
      99,   100,   101,    97,    98,    99,   100,   101,    97,    98,
      99,   100,   101,    97,    98,    99,   100,   101,    97,    98,
      99,   100,   101,    97,    98,    99,   100,   101,    97,    98,
      99,   100,   101,    97,    98,    99,   100,   101,    94,    85,
      86,    97,   102,   103,   104,   102,   103,   104,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    95,    96,    95,    96,    95,    96,    95,    96,
      95,    96,    95,    96,    95,    96,    95,    96,    95,    96,
      95,    96,    95,    96,    95,    96,    89,    90,   310,   103,
      -1,   103,    -1,   103,   103,   103,   103,   103,   103,   103,
      -1,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,    95,    95,   103,   103,
      95,   103,    95,    95,    95,    95,   103,   103,    95,   103,
      95,    95,    95,    -1,   103,   103,    95,   103,    95,    95,
      95,   103,   103,   103,    95,   103,    95,    95,    95,   103,
     103,   103,    95,   103,    95,    -1,    -1,   103,   103,   103,
      -1,   103,    -1,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   219
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   112,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    84,   106,   108,   113,   114,   115,   116,   117,
     118,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   131,   132,   133,   134,   135,   136,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   191,
      85,    86,    87,    88,   109,   110,    85,    86,    85,    86,
      85,    86,    87,    88,   109,   110,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      87,    88,   109,   110,    85,    86,    87,    88,   109,   110,
      85,    86,    87,    88,   109,   110,    85,    86,    87,    88,
     109,   110,    85,    86,    87,    88,   109,   110,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    85,    86,    85,    86,    85,    86,
      85,    86,    85,    86,    87,    88,   109,   110,    85,    86,
      87,    88,   109,   110,    85,    86,    87,    88,   109,   110,
      85,    86,    85,    86,    85,    86,    85,    85,    86,    85,
      86,    85,    86,    85,    86,    85,    86,    85,    86,    85,
      86,    85,    86,    85,    86,    85,    86,    85,    86,    85,
      86,    85,    86,    85,    86,    85,    86,    85,    86,    85,
      86,    85,    86,    85,    86,    85,    86,    85,    86,    85,
      86,    85,    86,    87,    88,   109,   110,    85,    86,    87,
      88,   109,   110,    85,    86,    87,    88,   109,   110,    85,
      86,    87,    88,   109,   110,    85,    86,    85,    86,    85,
      86,    85,    86,    85,    86,    87,    88,   109,   110,   113,
       3,     6,     7,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    40,    55,    58,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    84,    89,    90,
     105,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,    72,    73,    74,
      75,    76,    77,    78,   119,   119,    94,    97,    94,    97,
      94,    97,    94,    97,    94,    97,    94,    97,    94,    97,
      94,    97,    94,    97,    94,    97,    97,    98,    99,   100,
     101,   130,    97,   130,    97,   130,    97,   130,    97,   130,
      97,   130,    97,   130,    97,   130,    97,   130,    97,   130,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
      82,    83,   137,   137,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    95,   103,    95,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,    95,    95,   103,   103,
      95,   103,    95,   103,    95,   103,    95,   103,    95,    95,
     103,   103,    95,    95,   103,   103,    95,    95,   103,   103,
      95,    95,   103,   103,    95,    95,   103,   103,    95,    95,
     103,   103,    95,    95,    95,    95,   103,   103,    95,   103,
      95,   103,    95,    95,   103,   103,    95,    96,    95,    96,
      95,    96,    95,    96,    79,    80,    81,   190,   190,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,    95,    96,    95,    96,    95,    96,    95,
      96,    95,    96,    95,    96,    95,    96,    95,    96,    92,
      93,    95,    96,   102,   103,   104,    92,    93,    95,    96,
     102,   103,   104,   102,   103,   104,   102,   103,   104,   102,
     103,   104,   102,   103,   104,   107,   113,   113
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   111,   112,   112,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   114,   114,   114,
     114,   114,   114,   114,   114,   115,   115,   116,   116,   117,
     117,   117,   117,   117,   117,   117,   117,   118,   118,   118,
     118,   119,   119,   119,   119,   119,   119,   119,   120,   120,
     120,   120,   121,   121,   121,   121,   122,   122,   122,   122,
     123,   123,   123,   123,   124,   124,   124,   124,   125,   125,
     125,   125,   126,   126,   126,   126,   127,   127,   127,   127,
     128,   128,   128,   128,   129,   129,   129,   129,   130,   130,
     130,   130,   131,   131,   131,   131,   131,   131,   131,   131,
     132,   132,   132,   132,   132,   132,   132,   132,   133,   133,
     133,   133,   133,   133,   133,   133,   134,   134,   134,   134,
     134,   134,   134,   134,   135,   135,   135,   135,   135,   135,
     135,   135,   136,   136,   136,   136,   137,   137,   138,   138,
     138,   138,   138,   138,   139,   139,   139,   139,   139,   139,
     140,   140,   140,   140,   140,   140,   141,   141,   141,   141,
     141,   141,   142,   142,   142,   142,   142,   142,   143,   143,
     143,   143,   143,   143,   144,   144,   144,   144,   144,   144,
     145,   145,   145,   145,   145,   145,   146,   146,   146,   146,
     146,   146,   147,   147,   147,   147,   147,   147,   148,   148,
     148,   148,   149,   149,   150,   150,   151,   151,   152,   152,
     153,   153,   154,   154,   155,   155,   156,   156,   157,   157,
     158,   158,   159,   159,   160,   160,   161,   161,   162,   162,
     163,   163,   164,   164,   165,   165,   166,   166,   167,   167,
     168,   168,   169,   169,   169,   169,   170,   170,   171,   171,
     172,   172,   173,   173,   174,   174,   174,   174,   174,   174,
     175,   175,   175,   175,   175,   175,   176,   176,   176,   176,
     176,   176,   177,   177,   177,   177,   177,   177,   177,   177,
     178,   178,   178,   178,   178,   178,   178,   178,   179,   179,
     179,   179,   179,   179,   179,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   181,   181,   181,   181,   181,   181,
     182,   182,   182,   182,   182,   182,   183,   183,   183,   183,
     183,   183,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     185,   185,   185,   185,   185,   185,   185,   185,   186,   186,
     186,   186,   186,   186,   186,   186,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   188,   188,   188,   188,   188,
     189,   189,   189,   189,   190,   190,   190,   191,   191,   191,
     191
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
       1,     1,     1,     1,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     1,     1,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     2,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     1,     1,     1,     1,     2,     3,
       3
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
  case 74: /* expr: expr BOOL_OR expr  */
#line 299 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) || (yyvsp[0].bool_result)));
        _NDFP_debugf("OR (%d || %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 2349 "nd-flow-expr.cpp"
    break;

  case 75: /* expr: expr BOOL_AND expr  */
#line 303 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) && (yyvsp[0].bool_result)));
        _NDFP_debugf("AND (%d && %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 2358 "nd-flow-expr.cpp"
    break;

  case 76: /* expr: '(' expr ')'  */
#line 307 "nd-flow-expr.ypp"
                   { _NDFP_result = ((yyval.bool_result) = (yyvsp[-1].bool_result)); }
#line 2364 "nd-flow-expr.cpp"
    break;

  case 77: /* expr_ip_proto: FLOW_IP_PROTO  */
#line 311 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != 0));
        _NDFP_debugf(
            "IP Protocol is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2374 "nd-flow-expr.cpp"
    break;

  case 78: /* expr_ip_proto: '!' FLOW_IP_PROTO  */
#line 316 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == 0));
        _NDFP_debugf("IP Protocol is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2383 "nd-flow-expr.cpp"
    break;

  case 79: /* expr_ip_proto: FLOW_IP_PROTO CMP_EQUAL VALUE_UNSIGNED  */
#line 320 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2392 "nd-flow-expr.cpp"
    break;

  case 80: /* expr_ip_proto: FLOW_IP_PROTO CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 324 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2401 "nd-flow-expr.cpp"
    break;

  case 81: /* expr_ip_proto: FLOW_IP_PROTO CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 328 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol >= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2410 "nd-flow-expr.cpp"
    break;

  case 82: /* expr_ip_proto: FLOW_IP_PROTO CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 332 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol <= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2419 "nd-flow-expr.cpp"
    break;

  case 83: /* expr_ip_proto: FLOW_IP_PROTO '>' VALUE_UNSIGNED  */
#line 336 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol > (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2428 "nd-flow-expr.cpp"
    break;

  case 84: /* expr_ip_proto: FLOW_IP_PROTO '<' VALUE_UNSIGNED  */
#line 340 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol < (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2437 "nd-flow-expr.cpp"
    break;

  case 85: /* expr_ip_dscp: FLOW_IP_DSCP CMP_EQUAL VALUE_UNSIGNED  */
#line 347 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2446 "nd-flow-expr.cpp"
    break;

  case 86: /* expr_ip_dscp: FLOW_IP_DSCP CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 351 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2455 "nd-flow-expr.cpp"
    break;

  case 87: /* expr_ip_version: FLOW_IP_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 358 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2464 "nd-flow-expr.cpp"
    break;

  case 88: /* expr_ip_version: FLOW_IP_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 362 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2473 "nd-flow-expr.cpp"
    break;

  case 89: /* expr_vlan_id: FLOW_VLAN_ID  */
#line 369 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0));
        _NDFP_debugf("VLAN ID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2482 "nd-flow-expr.cpp"
    break;

  case 90: /* expr_vlan_id: '!' FLOW_VLAN_ID  */
#line 373 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0));
        _NDFP_debugf("VLAN ID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2491 "nd-flow-expr.cpp"
    break;

  case 91: /* expr_vlan_id: FLOW_VLAN_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 377 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2500 "nd-flow-expr.cpp"
    break;

  case 92: /* expr_vlan_id: FLOW_VLAN_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 381 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2509 "nd-flow-expr.cpp"
    break;

  case 93: /* expr_vlan_id: FLOW_VLAN_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 385 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2518 "nd-flow-expr.cpp"
    break;

  case 94: /* expr_vlan_id: FLOW_VLAN_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 389 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2527 "nd-flow-expr.cpp"
    break;

  case 95: /* expr_vlan_id: FLOW_VLAN_ID '>' VALUE_UNSIGNED  */
#line 393 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2536 "nd-flow-expr.cpp"
    break;

  case 96: /* expr_vlan_id: FLOW_VLAN_ID '<' VALUE_UNSIGNED  */
#line 397 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2545 "nd-flow-expr.cpp"
    break;

  case 97: /* expr_other_type: FLOW_OTHER_TYPE  */
#line 404 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2556 "nd-flow-expr.cpp"
    break;

  case 98: /* expr_other_type: '!' FLOW_OTHER_TYPE  */
#line 410 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2567 "nd-flow-expr.cpp"
    break;

  case 99: /* expr_other_type: FLOW_OTHER_TYPE CMP_EQUAL value_other_type  */
#line 416 "nd-flow-expr.ypp"
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
#line 2616 "nd-flow-expr.cpp"
    break;

  case 100: /* expr_other_type: FLOW_OTHER_TYPE CMP_NOTEQUAL value_other_type  */
#line 460 "nd-flow-expr.ypp"
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
#line 2665 "nd-flow-expr.cpp"
    break;

  case 101: /* value_other_type: FLOW_OTHER_UNKNOWN  */
#line 507 "nd-flow-expr.ypp"
                         { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2671 "nd-flow-expr.cpp"
    break;

  case 102: /* value_other_type: FLOW_OTHER_UNSUPPORTED  */
#line 508 "nd-flow-expr.ypp"
                             { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2677 "nd-flow-expr.cpp"
    break;

  case 103: /* value_other_type: FLOW_OTHER_LOCAL  */
#line 509 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2683 "nd-flow-expr.cpp"
    break;

  case 104: /* value_other_type: FLOW_OTHER_MULTICAST  */
#line 510 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2689 "nd-flow-expr.cpp"
    break;

  case 105: /* value_other_type: FLOW_OTHER_BROADCAST  */
#line 511 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2695 "nd-flow-expr.cpp"
    break;

  case 106: /* value_other_type: FLOW_OTHER_REMOTE  */
#line 512 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2701 "nd-flow-expr.cpp"
    break;

  case 107: /* value_other_type: FLOW_OTHER_ERROR  */
#line 513 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2707 "nd-flow-expr.cpp"
    break;

  case 108: /* expr_any_mac: FLOW_ANY_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 517 "nd-flow-expr.ypp"
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
#line 2727 "nd-flow-expr.cpp"
    break;

  case 109: /* expr_any_mac: FLOW_ANY_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 532 "nd-flow-expr.ypp"
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
#line 2745 "nd-flow-expr.cpp"
    break;

  case 110: /* expr_any_mac: FLOW_ANY_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 545 "nd-flow-expr.ypp"
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
#line 2766 "nd-flow-expr.cpp"
    break;

  case 111: /* expr_any_mac: FLOW_ANY_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 561 "nd-flow-expr.ypp"
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
#line 2786 "nd-flow-expr.cpp"
    break;

  case 112: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 579 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2798 "nd-flow-expr.cpp"
    break;

  case 113: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 586 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2809 "nd-flow-expr.cpp"
    break;

  case 114: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 592 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == true));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2823 "nd-flow-expr.cpp"
    break;

  case 115: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 601 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == false));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2837 "nd-flow-expr.cpp"
    break;

  case 116: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 613 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2849 "nd-flow-expr.cpp"
    break;

  case 117: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 620 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2861 "nd-flow-expr.cpp"
    break;

  case 118: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 627 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == true));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2875 "nd-flow-expr.cpp"
    break;

  case 119: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 636 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == false));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2889 "nd-flow-expr.cpp"
    break;

  case 120: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 648 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2901 "nd-flow-expr.cpp"
    break;

  case 121: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 655 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2913 "nd-flow-expr.cpp"
    break;

  case 122: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 662 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == true));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2927 "nd-flow-expr.cpp"
    break;

  case 123: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 671 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == false));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2941 "nd-flow-expr.cpp"
    break;

  case 124: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 683 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2953 "nd-flow-expr.cpp"
    break;

  case 125: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 690 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2965 "nd-flow-expr.cpp"
    break;

  case 126: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 697 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == true));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2979 "nd-flow-expr.cpp"
    break;

  case 127: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 706 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == false));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2993 "nd-flow-expr.cpp"
    break;

  case 128: /* expr_any_ip: FLOW_ANY_IP CMP_EQUAL value_addr_ip  */
#line 718 "nd-flow-expr.ypp"
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
#line 3011 "nd-flow-expr.cpp"
    break;

  case 129: /* expr_any_ip: FLOW_ANY_IP CMP_NOTEQUAL value_addr_ip  */
#line 731 "nd-flow-expr.ypp"
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
#line 3029 "nd-flow-expr.cpp"
    break;

  case 130: /* expr_any_ip: FLOW_ANY_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 744 "nd-flow-expr.ypp"
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
#line 3049 "nd-flow-expr.cpp"
    break;

  case 131: /* expr_any_ip: FLOW_ANY_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 759 "nd-flow-expr.ypp"
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
#line 3069 "nd-flow-expr.cpp"
    break;

  case 132: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL value_addr_ip  */
#line 777 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3080 "nd-flow-expr.cpp"
    break;

  case 133: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL value_addr_ip  */
#line 783 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3091 "nd-flow-expr.cpp"
    break;

  case 134: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 789 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == true));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3105 "nd-flow-expr.cpp"
    break;

  case 135: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 798 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == false));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3119 "nd-flow-expr.cpp"
    break;

  case 136: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL value_addr_ip  */
#line 810 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3130 "nd-flow-expr.cpp"
    break;

  case 137: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL value_addr_ip  */
#line 816 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3141 "nd-flow-expr.cpp"
    break;

  case 138: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 822 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == true));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3155 "nd-flow-expr.cpp"
    break;

  case 139: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 831 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == false));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3169 "nd-flow-expr.cpp"
    break;

  case 140: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL value_addr_ip  */
#line 843 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Source IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3180 "nd-flow-expr.cpp"
    break;

  case 141: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL value_addr_ip  */
#line 849 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Source IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3191 "nd-flow-expr.cpp"
    break;

  case 142: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 855 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == true));
        _NDFP_debugf("Source IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3205 "nd-flow-expr.cpp"
    break;

  case 143: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 864 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == false));
        _NDFP_debugf("Source IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3219 "nd-flow-expr.cpp"
    break;

  case 144: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL value_addr_ip  */
#line 876 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Destination IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3230 "nd-flow-expr.cpp"
    break;

  case 145: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL value_addr_ip  */
#line 882 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Destination IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3241 "nd-flow-expr.cpp"
    break;

  case 146: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 888 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == true));
        _NDFP_debugf("Destination IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3255 "nd-flow-expr.cpp"
    break;

  case 147: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 897 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == false));
        _NDFP_debugf("Destination IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3269 "nd-flow-expr.cpp"
    break;

  case 148: /* value_addr_ip: VALUE_ADDR_IPV4  */
#line 909 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3275 "nd-flow-expr.cpp"
    break;

  case 149: /* value_addr_ip: VALUE_ADDR_IPV4_CIDR  */
#line 910 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3281 "nd-flow-expr.cpp"
    break;

  case 150: /* value_addr_ip: VALUE_ADDR_IPV6  */
#line 911 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3287 "nd-flow-expr.cpp"
    break;

  case 151: /* value_addr_ip: VALUE_ADDR_IPV6_CIDR  */
#line 912 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3293 "nd-flow-expr.cpp"
    break;

  case 152: /* expr_any_port: FLOW_ANY_PORT  */
#line 916 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Any port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3304 "nd-flow-expr.cpp"
    break;

  case 153: /* expr_any_port: '!' FLOW_ANY_PORT  */
#line 922 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Any port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3315 "nd-flow-expr.cpp"
    break;

  case 154: /* expr_any_port: FLOW_ANY_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 928 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3326 "nd-flow-expr.cpp"
    break;

  case 155: /* expr_any_port: FLOW_ANY_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 934 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3337 "nd-flow-expr.cpp"
    break;

  case 156: /* expr_any_port: FLOW_ANY_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 940 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3348 "nd-flow-expr.cpp"
    break;

  case 157: /* expr_any_port: FLOW_ANY_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 946 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3359 "nd-flow-expr.cpp"
    break;

  case 158: /* expr_any_port: FLOW_ANY_PORT '>' VALUE_UNSIGNED  */
#line 952 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3370 "nd-flow-expr.cpp"
    break;

  case 159: /* expr_any_port: FLOW_ANY_PORT '<' VALUE_UNSIGNED  */
#line 958 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        if (! _NDFP_result)
            _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Any port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3381 "nd-flow-expr.cpp"
    break;

  case 160: /* expr_local_port: FLOW_LOCAL_PORT  */
#line 967 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        _NDFP_debugf("Local port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3390 "nd-flow-expr.cpp"
    break;

  case 161: /* expr_local_port: '!' FLOW_LOCAL_PORT  */
#line 971 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        _NDFP_debugf("Local port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3399 "nd-flow-expr.cpp"
    break;

  case 162: /* expr_local_port: FLOW_LOCAL_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 975 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3408 "nd-flow-expr.cpp"
    break;

  case 163: /* expr_local_port: FLOW_LOCAL_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 979 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3417 "nd-flow-expr.cpp"
    break;

  case 164: /* expr_local_port: FLOW_LOCAL_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 983 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3426 "nd-flow-expr.cpp"
    break;

  case 165: /* expr_local_port: FLOW_LOCAL_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 987 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3435 "nd-flow-expr.cpp"
    break;

  case 166: /* expr_local_port: FLOW_LOCAL_PORT '>' VALUE_UNSIGNED  */
#line 991 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3444 "nd-flow-expr.cpp"
    break;

  case 167: /* expr_local_port: FLOW_LOCAL_PORT '<' VALUE_UNSIGNED  */
#line 995 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3453 "nd-flow-expr.cpp"
    break;

  case 168: /* expr_other_port: FLOW_OTHER_PORT  */
#line 1002 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Other port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3462 "nd-flow-expr.cpp"
    break;

  case 169: /* expr_other_port: '!' FLOW_OTHER_PORT  */
#line 1006 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Other port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3471 "nd-flow-expr.cpp"
    break;

  case 170: /* expr_other_port: FLOW_OTHER_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1010 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3480 "nd-flow-expr.cpp"
    break;

  case 171: /* expr_other_port: FLOW_OTHER_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1014 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3489 "nd-flow-expr.cpp"
    break;

  case 172: /* expr_other_port: FLOW_OTHER_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1018 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3498 "nd-flow-expr.cpp"
    break;

  case 173: /* expr_other_port: FLOW_OTHER_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1022 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3507 "nd-flow-expr.cpp"
    break;

  case 174: /* expr_other_port: FLOW_OTHER_PORT '>' VALUE_UNSIGNED  */
#line 1026 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3516 "nd-flow-expr.cpp"
    break;

  case 175: /* expr_other_port: FLOW_OTHER_PORT '<' VALUE_UNSIGNED  */
#line 1030 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3525 "nd-flow-expr.cpp"
    break;

  case 176: /* expr_src_port: FLOW_SRC_PORT  */
#line 1037 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != 0));
        _NDFP_debugf("Source port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3534 "nd-flow-expr.cpp"
    break;

  case 177: /* expr_src_port: '!' FLOW_SRC_PORT  */
#line 1041 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == 0));
        _NDFP_debugf("Source port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3543 "nd-flow-expr.cpp"
    break;

  case 178: /* expr_src_port: FLOW_SRC_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1045 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3552 "nd-flow-expr.cpp"
    break;

  case 179: /* expr_src_port: FLOW_SRC_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1049 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3561 "nd-flow-expr.cpp"
    break;

  case 180: /* expr_src_port: FLOW_SRC_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1053 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3570 "nd-flow-expr.cpp"
    break;

  case 181: /* expr_src_port: FLOW_SRC_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1057 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3579 "nd-flow-expr.cpp"
    break;

  case 182: /* expr_src_port: FLOW_SRC_PORT '>' VALUE_UNSIGNED  */
#line 1061 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3588 "nd-flow-expr.cpp"
    break;

  case 183: /* expr_src_port: FLOW_SRC_PORT '<' VALUE_UNSIGNED  */
#line 1065 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3597 "nd-flow-expr.cpp"
    break;

  case 184: /* expr_dst_port: FLOW_DST_PORT  */
#line 1072 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != 0));
        _NDFP_debugf("Destination port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3606 "nd-flow-expr.cpp"
    break;

  case 185: /* expr_dst_port: '!' FLOW_DST_PORT  */
#line 1076 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == 0));
        _NDFP_debugf("Destination port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3615 "nd-flow-expr.cpp"
    break;

  case 186: /* expr_dst_port: FLOW_DST_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 1080 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3624 "nd-flow-expr.cpp"
    break;

  case 187: /* expr_dst_port: FLOW_DST_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1084 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3633 "nd-flow-expr.cpp"
    break;

  case 188: /* expr_dst_port: FLOW_DST_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 1088 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3642 "nd-flow-expr.cpp"
    break;

  case 189: /* expr_dst_port: FLOW_DST_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 1092 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3651 "nd-flow-expr.cpp"
    break;

  case 190: /* expr_dst_port: FLOW_DST_PORT '>' VALUE_UNSIGNED  */
#line 1096 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3660 "nd-flow-expr.cpp"
    break;

  case 191: /* expr_dst_port: FLOW_DST_PORT '<' VALUE_UNSIGNED  */
#line 1100 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3669 "nd-flow-expr.cpp"
    break;

  case 192: /* expr_tunnel_type: FLOW_TUNNEL_TYPE  */
#line 1107 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3680 "nd-flow-expr.cpp"
    break;

  case 193: /* expr_tunnel_type: '!' FLOW_TUNNEL_TYPE  */
#line 1113 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type is none? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3691 "nd-flow-expr.cpp"
    break;

  case 194: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_EQUAL value_tunnel_type  */
#line 1119 "nd-flow-expr.ypp"
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
#line 3715 "nd-flow-expr.cpp"
    break;

  case 195: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_NOTEQUAL value_tunnel_type  */
#line 1138 "nd-flow-expr.ypp"
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
#line 3739 "nd-flow-expr.cpp"
    break;

  case 196: /* value_tunnel_type: FLOW_TUNNEL_NONE  */
#line 1160 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3745 "nd-flow-expr.cpp"
    break;

  case 197: /* value_tunnel_type: FLOW_TUNNEL_GTP  */
#line 1161 "nd-flow-expr.ypp"
                      { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3751 "nd-flow-expr.cpp"
    break;

  case 198: /* expr_detection_complete: FLOW_DETECTION_COMPLETE  */
#line 1164 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf("Detection was complete? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3760 "nd-flow-expr.cpp"
    break;

  case 199: /* expr_detection_complete: '!' FLOW_DETECTION_COMPLETE  */
#line 1168 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf(
            "Detection was not complete? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3771 "nd-flow-expr.cpp"
    break;

  case 200: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_TRUE  */
#line 1174 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == true
        ));
        _NDFP_debugf(
            "Detection complete == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3784 "nd-flow-expr.cpp"
    break;

  case 201: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_FALSE  */
#line 1182 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == false
        ));
        _NDFP_debugf(
            "Detection complete == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3797 "nd-flow-expr.cpp"
    break;

  case 202: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_TRUE  */
#line 1190 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != true
        ));
        _NDFP_debugf(
            "Detection complete != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3810 "nd-flow-expr.cpp"
    break;

  case 203: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_FALSE  */
#line 1198 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != false
        ));
        _NDFP_debugf(
            "Detection complete != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3823 "nd-flow-expr.cpp"
    break;

  case 204: /* expr_detection_guessed: FLOW_DETECTION_GUESSED  */
#line 1209 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf("Detection was guessed? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3832 "nd-flow-expr.cpp"
    break;

  case 205: /* expr_detection_guessed: '!' FLOW_DETECTION_GUESSED  */
#line 1213 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf(
            "Detection was not guessed? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3843 "nd-flow-expr.cpp"
    break;

  case 206: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_TRUE  */
#line 1219 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == true
        ));
        _NDFP_debugf(
            "Detection guessed == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3856 "nd-flow-expr.cpp"
    break;

  case 207: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_FALSE  */
#line 1227 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == false
        ));
        _NDFP_debugf(
            "Detection guessed == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3869 "nd-flow-expr.cpp"
    break;

  case 208: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_TRUE  */
#line 1235 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != true
        ));
        _NDFP_debugf(
            "Detection guessed != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3882 "nd-flow-expr.cpp"
    break;

  case 209: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_FALSE  */
#line 1243 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != false
        ));
        _NDFP_debugf(
            "Detection guessed != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3895 "nd-flow-expr.cpp"
    break;

  case 210: /* expr_detection_init: FLOW_DETECTION_INIT  */
#line 1254 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf("Detection was init? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3904 "nd-flow-expr.cpp"
    break;

  case 211: /* expr_detection_init: '!' FLOW_DETECTION_INIT  */
#line 1258 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf(
            "Detection was not init? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3915 "nd-flow-expr.cpp"
    break;

  case 212: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_TRUE  */
#line 1264 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == true
        ));
        _NDFP_debugf(
            "Detection init == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3928 "nd-flow-expr.cpp"
    break;

  case 213: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_FALSE  */
#line 1272 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == false
        ));
        _NDFP_debugf(
            "Detection init == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3941 "nd-flow-expr.cpp"
    break;

  case 214: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1280 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != true
        ));
        _NDFP_debugf(
            "Detection init != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3954 "nd-flow-expr.cpp"
    break;

  case 215: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1288 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != false
        ));
        _NDFP_debugf(
            "Detection init != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3967 "nd-flow-expr.cpp"
    break;

  case 216: /* expr_detection_updated: FLOW_DETECTION_UPDATED  */
#line 1299 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf("Detection was updated? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3976 "nd-flow-expr.cpp"
    break;

  case 217: /* expr_detection_updated: '!' FLOW_DETECTION_UPDATED  */
#line 1303 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf(
            "Detection was not updated? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3987 "nd-flow-expr.cpp"
    break;

  case 218: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_TRUE  */
#line 1309 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == true
        ));
        _NDFP_debugf(
            "Detection updated == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4000 "nd-flow-expr.cpp"
    break;

  case 219: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_FALSE  */
#line 1317 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == false
        ));
        _NDFP_debugf(
            "Detection updated == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4013 "nd-flow-expr.cpp"
    break;

  case 220: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_TRUE  */
#line 1325 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != true
        ));
        _NDFP_debugf(
            "Detection updated != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4026 "nd-flow-expr.cpp"
    break;

  case 221: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_FALSE  */
#line 1333 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != false
        ));
        _NDFP_debugf(
            "Detection updated != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4039 "nd-flow-expr.cpp"
    break;

  case 222: /* expr_dhc_hit: FLOW_DHC_HIT  */
#line 1344 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf("DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4048 "nd-flow-expr.cpp"
    break;

  case 223: /* expr_dhc_hit: '!' FLOW_DHC_HIT  */
#line 1348 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf(
            "DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4059 "nd-flow-expr.cpp"
    break;

  case 224: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1354 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == true
        ));
        _NDFP_debugf(
            "DHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4072 "nd-flow-expr.cpp"
    break;

  case 225: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1362 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == false
        ));
        _NDFP_debugf(
            "DHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4085 "nd-flow-expr.cpp"
    break;

  case 226: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1370 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != true
        ));
        _NDFP_debugf(
            "DHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4098 "nd-flow-expr.cpp"
    break;

  case 227: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1378 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != false
        ));
        _NDFP_debugf(
            "DHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4111 "nd-flow-expr.cpp"
    break;

  case 228: /* expr_fhc_hit: FLOW_FHC_HIT  */
#line 1389 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf("FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4120 "nd-flow-expr.cpp"
    break;

  case 229: /* expr_fhc_hit: '!' FLOW_FHC_HIT  */
#line 1393 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf(
            "FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4131 "nd-flow-expr.cpp"
    break;

  case 230: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1399 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == true
        ));
        _NDFP_debugf(
            "FHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4144 "nd-flow-expr.cpp"
    break;

  case 231: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1407 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == false
        ));
        _NDFP_debugf(
            "FHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4157 "nd-flow-expr.cpp"
    break;

  case 232: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1415 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != true
        ));
        _NDFP_debugf(
            "FHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4170 "nd-flow-expr.cpp"
    break;

  case 233: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1423 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != false
        ));
        _NDFP_debugf(
            "FHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4183 "nd-flow-expr.cpp"
    break;

  case 234: /* expr_ip_nat: FLOW_IP_NAT  */
#line 1434 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4192 "nd-flow-expr.cpp"
    break;

  case 235: /* expr_ip_nat: '!' FLOW_IP_NAT  */
#line 1438 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4201 "nd-flow-expr.cpp"
    break;

  case 236: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_TRUE  */
#line 1442 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4210 "nd-flow-expr.cpp"
    break;

  case 237: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_FALSE  */
#line 1446 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4219 "nd-flow-expr.cpp"
    break;

  case 238: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_TRUE  */
#line 1450 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != true));
        _NDFP_debugf("IP NAT != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4228 "nd-flow-expr.cpp"
    break;

  case 239: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_FALSE  */
#line 1454 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != false));
        _NDFP_debugf("IP NAT != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4237 "nd-flow-expr.cpp"
    break;

  case 240: /* expr_expiring: FLOW_EXPIRING  */
#line 1461 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4246 "nd-flow-expr.cpp"
    break;

  case 241: /* expr_expiring: '!' FLOW_EXPIRING  */
#line 1465 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4255 "nd-flow-expr.cpp"
    break;

  case 242: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_TRUE  */
#line 1469 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4264 "nd-flow-expr.cpp"
    break;

  case 243: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_FALSE  */
#line 1473 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4273 "nd-flow-expr.cpp"
    break;

  case 244: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_TRUE  */
#line 1477 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != true));
        _NDFP_debugf("Flow expiring != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4282 "nd-flow-expr.cpp"
    break;

  case 245: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_FALSE  */
#line 1481 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != false));
        _NDFP_debugf("Flow expiring != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4291 "nd-flow-expr.cpp"
    break;

  case 246: /* expr_expired: FLOW_EXPIRED  */
#line 1488 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4300 "nd-flow-expr.cpp"
    break;

  case 247: /* expr_expired: '!' FLOW_EXPIRED  */
#line 1492 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4309 "nd-flow-expr.cpp"
    break;

  case 248: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_TRUE  */
#line 1496 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4318 "nd-flow-expr.cpp"
    break;

  case 249: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_FALSE  */
#line 1500 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4327 "nd-flow-expr.cpp"
    break;

  case 250: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_TRUE  */
#line 1504 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != true));
        _NDFP_debugf("Flow expired != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4336 "nd-flow-expr.cpp"
    break;

  case 251: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_FALSE  */
#line 1508 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != false));
        _NDFP_debugf("Flow expired != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4345 "nd-flow-expr.cpp"
    break;

  case 252: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR  */
#line 1515 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4354 "nd-flow-expr.cpp"
    break;

  case 253: /* expr_soft_dissector: '!' FLOW_SOFT_DISSECTOR  */
#line 1519 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4363 "nd-flow-expr.cpp"
    break;

  case 254: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_TRUE  */
#line 1523 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4372 "nd-flow-expr.cpp"
    break;

  case 255: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_FALSE  */
#line 1527 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4381 "nd-flow-expr.cpp"
    break;

  case 256: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_TRUE  */
#line 1531 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != true));
        _NDFP_debugf("Soft dissector matched != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4390 "nd-flow-expr.cpp"
    break;

  case 257: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_FALSE  */
#line 1535 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != false));
        _NDFP_debugf("Soft dissector matched != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4399 "nd-flow-expr.cpp"
    break;

  case 258: /* expr_app: FLOW_APPLICATION  */
#line 1542 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) != 0
        ));
        _NDFP_debugf("Application detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4410 "nd-flow-expr.cpp"
    break;

  case 259: /* expr_app: '!' FLOW_APPLICATION  */
#line 1548 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) == 0
        ));
        _NDFP_debugf(
            "Application not detected? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4423 "nd-flow-expr.cpp"
    break;

  case 262: /* expr_app_id: FLOW_APPLICATION CMP_EQUAL VALUE_UNSIGNED  */
#line 1560 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf(
            "Application ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4437 "nd-flow-expr.cpp"
    break;

  case 263: /* expr_app_id: FLOW_APPLICATION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1569 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = false);

        _NDFP_debugf(
            "Application ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4451 "nd-flow-expr.cpp"
    break;

  case 264: /* expr_app_name: FLOW_APPLICATION CMP_EQUAL VALUE_NAME  */
#line 1581 "nd-flow-expr.ypp"
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
#line 4483 "nd-flow-expr.cpp"
    break;

  case 265: /* expr_app_name: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NAME  */
#line 1608 "nd-flow-expr.ypp"
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
#line 4515 "nd-flow-expr.cpp"
    break;

  case 266: /* expr_category: FLOW_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1638 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App/domain/network/tag category == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4577 "nd-flow-expr.cpp"
    break;

  case 267: /* expr_category: FLOW_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1695 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App/domain/network/tag category != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4635 "nd-flow-expr.cpp"
    break;

  case 268: /* expr_category_id: FLOW_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1751 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App/domain/network/tag category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4682 "nd-flow-expr.cpp"
    break;

  case 269: /* expr_category_id: FLOW_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1793 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App/domain/network/tag category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4729 "nd-flow-expr.cpp"
    break;

  case 270: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1838 "nd-flow-expr.ypp"
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
#line 4751 "nd-flow-expr.cpp"
    break;

  case 271: /* expr_app_category: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_NAME  */
#line 1855 "nd-flow-expr.ypp"
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
#line 4773 "nd-flow-expr.cpp"
    break;

  case 272: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1875 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4785 "nd-flow-expr.cpp"
    break;

  case 273: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1882 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category != %s? %s\n",
        (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4798 "nd-flow-expr.cpp"
    break;

  case 274: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1893 "nd-flow-expr.ypp"
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
#line 4820 "nd-flow-expr.cpp"
    break;

  case 275: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1910 "nd-flow-expr.ypp"
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
#line 4842 "nd-flow-expr.cpp"
    break;

  case 276: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1930 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4854 "nd-flow-expr.cpp"
    break;

  case 277: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1937 "nd-flow-expr.ypp"
                                                          {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4867 "nd-flow-expr.cpp"
    break;

  case 278: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1948 "nd-flow-expr.ypp"
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
#line 4898 "nd-flow-expr.cpp"
    break;

  case 279: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1974 "nd-flow-expr.ypp"
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
#line 4929 "nd-flow-expr.cpp"
    break;

  case 280: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2003 "nd-flow-expr.ypp"
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
#line 4948 "nd-flow-expr.cpp"
    break;

  case 281: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2017 "nd-flow-expr.ypp"
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
#line 4967 "nd-flow-expr.cpp"
    break;

  case 282: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2034 "nd-flow-expr.ypp"
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
#line 4989 "nd-flow-expr.cpp"
    break;

  case 283: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2051 "nd-flow-expr.ypp"
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
#line 5011 "nd-flow-expr.cpp"
    break;

  case 284: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2071 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5024 "nd-flow-expr.cpp"
    break;

  case 285: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2079 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5037 "nd-flow-expr.cpp"
    break;

  case 286: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2090 "nd-flow-expr.ypp"
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
#line 5059 "nd-flow-expr.cpp"
    break;

  case 287: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2107 "nd-flow-expr.ypp"
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
#line 5081 "nd-flow-expr.cpp"
    break;

  case 288: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2127 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5094 "nd-flow-expr.cpp"
    break;

  case 289: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2135 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5107 "nd-flow-expr.cpp"
    break;

  case 290: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2146 "nd-flow-expr.ypp"
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
#line 5129 "nd-flow-expr.cpp"
    break;

  case 291: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2163 "nd-flow-expr.ypp"
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
#line 5151 "nd-flow-expr.cpp"
    break;

  case 292: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2183 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5164 "nd-flow-expr.cpp"
    break;

  case 293: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2191 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5177 "nd-flow-expr.cpp"
    break;

  case 294: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2202 "nd-flow-expr.ypp"
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
#line 5199 "nd-flow-expr.cpp"
    break;

  case 295: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2219 "nd-flow-expr.ypp"
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
#line 5221 "nd-flow-expr.cpp"
    break;

  case 296: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2239 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5234 "nd-flow-expr.cpp"
    break;

  case 297: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2247 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5247 "nd-flow-expr.cpp"
    break;

  case 298: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2258 "nd-flow-expr.ypp"
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
#line 5269 "nd-flow-expr.cpp"
    break;

  case 299: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2275 "nd-flow-expr.ypp"
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
#line 5291 "nd-flow-expr.cpp"
    break;

  case 300: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2295 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5304 "nd-flow-expr.cpp"
    break;

  case 301: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2303 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5317 "nd-flow-expr.cpp"
    break;

  case 302: /* expr_proto: FLOW_PROTOCOL  */
#line 2314 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol != ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5328 "nd-flow-expr.cpp"
    break;

  case 303: /* expr_proto: '!' FLOW_PROTOCOL  */
#line 2320 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol == ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5339 "nd-flow-expr.cpp"
    break;

  case 306: /* expr_proto_id: FLOW_PROTOCOL CMP_EQUAL VALUE_UNSIGNED  */
#line 2330 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5350 "nd-flow-expr.cpp"
    break;

  case 307: /* expr_proto_id: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2336 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5361 "nd-flow-expr.cpp"
    break;

  case 308: /* expr_proto_name: FLOW_PROTOCOL CMP_EQUAL VALUE_NAME  */
#line 2345 "nd-flow-expr.ypp"
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
#line 5386 "nd-flow-expr.cpp"
    break;

  case 309: /* expr_proto_name: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NAME  */
#line 2365 "nd-flow-expr.ypp"
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
#line 5410 "nd-flow-expr.cpp"
    break;

  case 310: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2387 "nd-flow-expr.ypp"
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
#line 5432 "nd-flow-expr.cpp"
    break;

  case 311: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2404 "nd-flow-expr.ypp"
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
#line 5454 "nd-flow-expr.cpp"
    break;

  case 312: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2424 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5467 "nd-flow-expr.cpp"
    break;

  case 313: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2432 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5480 "nd-flow-expr.cpp"
    break;

  case 314: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME  */
#line 2443 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == false
        ));
        _NDFP_debugf("Detected hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5492 "nd-flow-expr.cpp"
    break;

  case 315: /* expr_detected_hostname: '!' FLOW_DETECTED_HOSTNAME  */
#line 2450 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == true
        ));
        _NDFP_debugf("Detected hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5504 "nd-flow-expr.cpp"
    break;

  case 316: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 2457 "nd-flow-expr.ypp"
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
#line 5528 "nd-flow-expr.cpp"
    break;

  case 317: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 2476 "nd-flow-expr.ypp"
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
#line 5552 "nd-flow-expr.cpp"
    break;

  case 318: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 2495 "nd-flow-expr.ypp"
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
#line 5571 "nd-flow-expr.cpp"
    break;

  case 319: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 2509 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Detected hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 5582 "nd-flow-expr.cpp"
    break;

  case 320: /* expr_dns_hostname: FLOW_DNS_HOSTNAME  */
#line 2518 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == false
        ));
        _NDFP_debugf("DNS hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5594 "nd-flow-expr.cpp"
    break;

  case 321: /* expr_dns_hostname: '!' FLOW_DNS_HOSTNAME  */
#line 2525 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == true
        ));
        _NDFP_debugf("DNS hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5606 "nd-flow-expr.cpp"
    break;

  case 322: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 2532 "nd-flow-expr.ypp"
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
#line 5630 "nd-flow-expr.cpp"
    break;

  case 323: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 2551 "nd-flow-expr.ypp"
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
#line 5654 "nd-flow-expr.cpp"
    break;

  case 324: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 2570 "nd-flow-expr.ypp"
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
#line 5673 "nd-flow-expr.cpp"
    break;

  case 325: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 2584 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("DNS hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 5684 "nd-flow-expr.cpp"
    break;

  case 326: /* expr_risks: FLOW_RISKS  */
#line 2593 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() != 0));
        _NDFP_debugf("Risks detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5693 "nd-flow-expr.cpp"
    break;

  case 327: /* expr_risks: '!' FLOW_RISKS  */
#line 2597 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() == 0));
        _NDFP_debugf("Risks not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5702 "nd-flow-expr.cpp"
    break;

  case 328: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NAME  */
#line 2601 "nd-flow-expr.ypp"
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
#line 5726 "nd-flow-expr.cpp"
    break;

  case 329: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NAME  */
#line 2620 "nd-flow-expr.ypp"
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
#line 5750 "nd-flow-expr.cpp"
    break;

  case 330: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_UNSIGNED  */
#line 2639 "nd-flow-expr.ypp"
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
#line 5768 "nd-flow-expr.cpp"
    break;

  case 331: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2652 "nd-flow-expr.ypp"
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
#line 5786 "nd-flow-expr.cpp"
    break;

  case 332: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE  */
#line 2668 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != 0));
        _NDFP_debugf("nDPI risk score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5795 "nd-flow-expr.cpp"
    break;

  case 333: /* expr_risk_ndpi_score: '!' FLOW_NDPI_RISK_SCORE  */
#line 2672 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == 0));
        _NDFP_debugf("nDPI risk score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5804 "nd-flow-expr.cpp"
    break;

  case 334: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_EQUAL VALUE_UNSIGNED  */
#line 2676 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5813 "nd-flow-expr.cpp"
    break;

  case 335: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2680 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5822 "nd-flow-expr.cpp"
    break;

  case 336: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2684 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5831 "nd-flow-expr.cpp"
    break;

  case 337: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2688 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5840 "nd-flow-expr.cpp"
    break;

  case 338: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '>' VALUE_UNSIGNED  */
#line 2692 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5849 "nd-flow-expr.cpp"
    break;

  case 339: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '<' VALUE_UNSIGNED  */
#line 2696 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5858 "nd-flow-expr.cpp"
    break;

  case 340: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 2703 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != 0));
        _NDFP_debugf("nDPI risk client score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5867 "nd-flow-expr.cpp"
    break;

  case 341: /* expr_risk_ndpi_score_client: '!' FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 2707 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == 0));
        _NDFP_debugf("nDPI risk client score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5876 "nd-flow-expr.cpp"
    break;

  case 342: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_EQUAL VALUE_UNSIGNED  */
#line 2711 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5885 "nd-flow-expr.cpp"
    break;

  case 343: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2715 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5894 "nd-flow-expr.cpp"
    break;

  case 344: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2719 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5903 "nd-flow-expr.cpp"
    break;

  case 345: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2723 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5912 "nd-flow-expr.cpp"
    break;

  case 346: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '>' VALUE_UNSIGNED  */
#line 2727 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5921 "nd-flow-expr.cpp"
    break;

  case 347: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '<' VALUE_UNSIGNED  */
#line 2731 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5930 "nd-flow-expr.cpp"
    break;

  case 348: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER  */
#line 2738 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != 0));
        _NDFP_debugf("nDPI risk server score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5939 "nd-flow-expr.cpp"
    break;

  case 349: /* expr_risk_ndpi_score_server: '!' FLOW_NDPI_RISK_SCORE_SERVER  */
#line 2742 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == 0));
        _NDFP_debugf("nDPI risk server score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5948 "nd-flow-expr.cpp"
    break;

  case 350: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_EQUAL VALUE_UNSIGNED  */
#line 2746 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5957 "nd-flow-expr.cpp"
    break;

  case 351: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2750 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5966 "nd-flow-expr.cpp"
    break;

  case 352: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2754 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5975 "nd-flow-expr.cpp"
    break;

  case 353: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2758 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5984 "nd-flow-expr.cpp"
    break;

  case 354: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '>' VALUE_UNSIGNED  */
#line 2762 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5993 "nd-flow-expr.cpp"
    break;

  case 355: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '<' VALUE_UNSIGNED  */
#line 2766 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6002 "nd-flow-expr.cpp"
    break;

  case 356: /* expr_fwmark: FLOW_CT_MARK  */
#line 2773 "nd-flow-expr.ypp"
                   {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != 0));
        _NDFP_debugf("FWMARK set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6015 "nd-flow-expr.cpp"
    break;

  case 357: /* expr_fwmark: '!' FLOW_CT_MARK  */
#line 2781 "nd-flow-expr.ypp"
                       {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == 0));
        _NDFP_debugf("FWMARK not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6028 "nd-flow-expr.cpp"
    break;

  case 358: /* expr_fwmark: FLOW_CT_MARK CMP_EQUAL VALUE_UNSIGNED  */
#line 2789 "nd-flow-expr.ypp"
                                            {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6041 "nd-flow-expr.cpp"
    break;

  case 359: /* expr_fwmark: FLOW_CT_MARK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2797 "nd-flow-expr.ypp"
                                               {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6054 "nd-flow-expr.cpp"
    break;

  case 360: /* expr_fwmark: FLOW_CT_MARK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2805 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark >= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6067 "nd-flow-expr.cpp"
    break;

  case 361: /* expr_fwmark: FLOW_CT_MARK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2813 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark <= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6080 "nd-flow-expr.cpp"
    break;

  case 362: /* expr_fwmark: FLOW_CT_MARK '>' VALUE_UNSIGNED  */
#line 2821 "nd-flow-expr.ypp"
                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark > (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6093 "nd-flow-expr.cpp"
    break;

  case 363: /* expr_fwmark: FLOW_CT_MARK '<' VALUE_UNSIGNED  */
#line 2829 "nd-flow-expr.ypp"
                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark < (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 6106 "nd-flow-expr.cpp"
    break;

  case 364: /* expr_iface: FLOW_IFACE  */
#line 2840 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == false
        ));
        _NDFP_debugf("Interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6118 "nd-flow-expr.cpp"
    break;

  case 365: /* expr_iface: '!' FLOW_IFACE  */
#line 2847 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == true
        ));
        _NDFP_debugf("Interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6130 "nd-flow-expr.cpp"
    break;

  case 366: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_NAME  */
#line 2854 "nd-flow-expr.ypp"
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
#line 6153 "nd-flow-expr.cpp"
    break;

  case 367: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_NAME  */
#line 2872 "nd-flow-expr.ypp"
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
#line 6176 "nd-flow-expr.cpp"
    break;

  case 368: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_REGEX  */
#line 2890 "nd-flow-expr.ypp"
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
#line 6195 "nd-flow-expr.cpp"
    break;

  case 369: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_REGEX  */
#line 2904 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 6206 "nd-flow-expr.cpp"
    break;

  case 370: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC  */
#line 2913 "nd-flow-expr.ypp"
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
#line 6222 "nd-flow-expr.cpp"
    break;

  case 371: /* expr_iface_nfq_src: '!' FLOW_IFACE_NFQ_SRC  */
#line 2924 "nd-flow-expr.ypp"
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
#line 6238 "nd-flow-expr.cpp"
    break;

  case 372: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_NAME  */
#line 2935 "nd-flow-expr.ypp"
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
#line 6264 "nd-flow-expr.cpp"
    break;

  case 373: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_NAME  */
#line 2956 "nd-flow-expr.ypp"
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
#line 6288 "nd-flow-expr.cpp"
    break;

  case 374: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_REGEX  */
#line 2975 "nd-flow-expr.ypp"
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
#line 6307 "nd-flow-expr.cpp"
    break;

  case 375: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_REGEX  */
#line 2989 "nd-flow-expr.ypp"
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
#line 6326 "nd-flow-expr.cpp"
    break;

  case 376: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST  */
#line 3006 "nd-flow-expr.ypp"
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
#line 6342 "nd-flow-expr.cpp"
    break;

  case 377: /* expr_iface_nfq_dst: '!' FLOW_IFACE_NFQ_DST  */
#line 3017 "nd-flow-expr.ypp"
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
#line 6358 "nd-flow-expr.cpp"
    break;

  case 378: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_NAME  */
#line 3028 "nd-flow-expr.ypp"
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
#line 6382 "nd-flow-expr.cpp"
    break;

  case 379: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_NAME  */
#line 3047 "nd-flow-expr.ypp"
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
#line 6406 "nd-flow-expr.cpp"
    break;

  case 380: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_REGEX  */
#line 3066 "nd-flow-expr.ypp"
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
#line 6426 "nd-flow-expr.cpp"
    break;

  case 381: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_REGEX  */
#line 3081 "nd-flow-expr.ypp"
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
#line 6445 "nd-flow-expr.cpp"
    break;

  case 382: /* expr_intel: FLOW_INTEL  */
#line 3098 "nd-flow-expr.ypp"
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
#line 6471 "nd-flow-expr.cpp"
    break;

  case 383: /* expr_intel: '!' FLOW_INTEL  */
#line 3119 "nd-flow-expr.ypp"
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
#line 6496 "nd-flow-expr.cpp"
    break;

  case 384: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_NAME  */
#line 3141 "nd-flow-expr.ypp"
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
#line 6521 "nd-flow-expr.cpp"
    break;

  case 385: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_REGEX  */
#line 3161 "nd-flow-expr.ypp"
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
#line 6542 "nd-flow-expr.cpp"
    break;

  case 386: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_UNSIGNED  */
#line 3177 "nd-flow-expr.ypp"
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
#line 6559 "nd-flow-expr.cpp"
    break;

  case 387: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_SIGNED  */
#line 3189 "nd-flow-expr.ypp"
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
#line 6576 "nd-flow-expr.cpp"
    break;

  case 388: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FLOAT  */
#line 3201 "nd-flow-expr.ypp"
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
#line 6593 "nd-flow-expr.cpp"
    break;

  case 389: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_TRUE  */
#line 3213 "nd-flow-expr.ypp"
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
#line 6611 "nd-flow-expr.cpp"
    break;

  case 390: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FALSE  */
#line 3226 "nd-flow-expr.ypp"
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
#line 6629 "nd-flow-expr.cpp"
    break;

  case 391: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_NAME  */
#line 3241 "nd-flow-expr.ypp"
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
#line 6654 "nd-flow-expr.cpp"
    break;

  case 392: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_REGEX  */
#line 3261 "nd-flow-expr.ypp"
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
#line 6675 "nd-flow-expr.cpp"
    break;

  case 393: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3277 "nd-flow-expr.ypp"
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
#line 6692 "nd-flow-expr.cpp"
    break;

  case 394: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_SIGNED  */
#line 3289 "nd-flow-expr.ypp"
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
#line 6709 "nd-flow-expr.cpp"
    break;

  case 395: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FLOAT  */
#line 3301 "nd-flow-expr.ypp"
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
#line 6726 "nd-flow-expr.cpp"
    break;

  case 396: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_TRUE  */
#line 3313 "nd-flow-expr.ypp"
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
#line 6744 "nd-flow-expr.cpp"
    break;

  case 397: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FALSE  */
#line 3326 "nd-flow-expr.ypp"
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
#line 6762 "nd-flow-expr.cpp"
    break;

  case 398: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3341 "nd-flow-expr.ypp"
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
#line 6779 "nd-flow-expr.cpp"
    break;

  case 399: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_SIGNED  */
#line 3353 "nd-flow-expr.ypp"
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
#line 6796 "nd-flow-expr.cpp"
    break;

  case 400: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_FLOAT  */
#line 3365 "nd-flow-expr.ypp"
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
#line 6813 "nd-flow-expr.cpp"
    break;

  case 401: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3379 "nd-flow-expr.ypp"
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
#line 6830 "nd-flow-expr.cpp"
    break;

  case 402: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_SIGNED  */
#line 3391 "nd-flow-expr.ypp"
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
#line 6847 "nd-flow-expr.cpp"
    break;

  case 403: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_FLOAT  */
#line 3403 "nd-flow-expr.ypp"
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
#line 6864 "nd-flow-expr.cpp"
    break;

  case 404: /* expr_intel: FLOW_INTEL '>' VALUE_UNSIGNED  */
#line 3417 "nd-flow-expr.ypp"
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
#line 6881 "nd-flow-expr.cpp"
    break;

  case 405: /* expr_intel: FLOW_INTEL '>' VALUE_SIGNED  */
#line 3429 "nd-flow-expr.ypp"
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
#line 6898 "nd-flow-expr.cpp"
    break;

  case 406: /* expr_intel: FLOW_INTEL '>' VALUE_FLOAT  */
#line 3441 "nd-flow-expr.ypp"
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
#line 6915 "nd-flow-expr.cpp"
    break;

  case 407: /* expr_intel: FLOW_INTEL '<' VALUE_UNSIGNED  */
#line 3455 "nd-flow-expr.ypp"
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
#line 6932 "nd-flow-expr.cpp"
    break;

  case 408: /* expr_intel: FLOW_INTEL '<' VALUE_SIGNED  */
#line 3467 "nd-flow-expr.ypp"
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
#line 6949 "nd-flow-expr.cpp"
    break;

  case 409: /* expr_intel: FLOW_INTEL '<' VALUE_FLOAT  */
#line 3479 "nd-flow-expr.ypp"
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
#line 6966 "nd-flow-expr.cpp"
    break;

  case 410: /* expr_tls_version: FLOW_TLS_VERSION  */
#line 3494 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != 0));
        _NDFP_debugf("TLS version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6975 "nd-flow-expr.cpp"
    break;

  case 411: /* expr_tls_version: '!' FLOW_TLS_VERSION  */
#line 3498 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == 0));
        _NDFP_debugf("TLS version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6984 "nd-flow-expr.cpp"
    break;

  case 412: /* expr_tls_version: FLOW_TLS_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 3502 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6993 "nd-flow-expr.cpp"
    break;

  case 413: /* expr_tls_version: FLOW_TLS_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3506 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7002 "nd-flow-expr.cpp"
    break;

  case 414: /* expr_tls_version: FLOW_TLS_VERSION CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3510 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7011 "nd-flow-expr.cpp"
    break;

  case 415: /* expr_tls_version: FLOW_TLS_VERSION CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3514 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7020 "nd-flow-expr.cpp"
    break;

  case 416: /* expr_tls_version: FLOW_TLS_VERSION '>' VALUE_UNSIGNED  */
#line 3518 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7029 "nd-flow-expr.cpp"
    break;

  case 417: /* expr_tls_version: FLOW_TLS_VERSION '<' VALUE_UNSIGNED  */
#line 3522 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7038 "nd-flow-expr.cpp"
    break;

  case 418: /* expr_tls_cipher: FLOW_TLS_CIPHER  */
#line 3529 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != 0));
        _NDFP_debugf("TLS cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7047 "nd-flow-expr.cpp"
    break;

  case 419: /* expr_tls_cipher: '!' FLOW_TLS_CIPHER  */
#line 3533 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == 0));
        _NDFP_debugf("TLS cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7056 "nd-flow-expr.cpp"
    break;

  case 420: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_EQUAL VALUE_UNSIGNED  */
#line 3537 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7065 "nd-flow-expr.cpp"
    break;

  case 421: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3541 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7074 "nd-flow-expr.cpp"
    break;

  case 422: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3545 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7083 "nd-flow-expr.cpp"
    break;

  case 423: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3549 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7092 "nd-flow-expr.cpp"
    break;

  case 424: /* expr_tls_cipher: FLOW_TLS_CIPHER '>' VALUE_UNSIGNED  */
#line 3553 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7101 "nd-flow-expr.cpp"
    break;

  case 425: /* expr_tls_cipher: FLOW_TLS_CIPHER '<' VALUE_UNSIGNED  */
#line 3557 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7110 "nd-flow-expr.cpp"
    break;

  case 426: /* expr_tls_ech: FLOW_TLS_ECH  */
#line 3564 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != 0));
        _NDFP_debugf("TLS ECH version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7119 "nd-flow-expr.cpp"
    break;

  case 427: /* expr_tls_ech: '!' FLOW_TLS_ECH  */
#line 3568 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == 0));
        _NDFP_debugf("TLS ECH version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7128 "nd-flow-expr.cpp"
    break;

  case 428: /* expr_tls_ech: FLOW_TLS_ECH CMP_EQUAL VALUE_UNSIGNED  */
#line 3572 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7137 "nd-flow-expr.cpp"
    break;

  case 429: /* expr_tls_ech: FLOW_TLS_ECH CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3576 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7146 "nd-flow-expr.cpp"
    break;

  case 430: /* expr_tls_ech: FLOW_TLS_ECH CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3580 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7155 "nd-flow-expr.cpp"
    break;

  case 431: /* expr_tls_ech: FLOW_TLS_ECH CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3584 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7164 "nd-flow-expr.cpp"
    break;

  case 432: /* expr_tls_ech: FLOW_TLS_ECH '>' VALUE_UNSIGNED  */
#line 3588 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7173 "nd-flow-expr.cpp"
    break;

  case 433: /* expr_tls_ech: FLOW_TLS_ECH '<' VALUE_UNSIGNED  */
#line 3592 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7182 "nd-flow-expr.cpp"
    break;

  case 434: /* expr_tls_ja4: FLOW_TLS_JA4  */
#line 3599 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7194 "nd-flow-expr.cpp"
    break;

  case 435: /* expr_tls_ja4: '!' FLOW_TLS_JA4  */
#line 3606 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7206 "nd-flow-expr.cpp"
    break;

  case 436: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_NAME  */
#line 3613 "nd-flow-expr.ypp"
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
#line 7230 "nd-flow-expr.cpp"
    break;

  case 437: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_NAME  */
#line 3632 "nd-flow-expr.ypp"
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
#line 7254 "nd-flow-expr.cpp"
    break;

  case 438: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_REGEX  */
#line 3651 "nd-flow-expr.ypp"
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
#line 7273 "nd-flow-expr.cpp"
    break;

  case 439: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_REGEX  */
#line 3665 "nd-flow-expr.ypp"
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
#line 7292 "nd-flow-expr.cpp"
    break;

  case 440: /* expr_origin: FLOW_ORIGIN  */
#line 3682 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7301 "nd-flow-expr.cpp"
    break;

  case 441: /* expr_origin: '!' FLOW_ORIGIN  */
#line 3686 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7310 "nd-flow-expr.cpp"
    break;

  case 442: /* expr_origin: FLOW_ORIGIN CMP_EQUAL value_origin_type  */
#line 3690 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7319 "nd-flow-expr.cpp"
    break;

  case 443: /* expr_origin: FLOW_ORIGIN CMP_NOTEQUAL value_origin_type  */
#line 3694 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7328 "nd-flow-expr.cpp"
    break;

  case 444: /* value_origin_type: FLOW_ORIGIN_LOCAL  */
#line 3701 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 7334 "nd-flow-expr.cpp"
    break;

  case 445: /* value_origin_type: FLOW_ORIGIN_OTHER  */
#line 3702 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 7340 "nd-flow-expr.cpp"
    break;

  case 446: /* value_origin_type: FLOW_ORIGIN_UNKNOWN  */
#line 3703 "nd-flow-expr.ypp"
                          { (yyval.us_number) = (yyvsp[0].us_number); }
#line 7346 "nd-flow-expr.cpp"
    break;

  case 447: /* expr_tag: FLOW_TAG  */
#line 3707 "nd-flow-expr.ypp"
               {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags not empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7358 "nd-flow-expr.cpp"
    break;

  case 448: /* expr_tag: '!' FLOW_TAG  */
#line 3714 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7370 "nd-flow-expr.cpp"
    break;

  case 449: /* expr_tag: FLOW_TAG CMP_EQUAL VALUE_NAME  */
#line 3721 "nd-flow-expr.ypp"
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

            if ((p = search.find_first_of(".")) != string::npos) {
                if (strncasecmp(search.substr(0, p).c_str(),
                    tag.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                    _NDFP_result = ((yyval.bool_result) = true);
                    break;
                }
            }
        }

        _NDFP_debugf("Overlay tag == %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7403 "nd-flow-expr.cpp"
    break;

  case 450: /* expr_tag: FLOW_TAG CMP_NOTEQUAL VALUE_NAME  */
#line 3749 "nd-flow-expr.ypp"
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

            if ((p = search.find_first_of(".")) != string::npos) {
                if (strncasecmp(search.substr(0, p).c_str(),
                    tag.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                    _NDFP_result = ((yyval.bool_result) = false);
                    break;
                }
            }
        }

        _NDFP_debugf("Overlay tag != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 7436 "nd-flow-expr.cpp"
    break;


#line 7440 "nd-flow-expr.cpp"

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

#line 3779 "nd-flow-expr.ypp"


ndFlowParser::ndFlowParser() {
    yyscan_t scanner;
    yylex_init_extra((void *)this, &scanner);

    if (scanner == nullptr)
        throw string("Error creating scanner context");

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

    auto ji = params.find("intel");
    params_intel = (ji != params.end()) ? &ji->second : nullptr;

    YY_BUFFER_STATE flow_expr_scan_buffer;
    flow_expr_scan_buffer = yy_scan_bytes(
        expr.c_str(), expr.size(), (yyscan_t)scanner
    );

    if (flow_expr_scan_buffer == nullptr)
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
