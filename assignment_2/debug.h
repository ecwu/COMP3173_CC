#include <stdio.h>

extern int indent;

#define FUNCCALL {if(indent >= 0){\
  int i;\
  for(i = 0; i < 2 * indent; i++){putchar(' ');}\
  indent++;\
  printf("%s => %s at line %d in %s\n", func, __func__, line, file);\
  fflush(stdout);\
}}

#define FUNCRET {if(indent >= 0){\
  int i;\
  indent--;\
  for(i = 0; i < 2 * indent; i++){putchar(' ');}\
  printf("%s <= %s at line %d in %s\n", func, __func__, line, file);\
  fflush(stdout);\
}}
