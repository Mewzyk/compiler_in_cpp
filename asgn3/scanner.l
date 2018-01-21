/* $Id: lexer.l,v 1.7 2016-10-27 19:45:59-07 - - $ */
/*
 * Stephen Thomas, sgthomas@ucsc.edu
 * Alex Hancock, abhancoc@ucsc.edu
 */
%{

#include "lyutils.h"

#define YY_USER_ACTION  { lexer::advance(); }

int yylval_token (int symbol) {
   yylval = new astree (symbol, lexer::lloc, yytext);
   
   fprintf(tok_file, "%lu %6.3f %5d %-15s (%s)\n",
                   lexer::lloc.filenr - 1, 
                   lexer::lloc.linenr + lexer::lloc.offset/1000.0,
                   symbol,
                   parser::get_tname(symbol),
                   yytext);
                    
   return symbol;
}

%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option warn
/*%option verbose*/

LETTER          [A-Za-z_]
DIGIT           [0-9]

CHAR            ('([^\\'\n]|\\[\\'"0nt])')
STRING          (\"([^\\"\n]|\\[\\'"0nt])*\")
INTEGER         ({DIGIT}+)
IDENT           ({LETTER}({LETTER}|{DIGIT})*)
BADIDENT        ({INTEGER}+{IDENT})
BADCHAR         ('[\n\'\\]'|'.[^\'])
BADSTRING       (\"[^\n\"\\]*(.|\n))


%%

"#".*           { lexer::include(); }
[ \t]+          { }
\n              { lexer::newline(); }

"if"            { return yylval_token (TOK_IF); }
"else"          { return yylval_token (TOK_ELSE); }
"while"         { return yylval_token (TOK_WHILE); }
"return"        { return yylval_token (TOK_RETURN); }
"int"           { return yylval_token (TOK_INT); }
"string"        { return yylval_token (TOK_STRING); }
"struct"        { return yylval_token (TOK_STRUCT); }
"void"          { return yylval_token (TOK_VOID); }
"new"           { return yylval_token (TOK_NEW); }
"null"          { return yylval_token (TOK_NULL); }


{INTEGER}       { return yylval_token (TOK_INTCON); }
{IDENT}         { return yylval_token (TOK_IDENT); }
{BADIDENT}      { lexer::badtoken(yytext); }
{CHAR}          { return yylval_token (TOK_CHARCON); }
{STRING}        { return yylval_token (TOK_STRINGCON); }
{BADCHAR}       { lexer::badtoken(yytext); }
{BADSTRING}     { lexer::badtoken(yytext); }

"+"             { return yylval_token ('+'); }
"-"             { return yylval_token ('-'); }
"*"             { return yylval_token ('*'); }
"/"             { return yylval_token ('/'); }
"%"             { return yylval_token ('%'); }
"!"             { return yylval_token ('!'); }
"="             { return yylval_token ('='); }
"=="            { return yylval_token (TOK_EQ); }
"!="            { return yylval_token (TOK_NE); }
"<"             { return yylval_token (TOK_LT); }
"<="            { return yylval_token (TOK_LE); }
">"             { return yylval_token (TOK_GT); }
">="            { return yylval_token (TOK_GE); }
","             { return yylval_token (','); }
"."             { return yylval_token ('.'); }
";"             { return yylval_token (';'); }
"("             { return yylval_token ('('); }
")"             { return yylval_token (')'); }
"["             { return yylval_token ('['); }
"]"             { return yylval_token (']'); }
"{"             { return yylval_token ('{'); }
"}"             { return yylval_token ('}'); }
"[]"            { return yylval_token (TOK_ARRAY); }

.               { lexer::badchar (*yytext); }

%%

