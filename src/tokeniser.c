#include "tokeniser.h"
#include "logger.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t line_no;
static size_t char_no;

static void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_error_at(line_no, char_no, fmt, args);
    va_end(args);
}

static int ends_token(char c) {
    /* TODO: Operators should also end a token */
    return isspace(c);
}

struct token tokenise_keyword(char *src) {
    struct token res = {0};
    if (!src || !*src) return res;

    const struct {
        int type;
        const char *value;
    } keywords[] = {{TOKEN_SET, "SET"}, {TOKEN_TO, "TO"}};

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        int type          = keywords[i].type;
        const char *value = keywords[i].value;
        size_t length     = strlen(value);

        if (strncmp(value, src, length) == 0) {
            if (!ends_token(src[length])) continue;
            res.type   = type;
            res.value  = src;
            res.length = length;
            return res;
        }
    }

    return res;
}

static int is_identifier_char(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    if (c == '_') return 1;
    return 0;
}

struct token tokenise_identifier(char *src) {
    struct token res = {0};
    if (!src || !*src) return res;

    char *test    = src;
    size_t length = 0;
    char c;
    while ((c = *test++)) {
        if (ends_token(c)) break;
        if (!is_identifier_char(c)) return res;
        length++;
    }

    if (length == 0) return res;

    res.type   = TOKEN_IDENTIFIER;
    res.value  = src;
    res.length = length;
    return res;
}

struct token tokenise_integer_literal(char *src) {
    struct token res = {0};
    if (!src || !*src) return res;

    int negative = *src == '-';

    char *test = negative ? src + 1 : src;
    size_t length = negative ? 1 : 0;
    char c;
    while ((c = *test++)) {
        if (ends_token(c)) break;
        if (!isdigit(c)) return res;
        length++;
    }

    if (length == 0 || (negative && length == 1)) return res;
    
    res.type = TOKEN_INTEGER_LITERAL;
    res.value = src;
    res.length = length;
    return res;
}

struct token *tokenise(char *src) {
    if (!src) return NULL;

    size_t cap        = 1024;
    size_t count      = 0;
    struct token *buf = malloc(sizeof(struct token) * cap);
    if (!buf) {
        fprintf(stderr, "malloc() failed\n");
        return NULL;
    }

    line_no = 1;
    char_no = 1;

    char c;
    while ((c = *src)) {
        /* Skip whitespace */
        if (isspace(c)) {
            if (c == '\n') {
                line_no++;
                char_no = 1;
            } else char_no++;
            src++;
            continue;
        }

        struct token (*tokenise_funcs[])(char *) = {
            tokenise_keyword, tokenise_identifier, tokenise_integer_literal};

        struct token longest = {0};

        for (size_t i = 0; i < sizeof(tokenise_funcs) / sizeof(tokenise_funcs[0]);
            i++) {
            struct token local = tokenise_funcs[i](src);
            if (local.length > longest.length) longest = local;
        }

        if (longest.length == 0 || longest.value == NULL) {
            error("No valid token found\n");
            free(buf);
            return NULL;
        }

        if (count + 1 >= cap) { /* + 1 for TOKEN_END */
            cap *= 2;
            struct token *new = realloc(buf, sizeof(struct token) * cap);
            if (!new) {
                free(buf);
                fprintf(stderr, "realloc() failed\n");
                return NULL;
            }
            buf = new;
        }

        longest.line_no = line_no;
        longest.char_no = char_no;

        buf[count++] = longest;
        src += longest.length;
        char_no += longest.length;
    }

    buf[count] = (struct token){.type = TOKEN_END};

    return buf;
}
