%{
#include <stdio.h>
%}

%%
[0-9]+	printf("NUMBER\n");
[a-zA-z][a-zA-Z0-9]*	printf("WORD\n");
%%
