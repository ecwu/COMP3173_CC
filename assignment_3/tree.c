#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "debug.h"
#include "tokens.h"
#include "tree.h"
#include "utils.h"

int indent = -1;

/******************** Tags ********************/

#define TAG_TOK 45896
#define TAG_EPS 50132
#define TAG_B   14693
#define TAG_L1  80071
#define TAG_L2  20875
#define TAG_L3  97511
#define TAG_D   47871
#define TAG_S1  60128
#define TAG_S2  95110
#define TAG_E   31129
#define TAG_Ep1 49741
#define TAG_Ep2 20703
#define TAG_T   76328
#define TAG_Tp1 22933
#define TAG_Tp2 82172
#define TAG_F1  16671
#define TAG_F2  48103
#define TAG_F3  59498

void parse_tree_internal_error_unknown_tag(const char *s, int tag){
  fprintf(stderr,
    "parse tree internal error in %s, unknown node tag: %d\n",
    s, tag);
  exit(1);
}

/* It is the responsibility of the consumer to free the string... */
char * tag_to_string(int tag){
  switch(tag){
    case TAG_TOK: return strdup("token");
    case TAG_EPS: return strdup("eps");
    case TAG_B:   return strdup("B");
    case TAG_L1:  return strdup("L1");
    case TAG_L2:  return strdup("L2");
    case TAG_L3:  return strdup("L3");
    case TAG_D:   return strdup("D");
    case TAG_S1:  return strdup("S1");
    case TAG_S2:  return strdup("S2");
    case TAG_E:   return strdup("E");
    case TAG_Ep1: return strdup("E'1");
    case TAG_Ep2: return strdup("E'2");
    case TAG_T:   return strdup("T");
    case TAG_Tp1: return strdup("T'1");
    case TAG_Tp2: return strdup("T'2");
    case TAG_F1:  return strdup("F1");
    case TAG_F2:  return strdup("F2");
    case TAG_F3:  return strdup("F3");
    default:{
      parse_tree_internal_error_unknown_tag(__func__, tag);
      return NULL;
    }
  }
}

void parse_tree_internal_error_wrong_node(char *expected_tag, int tag, const char *parent){
  char *s = tag_to_string(tag);
  fprintf(stderr,
    "%s error, expected %s node but found %s node instead\n",
    parent, expected_tag, s);
  exit(1);
}

void parse_tree_internal_error_wrong_number(char *expected_args, int args, const char *parent){
  fprintf(stderr,
    "%s error, expected %s nodes but got %d\n",
    parent, expected_args, args);
  exit(1);
}


/******************** Nodes ********************/

/* Using the token as the tag would simplify the code, at the expense
** of having to ensure that the integers for tags and tokens are
** always different...
*/
struct token_node{
  int tag;
  int token;
};

struct epsilon_node{
  int tag;
};

struct B_node{
  int tag;
  struct token_node *ob;
  union L_node *L;
  struct token_node *cb;
};

struct L1_node{
  int tag;
  struct D_node *D;
  struct token_node *sc;
  union L_node *L;
};

struct L2_node{
  int tag;
  union S_node *S;
  struct token_node *sc;
  union L_node *L;
};

struct L3_node{
  int tag;
  struct epsilon_node *e;
};

union L_node{
  struct L1_node L1;
  struct L2_node L2;
  struct L3_node L3;
};

struct D_node{
  int tag;
  struct token_node *int_n;
  struct token_node *id;
  struct token_node *eq;
  struct E_node *E;
};

struct S1_node{
  int tag;
  struct token_node *if_n;
  struct token_node *op;
  struct E_node *E;
  struct token_node *cp;
  struct B_node *Bthen;
  struct token_node *else_n;
  struct B_node *Belse;
};

struct S2_node{
  int tag;
  struct token_node *id;
  struct token_node *eq;
  struct E_node *E;
};

union S_node{
  struct S1_node S1;
  struct S2_node S2;
};

struct E_node{
  int tag;
  struct T_node *T;
  union Ep_node *Ep;
};

struct Ep1_node{
  int tag;
  struct token_node *p;
  struct T_node *T;
  union Ep_node *Ep;
};

struct Ep2_node{
  int tag;
  struct epsilon_node *e;
};

union Ep_node{
  struct Ep1_node Ep1;
  struct Ep2_node Ep2;
};

struct T_node{
  int tag;
  union F_node *F;
  union Tp_node *Tp;
};

struct Tp1_node{
  int tag;
  struct token_node *m;
  union F_node *F;
  union Tp_node *Tp;
};

struct Tp2_node{
  int tag;
  struct epsilon_node *e;
};

union Tp_node{
  struct Tp1_node Tp1;
  struct Tp2_node Tp2;
};

struct F1_node{
  int tag;
  struct token_node *op;
  struct E_node *E;
  struct token_node *cp;
};

struct F2_node{
  int tag;
  struct token_node *id;
};

struct F3_node{
  int tag;
  struct token_node *num;
};

union F_node{
  struct F1_node F1;
  struct F2_node F2;
  struct F3_node F3;
};

typedef struct token_node * token_node;
typedef struct epsilon_node * epsilon_node;
typedef struct B_node * B_node;
typedef struct L1_node * L1_node;
typedef struct L2_node * L2_node;
typedef struct L3_node * L3_node;
typedef union L_node * L_node;
typedef struct D_node * D_node;
typedef struct S1_node * S1_node;
typedef struct S2_node * S2_node;
typedef union S_node * S_node;
typedef struct E_node * E_node;
typedef struct Ep1_node * Ep1_node;
typedef struct Ep2_node * Ep2_node;
typedef union Ep_node * Ep_node;
typedef struct T_node * T_node;
typedef struct Tp1_node * Tp1_node;
typedef struct Tp2_node * Tp2_node;
typedef union Tp_node * Tp_node;
typedef struct F1_node * F1_node;
typedef struct F2_node * F2_node;
typedef struct F3_node * F3_node;
typedef union F_node * F_node;

/******************** New Nodes / Check Nodes ********************/

/* Prototypes for non-API functions */
ptn new_L1_node(char *file, const char *func, int line, ptn, ptn, ptn);
ptn new_L2_node(char *file, const char *func, int line, ptn, ptn, ptn);
ptn new_L3_node(char *file, const char *func, int line, ptn);
ptn new_S1_node(char *file, const char *func, int line, ptn, ptn, ptn, ptn, ptn, ptn, ptn);
ptn new_S2_node(char *file, const char *func, int line, ptn, ptn, ptn);
ptn new_Ep1_node(char *file, const char *func, int line, ptn, ptn, ptn);
ptn new_Ep2_node(char *file, const char *func, int line, ptn);
ptn new_Tp1_node(char *file, const char *func, int line, ptn, ptn, ptn);
ptn new_Tp2_node(char *file, const char *func, int line, ptn);
ptn new_F1_node(char *file, const char *func, int line, ptn, ptn, ptn);
ptn new_F2_node(char *file, const char *func, int line, ptn);
ptn new_F3_node(char *file, const char *func, int line, ptn);

token_node check_token_node(ptn n, int expected_token, const char *parent);
epsilon_node check_epsilon_node(ptn n, const char *parent);
B_node check_B_node(ptn n, const char *parent);
L_node check_L_node(ptn n, const char *parent);
D_node check_D_node(ptn n, const char *parent);
S_node check_S_node(ptn n, const char *parent);
E_node check_E_node(ptn n, const char *parent);
Ep_node check_Ep_node(ptn n, const char *parent);
T_node check_T_node(ptn n, const char *parent);
Tp_node check_Tp_node(ptn n, const char *parent);
F_node check_F_node(ptn n, const char *parent);

ptn new_token_node(char *file, const char *func, int line, int token){
  FUNCCALL
  token_node n = (token_node) malloc(sizeof(struct token_node));
  n->tag = TAG_TOK;
  n->token = token;
  FUNCRET
  return (ptn) n;
}

token_node check_token_node(ptn n, int expected_token, const char *parent){
  if(n->tag == TAG_TOK){
    token_node t = (token_node) n;
    if(t->token == expected_token){
      return t;
    }else{
      char *s1 = token_to_string(expected_token);
      char *s2 = token_to_string(t->token);
      fprintf(stderr,
        "%s error, expected token node for '%s' but found token node for '%s' instead\n",
        parent, s1, s2);
      exit(1);
    }
  }else{
    char *s1 = token_to_string(expected_token);
    char *s2 = tag_to_string(n->tag);
    fprintf(stderr,
      "%s error, expected token node for '%s' but found %s node instead\n",
      parent, s1, s2);
    exit(1);
  }
}

ptn new_epsilon_node(char *file, const char *func, int line){
  FUNCCALL
  epsilon_node n = (epsilon_node) malloc(sizeof(struct epsilon_node));
  n->tag = TAG_EPS;
  FUNCRET
  return (ptn) n;
}

epsilon_node check_epsilon_node(ptn n, const char *parent){
  if(n->tag == TAG_EPS){
    return (epsilon_node) n;
  }else{
    parse_tree_internal_error_wrong_node("epsilon", n->tag, parent);
    return NULL;
  }
}

ptn new_B_node(char *file, const char *func, int line, ptn ob_node, ptn L_node, ptn cb_node){
  FUNCCALL
  B_node n = (B_node) malloc(sizeof(struct B_node));
  n->tag = TAG_B;
  n->ob = check_token_node(ob_node, '{', __func__);
  n->L = check_L_node(L_node, __func__);
  n->cb = check_token_node(cb_node, '}', __func__);
  FUNCRET
  return (ptn) n;
}

B_node check_B_node(ptn n, const char *parent){
  if(n->tag == TAG_B){
    return (B_node) n;
  }else{
    parse_tree_internal_error_wrong_node("B", n->tag, parent);
    return NULL;
  }
}

ptn new_L1_node(char *file, const char *func, int line, ptn D_node, ptn sc_node, ptn L_node){
  FUNCCALL
  L1_node n = (L1_node) malloc(sizeof(struct L1_node));
  n->tag = TAG_L1;
  n->D = check_D_node(D_node, __func__);
  n->sc = check_token_node(sc_node, ';', __func__);
  n->L = check_L_node(L_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_L2_node(char *file, const char *func, int line, ptn S_node, ptn sc_node, ptn L_node){
  FUNCCALL
  L2_node n = (L2_node) malloc(sizeof(struct L2_node));
  n->tag = TAG_L2;
  n->S = check_S_node(S_node, __func__);
  n->sc = check_token_node(sc_node, ';', __func__);
  n->L = check_L_node(L_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_L3_node(char *file, const char *func, int line, ptn e_node){
  FUNCCALL
  L3_node n = (L3_node) malloc(sizeof(struct L3_node));
  n->tag = TAG_L3;
  n->e = check_epsilon_node(e_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_L_node(char *file, const char *func, int line, ...){
  va_list nl;
  ptn n;
  int i = 0;

  FUNCCALL
  va_start(nl, line);
  n = va_arg(nl, ptn);
  while(n){
    i++;
    n = va_arg(nl, ptn);
  }
  va_end(nl);

  switch(i){
    case 1:{
      ptn n1;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_L3_node(__FILE__, __func__, __LINE__, n1);
      FUNCRET
      return n1;
    }
    case 3:{
      ptn n1, n2, n3;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      n2 = va_arg(nl, ptn);
      n3 = va_arg(nl, ptn);
      va_end(nl);
      if(n1->tag == TAG_D){
        n1 = new_L1_node(__FILE__, __func__, __LINE__, n1, n2, n3);
        FUNCRET
        return n1;
      }else{
        n1 = new_L2_node(__FILE__, __func__, __LINE__, n1, n2, n3);
        FUNCRET
        return n1;
      }
    }
    default:{
      parse_tree_internal_error_wrong_number("1 or 3", i, __func__);
      return NULL;
    }
  }
}

L_node check_L_node(ptn n, const char *parent){
  if(n->tag == TAG_L1 || n->tag == TAG_L2 || n->tag == TAG_L3){
    return (L_node) n;
  }else{
    parse_tree_internal_error_wrong_node("L", n->tag, parent);
    return NULL;
  }
}

ptn new_D_node(char *file, const char *func, int line, ptn int_node, ptn id_node, ptn eq_node, ptn E_node){
  FUNCCALL
  D_node n = (D_node) malloc(sizeof(struct D_node));
  n->tag = TAG_D;
  n->int_n = check_token_node(int_node, INT, __func__);
  n->id = check_token_node(id_node, ID, __func__);
  n->eq = check_token_node(eq_node, '=', __func__);
  n->E = check_E_node(E_node, __func__);
  FUNCRET
  return (ptn) n;
}

D_node check_D_node(ptn n, const char *parent){
  if(n->tag == TAG_D){
    return (D_node) n;
  }else{
    parse_tree_internal_error_wrong_node("D", n->tag, parent);
    return NULL;
  }
}

ptn new_S1_node(char *file, const char *func, int line, ptn if_node, ptn op_node, ptn E_node, ptn cp_node, ptn Bthen_node, ptn else_node, ptn Belse_node){
  FUNCCALL
  S1_node n = (S1_node) malloc(sizeof(struct S1_node));
  n->tag = TAG_S1;
  n->if_n = check_token_node(if_node, IF, __func__);
  n->op = check_token_node(op_node, '(', __func__);
  n->E = check_E_node(E_node, __func__);
  n->cp = check_token_node(cp_node, ')', __func__);
  n->Bthen = check_B_node(Bthen_node, __func__);
  n->else_n = check_token_node(else_node, ELSE, __func__);
  n->Belse = check_B_node(Belse_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_S2_node(char *file, const char *func, int line, ptn id_node, ptn eq_node, ptn E_node){
  FUNCCALL
  S2_node n = (S2_node) malloc(sizeof(struct S2_node));
  n->tag = TAG_S2;
  n->id = check_token_node(id_node, ID, __func__);
  n->eq = check_token_node(eq_node, '=', __func__);
  n->E = check_E_node(E_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_S_node(char *file, const char *func, int line, ...){
  va_list nl;
  ptn n;
  int i = 0;

  FUNCCALL
  va_start(nl, line);
  n = va_arg(nl, ptn);
  while(n){
    i++;
    n = va_arg(nl, ptn);
  }
  va_end(nl);

  switch(i){
    case 3:{
      ptn n1, n2, n3;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      n2 = va_arg(nl, ptn);
      n3 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_S2_node(__FILE__, __func__, __LINE__, n1, n2, n3);
      FUNCRET
      return n1;
    }
    case 7:{
      ptn n1, n2, n3, n4, n5, n6, n7;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      n2 = va_arg(nl, ptn);
      n3 = va_arg(nl, ptn);
      n4 = va_arg(nl, ptn);
      n5 = va_arg(nl, ptn);
      n6 = va_arg(nl, ptn);
      n7 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_S1_node(__FILE__, __func__, __LINE__, n1, n2, n3, n4, n5, n6, n7);
      FUNCRET
      return n1;
    }
    default:{
      parse_tree_internal_error_wrong_number("3 or 7", i, __func__);
      return NULL;
    }
  }
}

S_node check_S_node(ptn n, const char *parent){
  if(n->tag == TAG_S1 || n->tag == TAG_S2){
    return (S_node) n;
  }else{
    parse_tree_internal_error_wrong_node("S", n->tag, parent);
    return NULL;
  }
}

ptn new_E_node(char *file, const char *func, int line, ptn T_node, ptn Ep_node){
  FUNCCALL
  E_node n = (E_node) malloc(sizeof(struct E_node));
  n->tag = TAG_E;
  n->T = check_T_node(T_node, __func__);
  n->Ep = check_Ep_node(Ep_node, __func__);
  FUNCRET
  return (ptn) n;
}

E_node check_E_node(ptn n, const char *parent){
  if(n->tag == TAG_E){
    return (E_node) n;
  }else{
    parse_tree_internal_error_wrong_node("E", n->tag, parent);
    return NULL;
  }
}

ptn new_Ep1_node(char *file, const char *func, int line, ptn p_node, ptn T_node, ptn Ep_node){
  FUNCCALL
  Ep1_node n = (Ep1_node) malloc(sizeof(struct Ep1_node));
  n->tag = TAG_Ep1;
  n->p = check_token_node(p_node, '+', __func__);
  n->T = check_T_node(T_node, __func__);
  n->Ep = check_Ep_node(Ep_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_Ep2_node(char *file, const char *func, int line, ptn e_node){
  FUNCCALL
  Ep2_node n = (Ep2_node) malloc(sizeof(struct Ep2_node));
  n->tag = TAG_Ep2;
  n->e = check_epsilon_node(e_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_Ep_node(char *file, const char *func, int line, ...){
  va_list nl;
  ptn n;
  int i = 0;

  FUNCCALL
  va_start(nl, line);
  n = va_arg(nl, ptn);
  while(n){
    i++;
    n = va_arg(nl, ptn);
  }
  va_end(nl);

  switch(i){
    case 1:{
      ptn n1;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_Ep2_node(__FILE__, __func__, __LINE__, n1);
      FUNCRET
      return n1;
    }
    case 3:{
      ptn n1, n2, n3;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      n2 = va_arg(nl, ptn);
      n3 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_Ep1_node(__FILE__, __func__, __LINE__, n1, n2, n3);
      FUNCRET
      return n1;
    }
    default:{
      parse_tree_internal_error_wrong_number("1 or 3", i, __func__);
      return NULL;
    }
  }
}

Ep_node check_Ep_node(ptn n, const char *parent){
  if(n->tag == TAG_Ep1 || n->tag == TAG_Ep2){
    return (Ep_node) n;
  }else{
    parse_tree_internal_error_wrong_node("Ep", n->tag, parent);
    return NULL;
  }
}

ptn new_T_node(char *file, const char *func, int line, ptn F_node, ptn Tp_node){
  FUNCCALL
  T_node n = (T_node) malloc(sizeof(struct T_node));
  n->tag = TAG_T;
  n->F = check_F_node(F_node, __func__);
  n->Tp = check_Tp_node(Tp_node, __func__);
  FUNCRET
  return (ptn) n;
}

T_node check_T_node(ptn n, const char *parent){
  if(n->tag == TAG_T){
    return (T_node) n;
  }else{
    parse_tree_internal_error_wrong_node("T", n->tag, parent);
    return NULL;
  }
}

ptn new_Tp1_node(char *file, const char *func, int line, ptn m_node, ptn F_node, ptn Tp_node){
  FUNCCALL
  Tp1_node n = (Tp1_node) malloc(sizeof(struct Tp1_node));
  n->tag = TAG_Tp1;
  n->m = check_token_node(m_node, '*', __func__);
  n->F = check_F_node(F_node, __func__);
  n->Tp = check_Tp_node(Tp_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_Tp2_node(char *file, const char *func, int line, ptn e_node){
  FUNCCALL
  Tp2_node n = (Tp2_node) malloc(sizeof(struct Tp2_node));
  n->tag = TAG_Tp2;
  n->e = check_epsilon_node(e_node, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_Tp_node(char *file, const char *func, int line, ...){
  va_list nl;
  ptn n;
  int i = 0;

  FUNCCALL
  va_start(nl, line);
  n = va_arg(nl, ptn);
  while(n){
    i++;
    n = va_arg(nl, ptn);
  }
  va_end(nl);

  switch(i){
    case 1:{
      ptn n1;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_Tp2_node(__FILE__, __func__, __LINE__, n1);
      FUNCRET
      return n1;
    }
    case 3:{
      ptn n1, n2, n3;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      n2 = va_arg(nl, ptn);
      n3 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_Tp1_node(__FILE__, __func__, __LINE__, n1, n2, n3);
      FUNCRET
      return n1;
    }
    default:{
      parse_tree_internal_error_wrong_number("1 or 3", i, __func__);
      return NULL;
    }
  }
}

Tp_node check_Tp_node(ptn n, const char *parent){
  if(n->tag == TAG_Tp1 || n->tag == TAG_Tp2){
    return (Tp_node) n;
  }else{
    parse_tree_internal_error_wrong_node("Tp", n->tag, parent);
    return NULL;
  }
}

ptn new_F1_node(char *file, const char *func, int line, ptn op_node, ptn E_node, ptn cp_node){
  FUNCCALL
  F1_node n = (F1_node) malloc(sizeof(struct F1_node));
  n->tag = TAG_F1;
  n->op = check_token_node(op_node, '(', __func__);
  n->E = check_E_node(E_node, __func__);
  n->cp = check_token_node(cp_node, ')', __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_F2_node(char *file, const char *func, int line, ptn id_node){
  FUNCCALL
  F2_node n = (F2_node) malloc(sizeof(struct F2_node));
  n->tag = TAG_F2;
  n->id = check_token_node(id_node, ID, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_F3_node(char *file, const char *func, int line, ptn num_node){
  FUNCCALL
  F3_node n = (F3_node) malloc(sizeof(struct F3_node));
  n->tag = TAG_F3;
  n->num = check_token_node(num_node, NUM, __func__);
  FUNCRET
  return (ptn) n;
}

ptn new_F_node(char *file, const char *func, int line, ...){
  va_list nl;
  ptn n;
  int i = 0;

  FUNCCALL
  va_start(nl, line);
  n = va_arg(nl, ptn);
  while(n){
    i++;
    n = va_arg(nl, ptn);
  }
  va_end(nl);

  switch(i){
    case 1:{
      ptn n1;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      va_end(nl);
      if(((token_node) n1)->token == ID){
        n1 = new_F2_node(__FILE__, __func__, __LINE__, n1);
        FUNCRET
        return n1;
      }else{
        n1 = new_F3_node(__FILE__, __func__, __LINE__, n1);
        FUNCRET
        return n1;
      }
    }
    case 3:{
      ptn n1, n2, n3;
      va_start(nl, line);
      n1 = va_arg(nl, ptn);
      n2 = va_arg(nl, ptn);
      n3 = va_arg(nl, ptn);
      va_end(nl);
      n1 = new_F1_node(__FILE__, __func__, __LINE__, n1, n2, n3);
      FUNCRET
      return n1;
    }
    default:{
      parse_tree_internal_error_wrong_number("1 or 3", i, __func__);
      return NULL;
    }
  }
}

F_node check_F_node(ptn n, const char *parent){
  if(n->tag == TAG_F1 || n->tag == TAG_F2 || n->tag == TAG_F3){
    return (F_node) n;
  }else{
    parse_tree_internal_error_wrong_node("F", n->tag, parent);
    return NULL;
  }
}

/******************** Boxes ********************/

#define HFILL 2
#define VFILL 2

struct Box{
  int h, w;
  char **data;
};

typedef struct Box * Box;

Box malloc_Box(int h, int w){
  int i;
  Box b = (Box) malloc(sizeof(struct Box));
  b->h = h;
  b->w = w;
  b->data = (char **) malloc(h * sizeof(char *));
  for(i = 0; i < h; i++){
    b->data[i] = (char *) malloc(w);
    memset(b->data[i], ' ', w);
  }
  return b;
}

void free_Box(Box b){
  int i;
  for(i = 0; i < b->h; i++){
    free(b->data[i]);
  }
  free(b->data);
  free(b);
}

/* Deallocates the string after use! */
Box string_to_Box(char *s){
  int len = strlen(s);
  Box b = malloc_Box(1, len);
  memcpy(b->data[0], s, len);
  free(s);
  return b;
}

Box print_Box(Box b){
  int i, j;
  for(i = 0; i < b->h; i++){
    for(j = 0; j < b->w; j++){
      putchar(b->data[i][j]);
    }
    putchar('\n');
  }
  return b;
}

/* Copies the inner box at position (start_i, start_j) in the outer box */
void copy_Box(Box ob, int start_i, int start_j, Box ib){
  int i, j;
  for(i = 0; i < ib->h; i++){
    for(j = 0; j < ib->w; j++){
      ob->data[start_i + i][start_j + j] = ib->data[i][j];
    }
  }
}

#define MAX(a,b) (((a)>(b))?(a):(b))

/* Creates a big box that combines all the sub-boxes plus the root */
Box combine_Boxes(char *root, int n, ...){
  int i, j, k;
  int len;
  va_list bl;
  Box b, newb;
  int h = 0;
  int w = 0;
  int ws = 0;
  int wf = 0;
  int wl = 0;

  /* C doesn't have "apply" and the va list cannot be reified,
  ** so there is no way to move this loop to two other functions
  ** to compute the height and width elsewhere and separately.
  */
  va_start(bl, n);
  for(i = 0; i < n; i++){
    b = va_arg(bl, Box);
    h = MAX(h, b->h);
    w += b->w + HFILL;
    if(i == 0){
      wf = b->w;
    }
    if(i == n - 1){
      wl = b->w;
    }
  }
  va_end(bl);

  h += 3 + VFILL;
  len = strlen(root);
  ws = w - HFILL;
  w = MAX(w - HFILL, len);

  newb = malloc_Box(h, w);
  memcpy(&(newb->data[0][(w + 1 - len) / 2]), root, len);
  free(root);
  newb->data[1][w / 2] = '|';
  for(i = wf / 2; i < MAX(w / 2, ws - (wl / 2)); i++){
    newb->data[2][i] = '-';
  }
  newb->data[2][w / 2] = '+';

  va_start(bl, n);
  for(i = 0, j = 0; i < n; i++){
    b = va_arg(bl, Box);
    newb->data[2][j + (b->w / 2)] = '+';
    for(k = 3; k < 3 + VFILL; k++){
      newb->data[k][j + (b->w / 2)] = '|';
    }
    copy_Box(newb, 3 + VFILL, j, b);
    j += b->w + HFILL;
    free_Box(b);
  }
  va_end(bl);

  return newb;
}

Box ptn_to_Box(ptn n){
  switch(n->tag){
    case TAG_TOK:{
      token_node t = (token_node) n;
      return string_to_Box(token_to_string(t->token));
    }
    case TAG_EPS:{
      return string_to_Box(tag_to_string(n->tag));
    }
    case TAG_B:{
      B_node B = (B_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) B->ob),
                           ptn_to_Box((ptn) B->L),
                           ptn_to_Box((ptn) B->cb));
    }
    case TAG_L1:{
      L1_node L1 = (L1_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) L1->D),
                           ptn_to_Box((ptn) L1->sc),
                           ptn_to_Box((ptn) L1->L));
    }
    case TAG_L2:{
      L2_node L2 = (L2_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) L2->S),
                           ptn_to_Box((ptn) L2->sc),
                           ptn_to_Box((ptn) L2->L));
    }
    case TAG_L3:{
      L3_node L3 = (L3_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           1,
                           ptn_to_Box((ptn) L3->e));
    }
    case TAG_D:{
      D_node D = (D_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           4,
                           ptn_to_Box((ptn) D->int_n),
                           ptn_to_Box((ptn) D->id),
                           ptn_to_Box((ptn) D->eq),
                           ptn_to_Box((ptn) D->E));
    }
    case TAG_S1:{
      S1_node S1 = (S1_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           7,
                           ptn_to_Box((ptn) S1->if_n),
                           ptn_to_Box((ptn) S1->op),
                           ptn_to_Box((ptn) S1->E),
                           ptn_to_Box((ptn) S1->cp),
                           ptn_to_Box((ptn) S1->Bthen),
                           ptn_to_Box((ptn) S1->else_n),
                           ptn_to_Box((ptn) S1->Belse));
    }
    case TAG_S2:{
      S2_node S2 = (S2_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) S2->id),
                           ptn_to_Box((ptn) S2->eq),
                           ptn_to_Box((ptn) S2->E));
    }
    case TAG_E:{
      E_node E = (E_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           2,
                           ptn_to_Box((ptn) E->T),
                           ptn_to_Box((ptn) E->Ep));
    }
    case TAG_Ep1:{
      Ep1_node Ep1 = (Ep1_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) Ep1->p),
                           ptn_to_Box((ptn) Ep1->T),
                           ptn_to_Box((ptn) Ep1->Ep));
    }
    case TAG_Ep2:{
      Ep2_node Ep2 = (Ep2_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           1,
                           ptn_to_Box((ptn) Ep2->e));
    }
    case TAG_T:{
      T_node T = (T_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           2,
                           ptn_to_Box((ptn) T->F),
                           ptn_to_Box((ptn) T->Tp));
    }
    case TAG_Tp1:{
      Tp1_node Tp1 = (Tp1_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) Tp1->m),
                           ptn_to_Box((ptn) Tp1->F),
                           ptn_to_Box((ptn) Tp1->Tp));
    }
    case TAG_Tp2:{
      Tp2_node Tp2 = (Tp2_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           1,
                           ptn_to_Box((ptn) Tp2->e));
    }
    case TAG_F1:{
      F1_node F1 = (F1_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           3,
                           ptn_to_Box((ptn) F1->op),
                           ptn_to_Box((ptn) F1->E),
                           ptn_to_Box((ptn) F1->cp));
    }
    case TAG_F2:{
      F2_node F2 = (F2_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           1,
                           ptn_to_Box((ptn) F2->id));
    }
    case TAG_F3:{
      F3_node F3 = (F3_node) n;
      return combine_Boxes(tag_to_string(n->tag),
                           1,
                           ptn_to_Box((ptn) F3->num));
    }
    default:{
      parse_tree_internal_error_unknown_tag("ptn_to_Box", n->tag);
      return NULL;
    }
  }
}

void print_parse_tree(ptn n){
  free_Box(print_Box(ptn_to_Box(n)));
}
