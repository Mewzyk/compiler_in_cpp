// $Id: tokens.h,v 1.9 2017-09-29 14:50:27-07 - - $

#ifndef __TOKENS_H__
#define __TOKENS_H__

enum {
   YYEOF  = 0,
   IDENT  = 260,
   NUMBER = 261,
   ERROR  = 262,
};

extern int yy_flex_debug;
extern char* yytext;
extern int yylex (); 

#endif

