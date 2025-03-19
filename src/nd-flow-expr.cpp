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


#line 162 "nd-flow-expr.cpp"




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
#line 99 "nd-flow-expr.ypp"

typedef void* yyscan_t;

#line 204 "nd-flow-expr.cpp"

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
    FLOW_CATEGORY = 280,           /* FLOW_CATEGORY  */
    FLOW_RISKS = 281,              /* FLOW_RISKS  */
    FLOW_NDPI_RISK_SCORE = 282,    /* FLOW_NDPI_RISK_SCORE  */
    FLOW_NDPI_RISK_SCORE_CLIENT = 283, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
    FLOW_NDPI_RISK_SCORE_SERVER = 284, /* FLOW_NDPI_RISK_SCORE_SERVER  */
    FLOW_DOMAIN_CATEGORY = 285,    /* FLOW_DOMAIN_CATEGORY  */
    FLOW_NETWORK_CATEGORY = 286,   /* FLOW_NETWORK_CATEGORY  */
    FLOW_APPLICATION = 287,        /* FLOW_APPLICATION  */
    FLOW_APPLICATION_CATEGORY = 288, /* FLOW_APPLICATION_CATEGORY  */
    FLOW_PROTOCOL = 289,           /* FLOW_PROTOCOL  */
    FLOW_PROTOCOL_CATEGORY = 290,  /* FLOW_PROTOCOL_CATEGORY  */
    FLOW_DETECTED_HOSTNAME = 291,  /* FLOW_DETECTED_HOSTNAME  */
    FLOW_DNS_HOSTNAME = 292,       /* FLOW_DNS_HOSTNAME  */
    FLOW_ORIGIN = 293,             /* FLOW_ORIGIN  */
    FLOW_CT_MARK = 294,            /* FLOW_CT_MARK  */
    FLOW_TLS_VERSION = 295,        /* FLOW_TLS_VERSION  */
    FLOW_TLS_CIPHER = 296,         /* FLOW_TLS_CIPHER  */
    FLOW_TLS_ECH = 297,            /* FLOW_TLS_ECH  */
    FLOW_TLS_ESNI = 298,           /* FLOW_TLS_ESNI  */
    FLOW_TLS_JA4 = 299,            /* FLOW_TLS_JA4  */
    FLOW_OTHER_UNKNOWN = 300,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 301,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 302,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 303,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 304,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 305,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 306,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 307,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 308,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 309,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 310,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 311,         /* FLOW_TUNNEL_GTP  */
    CMP_EQUAL = 312,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 313,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 314,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 315,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 316,                /* BOOL_AND  */
    BOOL_OR = 317,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 318,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 319,              /* VALUE_TRUE  */
    VALUE_FALSE = 320,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 321,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 322,              /* VALUE_NAME  */
    VALUE_REGEX = 323,             /* VALUE_REGEX  */
    VALUE_ADDR_IPV4 = 324,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 325,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 326,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 327,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_NUMBER = 328             /* VALUE_NUMBER  */
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
#define FLOW_CATEGORY 280
#define FLOW_RISKS 281
#define FLOW_NDPI_RISK_SCORE 282
#define FLOW_NDPI_RISK_SCORE_CLIENT 283
#define FLOW_NDPI_RISK_SCORE_SERVER 284
#define FLOW_DOMAIN_CATEGORY 285
#define FLOW_NETWORK_CATEGORY 286
#define FLOW_APPLICATION 287
#define FLOW_APPLICATION_CATEGORY 288
#define FLOW_PROTOCOL 289
#define FLOW_PROTOCOL_CATEGORY 290
#define FLOW_DETECTED_HOSTNAME 291
#define FLOW_DNS_HOSTNAME 292
#define FLOW_ORIGIN 293
#define FLOW_CT_MARK 294
#define FLOW_TLS_VERSION 295
#define FLOW_TLS_CIPHER 296
#define FLOW_TLS_ECH 297
#define FLOW_TLS_ESNI 298
#define FLOW_TLS_JA4 299
#define FLOW_OTHER_UNKNOWN 300
#define FLOW_OTHER_UNSUPPORTED 301
#define FLOW_OTHER_LOCAL 302
#define FLOW_OTHER_MULTICAST 303
#define FLOW_OTHER_BROADCAST 304
#define FLOW_OTHER_REMOTE 305
#define FLOW_OTHER_ERROR 306
#define FLOW_ORIGIN_LOCAL 307
#define FLOW_ORIGIN_OTHER 308
#define FLOW_ORIGIN_UNKNOWN 309
#define FLOW_TUNNEL_NONE 310
#define FLOW_TUNNEL_GTP 311
#define CMP_EQUAL 312
#define CMP_NOTEQUAL 313
#define CMP_GTHANEQUAL 314
#define CMP_LTHANEQUAL 315
#define BOOL_AND 316
#define BOOL_OR 317
#define VALUE_ADDR_IPMASK 318
#define VALUE_TRUE 319
#define VALUE_FALSE 320
#define VALUE_ADDR_MAC 321
#define VALUE_NAME 322
#define VALUE_REGEX 323
#define VALUE_ADDR_IPV4 324
#define VALUE_ADDR_IPV4_CIDR 325
#define VALUE_ADDR_IPV6 326
#define VALUE_ADDR_IPV6_CIDR 327
#define VALUE_NUMBER 328

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 106 "nd-flow-expr.ypp"

    char buffer[_NDFP_MAX_BUFLEN];

    bool bool_number;
    unsigned short us_number;
    unsigned long ul_number;

    bool bool_result;

#line 380 "nd-flow-expr.cpp"

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
  YYSYMBOL_FLOW_CATEGORY = 25,             /* FLOW_CATEGORY  */
  YYSYMBOL_FLOW_RISKS = 26,                /* FLOW_RISKS  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE = 27,      /* FLOW_NDPI_RISK_SCORE  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_CLIENT = 28, /* FLOW_NDPI_RISK_SCORE_CLIENT  */
  YYSYMBOL_FLOW_NDPI_RISK_SCORE_SERVER = 29, /* FLOW_NDPI_RISK_SCORE_SERVER  */
  YYSYMBOL_FLOW_DOMAIN_CATEGORY = 30,      /* FLOW_DOMAIN_CATEGORY  */
  YYSYMBOL_FLOW_NETWORK_CATEGORY = 31,     /* FLOW_NETWORK_CATEGORY  */
  YYSYMBOL_FLOW_APPLICATION = 32,          /* FLOW_APPLICATION  */
  YYSYMBOL_FLOW_APPLICATION_CATEGORY = 33, /* FLOW_APPLICATION_CATEGORY  */
  YYSYMBOL_FLOW_PROTOCOL = 34,             /* FLOW_PROTOCOL  */
  YYSYMBOL_FLOW_PROTOCOL_CATEGORY = 35,    /* FLOW_PROTOCOL_CATEGORY  */
  YYSYMBOL_FLOW_DETECTED_HOSTNAME = 36,    /* FLOW_DETECTED_HOSTNAME  */
  YYSYMBOL_FLOW_DNS_HOSTNAME = 37,         /* FLOW_DNS_HOSTNAME  */
  YYSYMBOL_FLOW_ORIGIN = 38,               /* FLOW_ORIGIN  */
  YYSYMBOL_FLOW_CT_MARK = 39,              /* FLOW_CT_MARK  */
  YYSYMBOL_FLOW_TLS_VERSION = 40,          /* FLOW_TLS_VERSION  */
  YYSYMBOL_FLOW_TLS_CIPHER = 41,           /* FLOW_TLS_CIPHER  */
  YYSYMBOL_FLOW_TLS_ECH = 42,              /* FLOW_TLS_ECH  */
  YYSYMBOL_FLOW_TLS_ESNI = 43,             /* FLOW_TLS_ESNI  */
  YYSYMBOL_FLOW_TLS_JA4 = 44,              /* FLOW_TLS_JA4  */
  YYSYMBOL_FLOW_OTHER_UNKNOWN = 45,        /* FLOW_OTHER_UNKNOWN  */
  YYSYMBOL_FLOW_OTHER_UNSUPPORTED = 46,    /* FLOW_OTHER_UNSUPPORTED  */
  YYSYMBOL_FLOW_OTHER_LOCAL = 47,          /* FLOW_OTHER_LOCAL  */
  YYSYMBOL_FLOW_OTHER_MULTICAST = 48,      /* FLOW_OTHER_MULTICAST  */
  YYSYMBOL_FLOW_OTHER_BROADCAST = 49,      /* FLOW_OTHER_BROADCAST  */
  YYSYMBOL_FLOW_OTHER_REMOTE = 50,         /* FLOW_OTHER_REMOTE  */
  YYSYMBOL_FLOW_OTHER_ERROR = 51,          /* FLOW_OTHER_ERROR  */
  YYSYMBOL_FLOW_ORIGIN_LOCAL = 52,         /* FLOW_ORIGIN_LOCAL  */
  YYSYMBOL_FLOW_ORIGIN_OTHER = 53,         /* FLOW_ORIGIN_OTHER  */
  YYSYMBOL_FLOW_ORIGIN_UNKNOWN = 54,       /* FLOW_ORIGIN_UNKNOWN  */
  YYSYMBOL_FLOW_TUNNEL_NONE = 55,          /* FLOW_TUNNEL_NONE  */
  YYSYMBOL_FLOW_TUNNEL_GTP = 56,           /* FLOW_TUNNEL_GTP  */
  YYSYMBOL_CMP_EQUAL = 57,                 /* CMP_EQUAL  */
  YYSYMBOL_CMP_NOTEQUAL = 58,              /* CMP_NOTEQUAL  */
  YYSYMBOL_CMP_GTHANEQUAL = 59,            /* CMP_GTHANEQUAL  */
  YYSYMBOL_CMP_LTHANEQUAL = 60,            /* CMP_LTHANEQUAL  */
  YYSYMBOL_BOOL_AND = 61,                  /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 62,                   /* BOOL_OR  */
  YYSYMBOL_VALUE_ADDR_IPMASK = 63,         /* VALUE_ADDR_IPMASK  */
  YYSYMBOL_VALUE_TRUE = 64,                /* VALUE_TRUE  */
  YYSYMBOL_VALUE_FALSE = 65,               /* VALUE_FALSE  */
  YYSYMBOL_VALUE_ADDR_MAC = 66,            /* VALUE_ADDR_MAC  */
  YYSYMBOL_VALUE_NAME = 67,                /* VALUE_NAME  */
  YYSYMBOL_VALUE_REGEX = 68,               /* VALUE_REGEX  */
  YYSYMBOL_VALUE_ADDR_IPV4 = 69,           /* VALUE_ADDR_IPV4  */
  YYSYMBOL_VALUE_ADDR_IPV4_CIDR = 70,      /* VALUE_ADDR_IPV4_CIDR  */
  YYSYMBOL_VALUE_ADDR_IPV6 = 71,           /* VALUE_ADDR_IPV6  */
  YYSYMBOL_VALUE_ADDR_IPV6_CIDR = 72,      /* VALUE_ADDR_IPV6_CIDR  */
  YYSYMBOL_VALUE_NUMBER = 73,              /* VALUE_NUMBER  */
  YYSYMBOL_74_ = 74,                       /* ';'  */
  YYSYMBOL_75_ = 75,                       /* '('  */
  YYSYMBOL_76_ = 76,                       /* ')'  */
  YYSYMBOL_77_ = 77,                       /* '!'  */
  YYSYMBOL_78_ = 78,                       /* '>'  */
  YYSYMBOL_79_ = 79,                       /* '<'  */
  YYSYMBOL_YYACCEPT = 80,                  /* $accept  */
  YYSYMBOL_exprs = 81,                     /* exprs  */
  YYSYMBOL_expr = 82,                      /* expr  */
  YYSYMBOL_expr_ip_proto = 83,             /* expr_ip_proto  */
  YYSYMBOL_expr_ip_dscp = 84,              /* expr_ip_dscp  */
  YYSYMBOL_expr_ip_version = 85,           /* expr_ip_version  */
  YYSYMBOL_expr_vlan_id = 86,              /* expr_vlan_id  */
  YYSYMBOL_expr_other_type = 87,           /* expr_other_type  */
  YYSYMBOL_value_other_type = 88,          /* value_other_type  */
  YYSYMBOL_expr_local_mac = 89,            /* expr_local_mac  */
  YYSYMBOL_expr_other_mac = 90,            /* expr_other_mac  */
  YYSYMBOL_expr_local_ip = 91,             /* expr_local_ip  */
  YYSYMBOL_expr_other_ip = 92,             /* expr_other_ip  */
  YYSYMBOL_value_addr_ip = 93,             /* value_addr_ip  */
  YYSYMBOL_expr_local_port = 94,           /* expr_local_port  */
  YYSYMBOL_expr_other_port = 95,           /* expr_other_port  */
  YYSYMBOL_expr_tunnel_type = 96,          /* expr_tunnel_type  */
  YYSYMBOL_value_tunnel_type = 97,         /* value_tunnel_type  */
  YYSYMBOL_expr_detection_complete = 98,   /* expr_detection_complete  */
  YYSYMBOL_expr_detection_guessed = 99,    /* expr_detection_guessed  */
  YYSYMBOL_expr_detection_init = 100,      /* expr_detection_init  */
  YYSYMBOL_expr_detection_updated = 101,   /* expr_detection_updated  */
  YYSYMBOL_expr_dhc_hit = 102,             /* expr_dhc_hit  */
  YYSYMBOL_expr_fhc_hit = 103,             /* expr_fhc_hit  */
  YYSYMBOL_expr_ip_nat = 104,              /* expr_ip_nat  */
  YYSYMBOL_expr_expiring = 105,            /* expr_expiring  */
  YYSYMBOL_expr_expired = 106,             /* expr_expired  */
  YYSYMBOL_expr_soft_dissector = 107,      /* expr_soft_dissector  */
  YYSYMBOL_expr_app = 108,                 /* expr_app  */
  YYSYMBOL_expr_app_id = 109,              /* expr_app_id  */
  YYSYMBOL_expr_app_name = 110,            /* expr_app_name  */
  YYSYMBOL_expr_category = 111,            /* expr_category  */
  YYSYMBOL_expr_risks = 112,               /* expr_risks  */
  YYSYMBOL_expr_risk_ndpi_score = 113,     /* expr_risk_ndpi_score  */
  YYSYMBOL_expr_risk_ndpi_score_client = 114, /* expr_risk_ndpi_score_client  */
  YYSYMBOL_expr_risk_ndpi_score_server = 115, /* expr_risk_ndpi_score_server  */
  YYSYMBOL_expr_app_category = 116,        /* expr_app_category  */
  YYSYMBOL_expr_domain_category = 117,     /* expr_domain_category  */
  YYSYMBOL_expr_network_category = 118,    /* expr_network_category  */
  YYSYMBOL_expr_proto = 119,               /* expr_proto  */
  YYSYMBOL_expr_proto_id = 120,            /* expr_proto_id  */
  YYSYMBOL_expr_proto_name = 121,          /* expr_proto_name  */
  YYSYMBOL_expr_proto_category = 122,      /* expr_proto_category  */
  YYSYMBOL_expr_detected_hostname = 123,   /* expr_detected_hostname  */
  YYSYMBOL_expr_dns_hostname = 124,        /* expr_dns_hostname  */
  YYSYMBOL_expr_fwmark = 125,              /* expr_fwmark  */
  YYSYMBOL_expr_tls_version = 126,         /* expr_tls_version  */
  YYSYMBOL_expr_tls_cipher = 127,          /* expr_tls_cipher  */
  YYSYMBOL_expr_tls_ech = 128,             /* expr_tls_ech  */
  YYSYMBOL_expr_tls_esni = 129,            /* expr_tls_esni  */
  YYSYMBOL_expr_tls_ja4 = 130,             /* expr_tls_ja4  */
  YYSYMBOL_expr_origin = 131,              /* expr_origin  */
  YYSYMBOL_value_origin_type = 132         /* value_origin_type  */
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
#define YYLAST   446

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  80
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  294
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  444

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   328


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
       2,     2,     2,    77,     2,     2,     2,     2,     2,     2,
      75,    76,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    74,
      79,     2,    78,     2,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   171,   171,   173,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   223,   227,   231,
     236,   240,   244,   248,   252,   256,   260,   267,   271,   278,
     282,   289,   293,   297,   301,   305,   309,   313,   317,   324,
     330,   336,   380,   427,   428,   429,   430,   431,   432,   433,
     437,   443,   452,   458,   467,   473,   482,   488,   497,   498,
     499,   500,   504,   508,   512,   516,   520,   524,   528,   532,
     539,   543,   547,   551,   555,   559,   563,   567,   574,   580,
     586,   605,   627,   628,   631,   635,   641,   649,   657,   665,
     676,   680,   686,   694,   702,   710,   721,   725,   731,   739,
     747,   755,   766,   770,   776,   784,   792,   800,   811,   815,
     821,   829,   837,   845,   856,   860,   866,   874,   882,   890,
     901,   905,   909,   913,   917,   921,   928,   932,   936,   940,
     944,   948,   955,   959,   963,   967,   971,   975,   982,   986,
     990,   994,   998,  1002,  1009,  1015,  1023,  1024,  1027,  1036,
    1048,  1073,  1101,  1137,  1174,  1178,  1182,  1200,  1219,  1231,
    1247,  1251,  1255,  1259,  1263,  1267,  1271,  1275,  1282,  1286,
    1290,  1294,  1298,  1302,  1306,  1310,  1317,  1321,  1325,  1329,
    1333,  1337,  1341,  1345,  1352,  1368,  1387,  1403,  1422,  1438,
    1457,  1463,  1469,  1470,  1473,  1479,  1488,  1507,  1528,  1545,
    1565,  1572,  1579,  1597,  1615,  1629,  1638,  1645,  1652,  1670,
    1688,  1702,  1711,  1719,  1727,  1735,  1743,  1751,  1759,  1767,
    1778,  1782,  1786,  1790,  1794,  1798,  1802,  1806,  1813,  1817,
    1821,  1825,  1829,  1833,  1837,  1841,  1848,  1852,  1856,  1860,
    1864,  1868,  1872,  1876,  1883,  1887,  1891,  1895,  1899,  1903,
    1907,  1911,  1918,  1925,  1932,  1950,  1968,  1982,  1991,  1995,
    1999,  2003,  2010,  2011,  2012
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
  "FLOW_SOFT_DISSECTOR", "FLOW_CATEGORY", "FLOW_RISKS",
  "FLOW_NDPI_RISK_SCORE", "FLOW_NDPI_RISK_SCORE_CLIENT",
  "FLOW_NDPI_RISK_SCORE_SERVER", "FLOW_DOMAIN_CATEGORY",
  "FLOW_NETWORK_CATEGORY", "FLOW_APPLICATION", "FLOW_APPLICATION_CATEGORY",
  "FLOW_PROTOCOL", "FLOW_PROTOCOL_CATEGORY", "FLOW_DETECTED_HOSTNAME",
  "FLOW_DNS_HOSTNAME", "FLOW_ORIGIN", "FLOW_CT_MARK", "FLOW_TLS_VERSION",
  "FLOW_TLS_CIPHER", "FLOW_TLS_ECH", "FLOW_TLS_ESNI", "FLOW_TLS_JA4",
  "FLOW_OTHER_UNKNOWN", "FLOW_OTHER_UNSUPPORTED", "FLOW_OTHER_LOCAL",
  "FLOW_OTHER_MULTICAST", "FLOW_OTHER_BROADCAST", "FLOW_OTHER_REMOTE",
  "FLOW_OTHER_ERROR", "FLOW_ORIGIN_LOCAL", "FLOW_ORIGIN_OTHER",
  "FLOW_ORIGIN_UNKNOWN", "FLOW_TUNNEL_NONE", "FLOW_TUNNEL_GTP",
  "CMP_EQUAL", "CMP_NOTEQUAL", "CMP_GTHANEQUAL", "CMP_LTHANEQUAL",
  "BOOL_AND", "BOOL_OR", "VALUE_ADDR_IPMASK", "VALUE_TRUE", "VALUE_FALSE",
  "VALUE_ADDR_MAC", "VALUE_NAME", "VALUE_REGEX", "VALUE_ADDR_IPV4",
  "VALUE_ADDR_IPV4_CIDR", "VALUE_ADDR_IPV6", "VALUE_ADDR_IPV6_CIDR",
  "VALUE_NUMBER", "';'", "'('", "')'", "'!'", "'>'", "'<'", "$accept",
  "exprs", "expr", "expr_ip_proto", "expr_ip_dscp", "expr_ip_version",
  "expr_vlan_id", "expr_other_type", "value_other_type", "expr_local_mac",
  "expr_other_mac", "expr_local_ip", "expr_other_ip", "value_addr_ip",
  "expr_local_port", "expr_other_port", "expr_tunnel_type",
  "value_tunnel_type", "expr_detection_complete", "expr_detection_guessed",
  "expr_detection_init", "expr_detection_updated", "expr_dhc_hit",
  "expr_fhc_hit", "expr_ip_nat", "expr_expiring", "expr_expired",
  "expr_soft_dissector", "expr_app", "expr_app_id", "expr_app_name",
  "expr_category", "expr_risks", "expr_risk_ndpi_score",
  "expr_risk_ndpi_score_client", "expr_risk_ndpi_score_server",
  "expr_app_category", "expr_domain_category", "expr_network_category",
  "expr_proto", "expr_proto_id", "expr_proto_name", "expr_proto_category",
  "expr_detected_hostname", "expr_dns_hostname", "expr_fwmark",
  "expr_tls_version", "expr_tls_cipher", "expr_tls_ech", "expr_tls_esni",
  "expr_tls_ja4", "expr_origin", "value_origin_type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-56)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -56,     1,   -56,   -11,   -55,    69,    -7,    91,   129,   133,
     154,   169,   106,   110,   177,   181,   192,   203,   205,   207,
     209,   211,   213,   215,   217,   219,   221,   114,   119,   123,
     223,   225,   227,   229,   231,   233,   235,   237,   239,   146,
     150,   158,   162,   173,   241,    76,   118,    -1,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -19,     4,    70,   102,   282,   284,
     285,   286,   287,   288,   289,   290,   292,   293,   294,   295,
     197,   197,    -4,     0,     8,   157,   184,   184,   184,   184,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   144,   144,   236,   238,   240,   242,   244,   246,
     248,   250,   252,   254,   256,   258,   260,   262,   264,   266,
     268,   270,   272,   274,   314,   315,   -10,    -9,   310,   311,
     312,   313,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   336,   337,   338,   339,
      -8,     2,   340,   341,    55,    56,   342,   343,   273,   275,
     277,   279,   142,   142,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   281,   283,    -6,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,    76,    76,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   291,   291
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,    49,     0,     0,    61,    69,     0,     0,
       0,     0,    92,   100,   108,   114,   120,   126,   132,   138,
     144,   150,   156,   162,   168,     0,   184,   190,   198,   206,
       0,     0,   174,     0,   220,     0,   230,   236,   288,   242,
     250,   258,   266,   274,   282,     0,     0,     0,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      31,   176,   177,    26,    27,    28,    29,    30,    32,    33,
      34,    35,   222,   223,    36,    37,    38,    45,    39,    40,
      41,    42,    43,    44,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,    50,    62,    70,
      93,   101,   109,   115,   121,   127,   133,   139,   145,   151,
     157,   163,   169,   185,   191,   199,   207,   175,   221,   231,
     237,   289,   243,   251,   259,   267,   275,   283,     0,     0,
       3,    51,    52,    53,    54,    55,    56,    59,    60,    57,
      58,    63,    64,    65,    66,    67,    68,    73,    74,    75,
      76,    77,    78,    79,    71,    72,    80,    81,    82,    83,
      88,    89,    90,    91,    84,    85,    86,    87,    94,    95,
      96,    97,    98,    99,   102,   103,   104,   105,   106,   107,
     112,   113,   110,   111,   116,   117,   118,   119,   122,   123,
     124,   125,   128,   129,   130,   131,   134,   135,   136,   137,
     140,   141,   142,   143,   146,   147,   148,   149,   152,   153,
     154,   155,   158,   159,   160,   161,   164,   165,   166,   167,
     170,   171,   172,   173,   182,   183,   186,   188,   187,   189,
     192,   193,   194,   195,   196,   197,   200,   201,   202,   203,
     204,   205,   208,   209,   210,   211,   212,   213,   216,   217,
     218,   219,   180,   178,   181,   179,   214,   215,   226,   224,
     227,   225,   228,   229,   232,   234,   233,   235,   238,   240,
     239,   241,   292,   293,   294,   290,   291,   244,   245,   246,
     247,   248,   249,   252,   253,   254,   255,   256,   257,   260,
     261,   262,   263,   264,   265,   268,   269,   270,   271,   272,
     273,   276,   277,   278,   279,   280,   281,   284,   286,   285,
     287,    48,    47,    46
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -56,   -56,   -45,   -56,   -56,   -56,   -56,   -56,   243,   -56,
     -56,   -56,   -56,   140,   -56,   -56,   -56,   255,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   163
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    47,    48,    49,    50,    51,    52,   284,    53,
      54,    55,    56,   294,    57,    58,    59,   312,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,   405
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     226,     2,   100,   101,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    94,    95,    96,    97,
     104,   105,   106,   107,   261,   258,   259,   356,   358,   382,
     258,   259,   286,   357,   359,   383,   287,    98,    99,   384,
     441,   108,   109,   260,   288,   385,    45,   262,    46,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,   227,   388,   390,   228,   229,   102,   103,   389,   391,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   263,   243,   244,   245,   246,   110,   111,
     247,    45,   248,    46,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   120,   121,   122,   123,   126,   127,   128,
     129,   158,   159,   160,   161,   264,   164,   165,   166,   167,
     170,   171,   172,   173,   124,   125,   112,   113,   130,   131,
     114,   115,   162,   163,   402,   403,   404,   168,   169,   310,
     311,   174,   175,   194,   195,   196,   197,   200,   201,   202,
     203,   116,   117,   442,   443,   206,   207,   208,   209,   212,
     213,   214,   215,   289,   198,   199,   118,   119,   204,   205,
     218,   219,   220,   221,   132,   133,   210,   211,   134,   135,
     216,   217,   277,   278,   279,   280,   281,   282,   283,   136,
     137,   222,   223,   290,   291,   292,   293,   295,   296,   297,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   224,   225,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     394,   395,   396,   397,   398,   399,   400,   401,   437,   438,
     439,   440,   258,   259,   285,   265,   406,   266,   267,   268,
     269,   270,   271,   272,     0,   273,   274,   275,   276,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   354,   355,   360,   361,   362,   363,     0,   313,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   386,   387,   392,
     393,     0,     0,     0,     0,     0,     0,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436
};

static const yytype_int16 yycheck[] =
{
      45,     0,    57,    58,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    57,    58,    59,    60,
      57,    58,    59,    60,    73,    61,    62,    67,    67,    67,
      61,    62,    66,    73,    73,    73,    66,    78,    79,    67,
      76,    78,    79,    74,    66,    73,    75,    73,    77,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     3,    67,    67,     6,     7,    57,    58,    73,    73,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    73,    26,    27,    28,    29,    57,    58,
      32,    75,    34,    77,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    57,    58,    59,    60,    57,    58,    59,
      60,    57,    58,    59,    60,    73,    57,    58,    59,    60,
      57,    58,    59,    60,    78,    79,    57,    58,    78,    79,
      57,    58,    78,    79,    52,    53,    54,    78,    79,    55,
      56,    78,    79,    57,    58,    59,    60,    57,    58,    59,
      60,    57,    58,   258,   259,    57,    58,    59,    60,    57,
      58,    59,    60,    66,    78,    79,    57,    58,    78,    79,
      57,    58,    59,    60,    57,    58,    78,    79,    57,    58,
      78,    79,    45,    46,    47,    48,    49,    50,    51,    57,
      58,    78,    79,    69,    70,    71,    72,   117,   118,   119,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      67,    68,    67,    68,    67,    68,    67,    68,    67,    68,
      67,    68,    61,    62,   111,    73,   193,    73,    73,    73,
      73,    73,    73,    73,    -1,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    67,    67,    73,    73,    73,    73,    -1,   133,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    67,    67,    67,    67,    67,    67,    67,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    81,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    75,    77,    82,    83,    84,
      85,    86,    87,    89,    90,    91,    92,    94,    95,    96,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    57,    58,    59,    60,    78,    79,
      57,    58,    57,    58,    57,    58,    59,    60,    78,    79,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    59,    60,    78,    79,    57,    58,    59,    60,
      78,    79,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      59,    60,    78,    79,    57,    58,    59,    60,    78,    79,
      57,    58,    59,    60,    78,    79,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    57,    58,    57,    58,
      57,    58,    57,    58,    57,    58,    59,    60,    78,    79,
      57,    58,    59,    60,    78,    79,    57,    58,    59,    60,
      78,    79,    57,    58,    59,    60,    78,    79,    57,    58,
      59,    60,    78,    79,    57,    58,    82,     3,     6,     7,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    26,    27,    28,    29,    32,    34,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    61,    62,
      74,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    45,    46,    47,
      48,    49,    50,    51,    88,    88,    66,    66,    66,    66,
      69,    70,    71,    72,    93,    93,    93,    93,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      55,    56,    97,    97,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    64,    65,    64,    65,    64,    65,
      64,    65,    64,    65,    67,    67,    67,    73,    67,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    67,    67,
      67,    67,    67,    73,    67,    73,    67,    67,    67,    73,
      67,    73,    67,    67,    67,    68,    67,    68,    67,    68,
      67,    68,    52,    53,    54,   132,   132,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    67,    68,    67,
      68,    76,    82,    82
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    80,    81,    81,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    83,
      83,    83,    83,    83,    83,    83,    83,    84,    84,    85,
      85,    86,    86,    86,    86,    86,    86,    86,    86,    87,
      87,    87,    87,    88,    88,    88,    88,    88,    88,    88,
      89,    89,    90,    90,    91,    91,    92,    92,    93,    93,
      93,    93,    94,    94,    94,    94,    94,    94,    94,    94,
      95,    95,    95,    95,    95,    95,    95,    95,    96,    96,
      96,    96,    97,    97,    98,    98,    98,    98,    98,    98,
      99,    99,    99,    99,    99,    99,   100,   100,   100,   100,
     100,   100,   101,   101,   101,   101,   101,   101,   102,   102,
     102,   102,   102,   102,   103,   103,   103,   103,   103,   103,
     104,   104,   104,   104,   104,   104,   105,   105,   105,   105,
     105,   105,   106,   106,   106,   106,   106,   106,   107,   107,
     107,   107,   107,   107,   108,   108,   108,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   112,   112,   112,   112,
     113,   113,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   114,   114,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   116,   116,   117,   117,   118,   118,
     119,   119,   119,   119,   120,   120,   121,   121,   122,   122,
     123,   123,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   124,   125,   125,   125,   125,   125,   125,   125,   125,
     126,   126,   126,   126,   126,   126,   126,   126,   127,   127,
     127,   127,   127,   127,   127,   127,   128,   128,   128,   128,
     128,   128,   128,   128,   129,   129,   129,   129,   129,   129,
     129,   129,   130,   130,   130,   130,   130,   130,   131,   131,
     131,   131,   132,   132,   132
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     1,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     1,     1,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     2,     1,     1,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     2,
       3,     3,     1,     1,     1
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
  case 46: /* expr: expr BOOL_OR expr  */
#line 219 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) || (yyvsp[0].bool_result)));
        _NDFP_debugf("OR (%d || %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 1979 "nd-flow-expr.cpp"
    break;

  case 47: /* expr: expr BOOL_AND expr  */
#line 223 "nd-flow-expr.ypp"
                         {
        _NDFP_result = ((yyval.bool_result) = ((yyvsp[-2].bool_result) && (yyvsp[0].bool_result)));
        _NDFP_debugf("AND (%d && %d == %d)\n", (yyvsp[-2].bool_result), (yyvsp[0].bool_result), (yyval.bool_result));
    }
#line 1988 "nd-flow-expr.cpp"
    break;

  case 48: /* expr: '(' expr ')'  */
#line 227 "nd-flow-expr.ypp"
                   { _NDFP_result = ((yyval.bool_result) = (yyvsp[-1].bool_result)); }
#line 1994 "nd-flow-expr.cpp"
    break;

  case 49: /* expr_ip_proto: FLOW_IP_PROTO  */
#line 231 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != 0));
        _NDFP_debugf(
            "IP Protocol is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2004 "nd-flow-expr.cpp"
    break;

  case 50: /* expr_ip_proto: '!' FLOW_IP_PROTO  */
#line 236 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == 0));
        _NDFP_debugf("IP Protocol is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2013 "nd-flow-expr.cpp"
    break;

  case 51: /* expr_ip_proto: FLOW_IP_PROTO CMP_EQUAL VALUE_NUMBER  */
#line 240 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2022 "nd-flow-expr.cpp"
    break;

  case 52: /* expr_ip_proto: FLOW_IP_PROTO CMP_NOTEQUAL VALUE_NUMBER  */
#line 244 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2031 "nd-flow-expr.cpp"
    break;

  case 53: /* expr_ip_proto: FLOW_IP_PROTO CMP_GTHANEQUAL VALUE_NUMBER  */
#line 248 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol >= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2040 "nd-flow-expr.cpp"
    break;

  case 54: /* expr_ip_proto: FLOW_IP_PROTO CMP_LTHANEQUAL VALUE_NUMBER  */
#line 252 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol <= (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2049 "nd-flow-expr.cpp"
    break;

  case 55: /* expr_ip_proto: FLOW_IP_PROTO '>' VALUE_NUMBER  */
#line 256 "nd-flow-expr.ypp"
                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol > (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2058 "nd-flow-expr.cpp"
    break;

  case 56: /* expr_ip_proto: FLOW_IP_PROTO '<' VALUE_NUMBER  */
#line 260 "nd-flow-expr.ypp"
                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_protocol < (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Protocol > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2067 "nd-flow-expr.cpp"
    break;

  case 57: /* expr_ip_dscp: FLOW_IP_DSCP CMP_EQUAL VALUE_NUMBER  */
#line 267 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2076 "nd-flow-expr.cpp"
    break;

  case 58: /* expr_ip_dscp: FLOW_IP_DSCP CMP_NOTEQUAL VALUE_NUMBER  */
#line 271 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_dscp != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP DSCP != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2085 "nd-flow-expr.cpp"
    break;

  case 59: /* expr_ip_version: FLOW_IP_VERSION CMP_EQUAL VALUE_NUMBER  */
#line 278 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version == (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2094 "nd-flow-expr.cpp"
    break;

  case 60: /* expr_ip_version: FLOW_IP_VERSION CMP_NOTEQUAL VALUE_NUMBER  */
#line 282 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->ip_version != (yyvsp[0].ul_number)));
        _NDFP_debugf("IP Version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2103 "nd-flow-expr.cpp"
    break;

  case 61: /* expr_vlan_id: FLOW_VLAN_ID  */
#line 289 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != 0));
        _NDFP_debugf("VLAN ID is non-zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2112 "nd-flow-expr.cpp"
    break;

  case 62: /* expr_vlan_id: '!' FLOW_VLAN_ID  */
#line 293 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == 0));
        _NDFP_debugf("VLAN ID is zero? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2121 "nd-flow-expr.cpp"
    break;

  case 63: /* expr_vlan_id: FLOW_VLAN_ID CMP_EQUAL VALUE_NUMBER  */
#line 297 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id == (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2130 "nd-flow-expr.cpp"
    break;

  case 64: /* expr_vlan_id: FLOW_VLAN_ID CMP_NOTEQUAL VALUE_NUMBER  */
#line 301 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id != (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2139 "nd-flow-expr.cpp"
    break;

  case 65: /* expr_vlan_id: FLOW_VLAN_ID CMP_GTHANEQUAL VALUE_NUMBER  */
#line 305 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id >= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2148 "nd-flow-expr.cpp"
    break;

  case 66: /* expr_vlan_id: FLOW_VLAN_ID CMP_LTHANEQUAL VALUE_NUMBER  */
#line 309 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id <= (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2157 "nd-flow-expr.cpp"
    break;

  case 67: /* expr_vlan_id: FLOW_VLAN_ID '>' VALUE_NUMBER  */
#line 313 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id > (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2166 "nd-flow-expr.cpp"
    break;

  case 68: /* expr_vlan_id: FLOW_VLAN_ID '<' VALUE_NUMBER  */
#line 317 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->vlan_id < (yyvsp[0].ul_number)));
        _NDFP_debugf("VLAN ID < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2175 "nd-flow-expr.cpp"
    break;

  case 69: /* expr_other_type: FLOW_OTHER_TYPE  */
#line 324 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type != ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2186 "nd-flow-expr.cpp"
    break;

  case 70: /* expr_other_type: '!' FLOW_OTHER_TYPE  */
#line 330 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->other_type == ndFlow::OtherType::UNKNOWN
        ));
        _NDFP_debugf("Other type unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2197 "nd-flow-expr.cpp"
    break;

  case 71: /* expr_other_type: FLOW_OTHER_TYPE CMP_EQUAL value_other_type  */
#line 336 "nd-flow-expr.ypp"
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
#line 2246 "nd-flow-expr.cpp"
    break;

  case 72: /* expr_other_type: FLOW_OTHER_TYPE CMP_NOTEQUAL value_other_type  */
#line 380 "nd-flow-expr.ypp"
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
#line 2295 "nd-flow-expr.cpp"
    break;

  case 73: /* value_other_type: FLOW_OTHER_UNKNOWN  */
#line 427 "nd-flow-expr.ypp"
                         { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2301 "nd-flow-expr.cpp"
    break;

  case 74: /* value_other_type: FLOW_OTHER_UNSUPPORTED  */
#line 428 "nd-flow-expr.ypp"
                             { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2307 "nd-flow-expr.cpp"
    break;

  case 75: /* value_other_type: FLOW_OTHER_LOCAL  */
#line 429 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2313 "nd-flow-expr.cpp"
    break;

  case 76: /* value_other_type: FLOW_OTHER_MULTICAST  */
#line 430 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2319 "nd-flow-expr.cpp"
    break;

  case 77: /* value_other_type: FLOW_OTHER_BROADCAST  */
#line 431 "nd-flow-expr.ypp"
                           { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2325 "nd-flow-expr.cpp"
    break;

  case 78: /* value_other_type: FLOW_OTHER_REMOTE  */
#line 432 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2331 "nd-flow-expr.cpp"
    break;

  case 79: /* value_other_type: FLOW_OTHER_ERROR  */
#line 433 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2337 "nd-flow-expr.cpp"
    break;

  case 80: /* expr_local_mac: FLOW_LOCAL_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 437 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Local MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2348 "nd-flow-expr.cpp"
    break;

  case 81: /* expr_local_mac: FLOW_LOCAL_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 443 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_local_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Local MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2359 "nd-flow-expr.cpp"
    break;

  case 82: /* expr_other_mac: FLOW_OTHER_MAC CMP_EQUAL VALUE_ADDR_MAC  */
#line 452 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_other_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) == 0
        ));
        _NDFP_debugf("Other MAC == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2370 "nd-flow-expr.cpp"
    break;

  case 83: /* expr_other_mac: FLOW_OTHER_MAC CMP_NOTEQUAL VALUE_ADDR_MAC  */
#line 458 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (
            strncasecmp(_NDFP_other_mac, (yyvsp[0].buffer), ND_STR_ETHALEN) != 0
        ));
        _NDFP_debugf("Other MAC != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2381 "nd-flow-expr.cpp"
    break;

  case 84: /* expr_local_ip: FLOW_LOCAL_IP CMP_EQUAL value_addr_ip  */
#line 467 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Local IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2392 "nd-flow-expr.cpp"
    break;

  case 85: /* expr_local_ip: FLOW_LOCAL_IP CMP_NOTEQUAL value_addr_ip  */
#line 473 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_local_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Local IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2403 "nd-flow-expr.cpp"
    break;

  case 86: /* expr_other_ip: FLOW_OTHER_IP CMP_EQUAL value_addr_ip  */
#line 482 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == true
        ));
        _NDFP_debugf("Other IP == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2414 "nd-flow-expr.cpp"
    break;

  case 87: /* expr_other_ip: FLOW_OTHER_IP CMP_NOTEQUAL value_addr_ip  */
#line 488 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            is_addr_equal(_NDFP_other_ip, (yyvsp[0].buffer)) == false
        ));
        _NDFP_debugf("Other IP != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 2425 "nd-flow-expr.cpp"
    break;

  case 88: /* value_addr_ip: VALUE_ADDR_IPV4  */
#line 497 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2431 "nd-flow-expr.cpp"
    break;

  case 89: /* value_addr_ip: VALUE_ADDR_IPV4_CIDR  */
#line 498 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2437 "nd-flow-expr.cpp"
    break;

  case 90: /* value_addr_ip: VALUE_ADDR_IPV6  */
#line 499 "nd-flow-expr.ypp"
                      { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2443 "nd-flow-expr.cpp"
    break;

  case 91: /* value_addr_ip: VALUE_ADDR_IPV6_CIDR  */
#line 500 "nd-flow-expr.ypp"
                           { strncpy((yyval.buffer), (yyvsp[0].buffer), _NDFP_MAX_BUFLEN); }
#line 2449 "nd-flow-expr.cpp"
    break;

  case 92: /* expr_local_port: FLOW_LOCAL_PORT  */
#line 504 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != 0));
        _NDFP_debugf("Local port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2458 "nd-flow-expr.cpp"
    break;

  case 93: /* expr_local_port: '!' FLOW_LOCAL_PORT  */
#line 508 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == 0));
        _NDFP_debugf("Local port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2467 "nd-flow-expr.cpp"
    break;

  case 94: /* expr_local_port: FLOW_LOCAL_PORT CMP_EQUAL VALUE_NUMBER  */
#line 512 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2476 "nd-flow-expr.cpp"
    break;

  case 95: /* expr_local_port: FLOW_LOCAL_PORT CMP_NOTEQUAL VALUE_NUMBER  */
#line 516 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2485 "nd-flow-expr.cpp"
    break;

  case 96: /* expr_local_port: FLOW_LOCAL_PORT CMP_GTHANEQUAL VALUE_NUMBER  */
#line 520 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2494 "nd-flow-expr.cpp"
    break;

  case 97: /* expr_local_port: FLOW_LOCAL_PORT CMP_LTHANEQUAL VALUE_NUMBER  */
#line 524 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2503 "nd-flow-expr.cpp"
    break;

  case 98: /* expr_local_port: FLOW_LOCAL_PORT '>' VALUE_NUMBER  */
#line 528 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2512 "nd-flow-expr.cpp"
    break;

  case 99: /* expr_local_port: FLOW_LOCAL_PORT '<' VALUE_NUMBER  */
#line 532 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_local_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Local port > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2521 "nd-flow-expr.cpp"
    break;

  case 100: /* expr_other_port: FLOW_OTHER_PORT  */
#line 539 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != 0));
        _NDFP_debugf("Other port is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2530 "nd-flow-expr.cpp"
    break;

  case 101: /* expr_other_port: '!' FLOW_OTHER_PORT  */
#line 543 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == 0));
        _NDFP_debugf("Other port is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2539 "nd-flow-expr.cpp"
    break;

  case 102: /* expr_other_port: FLOW_OTHER_PORT CMP_EQUAL VALUE_NUMBER  */
#line 547 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port == (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2548 "nd-flow-expr.cpp"
    break;

  case 103: /* expr_other_port: FLOW_OTHER_PORT CMP_NOTEQUAL VALUE_NUMBER  */
#line 551 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port != (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2557 "nd-flow-expr.cpp"
    break;

  case 104: /* expr_other_port: FLOW_OTHER_PORT CMP_GTHANEQUAL VALUE_NUMBER  */
#line 555 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port >= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2566 "nd-flow-expr.cpp"
    break;

  case 105: /* expr_other_port: FLOW_OTHER_PORT CMP_LTHANEQUAL VALUE_NUMBER  */
#line 559 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port <= (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2575 "nd-flow-expr.cpp"
    break;

  case 106: /* expr_other_port: FLOW_OTHER_PORT '>' VALUE_NUMBER  */
#line 563 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port > (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2584 "nd-flow-expr.cpp"
    break;

  case 107: /* expr_other_port: FLOW_OTHER_PORT '<' VALUE_NUMBER  */
#line 567 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_other_port < (yyvsp[0].ul_number)));
        _NDFP_debugf("Other port > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 2593 "nd-flow-expr.cpp"
    break;

  case 108: /* expr_tunnel_type: FLOW_TUNNEL_TYPE  */
#line 574 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type != ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2604 "nd-flow-expr.cpp"
    break;

  case 109: /* expr_tunnel_type: '!' FLOW_TUNNEL_TYPE  */
#line 580 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tunnel_type == ndFlow::TunnelType::NONE
        ));
        _NDFP_debugf("Tunnel type is none? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2615 "nd-flow-expr.cpp"
    break;

  case 110: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_EQUAL value_tunnel_type  */
#line 586 "nd-flow-expr.ypp"
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
#line 2639 "nd-flow-expr.cpp"
    break;

  case 111: /* expr_tunnel_type: FLOW_TUNNEL_TYPE CMP_NOTEQUAL value_tunnel_type  */
#line 605 "nd-flow-expr.ypp"
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
#line 2663 "nd-flow-expr.cpp"
    break;

  case 112: /* value_tunnel_type: FLOW_TUNNEL_NONE  */
#line 627 "nd-flow-expr.ypp"
                       { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2669 "nd-flow-expr.cpp"
    break;

  case 113: /* value_tunnel_type: FLOW_TUNNEL_GTP  */
#line 628 "nd-flow-expr.ypp"
                      { (yyval.us_number) = (yyvsp[0].us_number); }
#line 2675 "nd-flow-expr.cpp"
    break;

  case 114: /* expr_detection_complete: FLOW_DETECTION_COMPLETE  */
#line 631 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf("Detection was complete? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2684 "nd-flow-expr.cpp"
    break;

  case 115: /* expr_detection_complete: '!' FLOW_DETECTION_COMPLETE  */
#line 635 "nd-flow-expr.ypp"
                                   {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_complete.load()));
        _NDFP_debugf(
            "Detection was not complete? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2695 "nd-flow-expr.cpp"
    break;

  case 116: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_TRUE  */
#line 641 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == true
        ));
        _NDFP_debugf(
            "Detection complete == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2708 "nd-flow-expr.cpp"
    break;

  case 117: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_EQUAL VALUE_FALSE  */
#line 649 "nd-flow-expr.ypp"
                                                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() == false
        ));
        _NDFP_debugf(
            "Detection complete == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2721 "nd-flow-expr.cpp"
    break;

  case 118: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_TRUE  */
#line 657 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != true
        ));
        _NDFP_debugf(
            "Detection complete != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2734 "nd-flow-expr.cpp"
    break;

  case 119: /* expr_detection_complete: FLOW_DETECTION_COMPLETE CMP_NOTEQUAL VALUE_FALSE  */
#line 665 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_complete.load() != false
        ));
        _NDFP_debugf(
            "Detection complete != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2747 "nd-flow-expr.cpp"
    break;

  case 120: /* expr_detection_guessed: FLOW_DETECTION_GUESSED  */
#line 676 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf("Detection was guessed? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2756 "nd-flow-expr.cpp"
    break;

  case 121: /* expr_detection_guessed: '!' FLOW_DETECTION_GUESSED  */
#line 680 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_guessed.load()));
        _NDFP_debugf(
            "Detection was not guessed? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2767 "nd-flow-expr.cpp"
    break;

  case 122: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_TRUE  */
#line 686 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == true
        ));
        _NDFP_debugf(
            "Detection guessed == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2780 "nd-flow-expr.cpp"
    break;

  case 123: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_EQUAL VALUE_FALSE  */
#line 694 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() == false
        ));
        _NDFP_debugf(
            "Detection guessed == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2793 "nd-flow-expr.cpp"
    break;

  case 124: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_TRUE  */
#line 702 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != true
        ));
        _NDFP_debugf(
            "Detection guessed != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2806 "nd-flow-expr.cpp"
    break;

  case 125: /* expr_detection_guessed: FLOW_DETECTION_GUESSED CMP_NOTEQUAL VALUE_FALSE  */
#line 710 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_guessed.load() != false
        ));
        _NDFP_debugf(
            "Detection guessed != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2819 "nd-flow-expr.cpp"
    break;

  case 126: /* expr_detection_init: FLOW_DETECTION_INIT  */
#line 721 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf("Detection was init? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2828 "nd-flow-expr.cpp"
    break;

  case 127: /* expr_detection_init: '!' FLOW_DETECTION_INIT  */
#line 725 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_init.load()));
        _NDFP_debugf(
            "Detection was not init? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2839 "nd-flow-expr.cpp"
    break;

  case 128: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_TRUE  */
#line 731 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == true
        ));
        _NDFP_debugf(
            "Detection init == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2852 "nd-flow-expr.cpp"
    break;

  case 129: /* expr_detection_init: FLOW_DETECTION_INIT CMP_EQUAL VALUE_FALSE  */
#line 739 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() == false
        ));
        _NDFP_debugf(
            "Detection init == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2865 "nd-flow-expr.cpp"
    break;

  case 130: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_TRUE  */
#line 747 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != true
        ));
        _NDFP_debugf(
            "Detection init != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2878 "nd-flow-expr.cpp"
    break;

  case 131: /* expr_detection_init: FLOW_DETECTION_INIT CMP_NOTEQUAL VALUE_FALSE  */
#line 755 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_init.load() != false
        ));
        _NDFP_debugf(
            "Detection init != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2891 "nd-flow-expr.cpp"
    break;

  case 132: /* expr_detection_updated: FLOW_DETECTION_UPDATED  */
#line 766 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf("Detection was updated? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2900 "nd-flow-expr.cpp"
    break;

  case 133: /* expr_detection_updated: '!' FLOW_DETECTION_UPDATED  */
#line 770 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.detection_updated.load()));
        _NDFP_debugf(
            "Detection was not updated? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2911 "nd-flow-expr.cpp"
    break;

  case 134: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_TRUE  */
#line 776 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == true
        ));
        _NDFP_debugf(
            "Detection updated == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2924 "nd-flow-expr.cpp"
    break;

  case 135: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_EQUAL VALUE_FALSE  */
#line 784 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() == false
        ));
        _NDFP_debugf(
            "Detection updated == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2937 "nd-flow-expr.cpp"
    break;

  case 136: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_TRUE  */
#line 792 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != true
        ));
        _NDFP_debugf(
            "Detection updated != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2950 "nd-flow-expr.cpp"
    break;

  case 137: /* expr_detection_updated: FLOW_DETECTION_UPDATED CMP_NOTEQUAL VALUE_FALSE  */
#line 800 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.detection_updated.load() != false
        ));
        _NDFP_debugf(
            "Detection updated != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2963 "nd-flow-expr.cpp"
    break;

  case 138: /* expr_dhc_hit: FLOW_DHC_HIT  */
#line 811 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf("DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 2972 "nd-flow-expr.cpp"
    break;

  case 139: /* expr_dhc_hit: '!' FLOW_DHC_HIT  */
#line 815 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.dhc_hit.load()));
        _NDFP_debugf(
            "DHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2983 "nd-flow-expr.cpp"
    break;

  case 140: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 821 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == true
        ));
        _NDFP_debugf(
            "DHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 2996 "nd-flow-expr.cpp"
    break;

  case 141: /* expr_dhc_hit: FLOW_DHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 829 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() == false
        ));
        _NDFP_debugf(
            "DHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3009 "nd-flow-expr.cpp"
    break;

  case 142: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 837 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != true
        ));
        _NDFP_debugf(
            "DHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3022 "nd-flow-expr.cpp"
    break;

  case 143: /* expr_dhc_hit: FLOW_DHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 845 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.dhc_hit.load() != false
        ));
        _NDFP_debugf(
            "DHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3035 "nd-flow-expr.cpp"
    break;

  case 144: /* expr_fhc_hit: FLOW_FHC_HIT  */
#line 856 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf("FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3044 "nd-flow-expr.cpp"
    break;

  case 145: /* expr_fhc_hit: '!' FLOW_FHC_HIT  */
#line 860 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = !(_NDFP_flow->flags.fhc_hit.load()));
        _NDFP_debugf(
            "FHC was hit? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3055 "nd-flow-expr.cpp"
    break;

  case 146: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_TRUE  */
#line 866 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == true
        ));
        _NDFP_debugf(
            "FHC hit == true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3068 "nd-flow-expr.cpp"
    break;

  case 147: /* expr_fhc_hit: FLOW_FHC_HIT CMP_EQUAL VALUE_FALSE  */
#line 874 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() == false
        ));
        _NDFP_debugf(
            "FHC hit == false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3081 "nd-flow-expr.cpp"
    break;

  case 148: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_TRUE  */
#line 882 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != true
        ));
        _NDFP_debugf(
            "FHC hit != true? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3094 "nd-flow-expr.cpp"
    break;

  case 149: /* expr_fhc_hit: FLOW_FHC_HIT CMP_NOTEQUAL VALUE_FALSE  */
#line 890 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->flags.fhc_hit.load() != false
        ));
        _NDFP_debugf(
            "FHC hit != false? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3107 "nd-flow-expr.cpp"
    break;

  case 150: /* expr_ip_nat: FLOW_IP_NAT  */
#line 901 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3116 "nd-flow-expr.cpp"
    break;

  case 151: /* expr_ip_nat: '!' FLOW_IP_NAT  */
#line 905 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3125 "nd-flow-expr.cpp"
    break;

  case 152: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_TRUE  */
#line 909 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == true));
        _NDFP_debugf("IP NAT == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3134 "nd-flow-expr.cpp"
    break;

  case 153: /* expr_ip_nat: FLOW_IP_NAT CMP_EQUAL VALUE_FALSE  */
#line 913 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() == false));
        _NDFP_debugf("IP NAT == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3143 "nd-flow-expr.cpp"
    break;

  case 154: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_TRUE  */
#line 917 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != true));
        _NDFP_debugf("IP NAT != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3152 "nd-flow-expr.cpp"
    break;

  case 155: /* expr_ip_nat: FLOW_IP_NAT CMP_NOTEQUAL VALUE_FALSE  */
#line 921 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.ip_nat.load() != false));
        _NDFP_debugf("IP NAT != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3161 "nd-flow-expr.cpp"
    break;

  case 156: /* expr_expiring: FLOW_EXPIRING  */
#line 928 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3170 "nd-flow-expr.cpp"
    break;

  case 157: /* expr_expiring: '!' FLOW_EXPIRING  */
#line 932 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3179 "nd-flow-expr.cpp"
    break;

  case 158: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_TRUE  */
#line 936 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == true));
        _NDFP_debugf("Flow expiring == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3188 "nd-flow-expr.cpp"
    break;

  case 159: /* expr_expiring: FLOW_EXPIRING CMP_EQUAL VALUE_FALSE  */
#line 940 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() == false));
        _NDFP_debugf("Flow expiring == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3197 "nd-flow-expr.cpp"
    break;

  case 160: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_TRUE  */
#line 944 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != true));
        _NDFP_debugf("Flow expiring != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3206 "nd-flow-expr.cpp"
    break;

  case 161: /* expr_expiring: FLOW_EXPIRING CMP_NOTEQUAL VALUE_FALSE  */
#line 948 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expiring.load() != false));
        _NDFP_debugf("Flow expiring != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3215 "nd-flow-expr.cpp"
    break;

  case 162: /* expr_expired: FLOW_EXPIRED  */
#line 955 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3224 "nd-flow-expr.cpp"
    break;

  case 163: /* expr_expired: '!' FLOW_EXPIRED  */
#line 959 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3233 "nd-flow-expr.cpp"
    break;

  case 164: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_TRUE  */
#line 963 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == true));
        _NDFP_debugf("Flow expired == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3242 "nd-flow-expr.cpp"
    break;

  case 165: /* expr_expired: FLOW_EXPIRED CMP_EQUAL VALUE_FALSE  */
#line 967 "nd-flow-expr.ypp"
                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() == false));
        _NDFP_debugf("Flow expired == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3251 "nd-flow-expr.cpp"
    break;

  case 166: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_TRUE  */
#line 971 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != true));
        _NDFP_debugf("Flow expired != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3260 "nd-flow-expr.cpp"
    break;

  case 167: /* expr_expired: FLOW_EXPIRED CMP_NOTEQUAL VALUE_FALSE  */
#line 975 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.expired.load() != false));
        _NDFP_debugf("Flow expired != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3269 "nd-flow-expr.cpp"
    break;

  case 168: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR  */
#line 982 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3278 "nd-flow-expr.cpp"
    break;

  case 169: /* expr_soft_dissector: '!' FLOW_SOFT_DISSECTOR  */
#line 986 "nd-flow-expr.ypp"
                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3287 "nd-flow-expr.cpp"
    break;

  case 170: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_TRUE  */
#line 990 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == true));
        _NDFP_debugf("Soft dissector matched == true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3296 "nd-flow-expr.cpp"
    break;

  case 171: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_EQUAL VALUE_FALSE  */
#line 994 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() == false));
        _NDFP_debugf("Soft dissector matched == false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3305 "nd-flow-expr.cpp"
    break;

  case 172: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_TRUE  */
#line 998 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != true));
        _NDFP_debugf("Soft dissector matched != true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3314 "nd-flow-expr.cpp"
    break;

  case 173: /* expr_soft_dissector: FLOW_SOFT_DISSECTOR CMP_NOTEQUAL VALUE_FALSE  */
#line 1002 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->flags.soft_dissector.load() != false));
        _NDFP_debugf("Soft dissector matched != false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3323 "nd-flow-expr.cpp"
    break;

  case 174: /* expr_app: FLOW_APPLICATION  */
#line 1009 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) != 0
        ));
        _NDFP_debugf("Application detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3334 "nd-flow-expr.cpp"
    break;

  case 175: /* expr_app: '!' FLOW_APPLICATION  */
#line 1015 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_application) == 0
        ));
        _NDFP_debugf(
            "Application not detected? %s\n", (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3347 "nd-flow-expr.cpp"
    break;

  case 178: /* expr_app_id: FLOW_APPLICATION CMP_EQUAL VALUE_NUMBER  */
#line 1027 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = false);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf(
            "Application ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3361 "nd-flow-expr.cpp"
    break;

  case 179: /* expr_app_id: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NUMBER  */
#line 1036 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);
        if ((yyvsp[0].ul_number) == static_cast<unsigned>(_NDFP_flow->detected_application))
            _NDFP_result = ((yyval.bool_result) = false);

        _NDFP_debugf(
            "Application ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3375 "nd-flow-expr.cpp"
    break;

  case 180: /* expr_app_name: FLOW_APPLICATION CMP_EQUAL VALUE_NAME  */
#line 1048 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = false);
        if (! _NDFP_flow->detected_application_name.empty()) {

            size_t p;
            string search((yyvsp[0].buffer));
            string app(_NDFP_flow->detected_application_name);

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(
                app.c_str(), search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
            else if ((p = app.find_first_of(".")) != string::npos && strncasecmp(
                app.substr(p + 1).c_str(), search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = true);
            }
        }

        _NDFP_debugf(
            "Application name == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3405 "nd-flow-expr.cpp"
    break;

  case 181: /* expr_app_name: FLOW_APPLICATION CMP_NOTEQUAL VALUE_NAME  */
#line 1073 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = true);
        if (! _NDFP_flow->detected_application_name.empty()) {

            size_t p;
            string search((yyvsp[0].buffer));
            string app(_NDFP_flow->detected_application_name);

            while ((p = search.find_first_of("'\"")) != string::npos)
                search.erase(p, 1);

            if (strncasecmp(
                app.c_str(), search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
            else if ((p = app.find_first_of(".")) != string::npos && strncasecmp(
                app.substr(p + 1).c_str(), search.c_str(), _NDFP_MAX_BUFLEN) == 0) {
                _NDFP_result = ((yyval.bool_result) = false);
            }
        }

        _NDFP_debugf(
            "Application name != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no"
        );
    }
#line 3435 "nd-flow-expr.cpp"
    break;

  case 182: /* expr_category: FLOW_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1101 "nd-flow-expr.ypp"
                                         {
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

        _NDFP_debugf("App/domain category == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3476 "nd-flow-expr.cpp"
    break;

  case 183: /* expr_category: FLOW_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1137 "nd-flow-expr.ypp"
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

        _NDFP_debugf("App/domain category != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3515 "nd-flow-expr.cpp"
    break;

  case 184: /* expr_risks: FLOW_RISKS  */
#line 1174 "nd-flow-expr.ypp"
                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() != 0));
        _NDFP_debugf("Risks detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3524 "nd-flow-expr.cpp"
    break;

  case 185: /* expr_risks: '!' FLOW_RISKS  */
#line 1178 "nd-flow-expr.ypp"
                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.risks.size() == 0));
        _NDFP_debugf("Risks not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3533 "nd-flow-expr.cpp"
    break;

  case 186: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NAME  */
#line 1182 "nd-flow-expr.ypp"
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

        _NDFP_debugf("Risks == %s %s\n", risk.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 3556 "nd-flow-expr.cpp"
    break;

  case 187: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NAME  */
#line 1200 "nd-flow-expr.ypp"
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

        _NDFP_result = !_NDFP_result;
        _NDFP_debugf("Risks != %s %s\n", risk.c_str(), (_NDFP_result) ? "yes" : "no");
    }
#line 3580 "nd-flow-expr.cpp"
    break;

  case 188: /* expr_risks: FLOW_RISKS CMP_EQUAL VALUE_NUMBER  */
#line 1219 "nd-flow-expr.ypp"
                                        {
        unsigned long id = (yyvsp[0].ul_number);

        _NDFP_result = false;
        for (auto &i : _NDFP_flow->risk.risks) {
            if (static_cast<unsigned long>(i) != id) continue;
            _NDFP_result = true;
            break;
        }

        _NDFP_debugf("Risks == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3597 "nd-flow-expr.cpp"
    break;

  case 189: /* expr_risks: FLOW_RISKS CMP_NOTEQUAL VALUE_NUMBER  */
#line 1231 "nd-flow-expr.ypp"
                                           {
        unsigned long id = (yyvsp[0].ul_number);

        _NDFP_result = false;
        for (auto &i : _NDFP_flow->risk.risks) {
            if (static_cast<unsigned long>(i) != id) continue;
            _NDFP_result = true;
            break;
        }

        _NDFP_result = !_NDFP_result;
        _NDFP_debugf("Risks != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3615 "nd-flow-expr.cpp"
    break;

  case 190: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE  */
#line 1247 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != 0));
        _NDFP_debugf("nDPI risk score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3624 "nd-flow-expr.cpp"
    break;

  case 191: /* expr_risk_ndpi_score: '!' FLOW_NDPI_RISK_SCORE  */
#line 1251 "nd-flow-expr.ypp"
                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == 0));
        _NDFP_debugf("nDPI risk score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3633 "nd-flow-expr.cpp"
    break;

  case 192: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_EQUAL VALUE_NUMBER  */
#line 1255 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3642 "nd-flow-expr.cpp"
    break;

  case 193: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_NOTEQUAL VALUE_NUMBER  */
#line 1259 "nd-flow-expr.ypp"
                                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3651 "nd-flow-expr.cpp"
    break;

  case 194: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1263 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3660 "nd-flow-expr.cpp"
    break;

  case 195: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1267 "nd-flow-expr.ypp"
                                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3669 "nd-flow-expr.cpp"
    break;

  case 196: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '>' VALUE_NUMBER  */
#line 1271 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3678 "nd-flow-expr.cpp"
    break;

  case 197: /* expr_risk_ndpi_score: FLOW_NDPI_RISK_SCORE '<' VALUE_NUMBER  */
#line 1275 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3687 "nd-flow-expr.cpp"
    break;

  case 198: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 1282 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != 0));
        _NDFP_debugf("nDPI risk client score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3696 "nd-flow-expr.cpp"
    break;

  case 199: /* expr_risk_ndpi_score_client: '!' FLOW_NDPI_RISK_SCORE_CLIENT  */
#line 1286 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == 0));
        _NDFP_debugf("nDPI risk client score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3705 "nd-flow-expr.cpp"
    break;

  case 200: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_EQUAL VALUE_NUMBER  */
#line 1290 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3714 "nd-flow-expr.cpp"
    break;

  case 201: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_NOTEQUAL VALUE_NUMBER  */
#line 1294 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3723 "nd-flow-expr.cpp"
    break;

  case 202: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1298 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3732 "nd-flow-expr.cpp"
    break;

  case 203: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1302 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3741 "nd-flow-expr.cpp"
    break;

  case 204: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '>' VALUE_NUMBER  */
#line 1306 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3750 "nd-flow-expr.cpp"
    break;

  case 205: /* expr_risk_ndpi_score_client: FLOW_NDPI_RISK_SCORE_CLIENT '<' VALUE_NUMBER  */
#line 1310 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_client < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk client score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3759 "nd-flow-expr.cpp"
    break;

  case 206: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER  */
#line 1317 "nd-flow-expr.ypp"
                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != 0));
        _NDFP_debugf("nDPI risk server score is true? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3768 "nd-flow-expr.cpp"
    break;

  case 207: /* expr_risk_ndpi_score_server: '!' FLOW_NDPI_RISK_SCORE_SERVER  */
#line 1321 "nd-flow-expr.ypp"
                                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == 0));
        _NDFP_debugf("nDPI risk server score is false? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3777 "nd-flow-expr.cpp"
    break;

  case 208: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_EQUAL VALUE_NUMBER  */
#line 1325 "nd-flow-expr.ypp"
                                                         {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server == (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score == %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3786 "nd-flow-expr.cpp"
    break;

  case 209: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_NOTEQUAL VALUE_NUMBER  */
#line 1329 "nd-flow-expr.ypp"
                                                            {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server != (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score != %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3795 "nd-flow-expr.cpp"
    break;

  case 210: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1333 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server >= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score >= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3804 "nd-flow-expr.cpp"
    break;

  case 211: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1337 "nd-flow-expr.ypp"
                                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server <= (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score <= %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3813 "nd-flow-expr.cpp"
    break;

  case 212: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '>' VALUE_NUMBER  */
#line 1341 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server > (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3822 "nd-flow-expr.cpp"
    break;

  case 213: /* expr_risk_ndpi_score_server: FLOW_NDPI_RISK_SCORE_SERVER '<' VALUE_NUMBER  */
#line 1345 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->risk.ndpi_score_server < (yyvsp[0].ul_number)));
        _NDFP_debugf("nDPI risk server score > %lu %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3831 "nd-flow-expr.cpp"
    break;

  case 214: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1352 "nd-flow-expr.ypp"
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
#line 3852 "nd-flow-expr.cpp"
    break;

  case 215: /* expr_app_category: FLOW_APPLICATION_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1368 "nd-flow-expr.ypp"
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
#line 3873 "nd-flow-expr.cpp"
    break;

  case 216: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1387 "nd-flow-expr.ypp"
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

        _NDFP_debugf("Domain category == %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3894 "nd-flow-expr.cpp"
    break;

  case 217: /* expr_domain_category: FLOW_DOMAIN_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1403 "nd-flow-expr.ypp"
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

        _NDFP_debugf("Domain category != %s? %s\n", (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 3915 "nd-flow-expr.cpp"
    break;

  case 218: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1422 "nd-flow-expr.ypp"
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
#line 3936 "nd-flow-expr.cpp"
    break;

  case 219: /* expr_network_category: FLOW_NETWORK_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1438 "nd-flow-expr.ypp"
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
#line 3957 "nd-flow-expr.cpp"
    break;

  case 220: /* expr_proto: FLOW_PROTOCOL  */
#line 1457 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol != ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3968 "nd-flow-expr.cpp"
    break;

  case 221: /* expr_proto: '!' FLOW_PROTOCOL  */
#line 1463 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->detected_protocol == ndProto::Id::UNKNOWN
        ));
        _NDFP_debugf("Protocol not detected? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 3979 "nd-flow-expr.cpp"
    break;

  case 224: /* expr_proto_id: FLOW_PROTOCOL CMP_EQUAL VALUE_NUMBER  */
#line 1473 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) == (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 3990 "nd-flow-expr.cpp"
    break;

  case 225: /* expr_proto_id: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NUMBER  */
#line 1479 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (
            static_cast<unsigned>(_NDFP_flow->detected_protocol) != (yyvsp[0].ul_number)
        ));
        _NDFP_debugf("Protocol ID != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4001 "nd-flow-expr.cpp"
    break;

  case 226: /* expr_proto_name: FLOW_PROTOCOL CMP_EQUAL VALUE_NAME  */
#line 1488 "nd-flow-expr.ypp"
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
#line 4025 "nd-flow-expr.cpp"
    break;

  case 227: /* expr_proto_name: FLOW_PROTOCOL CMP_NOTEQUAL VALUE_NAME  */
#line 1507 "nd-flow-expr.ypp"
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
#line 4048 "nd-flow-expr.cpp"
    break;

  case 228: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_EQUAL VALUE_NAME  */
#line 1528 "nd-flow-expr.ypp"
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
#line 4070 "nd-flow-expr.cpp"
    break;

  case 229: /* expr_proto_category: FLOW_PROTOCOL_CATEGORY CMP_NOTEQUAL VALUE_NAME  */
#line 1545 "nd-flow-expr.ypp"
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
#line 4092 "nd-flow-expr.cpp"
    break;

  case 230: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME  */
#line 1565 "nd-flow-expr.ypp"
                             {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == false
        ));
        _NDFP_debugf("Detected hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4104 "nd-flow-expr.cpp"
    break;

  case 231: /* expr_detected_hostname: '!' FLOW_DETECTED_HOSTNAME  */
#line 1572 "nd-flow-expr.ypp"
                                 {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->host_server_name.empty() == true
        ));
        _NDFP_debugf("Detected hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4116 "nd-flow-expr.cpp"
    break;

  case 232: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 1579 "nd-flow-expr.ypp"
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
#line 4139 "nd-flow-expr.cpp"
    break;

  case 233: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 1597 "nd-flow-expr.ypp"
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
#line 4162 "nd-flow-expr.cpp"
    break;

  case 234: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 1615 "nd-flow-expr.ypp"
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
#line 4181 "nd-flow-expr.cpp"
    break;

  case 235: /* expr_detected_hostname: FLOW_DETECTED_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 1629 "nd-flow-expr.ypp"
                                                      {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("Detected hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4192 "nd-flow-expr.cpp"
    break;

  case 236: /* expr_dns_hostname: FLOW_DNS_HOSTNAME  */
#line 1638 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == false
        ));
        _NDFP_debugf("DNS hostname detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4204 "nd-flow-expr.cpp"
    break;

  case 237: /* expr_dns_hostname: '!' FLOW_DNS_HOSTNAME  */
#line 1645 "nd-flow-expr.ypp"
                            {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->dns_host_name.empty() == true
        ));
        _NDFP_debugf("DNS hostname not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4216 "nd-flow-expr.cpp"
    break;

  case 238: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_NAME  */
#line 1652 "nd-flow-expr.ypp"
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
#line 4239 "nd-flow-expr.cpp"
    break;

  case 239: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_NAME  */
#line 1670 "nd-flow-expr.ypp"
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
#line 4262 "nd-flow-expr.cpp"
    break;

  case 240: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_EQUAL VALUE_REGEX  */
#line 1688 "nd-flow-expr.ypp"
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
#line 4281 "nd-flow-expr.cpp"
    break;

  case 241: /* expr_dns_hostname: FLOW_DNS_HOSTNAME CMP_NOTEQUAL VALUE_REGEX  */
#line 1702 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("DNS hostname (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4292 "nd-flow-expr.cpp"
    break;

  case 242: /* expr_fwmark: FLOW_CT_MARK  */
#line 1711 "nd-flow-expr.ypp"
                   {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != 0));
        _NDFP_debugf("FWMARK set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4305 "nd-flow-expr.cpp"
    break;

  case 243: /* expr_fwmark: '!' FLOW_CT_MARK  */
#line 1719 "nd-flow-expr.ypp"
                       {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == 0));
        _NDFP_debugf("FWMARK not set? %s\n", (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4318 "nd-flow-expr.cpp"
    break;

  case 244: /* expr_fwmark: FLOW_CT_MARK CMP_EQUAL VALUE_NUMBER  */
#line 1727 "nd-flow-expr.ypp"
                                          {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark == (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4331 "nd-flow-expr.cpp"
    break;

  case 245: /* expr_fwmark: FLOW_CT_MARK CMP_NOTEQUAL VALUE_NUMBER  */
#line 1735 "nd-flow-expr.ypp"
                                             {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark != (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4344 "nd-flow-expr.cpp"
    break;

  case 246: /* expr_fwmark: FLOW_CT_MARK CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1743 "nd-flow-expr.ypp"
                                               {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark >= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4357 "nd-flow-expr.cpp"
    break;

  case 247: /* expr_fwmark: FLOW_CT_MARK CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1751 "nd-flow-expr.ypp"
                                               {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark <= (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4370 "nd-flow-expr.cpp"
    break;

  case 248: /* expr_fwmark: FLOW_CT_MARK '>' VALUE_NUMBER  */
#line 1759 "nd-flow-expr.ypp"
                                    {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark > (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4383 "nd-flow-expr.cpp"
    break;

  case 249: /* expr_fwmark: FLOW_CT_MARK '<' VALUE_NUMBER  */
#line 1767 "nd-flow-expr.ypp"
                                    {
#if defined(_ND_ENABLE_CONNTRACK) && defined(_ND_ENABLE_CONNTRACK_MDATA)
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->conntrack.mark < (yyvsp[0].ul_number)));
        _NDFP_debugf("FWMARK < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
#else
        _NDFP_result = ((yyval.bool_result) = (false));
#endif
    }
#line 4396 "nd-flow-expr.cpp"
    break;

  case 250: /* expr_tls_version: FLOW_TLS_VERSION  */
#line 1778 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != 0));
        _NDFP_debugf("TLS version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4405 "nd-flow-expr.cpp"
    break;

  case 251: /* expr_tls_version: '!' FLOW_TLS_VERSION  */
#line 1782 "nd-flow-expr.ypp"
                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == 0));
        _NDFP_debugf("TLS version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4414 "nd-flow-expr.cpp"
    break;

  case 252: /* expr_tls_version: FLOW_TLS_VERSION CMP_EQUAL VALUE_NUMBER  */
#line 1786 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4423 "nd-flow-expr.cpp"
    break;

  case 253: /* expr_tls_version: FLOW_TLS_VERSION CMP_NOTEQUAL VALUE_NUMBER  */
#line 1790 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4432 "nd-flow-expr.cpp"
    break;

  case 254: /* expr_tls_version: FLOW_TLS_VERSION CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1794 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4441 "nd-flow-expr.cpp"
    break;

  case 255: /* expr_tls_version: FLOW_TLS_VERSION CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1798 "nd-flow-expr.ypp"
                                                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4450 "nd-flow-expr.cpp"
    break;

  case 256: /* expr_tls_version: FLOW_TLS_VERSION '>' VALUE_NUMBER  */
#line 1802 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4459 "nd-flow-expr.cpp"
    break;

  case 257: /* expr_tls_version: FLOW_TLS_VERSION '<' VALUE_NUMBER  */
#line 1806 "nd-flow-expr.ypp"
                                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4468 "nd-flow-expr.cpp"
    break;

  case 258: /* expr_tls_cipher: FLOW_TLS_CIPHER  */
#line 1813 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != 0));
        _NDFP_debugf("TLS cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4477 "nd-flow-expr.cpp"
    break;

  case 259: /* expr_tls_cipher: '!' FLOW_TLS_CIPHER  */
#line 1817 "nd-flow-expr.ypp"
                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == 0));
        _NDFP_debugf("TLS cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4486 "nd-flow-expr.cpp"
    break;

  case 260: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_EQUAL VALUE_NUMBER  */
#line 1821 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4495 "nd-flow-expr.cpp"
    break;

  case 261: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_NOTEQUAL VALUE_NUMBER  */
#line 1825 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4504 "nd-flow-expr.cpp"
    break;

  case 262: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1829 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4513 "nd-flow-expr.cpp"
    break;

  case 263: /* expr_tls_cipher: FLOW_TLS_CIPHER CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1833 "nd-flow-expr.ypp"
                                                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4522 "nd-flow-expr.cpp"
    break;

  case 264: /* expr_tls_cipher: FLOW_TLS_CIPHER '>' VALUE_NUMBER  */
#line 1837 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4531 "nd-flow-expr.cpp"
    break;

  case 265: /* expr_tls_cipher: FLOW_TLS_CIPHER '<' VALUE_NUMBER  */
#line 1841 "nd-flow-expr.ypp"
                                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4540 "nd-flow-expr.cpp"
    break;

  case 266: /* expr_tls_ech: FLOW_TLS_ECH  */
#line 1848 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != 0));
        _NDFP_debugf("TLS ECH version set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4549 "nd-flow-expr.cpp"
    break;

  case 267: /* expr_tls_ech: '!' FLOW_TLS_ECH  */
#line 1852 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == 0));
        _NDFP_debugf("TLS ECH version not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4558 "nd-flow-expr.cpp"
    break;

  case 268: /* expr_tls_ech: FLOW_TLS_ECH CMP_EQUAL VALUE_NUMBER  */
#line 1856 "nd-flow-expr.ypp"
                                          {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4567 "nd-flow-expr.cpp"
    break;

  case 269: /* expr_tls_ech: FLOW_TLS_ECH CMP_NOTEQUAL VALUE_NUMBER  */
#line 1860 "nd-flow-expr.ypp"
                                             {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4576 "nd-flow-expr.cpp"
    break;

  case 270: /* expr_tls_ech: FLOW_TLS_ECH CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1864 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4585 "nd-flow-expr.cpp"
    break;

  case 271: /* expr_tls_ech: FLOW_TLS_ECH CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1868 "nd-flow-expr.ypp"
                                               {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4594 "nd-flow-expr.cpp"
    break;

  case 272: /* expr_tls_ech: FLOW_TLS_ECH '>' VALUE_NUMBER  */
#line 1872 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4603 "nd-flow-expr.cpp"
    break;

  case 273: /* expr_tls_ech: FLOW_TLS_ECH '<' VALUE_NUMBER  */
#line 1876 "nd-flow-expr.ypp"
                                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.ech.version < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ECH version < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4612 "nd-flow-expr.cpp"
    break;

  case 274: /* expr_tls_esni: FLOW_TLS_ESNI  */
#line 1883 "nd-flow-expr.ypp"
                    {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite != 0));
        _NDFP_debugf("TLS ESNI cipher suite set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4621 "nd-flow-expr.cpp"
    break;

  case 275: /* expr_tls_esni: '!' FLOW_TLS_ESNI  */
#line 1887 "nd-flow-expr.ypp"
                        {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite == 0));
        _NDFP_debugf("TLS ESNI cipher suite not set? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4630 "nd-flow-expr.cpp"
    break;

  case 276: /* expr_tls_esni: FLOW_TLS_ESNI CMP_EQUAL VALUE_NUMBER  */
#line 1891 "nd-flow-expr.ypp"
                                           {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite == (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ESNI cipher suite == %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4639 "nd-flow-expr.cpp"
    break;

  case 277: /* expr_tls_esni: FLOW_TLS_ESNI CMP_NOTEQUAL VALUE_NUMBER  */
#line 1895 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite != (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ESNI cipher suite != %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4648 "nd-flow-expr.cpp"
    break;

  case 278: /* expr_tls_esni: FLOW_TLS_ESNI CMP_GTHANEQUAL VALUE_NUMBER  */
#line 1899 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite >= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ESNI cipher suite >= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4657 "nd-flow-expr.cpp"
    break;

  case 279: /* expr_tls_esni: FLOW_TLS_ESNI CMP_LTHANEQUAL VALUE_NUMBER  */
#line 1903 "nd-flow-expr.ypp"
                                                {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite <= (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ESNI cipher suite <= %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4666 "nd-flow-expr.cpp"
    break;

  case 280: /* expr_tls_esni: FLOW_TLS_ESNI '>' VALUE_NUMBER  */
#line 1907 "nd-flow-expr.ypp"
                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite > (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ESNI cipher suite > %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4675 "nd-flow-expr.cpp"
    break;

  case 281: /* expr_tls_esni: FLOW_TLS_ESNI '<' VALUE_NUMBER  */
#line 1911 "nd-flow-expr.ypp"
                                     {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_flow->tls.esni.cipher_suite < (yyvsp[0].ul_number)));
        _NDFP_debugf("TLS ESNI cipher suite < %lu? %s\n", (yyvsp[0].ul_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4684 "nd-flow-expr.cpp"
    break;

  case 282: /* expr_tls_ja4: FLOW_TLS_JA4  */
#line 1918 "nd-flow-expr.ypp"
                   {
        _NDFP_result = ((yyval.bool_result) = (
            ! _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4696 "nd-flow-expr.cpp"
    break;

  case 283: /* expr_tls_ja4: '!' FLOW_TLS_JA4  */
#line 1925 "nd-flow-expr.ypp"
                       {
        _NDFP_result = ((yyval.bool_result) = (
            _NDFP_flow->tls.client_ja4.empty()
        ));
        _NDFP_debugf("TLS JA4 not detected? %s\n",
            (_NDFP_result) ? "yes" : "no");
    }
#line 4708 "nd-flow-expr.cpp"
    break;

  case 284: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_NAME  */
#line 1932 "nd-flow-expr.ypp"
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
#line 4731 "nd-flow-expr.cpp"
    break;

  case 285: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_NAME  */
#line 1950 "nd-flow-expr.ypp"
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
#line 4754 "nd-flow-expr.cpp"
    break;

  case 286: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_EQUAL VALUE_REGEX  */
#line 1968 "nd-flow-expr.ypp"
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
#line 4773 "nd-flow-expr.cpp"
    break;

  case 287: /* expr_tls_ja4: FLOW_TLS_JA4 CMP_NOTEQUAL VALUE_REGEX  */
#line 1982 "nd-flow-expr.ypp"
                                            {
        _NDFP_result = ((yyval.bool_result) = true);

        _NDFP_debugf("TLS client JA4 (RX) != %s? %s\n",
            (yyvsp[0].buffer), (_NDFP_result) ? "yes" : "no");
    }
#line 4784 "nd-flow-expr.cpp"
    break;

  case 288: /* expr_origin: FLOW_ORIGIN  */
#line 1991 "nd-flow-expr.ypp"
                  {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin known? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4793 "nd-flow-expr.cpp"
    break;

  case 289: /* expr_origin: '!' FLOW_ORIGIN  */
#line 1995 "nd-flow-expr.ypp"
                      {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == _NDFP_ORIGIN_UNKNOWN));
        _NDFP_debugf("Flow origin unknown? %s\n", (_NDFP_result) ? "yes" : "no");
    }
#line 4802 "nd-flow-expr.cpp"
    break;

  case 290: /* expr_origin: FLOW_ORIGIN CMP_EQUAL value_origin_type  */
#line 1999 "nd-flow-expr.ypp"
                                              {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin == (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin == %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4811 "nd-flow-expr.cpp"
    break;

  case 291: /* expr_origin: FLOW_ORIGIN CMP_NOTEQUAL value_origin_type  */
#line 2003 "nd-flow-expr.ypp"
                                                 {
        _NDFP_result = ((yyval.bool_result) = (_NDFP_origin != (yyvsp[0].us_number)));
        _NDFP_debugf("Flow origin != %hu? %s\n", (yyvsp[0].us_number), (_NDFP_result) ? "yes" : "no");
    }
#line 4820 "nd-flow-expr.cpp"
    break;

  case 292: /* value_origin_type: FLOW_ORIGIN_LOCAL  */
#line 2010 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 4826 "nd-flow-expr.cpp"
    break;

  case 293: /* value_origin_type: FLOW_ORIGIN_OTHER  */
#line 2011 "nd-flow-expr.ypp"
                        { (yyval.us_number) = (yyvsp[0].us_number); }
#line 4832 "nd-flow-expr.cpp"
    break;

  case 294: /* value_origin_type: FLOW_ORIGIN_UNKNOWN  */
#line 2012 "nd-flow-expr.ypp"
                          { (yyval.us_number) = (yyvsp[0].us_number); }
#line 4838 "nd-flow-expr.cpp"
    break;


#line 4842 "nd-flow-expr.cpp"

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

#line 2014 "nd-flow-expr.ypp"


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

bool ndFlowParser::Parse(ndFlow::Ptr const& flow, const string &expr)
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
