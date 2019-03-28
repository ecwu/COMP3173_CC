extern ptn new_token_node(char *file, const char *func, int line, int token);
extern ptn new_epsilon_node(char *file, const char *func, int line);

extern ptn new_B_node(char *file, const char *func, int line, ptn, ptn, ptn);
extern ptn new_L_node(char *file, const char *func, int line, ...);
extern ptn new_D_node(char *file, const char *func, int line, ptn, ptn, ptn, ptn);
extern ptn new_S_node(char *file, const char *func, int line, ...);
extern ptn new_E_node(char *file, const char *func, int line, ptn, ptn);
extern ptn new_Ep_node(char *file, const char *func, int line, ...);
extern ptn new_T_node(char *file, const char *func, int line, ptn, ptn);
extern ptn new_Tp_node(char *file, const char *func, int line, ...);
extern ptn new_F_node(char *file, const char *func, int line, ...);
