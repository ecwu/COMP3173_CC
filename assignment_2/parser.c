#include "tokens.h"
#include "tree.h"
#include "varim.h"
#include "parser.h"

ptn match_token(int expected_token){
    int next_input_token = consume_next_input_token();
    if(expected_token == next_input_token){
        return new_token_node(expected_token);
    }else{
        raise_syntax_error(expected_token, next_input_token); 
    }
}
ptn match_epsilon(void){
    return new_epsilon_node();
}
ptn match_B(void){
    ptn ob_char_node = match_token('{');
    ptn L_node = match_L();
    ptn cb_char_node = match_token('}');
    return new_S_node(ob_char_node, L_node, cb_char_node);
}
ptn match_L(void){
    int lookahead_token = peek_at_next_input_token();
    if (lookahead_token == INT){
        ptn D_node = match_D();
        ptn sc_char_node = match_token(';');
        ptn L_node = match_L();
        return new_L_node(D_node, sc_char_node, L_node);
    } else if (lookahead_token == IF) {
        ptn S_node = match_S();
        ptn sc_char_node = match_token(';');
        ptn L_node = match_L();
        return new_L_node(S_node, sc_char_node, L_node);
    } else {
        ptn epsilon_node = match_epsilon();
        return new_L_node(epsilon_node);
    }
}
ptn match_D(void){
    ptn INT_token_node = match_token(INT);
    ptn ID_token_node = match_token(ID);
    ptn eq_char_node = match_token('=');
    ptn E_node = match_E();
    return new_D_node(INT_token_node, ID_token_node, eq_char_node, E_node);
}
ptn match_S(void){
    int lookahead_token = peek_at_next_input_token();
    if (lookahead_token == IF){
        ptn IF_token_node = match_token(IF);
        ptn op_char_node = match_token('(');
        ptn E_node = match_E();
        ptn cp_char_node = match_token(')');
        ptn B_node_1 = match_B();
        ptn ELSE_token_node = match_token(ELSE);
        ptn B_node_2 = match_B();
        return new_S_node(IF_token_node, op_char_node, E_node, cp_char_node, B_node_1, ELSE_token_node, B_node_2);
    } else {
        ptn ID_token_node = match_token(ID);
        ptn eq_char_node = match_token('=');
        ptn E_node = match_E();
        return new_S_node(ID_token_node, eq_char_node, E_node);
    }
}
ptn match_E(void){
    ptn T_node = match_T();
    ptn Ep_node = match_Ep();
    return new_E_node(T_node, Ep_node);
}
ptn match_Ep(void){
    int lookahead_token = peek_at_next_input_token();
    if (lookahead_token == '+'){
        ptn p_char_node = match_token('+');
        ptn T_node = match_T();
        ptn Ep_node = match_Ep();
        return new_Ep_node(p_char_node, T_node, Ep_node);
    } else {
        ptn epsilon_node = match_epsilon();
        return new_Ep_node(epsilon_node);
    }
    
}
ptn match_T(void){
    ptn F_node = match_F();
    ptn Tp_node = match_Tp();
    return new_T_node(F_node, Tp_node);
}
ptn match_Tp(void){
    int lookahead_token = peek_at_next_input_token();
    if (lookahead_token == '*'){
        ptn m_char_node = match_token('*');
        ptn F_node = match_F();
        ptn Tp_node = match_Tp();
        return new_Tp_node(m_char_node, F_node, Tp_node);
    } else {
        ptn epsilon_node = match_epsilon();
        return new_Tp_node(epsilon_node);
    }
}
ptn match_F(void){
    int lookahead_token = peek_at_next_input_token();
    if (lookahead_token == '(') {
        ptn op_char_node = match_token('(');
        ptn E_node = match_E();
        ptn cp_char_node = match_token(')');
        return new_F_node(op_char_node, E_node, cp_char_node);
    } else if (lookahead_token == ID) {
        ptn ID_token_node = match_token(ID);
        return new_F_node(ID_token_node);
    } else if (lookahead_token == NUM){
        ptn NUM_token_node = match_token(NUM);
        return new_F_node(NUM_token_node);
    }
}
