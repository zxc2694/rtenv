#include "utils.h"
#include "stm32f10x.h"
#include "RTOSConfig.h"
#include "stdarg.h"

int strcmp(const char *a, const char *b) __attribute__ ((naked));
int strcmp(const char *a, const char *b)
{
	asm(
        "strcmp_lop:                \n"
        "   ldrb    r2, [r0],#1     \n"
        "   ldrb    r3, [r1],#1     \n"
        "   cmp     r2, #1          \n"
        "   it      hi              \n"
        "   cmphi   r2, r3          \n"
        "   beq     strcmp_lop      \n"
		"	sub     r0, r2, r3  	\n"
        "   bx      lr              \n"
		:::
	);
}

size_t strlen(const char *s) __attribute__ ((naked));
size_t strlen(const char *s)
{
	asm(
		"	sub  r3, r0, #1			\n"
        "strlen_loop:               \n"
		"	ldrb r2, [r3, #1]!		\n"
		"	cmp  r2, #0				\n"
        "   bne  strlen_loop        \n"
		"	sub  r0, r3, r0			\n"
		"	bx   lr					\n"
		:::
	);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char *_s1 = (void *)s1, *_s2 = (void *)s2;

	if(!n--) {
		return 0;
	}

	for(; *_s1 && *_s2 && n && *_s1 == *_s2; _s1++, _s2++, n--);

	return *_s1 - *_s2;
}

void putchar(const char c) 
{
	int fdout;
	char tmpstr[2] = {'\0', '\0'};

	fdout = mq_open("/tmp/mqueue/out", 0);

	tmpstr[0] = c;

	write(fdout, tmpstr, 2);
}


void puts(char *s)
{
	int fdout;    

	fdout = mq_open("/tmp/mqueue/out", 0);

	write(fdout, s, strlen(s) + 1);
}

void int2str(int input, char *output) 
{
	char tmp[16];
	int num_len = 0, i;

	if(input == 0) {
	    output[0] = '0';
	    output[1] = '\0';
	    return;
	}

	while(input > 0) {
		tmp[num_len++] = '0' + (input % 10);
		input /= 10;
	}

	for(i = 0; i < num_len; i++) {
	    output[i] = tmp[num_len - i - 1];
	}
	output[num_len] = '\0';
}

void printf(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);
	vprintf(format, arg);
	va_end(arg);
}

void vprintf(const char *format, va_list va)
{
	int mode = 0; // 0: usual char; 1: specifiers
	char tmpstr[16];

	for(; *format; format++) {
		if(!mode) { // usual char
			if(*format == '%') {
				mode = 1;
				continue;
			}
			else if(*format == '\n') {
				putchar('\r');
			}
			putchar(*format);
		}
		else {
			switch(*format) {
				case 'c':
					putchar(va_arg(va, int));
					mode = 0;
					break;
				case 's':
					puts(va_arg(va, char *));
					mode = 0;
					break;
				case 'd':
					int2str(va_arg(va, int), tmpstr);
					puts(tmpstr);
					mode = 0;
					break;
				case '%':
					putchar('%');
					mode = 0;
					break;
			}
		}
	}
}
