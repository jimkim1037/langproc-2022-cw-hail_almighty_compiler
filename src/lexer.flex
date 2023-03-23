%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);

#include "parser.tab.hpp"
%}
D			[0-9]
L			[a-zA-Z_]
IS			(u|U|l|L)*

%%
[*]             { return T_TIMES; }
[+]             { return T_PLUS; }
[\^]            { return T_EXPONENT; }
[-]             { return T_MINUS; }
[/]             { return T_DIVIDE; }
[\{]             { return T_LCURLY; }
[\}]             { return T_RCURLY; }
[(]             { return T_LBRACKET; }
[)]             { return T_RBRACKET; }
[;]             {return T_SEMICOLON; }
"return"       { return T_RETURN; }
"log"             { return T_LOG; }
"exp"             { return T_EXP; }
"sqrt"            { return T_SQRT; }


[0-9]+([.][0-9]*)? { yylval.number = strtod(yytext, 0); return T_NUMBER; }

{L}({L}|{D})*  { yylval.string=new std::string(yytext); return T_IDENTIFIER; }


[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }
%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}
