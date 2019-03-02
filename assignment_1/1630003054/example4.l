%{
#include <stdio.h>
#include "y.tab.h"
extern YYSTYPE yylval;
%}

%%
[0-9]+		return NUMBER;
heat		return TOKHEAT;
on|off		return STATE;
target		return TOKTARGET;
temperature	return TOKTEMPERATURE;
\n		/* ignore end of line */
[ \t]+		/* ignore whitespace  */
%%
