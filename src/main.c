#include "tokeniser.h"
#include "logger.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    const char * src = "SET Number TO -100";

    logger_init("NOFILE", (char *)src);

    struct token *tokens = tokenise((char *)src);
    if (!tokens) {
        fprintf(stderr, "Failed to tokenise source\n");
        return 1;
    }

    struct token *cur = tokens;
    while (cur->type != TOKEN_EOF) {
        char buf[1024] = {0};
        strncpy(buf, cur->value, cur->length);
        printf("type = %-12d | value = %-12s | length = %-12zu | line_no = %-12zu | char_no = %-12zu\n", cur->type, buf, cur->length, cur->line_no, cur->char_no);
        cur++;
    }

    free(tokens);
    logger_deinit();

    return 0;
}
