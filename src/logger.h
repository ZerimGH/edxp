#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>
#include <stdarg.h>

void logger_init(const char *file_path, char *contents);
void logger_deinit(void);
void log_error(const char *fmt, ...);
void log_error_at(size_t line_no, size_t char_no, const char *fmt, ...);
void vlog_error(const char *fmt, va_list args);
void vlog_error_at(size_t line_no, size_t char_no, const char *fmt, va_list args);

#endif /* LOGGER_H */
