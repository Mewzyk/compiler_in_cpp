// $Id: main.cpp,v 1.1 2017-09-29 14:47:38-07 - - $

#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

int main (void) {
   yy_flex_debug = 1;
   int linenr = 1;

   for (;;) {
      int token = yylex();
      if (yy_flex_debug) fflush (NULL);
      switch (token) {
         case YYEOF:
            printf ("END OF FILE\n");
            return 0;
         case IDENT:
            printf ("IDENT \"%s\"\n", yytext);
            break;
         case NUMBER:
            printf ("NUMBER \"%s\"\n", yytext);
            break;
         case '+':
         case '-':
         case '*':
         case '/':
         case '=':
         case ';':
            printf ("OPERATOR \"%s\"\n", yytext);
            break;
         case '\n':
            printf ("NEWLINE\n");
            ++linenr;
            break;
         default:
            printf ("ERROR \"%s\"\n", yytext);
      }
   }

   return EXIT_SUCCESS;
}

