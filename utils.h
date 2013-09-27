#ifndef _H_UTILS_
#define _H_UTILS_
#include <stddef.h>
#include "stdarg.h"

void *memcpy(void *dest, const void *src, size_t n);

int strcmp(const char *a, const char *b);
size_t strlen(const char *s);
void putchar(const char c);
void puts(char *s);
void int2str(int input, char *output);
int strncmp(const char *s1, const char *s2, size_t n);

void printf(const char *format, ...);
void vprintf(const char *format, va_list arg);

#endif
