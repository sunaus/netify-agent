/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 53 "nd-flow-expr.hpp"

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
    VALUE_ADDR_IPV4 = 349,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 350,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 351,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 352,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 353,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 354,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 355              /* VALUE_FLOAT  */
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
#define VALUE_ADDR_IPV4 349
#define VALUE_ADDR_IPV4_CIDR 350
#define VALUE_ADDR_IPV6 351
#define VALUE_ADDR_IPV6_CIDR 352
#define VALUE_SIGNED 353
#define VALUE_UNSIGNED 354
#define VALUE_FLOAT 355

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

#line 285 "nd-flow-expr.hpp"

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
