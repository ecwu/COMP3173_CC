#include <stddef.h> /* for NULL */

#define new_token_node(token) new_token_node(__FILE__, __func__, __LINE__, token)
#define new_epsilon_node() new_epsilon_node(__FILE__, __func__, __LINE__)

#define new_B_node(ob_node, L_node, cb_node) new_B_node(__FILE__, __func__, __LINE__, ob_node, L_node, cb_node)
#define new_L_node(...) new_L_node(__FILE__, __func__, __LINE__, __VA_ARGS__, NULL)
#define new_D_node(int_node, id_node, eq_node, E_node) new_D_node(__FILE__, __func__, __LINE__, int_node, id_node, eq_node, E_node)
#define new_S_node(...) new_S_node(__FILE__, __func__, __LINE__, __VA_ARGS__, NULL)
#define new_E_node(T_node, Ep_node) new_E_node(__FILE__, __func__, __LINE__, T_node, Ep_node)
#define new_Ep_node(...) new_Ep_node(__FILE__, __func__, __LINE__, __VA_ARGS__, NULL)
#define new_T_node(F_node, Tp_node) new_T_node(__FILE__, __func__, __LINE__, F_node, Tp_node)
#define new_Tp_node(...) new_Tp_node(__FILE__, __func__, __LINE__, __VA_ARGS__, NULL)
#define new_F_node(...) new_F_node(__FILE__, __func__, __LINE__, __VA_ARGS__, NULL)
