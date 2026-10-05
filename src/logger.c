#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BOLD "\033[1m"
#define RED "\033[1;31m"
#define NONE "\033[1;0m"

struct logger {
    const char *file_path;
    char *contents;
} logger;

void logger_init(const char *file_path, char *contents) {
    logger.file_path = file_path;
    logger.contents  = contents;
}

void logger_deinit(void) { logger = (struct logger){0}; }

static void context(size_t line_no, size_t char_no, size_t length) {
    if (!logger.contents) return;

    /* Print error line */
    char *line = logger.contents;
    for (size_t i = 1; i < line_no; i++) {
        line = strchr(line, '\n');
        if (!line) return;
    }

    char *next = strchr(line, '\n');
    char old;
    if (next) {
        old   = *next;
        *next = '\0';
    }

    if (next) *next = old;

    fprintf(stderr, "%6zu | %s\n", line_no, line);

    /* Print underline */
    if (length) {
        fprintf(stderr, "       | ");
        fprintf(stderr, RED);
        for (size_t i = 1; i < char_no; i++) fprintf(stderr, " ");
        fprintf(stderr, "^");
        for (size_t i = 1; i < length; i++) fprintf(stderr, "~");
        fprintf(stderr, "\n");
        fprintf(stderr, NONE);
    }
}

void log_error(const char *fmt, ...) {
    fprintf(stderr, BOLD "%s: " NONE RED "error: " NONE, logger.file_path);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void log_error_at(size_t line_no, size_t char_no, const char *fmt, ...) {
    fprintf(stderr,
        BOLD "%s:%zu:%zu: " NONE RED "error: " NONE,
        logger.file_path,
        line_no,
        char_no);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    context(line_no, char_no, 1);
}

void vlog_error(const char *fmt, va_list args) {
    fprintf(stderr, BOLD "%s: " NONE RED "error: " NONE, logger.file_path);
    vfprintf(stderr, fmt, args);
}

void vlog_error_at(
    size_t line_no, size_t char_no, const char *fmt, va_list args) {
    fprintf(stderr,
        BOLD "%s:%zu:%zu: " NONE RED "error: " NONE,
        logger.file_path,
        line_no,
        char_no);
    vfprintf(stderr, fmt, args);
    context(line_no, char_no, 1);
}
