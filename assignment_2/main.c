#include <unistd.h>
#include <string.h>
#include "debug.h"
#include "tokens.h"
#include "utils.h"
#include "tree.h"
#include "parser.h"

ptn parser_main(void){
  ptn parse_tree = match_B(); /* Start Symbol */
  if(peek_at_next_input_token() != EOI){
    raise_syntax_error(EOI, consume_next_input_token());
    return NULL;
  }else{
    return parse_tree;
  }
}

int main(int argc, char **argv){
  if(argc > 1){
    if(argc == 2 && strlen(argv[1]) == 2 && argv[1][0] == '-' && argv[1][1] == 'd'){
      indent = 0;
    }else{
      printf("usage: %s [-d]\n", argv[0]);
      return -1;
    }
  }
  ptn parse_tree = parser_main();
  print_parse_tree(parse_tree);
  return 0;
}
