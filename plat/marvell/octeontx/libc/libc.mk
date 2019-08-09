# Copyright (C) 2019 Marvell International Ltd.
#
# SPDX-License-Identifier:     BSD-3-Clause
# https://spdx.org/licenses

LIBC_SRCS	:=	$(addprefix lib/libc/,		\
			abort.c				\
			assert.c			\
			exit.c				\
			memchr.c			\
			memcmp.c			\
			memcpy.c			\
			memmove.c			\
			memset.c			\
			putchar.c			\
			puts.c				\
			strchr.c			\
			strcmp.c			\
			strlcpy.c			\
			strlen.c			\
			strncmp.c			\
			strnlen.c			\
			strrchr.c)

LIBC_SRCS	+=	$(addprefix plat/marvell/octeontx/libc/,	\
			printf.c					\
			strncasecmp.c					\
			strtol.c)

ifeq (${ARCH},aarch64)
LIBC_SRCS	+=	$(addprefix lib/libc/aarch64/,	\
			setjmp.S)
endif

INCLUDES	+=	-Iinclude/lib/libc			\
			-Iinclude/lib/libc/$(ARCH)		\
			-Iinclude/plat/marvell/octeontx/libc	\
