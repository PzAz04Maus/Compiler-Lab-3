%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: <<<< Stephen Carter >>>>
//

#include "lex.h"
#include "tokens.h"

#include <stdio.h>

// Macros that can be used to create debug output from the scanner
// Uncomment the next line of code to generate debug output.
// Note: for this to work, use DO_RETURN(val) for all the return statements
//       for your tokens. See sample below for "program"
//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
    #define DO_RETURN(a) { return Return(a); }
#else
    #define DO_RETURN(a) { return (a); }
#endif

int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


digit           [0-9]
idstart         [A-Za-z_]
idchar          [A-Za-z0-9_]
identifier      {idstart}{idchar}*
integer         {digit}+
ws              [ \t\r\n]+
%%

"//".*                    
{ws}                      

"end"                     DO_RETURN(END);

{integer}                 DO_RETURN(INT_VAL);
{identifier}              DO_RETURN(IDENTIFIER);

"+"                       DO_RETURN('+');
"-"                       DO_RETURN('-');
"*"                       DO_RETURN('*');
"/"                       DO_RETURN('/');
"="                       DO_RETURN('=');
";"                       DO_RETURN(';');
"("                       DO_RETURN('(');
")"                       DO_RETURN(')');
"["                       DO_RETURN('[');
"]"                       DO_RETURN(']');
"{"                       DO_RETURN('{');
"}"                       DO_RETURN('}');

.                         DO_RETURN(yytext[0]);

%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}
