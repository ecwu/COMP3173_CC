/* Inside the program we represent tokens as integers.  It does not matter
** which integers are used for which tokens, as long as the lexer and the
** parser both agree on using the same integers for the same tokens.
** DO NOT USE THE INTEGERS 8107, 9770, etc., DIRECTLY IN YOUR CODE.
** INSTEAD USE THE MACRO NAMES LISTED BELOW: IF, ELSE, etc.
*/
#define IF   8107
#define ELSE 9770
#define INT  4772
#define ID   8325
#define NUM  5983

extern void raise_syntax_error(int expected_token, int next_input_token);
extern int consume_next_input_token(void);
extern int peek_at_next_input_token(void);
