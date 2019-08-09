/*
 * Copyright (c) 2014-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include <common/debug.h>

#define get_num_va_args(_args, _lcount)				\
	(((_lcount) > 1)  ? va_arg(_args, long long int) :	\
	(((_lcount) == 1) ? va_arg(_args, long int) :		\
			    va_arg(_args, int)))

#define get_unum_va_args(_args, _lcount)				\
	(((_lcount) > 1)  ? va_arg(_args, unsigned long long int) :	\
	(((_lcount) == 1) ? va_arg(_args, unsigned long int) :		\
			    va_arg(_args, unsigned int)))

#define MODE_PUTCH	0
#define MODE_STRN	1
#define MODE_STR	2

#define PUTCH(c, mode, s, n, count)					\
	do {								\
		if ((mode) == MODE_PUTCH) {				\
			(void)putchar(c);				\
			(count)++;					\
		} else if ((mode) == MODE_STR ||			\
			   ((mode) == MODE_STRN && (n) > 1)) {		\
			*(s)++ = c;					\
			(n)--;						\
			(count)++;					\
		}							\
	} while(0)

static int string_print(char **s, size_t *n, int mode, const char *str)
{
	int count = 0;

	assert(str != NULL);

	for ( ; *str != '\0'; str++) {
		PUTCH(*str, mode, *s, *n, count);
	}

	return count;
}

static int unsigned_num_print(char **s, size_t *n, int mode,
			      unsigned long long int unum, unsigned int radix,
			      char padc, int padn)
{
	/* Just need enough space to store 64 bit decimal integer */
	char num_buf[20];
	int i = 0, count = 0;
	unsigned int rem;

	do {
		rem = unum % radix;
		if (rem < 0xa)
			num_buf[i] = '0' + rem;
		else
			num_buf[i] = 'a' + (rem - 0xa);
		i++;
		unum /= radix;
	} while (unum > 0U);

	if (padn > 0) {
		while (i < padn) {
			PUTCH(padc, mode, *s, *n, count);
			padn--;
		}
	}

	while (--i >= 0) {
		PUTCH(num_buf[i], mode, *s, *n, count);
	}

	return count;
}

/*******************************************************************
 * Reduced format print for Trusted firmware.
 * The following type specifiers are supported by this print
 * %x - hexadecimal format
 * %s - string format
 * %d or %i - signed decimal format
 * %u - unsigned decimal format
 * %p - pointer format
 *
 * The following length specifiers are supported by this print
 * %l - long int (64-bit on AArch64)
 * %ll - long long int (64-bit on AArch64)
 * %z - size_t sized integer formats (64 bit on AArch64)
 *
 * The following padding specifiers are supported by this print
 * %0NN - Left-pad the number with 0s (NN is a decimal number)
 *
 * The print exits on all other formats specifiers other than valid
 * combinations of the above specifiers.
 *******************************************************************/
int kvnprintf(char *s, size_t n, int mode, const char *fmt, va_list args)
{
	int l_count;
	long long int num;
	unsigned long long int unum;
	char *str;
	char padc = '\0'; /* Padding character */
	int padn; /* Number of characters to pad */
	int count = 0; /* Number of printed characters */

	while (*fmt != '\0') {
		l_count = 0;
		padn = 0;

		if (*fmt == '%') {
			fmt++;
			/* Check the format specifier */
loop:
			switch (*fmt) {
			case 'i': /* Fall through to next one */
			case 'd':
				num = get_num_va_args(args, l_count);
				if (num < 0) {
					PUTCH('-', mode, s, n, count);
					unum = (unsigned long long int)-num;
					padn--;
				} else
					unum = (unsigned long long int)num;

				count += unsigned_num_print(&s, &n, mode, unum,
							    10, padc, padn);
				break;
			case 's':
				str = va_arg(args, char *);
				count += string_print(&s, &n, mode, str);
				break;
			case 'p':
				unum = (uintptr_t)va_arg(args, void *);
				if (unum > 0U) {
					count += string_print(&s, &n,
							      mode, "0x");
					padn -= 2;
				}

				count += unsigned_num_print(&s, &n, mode, unum,
							    16, padc, padn);
				break;
			case 'x':
				unum = get_unum_va_args(args, l_count);
				count += unsigned_num_print(&s, &n, mode, unum,
							    16, padc, padn);
				break;
			case 'z':
				if (sizeof(size_t) == 8U)
					l_count = 2;

				fmt++;
				goto loop;
			case 'l':
				l_count++;
				fmt++;
				goto loop;
			case 'u':
				unum = get_unum_va_args(args, l_count);
				count += unsigned_num_print(&s, &n, mode, unum,
							    10, padc, padn);
				break;
			case '0':
				padc = '0';
				padn = 0;
				fmt++;

				for (;;) {
					char ch = *fmt;
					if ((ch < '0') || (ch > '9')) {
						goto loop;
					}
					padn = (padn * 10) + (ch - '0');
					fmt++;
				}
				assert(0); /* Unreachable */
			default:
				/* Exit on any other format specifier */
				ERROR("kvnprintf: specifier with ASCII code '%d' not supported.",
				      *fmt);
				return -1;
			}
			fmt++;
			continue;
		}
		PUTCH(*fmt, mode, s, n, count);
		fmt++;
	}

	if (mode == MODE_STR || (mode == MODE_STRN && n > 0)) {
		*s = '\0';
	}

	return count;
}

int vprintf(const char *fmt, va_list args)
{
	return kvnprintf(NULL, 0 /* = n */, MODE_PUTCH, fmt, args);
}

int printf(const char *fmt, ...)
{
	int count;
	va_list va;

	va_start(va, fmt);
	count = vprintf(fmt, va);
	va_end(va);

	return count;
}

int vsnprintf(char *s, size_t n, const char *fmt, va_list args)
{
	return kvnprintf(s, n, MODE_STRN, fmt, args);
}

int snprintf(char *s, size_t n, const char *fmt, ...)
{
	int count;
	va_list va;

	va_start(va, fmt);
	count = vsnprintf(s, n, fmt, va);
	va_end(va);

	return count;
}

int vsprintf(char *s, const char *fmt, va_list args)
{
	return kvnprintf(s, 0 /* = n */, MODE_STR, fmt, args);
}

int sprintf(char *s, const char *fmt, ...)
{
	int count;
	va_list va;

	va_start(va, fmt);
	count = vsprintf(s, fmt, va);
	va_end(va);

	return count;
}

