#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "tokens.h"
#include "utils.h"

extern int yylex(void);

int yywrap(void){
  return 1;
}

void parser_internal_error_unknown_token(int token){
  fprintf(stderr,
    "parser internal error, unknown token code: %d\n",
    token);
  exit(1);
}

/* It is the responsibility of the consumer to free the string... */
char * token_to_string(int token){
  char *str;

  switch(token){
    case '{': case '}': case ';': case '(': case ')': case '=': case '+': case '*':
      str = (char *) malloc(2);
      str[0] = (char)token;
      str[1] = '\0';
      return str;
    case IF:   return strdup("IF");
    case ELSE: return strdup("ELSE");
    case INT:  return strdup("INT");
    case ID:   return strdup("ID");
    case NUM:  return strdup("NUM");
    case EOI:  return strdup("End Of Input");
    default:{
      parser_internal_error_unknown_token(token);
      return NULL;
    }
  }
}

void raise_syntax_error(int expected_token, int next_input_token){
  fprintf(stderr,
    "syntax error, expected token '%s' but found next input token '%s' instead\n",
    token_to_string(expected_token),
    token_to_string(next_input_token));
  exit(1);
}

#define NO_TOKEN -1

static int next_input_token_memorized = NO_TOKEN;

int consume_next_input_token(void){
  int next_input_token;
  if(next_input_token_memorized == NO_TOKEN){
    /* Directly get the next token from the lexer */
    next_input_token = yylex();
  }else{
    /* The parser peeked at the next token before */
    next_input_token = next_input_token_memorized;
    next_input_token_memorized = NO_TOKEN;
  }
  if(indent >= 0){
    int i;
    for(i = 0; i < 2 * indent; i++){putchar(' ');}
    printf("consuming input token: %s\n", token_to_string(next_input_token));
    fflush(stdout);
  }
  return next_input_token;
}

int peek_at_next_input_token(void){
  if(next_input_token_memorized == NO_TOKEN){
    /* Pretend that we are peeking at the token by memorizing it */
    next_input_token_memorized = yylex();
  }
  if(indent >= 0){
    int i;
    for(i = 0; i < 2 * indent; i++){putchar(' ');}
    printf("peeking at lookahead token: %s\n", token_to_string(next_input_token_memorized));
    fflush(stdout);
  }
  return next_input_token_memorized;
}
