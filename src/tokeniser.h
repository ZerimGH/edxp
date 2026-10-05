#ifndef TOKENISER_H
#define TOKENISER_H

#include <stddef.h>

enum { TOKEN_SET, TOKEN_TO, TOKEN_IDENTIFIER, TOKEN_INTEGER_LITERAL, TOKEN_END };

struct token {
    int type;      /* Type of token */
    char *value;   /* Pointer to the start of the token in the source */
    size_t length; /* Length of the token's value */

    size_t line_no; /* The line the token is on */
    size_t char_no; /* The character within that line */
};

struct token *tokenise(char *src);

#endif /* TOKENISER_H */
