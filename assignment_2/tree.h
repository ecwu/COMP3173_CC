typedef struct{
  int tag;
} * ptn;

#include "varif.h"

/* Functions to create parse tree nodes.  These functions are actually
** defined as macros (to simulate overloading), hence why the
** prototypes below are commented out, but these commented-out
** prototypes correctly tell you how to use the macros, so I have kept
** them here for reference.
*/
// extern ptn new_token_node(int);
// extern ptn new_epsilon_node(void);

// extern ptn new_B_node(ptn, ptn, ptn);

// extern ptn new_L_node(ptn, ptn, ptn);
// extern ptn new_L_node(ptn, ptn, ptn);
// extern ptn new_L_node(ptn);

// extern ptn new_D_node(ptn, ptn, ptn, ptn);

// extern ptn new_S_node(ptn, ptn, ptn, ptn, ptn, ptn, ptn);
// extern ptn new_S_node(ptn, ptn, ptn);

// extern ptn new_E_node(ptn, ptn);

// extern ptn new_Ep_node(ptn, ptn, ptn);
// extern ptn new_Ep_node(ptn);

// extern ptn new_T_node(ptn, ptn);

// extern ptn new_Tp_node(ptn, ptn, ptn);
// extern ptn new_Tp_node(ptn);

// extern ptn new_F_node(ptn, ptn, ptn);
// extern ptn new_F_node(ptn);
// extern ptn new_F_node(ptn);

/* You can use this function to help you debug your parse tree */
extern void print_parse_tree(ptn);
