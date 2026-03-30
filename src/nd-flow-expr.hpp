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
#line 130 "nd-flow-expr.ypp"

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
    FLOW_OTHER_UNKNOWN = 330,      /* FLOW_OTHER_UNKNOWN  */
    FLOW_OTHER_UNSUPPORTED = 331,  /* FLOW_OTHER_UNSUPPORTED  */
    FLOW_OTHER_LOCAL = 332,        /* FLOW_OTHER_LOCAL  */
    FLOW_OTHER_MULTICAST = 333,    /* FLOW_OTHER_MULTICAST  */
    FLOW_OTHER_BROADCAST = 334,    /* FLOW_OTHER_BROADCAST  */
    FLOW_OTHER_REMOTE = 335,       /* FLOW_OTHER_REMOTE  */
    FLOW_OTHER_ERROR = 336,        /* FLOW_OTHER_ERROR  */
    FLOW_ORIGIN_LOCAL = 337,       /* FLOW_ORIGIN_LOCAL  */
    FLOW_ORIGIN_OTHER = 338,       /* FLOW_ORIGIN_OTHER  */
    FLOW_ORIGIN_UNKNOWN = 339,     /* FLOW_ORIGIN_UNKNOWN  */
    FLOW_TUNNEL_NONE = 340,        /* FLOW_TUNNEL_NONE  */
    FLOW_TUNNEL_GTP = 341,         /* FLOW_TUNNEL_GTP  */
    FLOW_INTEL = 342,              /* FLOW_INTEL  */
    CMP_EQUAL = 343,               /* CMP_EQUAL  */
    CMP_NOTEQUAL = 344,            /* CMP_NOTEQUAL  */
    CMP_GTHANEQUAL = 345,          /* CMP_GTHANEQUAL  */
    CMP_LTHANEQUAL = 346,          /* CMP_LTHANEQUAL  */
    BOOL_AND = 347,                /* BOOL_AND  */
    BOOL_OR = 348,                 /* BOOL_OR  */
    VALUE_ADDR_IPMASK = 349,       /* VALUE_ADDR_IPMASK  */
    VALUE_TRUE = 350,              /* VALUE_TRUE  */
    VALUE_FALSE = 351,             /* VALUE_FALSE  */
    VALUE_ADDR_MAC = 352,          /* VALUE_ADDR_MAC  */
    VALUE_NAME = 353,              /* VALUE_NAME  */
    VALUE_REGEX = 354,             /* VALUE_REGEX  */
    VALUE_ADDR_TAG = 355,          /* VALUE_ADDR_TAG  */
    VALUE_ADDR_IPV4 = 356,         /* VALUE_ADDR_IPV4  */
    VALUE_ADDR_IPV4_CIDR = 357,    /* VALUE_ADDR_IPV4_CIDR  */
    VALUE_ADDR_IPV6 = 358,         /* VALUE_ADDR_IPV6  */
    VALUE_ADDR_IPV6_CIDR = 359,    /* VALUE_ADDR_IPV6_CIDR  */
    VALUE_SIGNED = 360,            /* VALUE_SIGNED  */
    VALUE_UNSIGNED = 361,          /* VALUE_UNSIGNED  */
    VALUE_FLOAT = 362              /* VALUE_FLOAT  */
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
#define FLOW_OTHER_UNKNOWN 330
#define FLOW_OTHER_UNSUPPORTED 331
#define FLOW_OTHER_LOCAL 332
#define FLOW_OTHER_MULTICAST 333
#define FLOW_OTHER_BROADCAST 334
#define FLOW_OTHER_REMOTE 335
#define FLOW_OTHER_ERROR 336
#define FLOW_ORIGIN_LOCAL 337
#define FLOW_ORIGIN_OTHER 338
#define FLOW_ORIGIN_UNKNOWN 339
#define FLOW_TUNNEL_NONE 340
#define FLOW_TUNNEL_GTP 341
#define FLOW_INTEL 342
#define CMP_EQUAL 343
#define CMP_NOTEQUAL 344
#define CMP_GTHANEQUAL 345
#define CMP_LTHANEQUAL 346
#define BOOL_AND 347
#define BOOL_OR 348
#define VALUE_ADDR_IPMASK 349
#define VALUE_TRUE 350
#define VALUE_FALSE 351
#define VALUE_ADDR_MAC 352
#define VALUE_NAME 353
#define VALUE_REGEX 354
#define VALUE_ADDR_TAG 355
#define VALUE_ADDR_IPV4 356
#define VALUE_ADDR_IPV4_CIDR 357
#define VALUE_ADDR_IPV6 358
#define VALUE_ADDR_IPV6_CIDR 359
#define VALUE_SIGNED 360
#define VALUE_UNSIGNED 361
#define VALUE_FLOAT 362

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

#line 299 "nd-flow-expr.hpp"

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
