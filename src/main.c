#include "tokeniser.h"
#include "logger.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    const char *src = "SET Number TO -100";

    logger_init("NOFILE", (char *)src);

    struct token *tokens = tokenise((char *)src);
    if (!tokens) {
        fprintf(stderr, "Failed to tokenise source\n");
        return 1;
    }

    struct token *cur = tokens;
    while (cur->type != TOKEN_END) {
        char buf[1024] = {0};
        strncpy(buf, cur->value, cur->length);
        printf("%d | %s | %zu\n", cur->type, buf, cur->length);
        cur++;
    }

    free(tokens);
    logger_deinit();

    return 0;
}
