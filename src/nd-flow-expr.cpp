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
    FLOW_SRC_MAC = 267,            /* FLOW_SRC_MAC  */
    FLOW_DST_MAC = 268,            /* FLOW_DST_MAC  */
    FLOW_SRC_IP = 269,             /* FLOW_SRC_IP  */
    FLOW_DST_IP = 270,             /* FLOW_DST_IP  */
    FLOW_LOCAL_PORT = 271,         /* FLOW_LOCAL_PORT  */
    FLOW_OTHER_PORT = 272,         /* FLOW_OTHER_PORT  */
    FLOW_SRC_PORT = 273,           /* FLOW_SRC_PORT  */
    FLOW_DST_PORT = 274,           /* FLOW_DST_PORT  */
    FLOW_TUNNEL_TYPE = 275,        /* FLOW_TUNNEL_TYPE  */
    FLOW_DETECTION_COMPLETE = 276, /* FLOW_DETECTION_COMPLETE  */
    FLOW_DETECTION_GUESSED = 277,  /* FLOW_DETECTION_GUESSED  */
    FLOW_DETECTION_INIT = 278,     /* FLOW_DETECTION_INIT  */
    FLOW_DETECTION_UPDATED = 279,  /* FLOW_DETECTION_UPDATED  */
    FLOW_DHC_HIT = 280,            /* FLOW_DHC_HIT  */
    FLOW_FHC_HIT = 281,            /* FLOW_FHC_HIT  */
    FLOW_IP_NAT = 282,             /* FLOW_IP_NAT  */
    FLOW_EXPIRING = 283,           /* FLOW_EXPIRING  */
    FLOW_EXPIRED = 284,            /* FLOW_EXPIRED  */
    FLOW_SOFT_DISSECTOR = 285,     /* FLOW_SOFT_DISSECTOR  */
    FLOW_RISKS = 286,              /* FLOW_RISKS  */
    FLOW_NDPI_RISK_SCORE = 287,    /* FLOW_NDPI_RISK_SCORE  */
    FLOW_NDPI_RISK_SCORE_CLIENT = 288, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
    FLOW_NDPI_RISK_SCORE_SERVER = 289, /* FLOW_NDPI_RISK_SCORE_SERVER  */
    FLOW_CATEGORY = 290,           /* FLOW_CATEGORY  */
    FLOW_CATEGORY_ID = 291,        /* FLOW_CATEGORY_ID  */
    FLOW_APPLICATION = 292,        /* FLOW_APPLICATION  */
    FLOW_APPLICATION_CATEGORY = 293, /* FLOW_APPLICATION_CATEGORY  */
    FLOW_APPLICATION_CATEGORY_ID = 294, /* FLOW_APPLICATION_CATEGORY_ID  */
    FLOW_DOMAIN_CATEGORY = 295,    /* FLOW_DOMAIN_CATEGORY  */
    FLOW_DOMAIN_CATEGORY_ID = 296, /* FLOW_DOMAIN_CATEGORY_ID  */
    FLOW_NETWORK_CATEGORY = 297,   /* FLOW_NETWORK_CATEGORY  */
    FLOW_NETWORK_CATEGORY_ID = 298, /* FLOW_NETWORK_CATEGORY_ID  */
    FLOW_LOCAL_NETWORK_CATEGORY = 299, /* FLOW_LOCAL_NETWORK_CATEGORY  */
    FLOW_LOCAL_NETWORK_CATEGORY_ID = 300, /* FLOW_LOCAL_NETWORK_CATEGORY_ID  */
    FLOW_OTHER_NETWORK_CATEGORY = 301, /* FLOW_OTHER_NETWORK_CATEGORY  */
    FLOW_OTHER_NETWORK_CATEGORY_ID = 302, /* FLOW_OTHER_NETWORK_CATEGORY_ID  */
    FLOW_SRC_NETWORK_CATEGORY = 303, /* FLOW_SRC_NETWORK_CATEGORY  */
    FLOW_SRC_NETWORK_CATEGORY_ID = 304, /* FLOW_SRC_NETWORK_CATEGORY_ID  */
    FLOW_DST_NETWORK_CATEGORY = 305, /* FLOW_DST_NETWORK_CATEGORY  */
    FLOW_DST_NETWORK_CATEGORY_ID = 306, /* FLOW_DST_NETWORK_CATEGORY_ID  */
    FLOW_TAG = 307,                /* FLOW_TAG  */
    FLOW_TAG_CATEGORY = 308,       /* FLOW_TAG_CATEGORY  */
    FLOW_TAG_CATEGORY_ID = 309,    /* FLOW_TAG_CATEGORY_ID  */
    FLOW_PROTOCOL = 310,           /* FLOW_PROTOCOL  */
    FLOW_PROTOCOL_CATEGORY = 311,  /* FLOW_PROTOCOL_CATEGORY  */
    FLOW_PROTOCOL_CATEGORY_ID = 312, /* FLOW_PROTOCOL_CATEGORY_ID  */
    FLOW_DETECTED_HOSTNAME = 313,  /* FLOW_DETECTED_HOSTNAME  */
    FLOW_DNS_HOSTNAME = 314,       /* FLOW_DNS_HOSTNAME  */
    FLOW_ORIGIN = 315,             /* FLOW_ORIGIN  */
    FLOW_CT_MARK = 316,            /* FLOW_CT_MARK  */
    FLOW_TLS_VERSION = 317,        /* FLOW_TLS_VERSION  */
    FLOW_TLS_CIPHER = 318,         /* FLOW_TLS_CIPHER  */
    FLOW_TLS_ECH = 319,            /* FLOW_TLS_ECH  */
    FLOW_TLS_JA4 = 320,            /* FLOW_TLS_JA4  */
    FLOW_IFACE = 321,              /* FLOW_IFACE  */
    FLOW_IFACE_NFQ_SRC = 322,      /* FLOW_IFACE_NFQ_SRC  */
    FLOW_IFACE_NFQ_DST = 323,      /* FLOW_IFACE_NFQ_DST  */
    FLOW_OTHER_UNKNOWN = 324,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 325,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 326,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 327,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 328,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 329,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 330,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 331,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 332,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 333,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 334,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 335,         /* FLOW_TUNNEL_GTP  */
    FLOW_INTEL = 336,              /* FLOW_INTEL  */
    CMP_EQUAL = 337,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 338,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 339,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 340,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 341,                /* BOOL_AND  */
    BOOL_OR = 342,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 343,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 344,              /* VALUE_TRUE  */
    VALUE_FALSE = 345,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 346,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 347,              /* VALUE_NAME  */
    VALUE_REGEX = 348,             /* VALUE_REGEX  */
    VALUE_ADDR_TAG = 349,          /* VALUE_ADDR_TAG  */
    VALUE_ADDR_IPV4 = 350,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 351,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 352,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 353,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 354,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 355,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 356              /* VALUE_FLOAT  */
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
#define FLOW_SRC_MAC 267
#define FLOW_DST_MAC 268
#define FLOW_SRC_IP 269
#define FLOW_DST_IP 270
#define FLOW_LOCAL_PORT 271
#define FLOW_OTHER_PORT 272
#define FLOW_SRC_PORT 273
#define FLOW_DST_PORT 274
#define FLOW_TUNNEL_TYPE 275
#define FLOW_DETECTION_COMPLETE 276
#define FLOW_DETECTION_GUESSED 277
#define FLOW_DETECTION_INIT 278
#define FLOW_DETECTION_UPDATED 279
#define FLOW_DHC_HIT 280
#define FLOW_FHC_HIT 281
#define FLOW_IP_NAT 282
#define FLOW_EXPIRING 283
#define FLOW_EXPIRED 284
#define FLOW_SOFT_DISSECTOR 285
#define FLOW_RISKS 286
#define FLOW_NDPI_RISK_SCORE 287
#define FLOW_NDPI_RISK_SCORE_CLIENT 288
#define FLOW_NDPI_RISK_SCORE_SERVER 289
#define FLOW_CATEGORY 290
#define FLOW_CATEGORY_ID 291
#define FLOW_APPLICATION 292
#define FLOW_APPLICATION_CATEGORY 293
#define FLOW_APPLICATION_CATEGORY_ID 294
#define FLOW_DOMAIN_CATEGORY 295
#define FLOW_DOMAIN_CATEGORY_ID 296
#define FLOW_NETWORK_CATEGORY 297
#define FLOW_NETWORK_CATEGORY_ID 298
#define FLOW_LOCAL_NETWORK_CATEGORY 299
#define FLOW_LOCAL_NETWORK_CATEGORY_ID 300
#define FLOW_OTHER_NETWORK_CATEGORY 301
#define FLOW_OTHER_NETWORK_CATEGORY_ID 302
#define FLOW_SRC_NETWORK_CATEGORY 303
#define FLOW_SRC_NETWORK_CATEGORY_ID 304
#define FLOW_DST_NETWORK_CATEGORY 305
#define FLOW_DST_NETWORK_CATEGORY_ID 306
#define FLOW_TAG 307
#define FLOW_TAG_CATEGORY 308
#define FLOW_TAG_CATEGORY_ID 309
#define FLOW_PROTOCOL 310
#define FLOW_PROTOCOL_CATEGORY 311
#define FLOW_PROTOCOL_CATEGORY_ID 312
#define FLOW_DETECTED_HOSTNAME 313
#define FLOW_DNS_HOSTNAME 314
#define FLOW_ORIGIN 315
#define FLOW_CT_MARK 316
#define FLOW_TLS_VERSION 317
#define FLOW_TLS_CIPHER 318
#define FLOW_TLS_ECH 319
#define FLOW_TLS_JA4 320
#define FLOW_IFACE 321
#define FLOW_IFACE_NFQ_SRC 322
#define FLOW_IFACE_NFQ_DST 323
#define FLOW_OTHER_UNKNOWN 324
#define FLOW_OTHER_UNSUPPORTED 325
#define FLOW_OTHER_LOCAL 326
#define FLOW_OTHER_MULTICAST 327
#define FLOW_OTHER_BROADCAST 328
#define FLOW_OTHER_REMOTE 329
#define FLOW_OTHER_ERROR 330
#define FLOW_ORIGIN_LOCAL 331
#define FLOW_ORIGIN_OTHER 332
#define FLOW_ORIGIN_UNKNOWN 333
#define FLOW_TUNNEL_NONE 334
#define FLOW_TUNNEL_GTP 335
#define FLOW_INTEL 336
#define CMP_EQUAL 337
#define CMP_NOTEQUAL 338
#define CMP_GTHANEQUAL 339
#define CMP_LTHANEQUAL 340
#define BOOL_AND 341
#define BOOL_OR 342
#define VALUE_ADDR_IPMASK 343
#define VALUE_TRUE 344
#define VALUE_FALSE 345
#define VALUE_ADDR_MAC 346
#define VALUE_NAME 347
#define VALUE_REGEX 348
#define VALUE_ADDR_TAG 349
#define VALUE_ADDR_IPV4 350
#define VALUE_ADDR_IPV4_CIDR 351
#define VALUE_ADDR_IPV6 352
#define VALUE_ADDR_IPV6_CIDR 353
#define VALUE_SIGNED 354
#define VALUE_UNSIGNED 355
#define VALUE_FLOAT 356

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

#line 467 "nd-flow-expr.cpp"

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
  YYSYMBOL_FLOW_SRC_MAC = 12,              /* FLOW_SRC_MAC  */
  YYSYMBOL_FLOW_DST_MAC = 13,              /* FLOW_DST_MAC  */
  YYSYMBOL_FLOW_SRC_IP = 14,               /* FLOW_SRC_IP  */
  YYSYMBOL_FLOW_DST_IP = 15,               /* FLOW_DST_IP  */
  YYSYMBOL_FLOW_LOCAL_PORT = 16,           /* FLOW_LOCAL_PORT  */
  YYSYMBOL_FLOW_OTHER_PORT = 17,           /* FLOW_OTHER_PORT  */
  YYSYMBOL_FLOW_SRC_PORT = 18,             /* FLOW_SRC_PORT  */
  YYSYMBOL_FLOW_DST_PORT = 19,             /* FLOW_DST_PORT  */
  YYSYMBOL_FLOW_TUNNEL_TYPE = 20,          /* FLOW_TUNNEL_TYPE  */
  YYSYMBOL_FLOW_DETECTION_COMPLETE = 21,   /* FLOW_DETECTION_COMPLETE  */
  YYSYMBOL_FLOW_DETECTION_GUESSED = 22,    /* FLOW_DETECTION_GUESSED  */
  YYSYMBOL_FLOW_DETECTION_INIT = 23,       /* FLOW_DETECTION_INIT  */
  YYSYMBOL_FLOW_DETECTION_UPDATED = 24,    /* FLOW_DETECTION_UPDATED  */
  YYSYMBOL_FLOW_DHC_HIT = 25,              /* FLOW_DHC_HIT  */
  YYSYMBOL_FLOW_FHC_HIT = 26,              /* FLOW_FHC_HIT  */
  YYSYMBOL_FLOW_IP_NAT = 27,               /* FLOW_IP_NAT  */
  YYSYMBOL_FLOW_EXPIRING = 28,             /* FLOW_EXPIRING  */
  YYSYMBOL_FLOW_EXPIRED = 29,              /* FLOW_EXPIRED  */
  YYSYMBOL_FLOW_SOFT_DISSECTOR = 30,       /* FLOW_SOFT_DISSECTOR  */
  YYSYMBOL_FLOW_RISKS = 31,                /* FLOW_RISKS  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE = 32,      /* FLOW_NDPI_RISK_SCORE  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_CLIENT = 33, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_SERVER = 34, /* FLOW_NDPI_RISK_SCORE_SERVER  */
  YYSYMBOL_FLOW_CATEGORY = 35,             /* FLOW_CATEGORY  */
  YYSYMBOL_FLOW_CATEGORY_ID = 36,          /* FLOW_CATEGORY_ID  */
  YYSYMBOL_FLOW_APPLICATION = 37,          /* FLOW_APPLICATION  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY = 38, /* FLOW_APPLICATION_CATEGORY  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY_ID = 39, /* FLOW_APPLICATION_CATEGORY_ID  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY = 40,      /* FLOW_DOMAIN_CATEGORY  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY_ID = 41,   /* FLOW_DOMAIN_CATEGORY_ID  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY = 42,     /* FLOW_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY_ID = 43,  /* FLOW_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_LOCAL_NETWORK_CATEGORY = 44, /* FLOW_LOCAL_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_LOCAL_NETWORK_CATEGORY_ID = 45, /* FLOW_LOCAL_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_OTHER_NETWORK_CATEGORY = 46, /* FLOW_OTHER_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_OTHER_NETWORK_CATEGORY_ID = 47, /* FLOW_OTHER_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_SRC_NETWORK_CATEGORY = 48, /* FLOW_SRC_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_SRC_NETWORK_CATEGORY_ID = 49, /* FLOW_SRC_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_DST_NETWORK_CATEGORY = 50, /* FLOW_DST_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_DST_NETWORK_CATEGORY_ID = 51, /* FLOW_DST_NETWORK_CATEGORY_ID  */
  YYSYMBOL_FLOW_TAG = 52,                  /* FLOW_TAG  */
  YYSYMBOL_FLOW_TAG_CATEGORY = 53,         /* FLOW_TAG_CATEGORY  */
  YYSYMBOL_FLOW_TAG_CATEGORY_ID = 54,      /* FLOW_TAG_CATEGORY_ID  */
  YYSYMBOL_FLOW_PROTOCOL = 55,             /* FLOW_PROTOCOL  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY = 56,    /* FLOW_PROTOCOL_CATEGORY  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY_ID = 57, /* FLOW_PROTOCOL_CATEGORY_ID  */
  YYSYMBOL_FLOW_DETECTED_HOSTNAME = 58,    /* FLOW_DETECTED_HOSTNAME  */
  YYSYMBOL_FLOW_DNS_HOSTNAME = 59,         /* FLOW_DNS_HOSTNAME  */
  YYSYMBOL_FLOW_ORIGIN = 60,               /* FLOW_ORIGIN  */
  YYSYMBOL_FLOW_CT_MARK = 61,              /* FLOW_CT_MARK  */
  YYSYMBOL_FLOW_TLS_VERSION = 62,          /* FLOW_TLS_VERSION  */
  YYSYMBOL_FLOW_TLS_CIPHER = 63,           /* FLOW_TLS_CIPHER  */
  YYSYMBOL_FLOW_TLS_ECH = 64,              /* FLOW_TLS_ECH  */
  YYSYMBOL_FLOW_TLS_JA4 = 65,              /* FLOW_TLS_JA4  */
  YYSYMBOL_FLOW_IFACE = 66,                /* FLOW_IFACE  */
  YYSYMBOL_FLOW_IFACE_NFQ_SRC = 67,        /* FLOW_IFACE_NFQ_SRC  */
  YYSYMBOL_FLOW_IFACE_NFQ_DST = 68,        /* FLOW_IFACE_NFQ_DST  */
  YYSYMBOL_FLOW_OTHER_UNKNOWN = 69,        /* FLOW_OTHER_UNKNOWN  */
  YYSYMBOL_FLOW_OTHER_UNSUPPORTED = 70,    /* FLOW_OTHER_UNSUPPORTED  */
  YYSYMBOL_FLOW_OTHER_LOCAL = 71,          /* FLOW_OTHER_LOCAL  */
  YYSYMBOL_FLOW_OTHER_MULTICAST = 72,      /* FLOW_OTHER_MULTICAST  */
  YYSYMBOL_FLOW_OTHER_BROADCAST = 73,      /* FLOW_OTHER_BROADCAST  */
  YYSYMBOL_FLOW_OTHER_REMOTE = 74,         /* FLOW_OTHER_REMOTE  */
  YYSYMBOL_FLOW_OTHER_ERROR = 75,          /* FLOW_OTHER_ERROR  */
  YYSYMBOL_FLOW_ORIGIN_LOCAL = 76,         /* FLOW_ORIGIN_LOCAL  */
  YYSYMBOL_FLOW_ORIGIN_OTHER = 77,         /* FLOW_ORIGIN_OTHER  */
  YYSYMBOL_FLOW_ORIGIN_UNKNOWN = 78,       /* FLOW_ORIGIN_UNKNOWN  */
  YYSYMBOL_FLOW_TUNNEL_NONE = 79,          /* FLOW_TUNNEL_NONE  */
  YYSYMBOL_FLOW_TUNNEL_GTP = 80,           /* FLOW_TUNNEL_GTP  */
  YYSYMBOL_FLOW_INTEL = 81,                /* FLOW_INTEL  */
  YYSYMBOL_CMP_EQUAL = 82,                 /* CMP_EQUAL  */
  YYSYMBOL_CMP_NOTEQUAL = 83,              /* CMP_NOTEQUAL  */
  YYSYMBOL_CMP_GTHANEQUAL = 84,            /* CMP_GTHANEQUAL  */
  YYSYMBOL_CMP_LTHANEQUAL = 85,            /* CMP_LTHANEQUAL  */
  YYSYMBOL_BOOL_AND = 86,                  /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 87,                   /* BOOL_OR  */
  YYSYMBOL_VALUE_ADDR_IPMASK = 88,         /* VALUE_ADDR_IPMASK  */
  YYSYMBOL_VALUE_TRUE = 89,                /* VALUE_TRUE  */
  YYSYMBOL_VALUE_FALSE = 90,               /* VALUE_FALSE  */
  YYSYMBOL_VALUE_ADDR_MAC = 91,            /* VALUE_ADDR_MAC  */
  YYSYMBOL_VALUE_NAME = 92,                /* VALUE_NAME  */
  YYSYMBOL_VALUE_REGEX = 93,               /* VALUE_REGEX  */
  YYSYMBOL_VALUE_ADDR_TAG = 94,            /* VALUE_ADDR_TAG  */
  YYSYMBOL_VALUE_ADDR_IPV4 = 95,           /* VALUE_ADDR_IPV4  */
  YYSYMBOL_VALUE_ADDR_IPV4_CIDR = 96,      /* VALUE_ADDR_IPV4_CIDR  */
  YYSYMBOL_VALUE_ADDR_IPV6 = 97,           /* VALUE_ADDR_IPV6  */
  YYSYMBOL_VALUE_ADDR_IPV6_CIDR = 98,      /* VALUE_ADDR_IPV6_CIDR  */
  YYSYMBOL_VALUE_SIGNED = 99,              /* VALUE_SIGNED  */
  YYSYMBOL_VALUE_UNSIGNED = 100,           /* VALUE_UNSIGNED  */
  YYSYMBOL_VALUE_FLOAT = 101,              /* VALUE_FLOAT  */
  YYSYMBOL_102_ = 102,                     /* ';'  */
  YYSYMBOL_103_ = 103,                     /* '('  */
  YYSYMBOL_104_ = 104,                     /* ')'  */
  YYSYMBOL_105_ = 105,                     /* '!'  */
  YYSYMBOL_106_ = 106,                     /* '>'  */
  YYSYMBOL_107_ = 107,                     /* '<'  */
  YYSYMBOL_YYACCEPT = 108,                 /* $accept  */
  YYSYMBOL_exprs = 109,                    /* exprs  */
  YYSYMBOL_expr = 110,                     /* expr  */
  YYSYMBOL_expr_ip_proto = 111,            /* expr_ip_proto  */
  YYSYMBOL_expr_ip_dscp = 112,             /* expr_ip_dscp  */
  YYSYMBOL_expr_ip_version = 113,          /* expr_ip_version  */
  YYSYMBOL_expr_vlan_id = 114,             /* expr_vlan_id  */
  YYSYMBOL_expr_other_type = 115,          /* expr_other_type  */
  YYSYMBOL_value_other_type = 116,         /* value_other_type  */
  YYSYMBOL_expr_local_mac = 117,           /* expr_local_mac  */
  YYSYMBOL_expr_other_mac = 118,           /* expr_other_mac  */
  YYSYMBOL_expr_src_mac = 119,             /* expr_src_mac  */
  YYSYMBOL_expr_dst_mac = 120,             /* expr_dst_mac  */
  YYSYMBOL_expr_local_ip = 121,            /* expr_local_ip  */
  YYSYMBOL_expr_other_ip = 122,            /* expr_other_ip  */
  YYSYMBOL_expr_src_ip = 123,              /* expr_src_ip  */
  YYSYMBOL_expr_dst_ip = 124,              /* expr_dst_ip  */
  YYSYMBOL_value_addr_ip = 125,            /* value_addr_ip  */
  YYSYMBOL_expr_local_port = 126,          /* expr_local_port  */
  YYSYMBOL_expr_other_port = 127,          /* expr_other_port  */
  YYSYMBOL_expr_src_port = 128,            /* expr_src_port  */
  YYSYMBOL_expr_dst_port = 129,            /* expr_dst_port  */
  YYSYMBOL_expr_tunnel_type = 130,         /* expr_tunnel_type  */
  YYSYMBOL_value_tunnel_type = 131,        /* value_tunnel_type  */
  YYSYMBOL_expr_detection_complete = 132,  /* expr_detection_complete  */
  YYSYMBOL_expr_detection_guessed = 133,   /* expr_detection_guessed  */
  YYSYMBOL_expr_detection_init = 134,      /* expr_detection_init  */
  YYSYMBOL_expr_detection_updated = 135,   /* expr_detection_updated  */
  YYSYMBOL_expr_dhc_hit = 136,             /* expr_dhc_hit  */
  YYSYMBOL_expr_fhc_hit = 137,             /* expr_fhc_hit  */
  YYSYMBOL_expr_ip_nat = 138,              /* expr_ip_nat  */
  YYSYMBOL_expr_expiring = 139,            /* expr_expiring  */
  YYSYMBOL_expr_expired = 140,             /* expr_expired  */
  YYSYMBOL_expr_soft_dissector = 141,      /* expr_soft_dissector  */
  YYSYMBOL_expr_app = 142,                 /* expr_app  */
  YYSYMBOL_expr_app_id = 143,              /* expr_app_id  */
  YYSYMBOL_expr_app_name = 144,            /* expr_app_name  */
  YYSYMBOL_expr_category = 145,            /* expr_category  */
  YYSYMBOL_expr_category_id = 146,         /* expr_category_id  */
  YYSYMBOL_expr_app_category = 147,        /* expr_app_category  */
  YYSYMBOL_expr_app_category_id = 148,     /* expr_app_category_id  */
  YYSYMBOL_expr_domain_category = 149,     /* expr_domain_category  */
  YYSYMBOL_expr_domain_category_id = 150,  /* expr_domain_category_id  */
  YYSYMBOL_expr_network_category = 151,    /* expr_network_category  */
  YYSYMBOL_expr_network_category_id = 152, /* expr_network_category_id  */
  YYSYMBOL_expr_local_network_category = 153, /* expr_local_network_category  */
  YYSYMBOL_expr_local_network_category_id = 154, /* expr_local_network_category_id  */
  YYSYMBOL_expr_other_network_category = 155, /* expr_other_network_category  */
  YYSYMBOL_expr_other_network_category_id = 156, /* expr_other_network_category_id  */
  YYSYMBOL_expr_src_network_category = 157, /* expr_src_network_category  */
  YYSYMBOL_expr_src_network_category_id = 158, /* expr_src_network_category_id  */
  YYSYMBOL_expr_dst_network_category = 159, /* expr_dst_network_category  */
  YYSYMBOL_expr_dst_network_category_id = 160, /* expr_dst_network_category_id  */
  YYSYMBOL_expr_tag_category = 161,        /* expr_tag_category  */
  YYSYMBOL_expr_tag_category_id = 162,     /* expr_tag_category_id  */
  YYSYMBOL_expr_proto = 163,               /* expr_proto  */
  YYSYMBOL_expr_proto_id = 164,            /* expr_proto_id  */
  YYSYMBOL_expr_proto_name = 165,          /* expr_proto_name  */
  YYSYMBOL_expr_proto_category = 166,      /* expr_proto_category  */
  YYSYMBOL_expr_proto_category_id = 167,   /* expr_proto_category_id  */
  YYSYMBOL_expr_detected_hostname = 168,   /* expr_detected_hostname  */
  YYSYMBOL_expr_dns_hostname = 169,        /* expr_dns_hostname  */
  YYSYMBOL_expr_risks = 170,               /* expr_risks  */
  YYSYMBOL_expr_risk_ndpi_score = 171,     /* expr_risk_ndpi_score  */
  YYSYMBOL_expr_risk_ndpi_score_client = 172, /* expr_risk_ndpi_score_client  */
  YYSYMBOL_expr_risk_ndpi_score_server = 173, /* expr_risk_ndpi_score_server  */
  YYSYMBOL_expr_fwmark = 174,              /* expr_fwmark  */
  YYSYMBOL_expr_iface = 175,               /* expr_iface  */
  YYSYMBOL_expr_iface_nfq_src = 176,       /* expr_iface_nfq_src  */
  YYSYMBOL_expr_iface_nfq_dst = 177,       /* expr_iface_nfq_dst  */
  YYSYMBOL_expr_intel = 178,               /* expr_intel  */
  YYSYMBOL_expr_tls_version = 179,         /* expr_tls_version  */
  YYSYMBOL_expr_tls_cipher = 180,          /* expr_tls_cipher  */
  YYSYMBOL_expr_tls_ech = 181,             /* expr_tls_ech  */
  YYSYMBOL_expr_tls_ja4 = 182,             /* expr_tls_ja4  */
  YYSYMBOL_expr_origin = 183,              /* expr_origin  */
  YYSYMBOL_value_origin_type = 184,        /* value_origin_type  */
  YYSYMBOL_expr_tag = 185                  /* expr_tag  */
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
#define YYLAST   686

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  78
/* YYNRULES -- Number of rules.  */
#define YYNRULES  431
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  657

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   356


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
       2,     2,     2,   105,     2,     2,     2,     2,     2,     2,
     103,   104,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   102,
     107,     2,   106,     2,     2,     2,     2,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   101
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
     295,   296,   300,   304,   308,   313,   317,   321,   325,   329,
     333,   337,   344,   348,   355,   359,   366,   370,   374,   378,
     382,   386,   390,   394,   401,   407,   413,   457,   504,   505,
     506,   507,   508,   509,   510,   514,   521,   527,   536,   548,
     555,   562,   571,   583,   590,   597,   606,   618,   625,   632,
     641,   653,   659,   665,   674,   686,   692,   698,   707,   719,
     725,   731,   740,   752,   758,   764,   773,   785,   786,   787,
     788,   792,   796,   800,   804,   808,   812,   816,   820,   827,
     831,   835,   839,   843,   847,   851,   855,   862,   866,   870,
     874,   878,   882,   886,   890,   897,   901,   905,   909,   913,
     917,   921,   925,   932,   938,   944,   963,   985,   986,   989,
     993,   999,  1007,  1015,  1023,  1034,  1038,  1044,  1052,  1060,
    1068,  1079,  1083,  1089,  1097,  1105,  1113,  1124,  1128,  1134,
    1142,  1150,  1158,  1169,  1173,  1179,  1187,  1195,  1203,  1214,
    1218,  1224,  1232,  1240,  1248,  1259,  1263,  1267,  1271,  1275,
    1279,  1286,  1290,  1294,  1298,  1302,  1306,  1313,  1317,  1321,
    1325,  1329,  1333,  1340,  1344,  1348,  1352,  1356,  1360,  1367,
    1373,  1381,  1382,  1385,  1394,  1406,  1433,  1463,  1520,  1576,
    1618,  1663,  1680,  1700,  1707,  1718,  1735,  1755,  1762,  1773,
    1799,  1828,  1842,  1859,  1876,  1896,  1904,  1915,  1932,  1952,
    1960,  1971,  1988,  2008,  2016,  2027,  2044,  2064,  2072,  2083,
    2100,  2120,  2128,  2139,  2145,  2151,  2152,  2155,  2161,  2170,
    2190,  2212,  2229,  2249,  2257,  2268,  2275,  2282,  2301,  2320,
    2334,  2343,  2350,  2357,  2376,  2395,  2409,  2418,  2422,  2426,
    2445,  2464,  2477,  2493,  2497,  2501,  2505,  2509,  2513,  2517,
    2521,  2528,  2532,  2536,  2540,  2544,  2548,  2552,  2556,  2563,
    2567,  2571,  2575,  2579,  2583,  2587,  2591,  2598,  2606,  2614,
    2622,  2630,  2638,  2646,  2654,  2665,  2672,  2679,  2697,  2715,
    2729,  2738,  2749,  2760,  2781,  2800,  2814,  2831,  2842,  2853,
    2872,  2891,  2906,  2923,  2944,  2966,  2986,  3001,  3013,  3025,
    3037,  3050,  3065,  3085,  3100,  3112,  3124,  3136,  3149,  3164,
    3176,  3188,  3202,  3214,  3226,  3240,  3252,  3264,  3278,  3290,
    3302,  3317,  3321,  3325,  3329,  3333,  3337,  3341,  3345,  3352,
    3356,  3360,  3364,  3368,  3372,  3376,  3380,  3387,  3391,  3395,
    3399,  3403,  3407,  3411,  3415,  3422,  3429,  3436,  3455,  3474,
    3488,  3505,  3509,  3513,  3517,  3524,  3525,  3526,  3530,  3537,
    3544,  3572
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
  "FLOW_SRC_MAC", "FLOW_DST_MAC", "FLOW_SRC_IP", "FLOW_DST_IP",
  "FLOW_LOCAL_PORT", "FLOW_OTHER_PORT", "FLOW_SRC_PORT", "FLOW_DST_PORT",
  "FLOW_TUNNEL_TYPE", "FLOW_DETECTION_COMPLETE", "FLOW_DETECTION_GUESSED",
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
  "value_other_type", "expr_local_mac", "expr_other_mac", "expr_src_mac",
  "expr_dst_mac", "expr_local_ip", "expr_other_ip", "expr_src_ip",
  "expr_dst_ip", "value_addr_ip", "expr_local_port", "expr_other_port",
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

#define YYPACT_NINF (-81)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -81,     1,   -81,   -12,   -80,    14,    -8,   198,   256,   318,
     326,   328,   330,   332,   334,   336,    -4,   134,   160,   164,
     338,   340,   342,   344,   346,   348,   350,   352,   354,   356,
     358,   360,   172,   176,   180,   362,   364,   366,     9,   368,
     370,   372,   374,   376,   378,   380,   382,   384,   386,   388,
     390,   392,   394,   396,   398,   400,   402,   404,   406,   408,
     410,   190,   206,   210,   221,   412,   414,   416,   418,   225,
     104,   171,     3,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,    80,    86,   106,   112,   115,   125,
     128,   151,   168,   185,   215,   230,   246,   472,   474,   476,
     249,   249,    88,   133,   159,   220,   257,   262,   267,   272,
     235,   300,   301,   302,   277,   282,   287,   292,   477,   478,
     479,   480,   481,   482,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   423,   423,   415,   417,   419,   421,   424,   426,
     428,   430,   432,   434,   436,   438,   440,   442,   444,   446,
     448,   450,   452,   454,    -7,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   233,   243,   520,   521,    81,    83,
     420,   522,    84,   531,   532,   525,   526,   535,   536,   529,
     530,   539,   540,   533,   534,   543,   544,   537,   538,   547,
     548,   541,   542,   551,   552,   545,   546,   555,   556,   557,
     558,   553,   554,   177,   184,   559,   560,   561,   562,   453,
     455,   457,   459,    10,    10,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   461,
     463,   465,   467,   469,   471,   473,   475,   121,   244,   199,
     298,   303,   306,    -3,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   104,   104,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   483,   483
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,    74,     0,     0,    86,    94,     0,     0,
       0,     0,     0,     0,     0,     0,   141,   149,   157,   165,
     173,   179,   185,   191,   197,   203,   209,   215,   221,   227,
     233,   307,   313,   321,   329,     0,     0,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,     0,     0,   283,     0,     0,   295,   301,
     421,   337,   391,   399,   407,   415,   345,   351,   357,   363,
       0,     0,     0,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    34,   241,   242,    32,    33,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,   285,   286,    52,    53,    54,    55,    56,
      57,    58,    59,    65,    66,    67,    68,    69,    60,    61,
      62,    63,    64,    70,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,    75,    87,    95,   142,   150,   158,
     166,   174,   180,   186,   192,   198,   204,   210,   216,   222,
     228,   234,   308,   314,   322,   330,   240,   429,   284,   296,
     302,   422,   338,   392,   400,   408,   416,   346,   352,   358,
     364,     0,     0,     3,    76,    77,    78,    79,    80,    81,
      84,    85,    82,    83,    88,    89,    90,    91,    92,    93,
      98,    99,   100,   101,   102,   103,   104,    96,    97,   105,
     107,   106,   108,   109,   111,   110,   112,   123,   137,   138,
     139,   140,   121,   124,   122,   127,   125,   128,   126,   113,
     115,   114,   116,   117,   119,   118,   120,   131,   129,   132,
     130,   135,   133,   136,   134,   143,   144,   145,   146,   147,
     148,   151,   152,   153,   154,   155,   156,   159,   160,   161,
     162,   163,   164,   167,   168,   169,   170,   171,   172,   177,
     178,   175,   176,   181,   182,   183,   184,   187,   188,   189,
     190,   193,   194,   195,   196,   199,   200,   201,   202,   205,
     206,   207,   208,   211,   212,   213,   214,   217,   218,   219,
     220,   223,   224,   225,   226,   229,   230,   231,   232,   235,
     236,   237,   238,   309,   311,   310,   312,   315,   316,   317,
     318,   319,   320,   323,   324,   325,   326,   327,   328,   331,
     332,   333,   334,   335,   336,   247,   248,   249,   250,   245,
     243,   246,   244,   251,   253,   252,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   430,   431,   279,   280,   281,   282,   289,   287,   290,
     288,   291,   292,   293,   294,   297,   299,   298,   300,   303,
     305,   304,   306,   425,   426,   427,   423,   424,   339,   340,
     341,   342,   343,   344,   393,   394,   395,   396,   397,   398,
     401,   402,   403,   404,   405,   406,   409,   410,   411,   412,
     413,   414,   417,   419,   418,   420,   347,   349,   348,   350,
     353,   355,   354,   356,   359,   361,   360,   362,   370,   371,
     365,   366,   368,   367,   369,   377,   378,   372,   373,   375,
     374,   376,   380,   379,   381,   383,   382,   384,   386,   385,
     387,   389,   388,   390,    73,    72,    71
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -81,   -81,   -70,   -81,   -81,   -81,   -81,   -81,   422,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   173,   -81,   -81,
     -81,   -81,   -81,   456,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   279,   -81
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    72,    73,    74,    75,    76,    77,   397,    78,
      79,    80,    81,    82,    83,    84,    85,   412,    86,    87,
      88,    89,    90,   461,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   586,   143
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     333,     2,   150,   151,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
     144,   145,   146,   147,   154,   155,   156,   157,   178,   179,
     180,   181,    69,   371,   372,   503,   583,   584,   585,   371,
     372,   250,   505,   504,   148,   149,   152,   153,   158,   159,
     506,   654,   182,   183,    70,   373,    71,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,   529,   334,   531,   535,   335,   336,   399,
     374,   530,   400,   532,   536,    69,   375,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   376,    70,   356,    71,
     628,   629,   377,   630,   631,   378,   184,   185,   186,   187,
     632,   633,   634,   357,   401,   379,   358,   402,   380,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     188,   189,   190,   191,   192,   193,   196,   197,   198,   199,
     403,   381,   370,   404,   226,   227,   228,   229,   232,   233,
     234,   235,   238,   239,   240,   241,   194,   195,   382,   567,
     200,   201,   295,   296,   297,   298,   569,   568,   230,   231,
     160,   161,   236,   237,   570,   383,   242,   243,   301,   302,
     303,   304,   307,   308,   309,   310,   299,   300,   642,   643,
     644,   655,   656,   313,   314,   315,   316,   327,   328,   329,
     330,   405,   305,   306,   406,   384,   311,   312,   390,   391,
     392,   393,   394,   395,   396,   525,   419,   317,   318,   420,
     385,   331,   332,   635,   636,   526,   637,   638,   162,   163,
     414,   416,   418,   639,   640,   641,   386,   428,   430,   432,
     434,   407,   408,   409,   410,   411,   413,   408,   409,   410,
     411,   415,   408,   409,   410,   411,   417,   408,   409,   410,
     411,   427,   408,   409,   410,   411,   429,   408,   409,   410,
     411,   431,   408,   409,   410,   411,   433,   408,   409,   410,
     411,   421,   423,   425,   422,   424,   426,   645,   646,   647,
     164,   165,   648,   649,   650,   651,   652,   653,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   244,   245,   246,   247,   248,   249,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   319,   320,   321,   322,   323,   324,
     325,   326,   459,   460,   463,   464,   465,   466,   467,   468,
     469,   470,   533,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   575,   576,   577,   578,   579,
     580,   581,   582,   612,   613,   614,   615,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   627,   371,
     372,     0,   387,   587,   388,     0,   389,   435,   436,   437,
     438,   439,   440,   398,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     527,   528,   534,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   571,   572,   565,   566,     0,     0,     0,     0,   462,
       0,   573,   574,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611
};

static const yytype_int16 yycheck[] =
{
      70,     0,    82,    83,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      82,    83,    84,    85,    82,    83,    84,    85,    82,    83,
      84,    85,    81,    86,    87,    92,    76,    77,    78,    86,
      87,    82,    92,   100,   106,   107,    82,    83,   106,   107,
     100,   104,   106,   107,   103,   102,   105,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    92,     3,    92,    92,     6,     7,    91,
     100,   100,    94,   100,   100,    81,   100,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,   100,   103,    37,   105,
      89,    90,   100,    92,    93,   100,    82,    83,    84,    85,
      99,   100,   101,    52,    91,   100,    55,    94,   100,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
     106,   107,    82,    83,    84,    85,    82,    83,    84,    85,
      91,   100,    81,    94,    82,    83,    84,    85,    82,    83,
      84,    85,    82,    83,    84,    85,   106,   107,   100,    92,
     106,   107,    82,    83,    84,    85,    92,   100,   106,   107,
      82,    83,   106,   107,   100,   100,   106,   107,    82,    83,
      84,    85,    82,    83,    84,    85,   106,   107,    99,   100,
     101,   371,   372,    82,    83,    84,    85,    82,    83,    84,
      85,    91,   106,   107,    94,   100,   106,   107,    69,    70,
      71,    72,    73,    74,    75,    92,    91,   106,   107,    94,
     100,   106,   107,    89,    90,    92,    92,    93,    82,    83,
     167,   168,   169,    99,   100,   101,   100,   174,   175,   176,
     177,    94,    95,    96,    97,    98,    94,    95,    96,    97,
      98,    94,    95,    96,    97,    98,    94,    95,    96,    97,
      98,    94,    95,    96,    97,    98,    94,    95,    96,    97,
      98,    94,    95,    96,    97,    98,    94,    95,    96,    97,
      98,    91,    91,    91,    94,    94,    94,    99,   100,   101,
      82,    83,    99,   100,   101,    99,   100,   101,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    79,    80,    89,    90,    89,    90,    89,    90,
      89,    90,    92,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    89,    90,    92,    93,    92,    93,    92,
      93,    92,    93,    92,    93,    92,    93,    92,    93,    92,
      93,    92,    93,    92,    93,    92,    93,    92,    93,    86,
      87,    -1,   100,   294,   100,    -1,   100,   100,   100,   100,
     100,   100,   100,   161,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,    92,    92,   100,   100,    92,    92,   100,
     100,    92,    92,   100,   100,    92,    92,   100,   100,    92,
      92,   100,   100,    92,    92,   100,   100,    92,    92,    92,
      92,    92,    92,   100,   100,    -1,    -1,    -1,    -1,   203,
      -1,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   109,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    81,
     103,   105,   110,   111,   112,   113,   114,   115,   117,   118,
     119,   120,   121,   122,   123,   124,   126,   127,   128,   129,
     130,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   185,    82,    83,    84,    85,   106,   107,
      82,    83,    82,    83,    82,    83,    84,    85,   106,   107,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      84,    85,   106,   107,    82,    83,    84,    85,   106,   107,
      82,    83,    84,    85,   106,   107,    82,    83,    84,    85,
     106,   107,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    82,    83,
      82,    83,    82,    83,    82,    83,    82,    83,    84,    85,
     106,   107,    82,    83,    84,    85,   106,   107,    82,    83,
      84,    85,   106,   107,    82,    83,    82,    83,    82,    83,
      82,    82,    83,    82,    83,    82,    83,    82,    83,    82,
      83,    82,    83,    82,    83,    82,    83,    82,    83,    82,
      83,    82,    83,    82,    83,    82,    83,    82,    83,    82,
      83,    82,    83,    82,    83,    82,    83,    82,    83,    82,
      83,    82,    83,    82,    83,    82,    83,    84,    85,   106,
     107,    82,    83,    84,    85,   106,   107,    82,    83,    84,
      85,   106,   107,    82,    83,    84,    85,   106,   107,    82,
      83,    82,    83,    82,    83,    82,    83,    82,    83,    84,
      85,   106,   107,   110,     3,     6,     7,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    37,    52,    55,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      81,    86,    87,   102,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
      69,    70,    71,    72,    73,    74,    75,   116,   116,    91,
      94,    91,    94,    91,    94,    91,    94,    94,    95,    96,
      97,    98,   125,    94,   125,    94,   125,    94,   125,    91,
      94,    91,    94,    91,    94,    91,    94,    94,   125,    94,
     125,    94,   125,    94,   125,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,    79,
      80,   131,   131,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    89,    90,    89,    90,    89,    90,    89,
      90,    89,    90,    92,   100,    92,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,    92,    92,   100,   100,    92,
     100,    92,   100,    92,   100,    92,   100,    92,    92,   100,
     100,    92,    92,   100,   100,    92,    92,   100,   100,    92,
      92,   100,   100,    92,    92,   100,   100,    92,    92,   100,
     100,    92,    92,    92,    92,   100,   100,    92,   100,    92,
     100,    92,    92,   100,   100,    92,    93,    92,    93,    92,
      93,    92,    93,    76,    77,    78,   184,   184,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,    92,    93,    92,    93,    92,    93,    92,    93,
      92,    93,    92,    93,    92,    93,    92,    93,    89,    90,
      92,    93,    99,   100,   101,    89,    90,    92,    93,    99,
     100,   101,    99,   100,   101,    99,   100,   101,    99,   100,
     101,    99,   100,   101,   104,   110,   110
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   109,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   111,   111,   111,   111,   111,   111,
     111,   111,   112,   112,   113,   113,   114,   114,   114,   114,
     114,   114,   114,   114,   115,   115,   115,   115,   116,   116,
     116,   116,   116,   116,   116,   117,   117,   117,   117,   118,
     118,   118,   118,   119,   119,   119,   119,   120,   120,   120,
     120,   121,   121,   121,   121,   122,   122,   122,   122,   123,
     123,   123,   123,   124,   124,   124,   124,   125,   125,   125,
     125,   126,   126,   126,   126,   126,   126,   126,   126,   127,
     127,   127,   127,   127,   127,   127,   127,   128,   128,   128,
     128,   128,   128,   128,   128,   129,   129,   129,   129,   129,
     129,   129,   129,   130,   130,   130,   130,   131,   131,   132,
     132,   132,   132,   132,   132,   133,   133,   133,   133,   133,
     133,   134,   134,   134,   134,   134,   134,   135,   135,   135,
     135,   135,   135,   136,   136,   136,   136,   136,   136,   137,
     137,   137,   137,   137,   137,   138,   138,   138,   138,   138,
     138,   139,   139,   139,   139,   139,   139,   140,   140,   140,
     140,   140,   140,   141,   141,   141,   141,   141,   141,   142,
     142,   142,   142,   143,   143,   144,   144,   145,   145,   146,
     146,   147,   147,   148,   148,   149,   149,   150,   150,   151,
     151,   152,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   157,   157,   158,   158,   159,   159,   160,   160,   161,
     161,   162,   162,   163,   163,   163,   163,   164,   164,   165,
     165,   166,   166,   167,   167,   168,   168,   168,   168,   168,
     168,   169,   169,   169,   169,   169,   169,   170,   170,   170,
     170,   170,   170,   171,   171,   171,   171,   171,   171,   171,
     171,   172,   172,   172,   172,   172,   172,   172,   172,   173,
     173,   173,   173,   173,   173,   173,   173,   174,   174,   174,
     174,   174,   174,   174,   174,   175,   175,   175,   175,   175,
     175,   176,   176,   176,   176,   176,   176,   177,   177,   177,
     177,   177,   177,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   179,   179,   179,   179,   179,   179,   180,
     180,   180,   180,   180,   180,   180,   180,   181,   181,   181,
     181,   181,   181,   181,   181,   182,   182,   182,   182,   182,
     182,   183,   183,   183,   183,   184,   184,   184,   185,   185,
     185,   185
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
       1,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     2,     3,     3,     3,     3,     3,     3,     1,
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
       3,     1,     2,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     2,     3,     3,     1,     1,     1,     1,     2,
       3,     3
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
  case 71: /* expr: expr BOOL_OR expr  */
#line 296 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) || (yyvsp[0].bool_result)));
        _NDFP_debugf("OR (%d || %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 2300 "nd-flow-expr.cpp"
    break;

  case 72: /* expr: expr BOOL_AND expr  */
#line 300 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) && (yyvsp[0].bool_result)));
        _NDFP_debugf("AND (%d && %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 2309 "nd-flow-expr.cpp"
    break;

  case 73: /* expr: '(' expr ')'  */
#line 304 "nd-flow-expr.ypp"
                   { _NDFP_result = ((yyval.bool_result) = (yyvsp[-1].bool_result)); }
#line 2315 "nd-flow-expr.cpp"
    break;

  case 74: /* expr_ip_proto: FLOW_IP_PROTO  */
#line 308 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != 0));
        _NDFP_debugf(
            "IP Protocol is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2325 "nd-flow-expr.cpp"
    break;

  case 75: /* expr_ip_proto: '!' FLOW_IP_PROTO  */
#line 313 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == 0));
        _NDFP_debugf("IP Protocol is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2334 "nd-flow-expr.cpp"
    break;

  case 76: /* expr_ip_proto: FLOW_IP_PROTO CMP_EQUAL VALUE_UNSIGNED  */
#line 317 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2343 "nd-flow-expr.cpp"
    break;

  case 77: /* expr_ip_proto: FLOW_IP_PROTO CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 321 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2352 "nd-flow-expr.cpp"
    break;

  case 78: /* expr_ip_proto: FLOW_IP_PROTO CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 325 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol >= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2361 "nd-flow-expr.cpp"
    break;

  case 79: /* expr_ip_proto: FLOW_IP_PROTO CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 329 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol <= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2370 "nd-flow-expr.cpp"
    break;

  case 80: /* expr_ip_proto: FLOW_IP_PROTO '>' VALUE_UNSIGNED  */
#line 333 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol > (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2379 "nd-flow-expr.cpp"
    break;

  case 81: /* expr_ip_proto: FLOW_IP_PROTO '<' VALUE_UNSIGNED  */
#line 337 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol < (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2388 "nd-flow-expr.cpp"
    break;

  case 82: /* expr_ip_dscp: FLOW_IP_DSCP CMP_EQUAL VALUE_UNSIGNED  */
#line 344 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2397 "nd-flow-expr.cpp"
    break;

  case 83: /* expr_ip_dscp: FLOW_IP_DSCP CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 348 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2406 "nd-flow-expr.cpp"
    break;

  case 84: /* expr_ip_version: FLOW_IP_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 355 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2415 "nd-flow-expr.cpp"
    break;

  case 85: /* expr_ip_version: FLOW_IP_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 359 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2424 "nd-flow-expr.cpp"
    break;

  case 86: /* expr_vlan_id: FLOW_VLAN_ID  */
#line 366 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0));
        _NDFP_debugf("VLAN ID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2433 "nd-flow-expr.cpp"
    break;

  case 87: /* expr_vlan_id: '!' FLOW_VLAN_ID  */
#line 370 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0));
        _NDFP_debugf("VLAN ID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2442 "nd-flow-expr.cpp"
    break;

  case 88: /* expr_vlan_id: FLOW_VLAN_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 374 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2451 "nd-flow-expr.cpp"
    break;

  case 89: /* expr_vlan_id: FLOW_VLAN_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 378 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2460 "nd-flow-expr.cpp"
    break;

  case 90: /* expr_vlan_id: FLOW_VLAN_ID CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 382 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2469 "nd-flow-expr.cpp"
    break;

  case 91: /* expr_vlan_id: FLOW_VLAN_ID CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 386 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2478 "nd-flow-expr.cpp"
    break;

  case 92: /* expr_vlan_id: FLOW_VLAN_ID '>' VALUE_UNSIGNED  */
#line 390 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2487 "nd-flow-expr.cpp"
    break;

  case 93: /* expr_vlan_id: FLOW_VLAN_ID '<' VALUE_UNSIGNED  */
#line 394 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2496 "nd-flow-expr.cpp"
    break;

  case 94: /* expr_other_type: FLOW_OTHER_TYPE  */
#line 401 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2507 "nd-flow-expr.cpp"
    break;

  case 95: /* expr_other_type: '!' FLOW_OTHER_TYPE  */
#line 407 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2518 "nd-flow-expr.cpp"
    break;

  case 96: /* expr_other_type: FLOW_OTHER_TYPE CMP_EQUAL value_other_type  */
#line 413 "nd-flow-expr.ypp"
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
#line 2567 "nd-flow-expr.cpp"
    break;

  case 97: /* expr_other_type: FLOW_OTHER_TYPE CMP_NOTEQUAL value_other_type  */
#line 457 "nd-flow-expr.ypp"
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
#line 2616 "nd-flow-expr.cpp"
    break;

  case 98: /* value_other_type: FLOW_OTHER_UNKNOWN  */
#line 504 "nd-flow-expr.ypp"
                         { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2622 "nd-flow-expr.cpp"
    break;

  case 99: /* value_other_type: FLOW_OTHER_UNSUPPORTED  */
#line 505 "nd-flow-expr.ypp"
                             { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2628 "nd-flow-expr.cpp"
    break;

  case 100: /* value_other_type: FLOW_OTHER_LOCAL  */
#line 506 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2634 "nd-flow-expr.cpp"
    break;

  case 101: /* value_other_type: FLOW_OTHER_MULTICAST  */
#line 507 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2640 "nd-flow-expr.cpp"
    break;

  case 102: /* value_other_type: FLOW_OTHER_BROADCAST  */
#line 508 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2646 "nd-flow-expr.cpp"
    break;

  case 103: /* value_other_type: FLOW_OTHER_REMOTE  */
#line 509 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2652 "nd-flow-expr.cpp"
    break;

  case 104: /* value_other_type: FLOW_OTHER_ERROR  */
#line 510 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2658 "nd-flow-expr.cpp"
    break;

  case 105: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 514 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2670 "nd-flow-expr.cpp"
    break;

  case 106: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 521 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2681 "nd-flow-expr.cpp"
    break;

  case 107: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 527 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == true));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2695 "nd-flow-expr.cpp"
    break;

  case 108: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 536 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_mac) == false));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2709 "nd-flow-expr.cpp"
    break;

  case 109: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 548 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2721 "nd-flow-expr.cpp"
    break;

  case 110: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 555 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_other_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2733 "nd-flow-expr.cpp"
    break;

  case 111: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 562 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == true));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2747 "nd-flow-expr.cpp"
    break;

  case 112: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 571 "nd-flow-expr.ypp"
                                                 {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_mac) == false));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2761 "nd-flow-expr.cpp"
    break;

  case 113: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 583 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2773 "nd-flow-expr.cpp"
    break;

  case 114: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 590 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_src_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2785 "nd-flow-expr.cpp"
    break;

  case 115: /* expr_src_mac: FLOW_SRC_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 597 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == true));
        _NDFP_debugf("Source MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2799 "nd-flow-expr.cpp"
    break;

  case 116: /* expr_src_mac: FLOW_SRC_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 606 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_mac) == false));
        _NDFP_debugf("Source MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2813 "nd-flow-expr.cpp"
    break;

  case 117: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 618 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2825 "nd-flow-expr.cpp"
    break;

  case 118: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 625 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(
                _NDFP_dst_mac->GetString().c_str(), (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2837 "nd-flow-expr.cpp"
    break;

  case 119: /* expr_dst_mac: FLOW_DST_MAC CMP_EQUAL VALUE_ADDR_TAG  */
#line 632 "nd-flow-expr.ypp"
                                            {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == true));
        _NDFP_debugf("Destination MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2851 "nd-flow-expr.cpp"
    break;

  case 120: /* expr_dst_mac: FLOW_DST_MAC CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 641 "nd-flow-expr.ypp"
                                               {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_mac) == false));
        _NDFP_debugf("Destination MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2865 "nd-flow-expr.cpp"
    break;

  case 121: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL value_addr_ip  */
#line 653 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2876 "nd-flow-expr.cpp"
    break;

  case 122: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL value_addr_ip  */
#line 659 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2887 "nd-flow-expr.cpp"
    break;

  case 123: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 665 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == true));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2901 "nd-flow-expr.cpp"
    break;

  case 124: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 674 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_local_ip) == false));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2915 "nd-flow-expr.cpp"
    break;

  case 125: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL value_addr_ip  */
#line 686 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2926 "nd-flow-expr.cpp"
    break;

  case 126: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL value_addr_ip  */
#line 692 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2937 "nd-flow-expr.cpp"
    break;

  case 127: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 698 "nd-flow-expr.ypp"
                                             {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == true));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2951 "nd-flow-expr.cpp"
    break;

  case 128: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 707 "nd-flow-expr.ypp"
                                                {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_other_ip) == false));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2965 "nd-flow-expr.cpp"
    break;

  case 129: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL value_addr_ip  */
#line 719 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Source IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2976 "nd-flow-expr.cpp"
    break;

  case 130: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL value_addr_ip  */
#line 725 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_src_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Source IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2987 "nd-flow-expr.cpp"
    break;

  case 131: /* expr_src_ip: FLOW_SRC_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 731 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == true));
        _NDFP_debugf("Source IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3001 "nd-flow-expr.cpp"
    break;

  case 132: /* expr_src_ip: FLOW_SRC_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 740 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_src_ip) == false));
        _NDFP_debugf("Source IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3015 "nd-flow-expr.cpp"
    break;

  case 133: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL value_addr_ip  */
#line 752 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Destination IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3026 "nd-flow-expr.cpp"
    break;

  case 134: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL value_addr_ip  */
#line 758 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_dst_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Destination IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3037 "nd-flow-expr.cpp"
    break;

  case 135: /* expr_dst_ip: FLOW_DST_IP CMP_EQUAL VALUE_ADDR_TAG  */
#line 764 "nd-flow-expr.ypp"
                                           {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == true));
        _NDFP_debugf("Destination IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3051 "nd-flow-expr.cpp"
    break;

  case 136: /* expr_dst_ip: FLOW_DST_IP CMP_NOTEQUAL VALUE_ADDR_TAG  */
#line 773 "nd-flow-expr.ypp"
                                              {
        string tag((yyvsp[0].buffer));
        size_t p = tag.find_first_of("@");
        if (p != string::npos) tag.erase(p, 1);

        _NDFP_result = ((yyval.bool_result) = (_NDFP_addr_lookup
          .LookupGroupAddress(tag, *_NDFP_dst_ip) == false));
        _NDFP_debugf("Destination IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3065 "nd-flow-expr.cpp"
    break;

  case 137: /* value_addr_ip: VALUE_ADDR_IPV4  */
#line 785 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3071 "nd-flow-expr.cpp"
    break;

  case 138: /* value_addr_ip: VALUE_ADDR_IPV4_CIDR  */
#line 786 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3077 "nd-flow-expr.cpp"
    break;

  case 139: /* value_addr_ip: VALUE_ADDR_IPV6  */
#line 787 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3083 "nd-flow-expr.cpp"
    break;

  case 140: /* value_addr_ip: VALUE_ADDR_IPV6_CIDR  */
#line 788 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 3089 "nd-flow-expr.cpp"
    break;

  case 141: /* expr_local_port: FLOW_LOCAL_PORT  */
#line 792 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        _NDFP_debugf("Local port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3098 "nd-flow-expr.cpp"
    break;

  case 142: /* expr_local_port: '!' FLOW_LOCAL_PORT  */
#line 796 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        _NDFP_debugf("Local port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3107 "nd-flow-expr.cpp"
    break;

  case 143: /* expr_local_port: FLOW_LOCAL_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 800 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3116 "nd-flow-expr.cpp"
    break;

  case 144: /* expr_local_port: FLOW_LOCAL_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 804 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3125 "nd-flow-expr.cpp"
    break;

  case 145: /* expr_local_port: FLOW_LOCAL_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 808 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3134 "nd-flow-expr.cpp"
    break;

  case 146: /* expr_local_port: FLOW_LOCAL_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 812 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3143 "nd-flow-expr.cpp"
    break;

  case 147: /* expr_local_port: FLOW_LOCAL_PORT '>' VALUE_UNSIGNED  */
#line 816 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3152 "nd-flow-expr.cpp"
    break;

  case 148: /* expr_local_port: FLOW_LOCAL_PORT '<' VALUE_UNSIGNED  */
#line 820 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3161 "nd-flow-expr.cpp"
    break;

  case 149: /* expr_other_port: FLOW_OTHER_PORT  */
#line 827 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Other port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3170 "nd-flow-expr.cpp"
    break;

  case 150: /* expr_other_port: '!' FLOW_OTHER_PORT  */
#line 831 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Other port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3179 "nd-flow-expr.cpp"
    break;

  case 151: /* expr_other_port: FLOW_OTHER_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 835 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3188 "nd-flow-expr.cpp"
    break;

  case 152: /* expr_other_port: FLOW_OTHER_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 839 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3197 "nd-flow-expr.cpp"
    break;

  case 153: /* expr_other_port: FLOW_OTHER_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 843 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3206 "nd-flow-expr.cpp"
    break;

  case 154: /* expr_other_port: FLOW_OTHER_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 847 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3215 "nd-flow-expr.cpp"
    break;

  case 155: /* expr_other_port: FLOW_OTHER_PORT '>' VALUE_UNSIGNED  */
#line 851 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3224 "nd-flow-expr.cpp"
    break;

  case 156: /* expr_other_port: FLOW_OTHER_PORT '<' VALUE_UNSIGNED  */
#line 855 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3233 "nd-flow-expr.cpp"
    break;

  case 157: /* expr_src_port: FLOW_SRC_PORT  */
#line 862 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != 0));
        _NDFP_debugf("Source port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3242 "nd-flow-expr.cpp"
    break;

  case 158: /* expr_src_port: '!' FLOW_SRC_PORT  */
#line 866 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == 0));
        _NDFP_debugf("Source port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3251 "nd-flow-expr.cpp"
    break;

  case 159: /* expr_src_port: FLOW_SRC_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 870 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3260 "nd-flow-expr.cpp"
    break;

  case 160: /* expr_src_port: FLOW_SRC_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 874 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3269 "nd-flow-expr.cpp"
    break;

  case 161: /* expr_src_port: FLOW_SRC_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 878 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3278 "nd-flow-expr.cpp"
    break;

  case 162: /* expr_src_port: FLOW_SRC_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 882 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3287 "nd-flow-expr.cpp"
    break;

  case 163: /* expr_src_port: FLOW_SRC_PORT '>' VALUE_UNSIGNED  */
#line 886 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3296 "nd-flow-expr.cpp"
    break;

  case 164: /* expr_src_port: FLOW_SRC_PORT '<' VALUE_UNSIGNED  */
#line 890 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_src_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Source port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3305 "nd-flow-expr.cpp"
    break;

  case 165: /* expr_dst_port: FLOW_DST_PORT  */
#line 897 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != 0));
        _NDFP_debugf("Destination port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3314 "nd-flow-expr.cpp"
    break;

  case 166: /* expr_dst_port: '!' FLOW_DST_PORT  */
#line 901 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == 0));
        _NDFP_debugf("Destination port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3323 "nd-flow-expr.cpp"
    break;

  case 167: /* expr_dst_port: FLOW_DST_PORT CMP_EQUAL VALUE_UNSIGNED  */
#line 905 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3332 "nd-flow-expr.cpp"
    break;

  case 168: /* expr_dst_port: FLOW_DST_PORT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 909 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3341 "nd-flow-expr.cpp"
    break;

  case 169: /* expr_dst_port: FLOW_DST_PORT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 913 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3350 "nd-flow-expr.cpp"
    break;

  case 170: /* expr_dst_port: FLOW_DST_PORT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 917 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3359 "nd-flow-expr.cpp"
    break;

  case 171: /* expr_dst_port: FLOW_DST_PORT '>' VALUE_UNSIGNED  */
#line 921 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3368 "nd-flow-expr.cpp"
    break;

  case 172: /* expr_dst_port: FLOW_DST_PORT '<' VALUE_UNSIGNED  */
#line 925 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_dst_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Destination port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3377 "nd-flow-expr.cpp"
    break;

  case 173: /* expr_tunnel_type: FLOW_TUNNEL_TYPE  */
#line 932 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3388 "nd-flow-expr.cpp"
    break;

  case 174: /* expr_tunnel_type: '!' FLOW_TUNNEL_TYPE  */
#line 938 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type is none? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3399 "nd-flow-expr.cpp"
    break;

  case 175: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_EQUAL value_tunnel_type  */
#line 944 "nd-flow-expr.ypp"
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
#line 3423 "nd-flow-expr.cpp"
    break;

  case 176: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_NOTEQUAL value_tunnel_type  */
#line 963 "nd-flow-expr.ypp"
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
#line 3447 "nd-flow-expr.cpp"
    break;

  case 177: /* value_tunnel_type: FLOW_TUNNEL_NONE  */
#line 985 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3453 "nd-flow-expr.cpp"
    break;

  case 178: /* value_tunnel_type: FLOW_TUNNEL_GTP  */
#line 986 "nd-flow-expr.ypp"
                      { (yyval.us_number) = (yyvsp[0].us_number); }
#line 3459 "nd-flow-expr.cpp"
    break;

  case 179: /* expr_detection_complete: FLOW_DETECTION_COMPLETE  */
#line 989 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf("Detection was complete? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3468 "nd-flow-expr.cpp"
    break;

  case 180: /* expr_detection_complete: '!' FLOW_DETECTION_COMPLETE  */
#line 993 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf(
            "Detection was not complete? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3479 "nd-flow-expr.cpp"
    break;

  case 181: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_TRUE  */
#line 999 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == true
        ));
        _NDFP_debugf(
            "Detection complete == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3492 "nd-flow-expr.cpp"
    break;

  case 182: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_FALSE  */
#line 1007 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == false
        ));
        _NDFP_debugf(
            "Detection complete == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3505 "nd-flow-expr.cpp"
    break;

  case 183: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_TRUE  */
#line 1015 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != true
        ));
        _NDFP_debugf(
            "Detection complete != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3518 "nd-flow-expr.cpp"
    break;

  case 184: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_FALSE  */
#line 1023 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != false
        ));
        _NDFP_debugf(
            "Detection complete != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3531 "nd-flow-expr.cpp"
    break;

  case 185: /* expr_detection_guessed: FLOW_DETECTION_GUESSED  */
#line 1034 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf("Detection was guessed? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3540 "nd-flow-expr.cpp"
    break;

  case 186: /* expr_detection_guessed: '!' FLOW_DETECTION_GUESSED  */
#line 1038 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf(
            "Detection was not guessed? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3551 "nd-flow-expr.cpp"
    break;

  case 187: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_TRUE  */
#line 1044 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == true
        ));
        _NDFP_debugf(
            "Detection guessed == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3564 "nd-flow-expr.cpp"
    break;

  case 188: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_FALSE  */
#line 1052 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == false
        ));
        _NDFP_debugf(
            "Detection guessed == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3577 "nd-flow-expr.cpp"
    break;

  case 189: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_TRUE  */
#line 1060 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != true
        ));
        _NDFP_debugf(
            "Detection guessed != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3590 "nd-flow-expr.cpp"
    break;

  case 190: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_FALSE  */
#line 1068 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != false
        ));
        _NDFP_debugf(
            "Detection guessed != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3603 "nd-flow-expr.cpp"
    break;

  case 191: /* expr_detection_init: FLOW_DETECTION_INIT  */
#line 1079 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf("Detection was init? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3612 "nd-flow-expr.cpp"
    break;

  case 192: /* expr_detection_init: '!' FLOW_DETECTION_INIT  */
#line 1083 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf(
            "Detection was not init? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3623 "nd-flow-expr.cpp"
    break;

  case 193: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_TRUE  */
#line 1089 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == true
        ));
        _NDFP_debugf(
            "Detection init == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3636 "nd-flow-expr.cpp"
    break;

  case 194: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_FALSE  */
#line 1097 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == false
        ));
        _NDFP_debugf(
            "Detection init == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3649 "nd-flow-expr.cpp"
    break;

  case 195: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1105 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != true
        ));
        _NDFP_debugf(
            "Detection init != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3662 "nd-flow-expr.cpp"
    break;

  case 196: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1113 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != false
        ));
        _NDFP_debugf(
            "Detection init != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3675 "nd-flow-expr.cpp"
    break;

  case 197: /* expr_detection_updated: FLOW_DETECTION_UPDATED  */
#line 1124 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf("Detection was updated? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3684 "nd-flow-expr.cpp"
    break;

  case 198: /* expr_detection_updated: '!' FLOW_DETECTION_UPDATED  */
#line 1128 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf(
            "Detection was not updated? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3695 "nd-flow-expr.cpp"
    break;

  case 199: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_TRUE  */
#line 1134 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == true
        ));
        _NDFP_debugf(
            "Detection updated == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3708 "nd-flow-expr.cpp"
    break;

  case 200: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_FALSE  */
#line 1142 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == false
        ));
        _NDFP_debugf(
            "Detection updated == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3721 "nd-flow-expr.cpp"
    break;

  case 201: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_TRUE  */
#line 1150 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != true
        ));
        _NDFP_debugf(
            "Detection updated != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3734 "nd-flow-expr.cpp"
    break;

  case 202: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_FALSE  */
#line 1158 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != false
        ));
        _NDFP_debugf(
            "Detection updated != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3747 "nd-flow-expr.cpp"
    break;

  case 203: /* expr_dhc_hit: FLOW_DHC_HIT  */
#line 1169 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf("DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3756 "nd-flow-expr.cpp"
    break;

  case 204: /* expr_dhc_hit: '!' FLOW_DHC_HIT  */
#line 1173 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf(
            "DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3767 "nd-flow-expr.cpp"
    break;

  case 205: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1179 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == true
        ));
        _NDFP_debugf(
            "DHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3780 "nd-flow-expr.cpp"
    break;

  case 206: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1187 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == false
        ));
        _NDFP_debugf(
            "DHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3793 "nd-flow-expr.cpp"
    break;

  case 207: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1195 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != true
        ));
        _NDFP_debugf(
            "DHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3806 "nd-flow-expr.cpp"
    break;

  case 208: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1203 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != false
        ));
        _NDFP_debugf(
            "DHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3819 "nd-flow-expr.cpp"
    break;

  case 209: /* expr_fhc_hit: FLOW_FHC_HIT  */
#line 1214 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf("FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3828 "nd-flow-expr.cpp"
    break;

  case 210: /* expr_fhc_hit: '!' FLOW_FHC_HIT  */
#line 1218 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf(
            "FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3839 "nd-flow-expr.cpp"
    break;

  case 211: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 1224 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == true
        ));
        _NDFP_debugf(
            "FHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3852 "nd-flow-expr.cpp"
    break;

  case 212: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 1232 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == false
        ));
        _NDFP_debugf(
            "FHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3865 "nd-flow-expr.cpp"
    break;

  case 213: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 1240 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != true
        ));
        _NDFP_debugf(
            "FHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3878 "nd-flow-expr.cpp"
    break;

  case 214: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 1248 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != false
        ));
        _NDFP_debugf(
            "FHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3891 "nd-flow-expr.cpp"
    break;

  case 215: /* expr_ip_nat: FLOW_IP_NAT  */
#line 1259 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3900 "nd-flow-expr.cpp"
    break;

  case 216: /* expr_ip_nat: '!' FLOW_IP_NAT  */
#line 1263 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3909 "nd-flow-expr.cpp"
    break;

  case 217: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_TRUE  */
#line 1267 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3918 "nd-flow-expr.cpp"
    break;

  case 218: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_FALSE  */
#line 1271 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3927 "nd-flow-expr.cpp"
    break;

  case 219: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_TRUE  */
#line 1275 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != true));
        _NDFP_debugf("IP NAT != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3936 "nd-flow-expr.cpp"
    break;

  case 220: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_FALSE  */
#line 1279 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != false));
        _NDFP_debugf("IP NAT != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3945 "nd-flow-expr.cpp"
    break;

  case 221: /* expr_expiring: FLOW_EXPIRING  */
#line 1286 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3954 "nd-flow-expr.cpp"
    break;

  case 222: /* expr_expiring: '!' FLOW_EXPIRING  */
#line 1290 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3963 "nd-flow-expr.cpp"
    break;

  case 223: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_TRUE  */
#line 1294 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3972 "nd-flow-expr.cpp"
    break;

  case 224: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_FALSE  */
#line 1298 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3981 "nd-flow-expr.cpp"
    break;

  case 225: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_TRUE  */
#line 1302 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != true));
        _NDFP_debugf("Flow expiring != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3990 "nd-flow-expr.cpp"
    break;

  case 226: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_FALSE  */
#line 1306 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != false));
        _NDFP_debugf("Flow expiring != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3999 "nd-flow-expr.cpp"
    break;

  case 227: /* expr_expired: FLOW_EXPIRED  */
#line 1313 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4008 "nd-flow-expr.cpp"
    break;

  case 228: /* expr_expired: '!' FLOW_EXPIRED  */
#line 1317 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4017 "nd-flow-expr.cpp"
    break;

  case 229: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_TRUE  */
#line 1321 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4026 "nd-flow-expr.cpp"
    break;

  case 230: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_FALSE  */
#line 1325 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4035 "nd-flow-expr.cpp"
    break;

  case 231: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_TRUE  */
#line 1329 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != true));
        _NDFP_debugf("Flow expired != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4044 "nd-flow-expr.cpp"
    break;

  case 232: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_FALSE  */
#line 1333 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != false));
        _NDFP_debugf("Flow expired != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4053 "nd-flow-expr.cpp"
    break;

  case 233: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR  */
#line 1340 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4062 "nd-flow-expr.cpp"
    break;

  case 234: /* expr_soft_dissector: '!' FLOW_SOFT_DISSECTOR  */
#line 1344 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4071 "nd-flow-expr.cpp"
    break;

  case 235: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_TRUE  */
#line 1348 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4080 "nd-flow-expr.cpp"
    break;

  case 236: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_FALSE  */
#line 1352 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4089 "nd-flow-expr.cpp"
    break;

  case 237: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_TRUE  */
#line 1356 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != true));
        _NDFP_debugf("Soft dissector matched != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4098 "nd-flow-expr.cpp"
    break;

  case 238: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_FALSE  */
#line 1360 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != false));
        _NDFP_debugf("Soft dissector matched != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4107 "nd-flow-expr.cpp"
    break;

  case 239: /* expr_app: FLOW_APPLICATION  */
#line 1367 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) != 0
        ));
        _NDFP_debugf("Application detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4118 "nd-flow-expr.cpp"
    break;

  case 240: /* expr_app: '!' FLOW_APPLICATION  */
#line 1373 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) == 0
        ));
        _NDFP_debugf(
            "Application not detected? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4131 "nd-flow-expr.cpp"
    break;

  case 243: /* expr_app_id: FLOW_APPLICATION CMP_EQUAL VALUE_UNSIGNED  */
#line 1385 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = false);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf(
            "Application ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4145 "nd-flow-expr.cpp"
    break;

  case 244: /* expr_app_id: FLOW_APPLICATION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1394 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = true);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = false);

        _NDFP_debugf(
            "Application ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 4159 "nd-flow-expr.cpp"
    break;

  case 245: /* expr_app_name: FLOW_APPLICATION CMP_EQUAL VALUE_NAME  */
#line 1406 "nd-flow-expr.ypp"
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
#line 4191 "nd-flow-expr.cpp"
    break;

  case 246: /* expr_app_name: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NAME  */
#line 1433 "nd-flow-expr.ypp"
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
#line 4223 "nd-flow-expr.cpp"
    break;

  case 247: /* expr_category: FLOW_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1463 "nd-flow-expr.ypp"
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

        if (! _NDFP_result && _NDFP_flow->category.lower_net != ND_CAT_UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.lower_net
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.upper_net != ND_CAT_UNKNOWN) {
            _NDFP_result = (
                (yyval.bool_result) = (
                    _NDFP_categories.LookupTag(
                        ndCategories::Type::APP, category) == _NDFP_flow->category.upper_net
                )
            );
        }

        if (! _NDFP_result && _NDFP_flow->category.overlay != ND_CAT_UNKNOWN) {
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
#line 4285 "nd-flow-expr.cpp"
    break;

  case 248: /* expr_category: FLOW_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1520 "nd-flow-expr.ypp"
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
#line 4343 "nd-flow-expr.cpp"
    break;

  case 249: /* expr_category_id: FLOW_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1576 "nd-flow-expr.ypp"
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
#line 4390 "nd-flow-expr.cpp"
    break;

  case 250: /* expr_category_id: FLOW_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1618 "nd-flow-expr.ypp"
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
#line 4437 "nd-flow-expr.cpp"
    break;

  case 251: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1663 "nd-flow-expr.ypp"
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
#line 4459 "nd-flow-expr.cpp"
    break;

  case 252: /* expr_app_category: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_NAME  */
#line 1680 "nd-flow-expr.ypp"
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
#line 4481 "nd-flow-expr.cpp"
    break;

  case 253: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1700 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4493 "nd-flow-expr.cpp"
    break;

  case 254: /* expr_app_category_id: FLOW_APPLICATION_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1707 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.application) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("App category != %s? %s\n",
        (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4506 "nd-flow-expr.cpp"
    break;

  case 255: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1718 "nd-flow-expr.ypp"
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
#line 4528 "nd-flow-expr.cpp"
    break;

  case 256: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1735 "nd-flow-expr.ypp"
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
#line 4550 "nd-flow-expr.cpp"
    break;

  case 257: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1755 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID == %s? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4562 "nd-flow-expr.cpp"
    break;

  case 258: /* expr_domain_category_id: FLOW_DOMAIN_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1762 "nd-flow-expr.ypp"
                                                          {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.domain) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Domain category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4575 "nd-flow-expr.cpp"
    break;

  case 259: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1773 "nd-flow-expr.ypp"
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
#line 4606 "nd-flow-expr.cpp"
    break;

  case 260: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1799 "nd-flow-expr.ypp"
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
#line 4637 "nd-flow-expr.cpp"
    break;

  case 261: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1828 "nd-flow-expr.ypp"
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
#line 4656 "nd-flow-expr.cpp"
    break;

  case 262: /* expr_network_category_id: FLOW_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1842 "nd-flow-expr.ypp"
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
#line 4675 "nd-flow-expr.cpp"
    break;

  case 263: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1859 "nd-flow-expr.ypp"
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
#line 4697 "nd-flow-expr.cpp"
    break;

  case 264: /* expr_local_network_category: FLOW_LOCAL_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1876 "nd-flow-expr.ypp"
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
#line 4719 "nd-flow-expr.cpp"
    break;

  case 265: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1896 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4732 "nd-flow-expr.cpp"
    break;

  case 266: /* expr_local_network_category_id: FLOW_LOCAL_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1904 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_local_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (local) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4745 "nd-flow-expr.cpp"
    break;

  case 267: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1915 "nd-flow-expr.ypp"
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
#line 4767 "nd-flow-expr.cpp"
    break;

  case 268: /* expr_other_network_category: FLOW_OTHER_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1932 "nd-flow-expr.ypp"
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
#line 4789 "nd-flow-expr.cpp"
    break;

  case 269: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 1952 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4802 "nd-flow-expr.cpp"
    break;

  case 270: /* expr_other_network_category_id: FLOW_OTHER_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 1960 "nd-flow-expr.ypp"
                                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_other_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (other) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4815 "nd-flow-expr.cpp"
    break;

  case 271: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1971 "nd-flow-expr.ypp"
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
#line 4837 "nd-flow-expr.cpp"
    break;

  case 272: /* expr_src_network_category: FLOW_SRC_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1988 "nd-flow-expr.ypp"
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
#line 4859 "nd-flow-expr.cpp"
    break;

  case 273: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2008 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4872 "nd-flow-expr.cpp"
    break;

  case 274: /* expr_src_network_category_id: FLOW_SRC_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2016 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_src_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (src) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4885 "nd-flow-expr.cpp"
    break;

  case 275: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2027 "nd-flow-expr.ypp"
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
#line 4907 "nd-flow-expr.cpp"
    break;

  case 276: /* expr_dst_network_category: FLOW_DST_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2044 "nd-flow-expr.ypp"
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
#line 4929 "nd-flow-expr.cpp"
    break;

  case 277: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2064 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4942 "nd-flow-expr.cpp"
    break;

  case 278: /* expr_dst_network_category_id: FLOW_DST_NETWORK_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2072 "nd-flow-expr.ypp"
                                                               {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_dst_net_cat) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Network (dst) category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4955 "nd-flow-expr.cpp"
    break;

  case 279: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2083 "nd-flow-expr.ypp"
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
#line 4977 "nd-flow-expr.cpp"
    break;

  case 280: /* expr_tag_category: FLOW_TAG_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2100 "nd-flow-expr.ypp"
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
#line 4999 "nd-flow-expr.cpp"
    break;

  case 281: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2120 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5012 "nd-flow-expr.cpp"
    break;

  case 282: /* expr_tag_category_id: FLOW_TAG_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2128 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.overlay) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Overlay tag category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5025 "nd-flow-expr.cpp"
    break;

  case 283: /* expr_proto: FLOW_PROTOCOL  */
#line 2139 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol != ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5036 "nd-flow-expr.cpp"
    break;

  case 284: /* expr_proto: '!' FLOW_PROTOCOL  */
#line 2145 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol == ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5047 "nd-flow-expr.cpp"
    break;

  case 287: /* expr_proto_id: FLOW_PROTOCOL CMP_EQUAL VALUE_UNSIGNED  */
#line 2155 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5058 "nd-flow-expr.cpp"
    break;

  case 288: /* expr_proto_id: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2161 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5069 "nd-flow-expr.cpp"
    break;

  case 289: /* expr_proto_name: FLOW_PROTOCOL CMP_EQUAL VALUE_NAME  */
#line 2170 "nd-flow-expr.ypp"
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
#line 5094 "nd-flow-expr.cpp"
    break;

  case 290: /* expr_proto_name: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NAME  */
#line 2190 "nd-flow-expr.ypp"
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
#line 5118 "nd-flow-expr.cpp"
    break;

  case 291: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 2212 "nd-flow-expr.ypp"
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
#line 5140 "nd-flow-expr.cpp"
    break;

  case 292: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 2229 "nd-flow-expr.ypp"
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
#line 5162 "nd-flow-expr.cpp"
    break;

  case 293: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_EQUAL VALUE_UNSIGNED  */
#line 2249 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) == (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID == %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5175 "nd-flow-expr.cpp"
    break;

  case 294: /* expr_proto_category_id: FLOW_PROTOCOL_CATEGORY_ID CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2257 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->category.protocol) != (yyvsp[0].ul_number)
        ));

        _NDFP_debugf("Protocol category ID != %s? %s\n",
            (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5188 "nd-flow-expr.cpp"
    break;

  case 295: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME  */
#line 2268 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == false
        ));
        _NDFP_debugf("Detected hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5200 "nd-flow-expr.cpp"
    break;

  case 296: /* expr_detected_hostname: '!' FLOW_DETECTED_HOSTNAME  */
#line 2275 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == true
        ));
        _NDFP_debugf("Detected hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5212 "nd-flow-expr.cpp"
    break;

  case 297: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 2282 "nd-flow-expr.ypp"
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
#line 5236 "nd-flow-expr.cpp"
    break;

  case 298: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 2301 "nd-flow-expr.ypp"
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
#line 5260 "nd-flow-expr.cpp"
    break;

  case 299: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 2320 "nd-flow-expr.ypp"
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
#line 5279 "nd-flow-expr.cpp"
    break;

  case 300: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 2334 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Detected hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 5290 "nd-flow-expr.cpp"
    break;

  case 301: /* expr_dns_hostname: FLOW_DNS_HOSTNAME  */
#line 2343 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == false
        ));
        _NDFP_debugf("DNS hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5302 "nd-flow-expr.cpp"
    break;

  case 302: /* expr_dns_hostname: '!' FLOW_DNS_HOSTNAME  */
#line 2350 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == true
        ));
        _NDFP_debugf("DNS hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5314 "nd-flow-expr.cpp"
    break;

  case 303: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 2357 "nd-flow-expr.ypp"
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
#line 5338 "nd-flow-expr.cpp"
    break;

  case 304: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 2376 "nd-flow-expr.ypp"
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
#line 5362 "nd-flow-expr.cpp"
    break;

  case 305: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 2395 "nd-flow-expr.ypp"
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
#line 5381 "nd-flow-expr.cpp"
    break;

  case 306: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 2409 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("DNS hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 5392 "nd-flow-expr.cpp"
    break;

  case 307: /* expr_risks: FLOW_RISKS  */
#line 2418 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() != 0));
        _NDFP_debugf("Risks detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5401 "nd-flow-expr.cpp"
    break;

  case 308: /* expr_risks: '!' FLOW_RISKS  */
#line 2422 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() == 0));
        _NDFP_debugf("Risks not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5410 "nd-flow-expr.cpp"
    break;

  case 309: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NAME  */
#line 2426 "nd-flow-expr.ypp"
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
#line 5434 "nd-flow-expr.cpp"
    break;

  case 310: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NAME  */
#line 2445 "nd-flow-expr.ypp"
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
#line 5458 "nd-flow-expr.cpp"
    break;

  case 311: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_UNSIGNED  */
#line 2464 "nd-flow-expr.ypp"
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
#line 5476 "nd-flow-expr.cpp"
    break;

  case 312: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2477 "nd-flow-expr.ypp"
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
#line 5494 "nd-flow-expr.cpp"
    break;

  case 313: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE  */
#line 2493 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != 0));
        _NDFP_debugf("nDPI risk score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5503 "nd-flow-expr.cpp"
    break;

  case 314: /* expr_risk_ndpi_score: '!' FLOW_NDPI_RISK_SCORE  */
#line 2497 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == 0));
        _NDFP_debugf("nDPI risk score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5512 "nd-flow-expr.cpp"
    break;

  case 315: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_EQUAL VALUE_UNSIGNED  */
#line 2501 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5521 "nd-flow-expr.cpp"
    break;

  case 316: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2505 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5530 "nd-flow-expr.cpp"
    break;

  case 317: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2509 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5539 "nd-flow-expr.cpp"
    break;

  case 318: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2513 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5548 "nd-flow-expr.cpp"
    break;

  case 319: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '>' VALUE_UNSIGNED  */
#line 2517 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5557 "nd-flow-expr.cpp"
    break;

  case 320: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '<' VALUE_UNSIGNED  */
#line 2521 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5566 "nd-flow-expr.cpp"
    break;

  case 321: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 2528 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != 0));
        _NDFP_debugf("nDPI risk client score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5575 "nd-flow-expr.cpp"
    break;

  case 322: /* expr_risk_ndpi_score_client: '!' FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 2532 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == 0));
        _NDFP_debugf("nDPI risk client score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5584 "nd-flow-expr.cpp"
    break;

  case 323: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_EQUAL VALUE_UNSIGNED  */
#line 2536 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5593 "nd-flow-expr.cpp"
    break;

  case 324: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2540 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5602 "nd-flow-expr.cpp"
    break;

  case 325: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2544 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5611 "nd-flow-expr.cpp"
    break;

  case 326: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2548 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5620 "nd-flow-expr.cpp"
    break;

  case 327: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '>' VALUE_UNSIGNED  */
#line 2552 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5629 "nd-flow-expr.cpp"
    break;

  case 328: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '<' VALUE_UNSIGNED  */
#line 2556 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5638 "nd-flow-expr.cpp"
    break;

  case 329: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER  */
#line 2563 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != 0));
        _NDFP_debugf("nDPI risk server score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5647 "nd-flow-expr.cpp"
    break;

  case 330: /* expr_risk_ndpi_score_server: '!' FLOW_NDPI_RISK_SCORE_SERVER  */
#line 2567 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == 0));
        _NDFP_debugf("nDPI risk server score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 5656 "nd-flow-expr.cpp"
    break;

  case 331: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_EQUAL VALUE_UNSIGNED  */
#line 2571 "nd-flow-expr.ypp"
                                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5665 "nd-flow-expr.cpp"
    break;

  case 332: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2575 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5674 "nd-flow-expr.cpp"
    break;

  case 333: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2579 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5683 "nd-flow-expr.cpp"
    break;

  case 334: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2583 "nd-flow-expr.ypp"
                                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5692 "nd-flow-expr.cpp"
    break;

  case 335: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '>' VALUE_UNSIGNED  */
#line 2587 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5701 "nd-flow-expr.cpp"
    break;

  case 336: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '<' VALUE_UNSIGNED  */
#line 2591 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 5710 "nd-flow-expr.cpp"
    break;

  case 337: /* expr_fwmark: FLOW_CT_MARK  */
#line 2598 "nd-flow-expr.ypp"
                   {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != 0));
        _NDFP_debugf("FWMARK set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5723 "nd-flow-expr.cpp"
    break;

  case 338: /* expr_fwmark: '!' FLOW_CT_MARK  */
#line 2606 "nd-flow-expr.ypp"
                       {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == 0));
        _NDFP_debugf("FWMARK not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5736 "nd-flow-expr.cpp"
    break;

  case 339: /* expr_fwmark: FLOW_CT_MARK CMP_EQUAL VALUE_UNSIGNED  */
#line 2614 "nd-flow-expr.ypp"
                                            {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5749 "nd-flow-expr.cpp"
    break;

  case 340: /* expr_fwmark: FLOW_CT_MARK CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 2622 "nd-flow-expr.ypp"
                                               {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5762 "nd-flow-expr.cpp"
    break;

  case 341: /* expr_fwmark: FLOW_CT_MARK CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 2630 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark >= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5775 "nd-flow-expr.cpp"
    break;

  case 342: /* expr_fwmark: FLOW_CT_MARK CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 2638 "nd-flow-expr.ypp"
                                                 {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark <= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5788 "nd-flow-expr.cpp"
    break;

  case 343: /* expr_fwmark: FLOW_CT_MARK '>' VALUE_UNSIGNED  */
#line 2646 "nd-flow-expr.ypp"
                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark > (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5801 "nd-flow-expr.cpp"
    break;

  case 344: /* expr_fwmark: FLOW_CT_MARK '<' VALUE_UNSIGNED  */
#line 2654 "nd-flow-expr.ypp"
                                      {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark < (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 5814 "nd-flow-expr.cpp"
    break;

  case 345: /* expr_iface: FLOW_IFACE  */
#line 2665 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == false
        ));
        _NDFP_debugf("Interface detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5826 "nd-flow-expr.cpp"
    break;

  case 346: /* expr_iface: '!' FLOW_IFACE  */
#line 2672 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->iface->ifname.empty() == true
        ));
        _NDFP_debugf("Interface not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 5838 "nd-flow-expr.cpp"
    break;

  case 347: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_NAME  */
#line 2679 "nd-flow-expr.ypp"
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
#line 5861 "nd-flow-expr.cpp"
    break;

  case 348: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_NAME  */
#line 2697 "nd-flow-expr.ypp"
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
#line 5884 "nd-flow-expr.cpp"
    break;

  case 349: /* expr_iface: FLOW_IFACE CMP_EQUAL VALUE_REGEX  */
#line 2715 "nd-flow-expr.ypp"
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
#line 5903 "nd-flow-expr.cpp"
    break;

  case 350: /* expr_iface: FLOW_IFACE CMP_NOTEQUAL VALUE_REGEX  */
#line 2729 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Interface (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 5914 "nd-flow-expr.cpp"
    break;

  case 351: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC  */
#line 2738 "nd-flow-expr.ypp"
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
#line 5930 "nd-flow-expr.cpp"
    break;

  case 352: /* expr_iface_nfq_src: '!' FLOW_IFACE_NFQ_SRC  */
#line 2749 "nd-flow-expr.ypp"
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
#line 5946 "nd-flow-expr.cpp"
    break;

  case 353: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_NAME  */
#line 2760 "nd-flow-expr.ypp"
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
#line 5972 "nd-flow-expr.cpp"
    break;

  case 354: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_NAME  */
#line 2781 "nd-flow-expr.ypp"
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
#line 5996 "nd-flow-expr.cpp"
    break;

  case 355: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_EQUAL VALUE_REGEX  */
#line 2800 "nd-flow-expr.ypp"
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
#line 6015 "nd-flow-expr.cpp"
    break;

  case 356: /* expr_iface_nfq_src: FLOW_IFACE_NFQ_SRC CMP_NOTEQUAL VALUE_REGEX  */
#line 2814 "nd-flow-expr.ypp"
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
#line 6034 "nd-flow-expr.cpp"
    break;

  case 357: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST  */
#line 2831 "nd-flow-expr.ypp"
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
#line 6050 "nd-flow-expr.cpp"
    break;

  case 358: /* expr_iface_nfq_dst: '!' FLOW_IFACE_NFQ_DST  */
#line 2842 "nd-flow-expr.ypp"
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
#line 6066 "nd-flow-expr.cpp"
    break;

  case 359: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_NAME  */
#line 2853 "nd-flow-expr.ypp"
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
#line 6090 "nd-flow-expr.cpp"
    break;

  case 360: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_NAME  */
#line 2872 "nd-flow-expr.ypp"
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
#line 6114 "nd-flow-expr.cpp"
    break;

  case 361: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_EQUAL VALUE_REGEX  */
#line 2891 "nd-flow-expr.ypp"
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
#line 6134 "nd-flow-expr.cpp"
    break;

  case 362: /* expr_iface_nfq_dst: FLOW_IFACE_NFQ_DST CMP_NOTEQUAL VALUE_REGEX  */
#line 2906 "nd-flow-expr.ypp"
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
#line 6153 "nd-flow-expr.cpp"
    break;

  case 363: /* expr_intel: FLOW_INTEL  */
#line 2923 "nd-flow-expr.ypp"
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
#line 6179 "nd-flow-expr.cpp"
    break;

  case 364: /* expr_intel: '!' FLOW_INTEL  */
#line 2944 "nd-flow-expr.ypp"
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
#line 6204 "nd-flow-expr.cpp"
    break;

  case 365: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_NAME  */
#line 2966 "nd-flow-expr.ypp"
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
#line 6229 "nd-flow-expr.cpp"
    break;

  case 366: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_REGEX  */
#line 2986 "nd-flow-expr.ypp"
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
#line 6249 "nd-flow-expr.cpp"
    break;

  case 367: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_UNSIGNED  */
#line 3001 "nd-flow-expr.ypp"
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
#line 6266 "nd-flow-expr.cpp"
    break;

  case 368: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_SIGNED  */
#line 3013 "nd-flow-expr.ypp"
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
#line 6283 "nd-flow-expr.cpp"
    break;

  case 369: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FLOAT  */
#line 3025 "nd-flow-expr.ypp"
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
#line 6300 "nd-flow-expr.cpp"
    break;

  case 370: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_TRUE  */
#line 3037 "nd-flow-expr.ypp"
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
#line 6318 "nd-flow-expr.cpp"
    break;

  case 371: /* expr_intel: FLOW_INTEL CMP_EQUAL VALUE_FALSE  */
#line 3050 "nd-flow-expr.ypp"
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
#line 6336 "nd-flow-expr.cpp"
    break;

  case 372: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_NAME  */
#line 3065 "nd-flow-expr.ypp"
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
#line 6361 "nd-flow-expr.cpp"
    break;

  case 373: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_REGEX  */
#line 3085 "nd-flow-expr.ypp"
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
#line 6381 "nd-flow-expr.cpp"
    break;

  case 374: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3100 "nd-flow-expr.ypp"
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
#line 6398 "nd-flow-expr.cpp"
    break;

  case 375: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_SIGNED  */
#line 3112 "nd-flow-expr.ypp"
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
#line 6415 "nd-flow-expr.cpp"
    break;

  case 376: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FLOAT  */
#line 3124 "nd-flow-expr.ypp"
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
#line 6432 "nd-flow-expr.cpp"
    break;

  case 377: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_TRUE  */
#line 3136 "nd-flow-expr.ypp"
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
#line 6450 "nd-flow-expr.cpp"
    break;

  case 378: /* expr_intel: FLOW_INTEL CMP_NOTEQUAL VALUE_FALSE  */
#line 3149 "nd-flow-expr.ypp"
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
#line 6468 "nd-flow-expr.cpp"
    break;

  case 379: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3164 "nd-flow-expr.ypp"
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
#line 6485 "nd-flow-expr.cpp"
    break;

  case 380: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_SIGNED  */
#line 3176 "nd-flow-expr.ypp"
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
#line 6502 "nd-flow-expr.cpp"
    break;

  case 381: /* expr_intel: FLOW_INTEL CMP_GTHANEQUAL VALUE_FLOAT  */
#line 3188 "nd-flow-expr.ypp"
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
#line 6519 "nd-flow-expr.cpp"
    break;

  case 382: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3202 "nd-flow-expr.ypp"
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
#line 6536 "nd-flow-expr.cpp"
    break;

  case 383: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_SIGNED  */
#line 3214 "nd-flow-expr.ypp"
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
#line 6553 "nd-flow-expr.cpp"
    break;

  case 384: /* expr_intel: FLOW_INTEL CMP_LTHANEQUAL VALUE_FLOAT  */
#line 3226 "nd-flow-expr.ypp"
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
#line 6570 "nd-flow-expr.cpp"
    break;

  case 385: /* expr_intel: FLOW_INTEL '>' VALUE_UNSIGNED  */
#line 3240 "nd-flow-expr.ypp"
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
#line 6587 "nd-flow-expr.cpp"
    break;

  case 386: /* expr_intel: FLOW_INTEL '>' VALUE_SIGNED  */
#line 3252 "nd-flow-expr.ypp"
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
#line 6604 "nd-flow-expr.cpp"
    break;

  case 387: /* expr_intel: FLOW_INTEL '>' VALUE_FLOAT  */
#line 3264 "nd-flow-expr.ypp"
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
#line 6621 "nd-flow-expr.cpp"
    break;

  case 388: /* expr_intel: FLOW_INTEL '<' VALUE_UNSIGNED  */
#line 3278 "nd-flow-expr.ypp"
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
#line 6638 "nd-flow-expr.cpp"
    break;

  case 389: /* expr_intel: FLOW_INTEL '<' VALUE_SIGNED  */
#line 3290 "nd-flow-expr.ypp"
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
#line 6655 "nd-flow-expr.cpp"
    break;

  case 390: /* expr_intel: FLOW_INTEL '<' VALUE_FLOAT  */
#line 3302 "nd-flow-expr.ypp"
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
#line 6672 "nd-flow-expr.cpp"
    break;

  case 391: /* expr_tls_version: FLOW_TLS_VERSION  */
#line 3317 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != 0));
        _NDFP_debugf("TLS version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6681 "nd-flow-expr.cpp"
    break;

  case 392: /* expr_tls_version: '!' FLOW_TLS_VERSION  */
#line 3321 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == 0));
        _NDFP_debugf("TLS version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6690 "nd-flow-expr.cpp"
    break;

  case 393: /* expr_tls_version: FLOW_TLS_VERSION CMP_EQUAL VALUE_UNSIGNED  */
#line 3325 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6699 "nd-flow-expr.cpp"
    break;

  case 394: /* expr_tls_version: FLOW_TLS_VERSION CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3329 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6708 "nd-flow-expr.cpp"
    break;

  case 395: /* expr_tls_version: FLOW_TLS_VERSION CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3333 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6717 "nd-flow-expr.cpp"
    break;

  case 396: /* expr_tls_version: FLOW_TLS_VERSION CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3337 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6726 "nd-flow-expr.cpp"
    break;

  case 397: /* expr_tls_version: FLOW_TLS_VERSION '>' VALUE_UNSIGNED  */
#line 3341 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6735 "nd-flow-expr.cpp"
    break;

  case 398: /* expr_tls_version: FLOW_TLS_VERSION '<' VALUE_UNSIGNED  */
#line 3345 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6744 "nd-flow-expr.cpp"
    break;

  case 399: /* expr_tls_cipher: FLOW_TLS_CIPHER  */
#line 3352 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != 0));
        _NDFP_debugf("TLS cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6753 "nd-flow-expr.cpp"
    break;

  case 400: /* expr_tls_cipher: '!' FLOW_TLS_CIPHER  */
#line 3356 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == 0));
        _NDFP_debugf("TLS cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6762 "nd-flow-expr.cpp"
    break;

  case 401: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_EQUAL VALUE_UNSIGNED  */
#line 3360 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6771 "nd-flow-expr.cpp"
    break;

  case 402: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3364 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6780 "nd-flow-expr.cpp"
    break;

  case 403: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3368 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6789 "nd-flow-expr.cpp"
    break;

  case 404: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3372 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6798 "nd-flow-expr.cpp"
    break;

  case 405: /* expr_tls_cipher: FLOW_TLS_CIPHER '>' VALUE_UNSIGNED  */
#line 3376 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6807 "nd-flow-expr.cpp"
    break;

  case 406: /* expr_tls_cipher: FLOW_TLS_CIPHER '<' VALUE_UNSIGNED  */
#line 3380 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6816 "nd-flow-expr.cpp"
    break;

  case 407: /* expr_tls_ech: FLOW_TLS_ECH  */
#line 3387 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != 0));
        _NDFP_debugf("TLS ECH version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6825 "nd-flow-expr.cpp"
    break;

  case 408: /* expr_tls_ech: '!' FLOW_TLS_ECH  */
#line 3391 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == 0));
        _NDFP_debugf("TLS ECH version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 6834 "nd-flow-expr.cpp"
    break;

  case 409: /* expr_tls_ech: FLOW_TLS_ECH CMP_EQUAL VALUE_UNSIGNED  */
#line 3395 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6843 "nd-flow-expr.cpp"
    break;

  case 410: /* expr_tls_ech: FLOW_TLS_ECH CMP_NOTEQUAL VALUE_UNSIGNED  */
#line 3399 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6852 "nd-flow-expr.cpp"
    break;

  case 411: /* expr_tls_ech: FLOW_TLS_ECH CMP_GTHANEQUAL VALUE_UNSIGNED  */
#line 3403 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6861 "nd-flow-expr.cpp"
    break;

  case 412: /* expr_tls_ech: FLOW_TLS_ECH CMP_LTHANEQUAL VALUE_UNSIGNED  */
#line 3407 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6870 "nd-flow-expr.cpp"
    break;

  case 413: /* expr_tls_ech: FLOW_TLS_ECH '>' VALUE_UNSIGNED  */
#line 3411 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6879 "nd-flow-expr.cpp"
    break;

  case 414: /* expr_tls_ech: FLOW_TLS_ECH '<' VALUE_UNSIGNED  */
#line 3415 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 6888 "nd-flow-expr.cpp"
    break;

  case 415: /* expr_tls_ja4: FLOW_TLS_JA4  */
#line 3422 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6900 "nd-flow-expr.cpp"
    break;

  case 416: /* expr_tls_ja4: '!' FLOW_TLS_JA4  */
#line 3429 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 6912 "nd-flow-expr.cpp"
    break;

  case 417: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_NAME  */
#line 3436 "nd-flow-expr.ypp"
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
#line 6936 "nd-flow-expr.cpp"
    break;

  case 418: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_NAME  */
#line 3455 "nd-flow-expr.ypp"
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
#line 6960 "nd-flow-expr.cpp"
    break;

  case 419: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_REGEX  */
#line 3474 "nd-flow-expr.ypp"
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
#line 6979 "nd-flow-expr.cpp"
    break;

  case 420: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_REGEX  */
#line 3488 "nd-flow-expr.ypp"
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
#line 6998 "nd-flow-expr.cpp"
    break;

  case 421: /* expr_origin: FLOW_ORIGIN  */
#line 3505 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7007 "nd-flow-expr.cpp"
    break;

  case 422: /* expr_origin: '!' FLOW_ORIGIN  */
#line 3509 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 7016 "nd-flow-expr.cpp"
    break;

  case 423: /* expr_origin: FLOW_ORIGIN CMP_EQUAL value_origin_type  */
#line 3513 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7025 "nd-flow-expr.cpp"
    break;

  case 424: /* expr_origin: FLOW_ORIGIN CMP_NOTEQUAL value_origin_type  */
#line 3517 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 7034 "nd-flow-expr.cpp"
    break;

  case 425: /* value_origin_type: FLOW_ORIGIN_LOCAL  */
#line 3524 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 7040 "nd-flow-expr.cpp"
    break;

  case 426: /* value_origin_type: FLOW_ORIGIN_OTHER  */
#line 3525 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 7046 "nd-flow-expr.cpp"
    break;

  case 427: /* value_origin_type: FLOW_ORIGIN_UNKNOWN  */
#line 3526 "nd-flow-expr.ypp"
                          { (yyval.us_number) = (yyvsp[0].us_number); }
#line 7052 "nd-flow-expr.cpp"
    break;

  case 428: /* expr_tag: FLOW_TAG  */
#line 3530 "nd-flow-expr.ypp"
               {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags not empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7064 "nd-flow-expr.cpp"
    break;

  case 429: /* expr_tag: '!' FLOW_TAG  */
#line 3537 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tags.empty()
        ));
        _NDFP_debugf("Tags empty? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 7076 "nd-flow-expr.cpp"
    break;

  case 430: /* expr_tag: FLOW_TAG CMP_EQUAL VALUE_NAME  */
#line 3544 "nd-flow-expr.ypp"
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
#line 7109 "nd-flow-expr.cpp"
    break;

  case 431: /* expr_tag: FLOW_TAG CMP_NOTEQUAL VALUE_NAME  */
#line 3572 "nd-flow-expr.ypp"
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
#line 7142 "nd-flow-expr.cpp"
    break;


#line 7146 "nd-flow-expr.cpp"

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

#line 3602 "nd-flow-expr.ypp"


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
