#ifndef TOKENISER_H
#define TOKENISER_H

#include <stddef.h>

enum {
    TOKEN_SET,
    TOKEN_TO,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_END,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_REPEAT,
    TOKEN_UNTIL,
    TOKEN_TIMES,
    TOKEN_FOR,
    TOKEN_FROM,
    TOKEN_STEP,
    TOKEN_EACH,
    TOKEN_FOREACH,
    TOKEN_SEND,
    TOKEN_RECEIVE,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,
    TOKEN_EOF
};

struct token {
    int type;      /* Type of token */
    char *value;   /* Pointer to the start of the token in the source */
    size_t length; /* Length of the token's value */

    size_t line_no; /* The line the token is on */
    size_t char_no; /* The character within that line */
};

struct token *tokenise(char *src);

#endif /* TOKENISER_H */
