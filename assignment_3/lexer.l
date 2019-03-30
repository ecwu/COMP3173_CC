%{
#include <stdlib.h>
#include <stdio.h>
#include "tokens.h"

void lexical_error(char c){
  fprintf(stderr, "lexical error, unknown character: %c\n", c);
  exit(1);
}
%}
%%
[{};()=+*]           return *yytext; /* token is ASCII code of symbol */
if                   return IF;
else                 return ELSE;
int                  return INT;
[a-zA-Z][a-zA-Z0-9]* return ID;
[0-9]+               return NUM;
[ \t]+               /* ignore white spaces */
\n                   /* ignore newlines */
.                    lexical_error(*yytext);
%%
