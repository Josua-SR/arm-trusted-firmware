#
# Copyright (C) 2019 Marvell International Ltd.
#
# SPDX-License-Identifier:     BSD-3-Clause
# https://spdx.org/licenses

SERVER = doimage_server
CLIENT = doimage_client
STANDALONE = doimage
SERVER_OBJECTS = server.o
CLIENT_OBJECTS = image.o client.o
STANDALONE_OBJECTS = doimage.o

HOSTCCFLAGS = -Wall -Werror
ifeq (${DEBUG},1)
  HOSTCCFLAGS += -g -O0 -DDEBUG
else
  HOSTCCFLAGS += -O2
endif

# TODO: always build with mbedTLS once it is updated on Docker to v2.8.0
ifeq (${MARVELL_SECURE_BOOT},1)
DOIMAGE_CC_FLAGS := -DCONFIG_MVEBU_SECURE_BOOT
DOIMAGE_LD_FLAGS := -lconfig -lmbedtls -lmbedcrypto -lmbedx509
endif

HOSTCCFLAGS += ${DOIMAGE_CC_FLAGS}
SERVER_LD_FLAGS := -lconfig -lmbedtls -lmbedcrypto -lmbedx509
CLIENT_LD_FLAGS := ${SERVER_LD_FLAGS}

# Make soft links and include from local directory otherwise wrong headers
# could get pulled in from firmware tree.
INCLUDE_PATHS = -I.

HOSTCC ?= gcc
RM := rm -rf
Q = @

.PHONY: all clean

all: ${STANDALONE}

server:
	@echo
	@echo "  Building Client-Server"
	@echo
	${Q}${MAKE} ${SERVER}
	${Q}${MAKE} ${CLIENT}

${STANDALONE}: ${STANDALONE_OBJECTS} Makefile
	@echo "  HOSTLD  $@"
	${Q}${HOSTCC} ${STANDALONE_OBJECTS} ${DOIMAGE_LD_FLAGS} -o $@
	@echo
	@echo "Built $@ successfully"
	@echo

${CLIENT}: ${CLIENT_OBJECTS} Makefile
	${Q}${HOSTCC} ${CLIENT_OBJECTS} ${CLIENT_LD_FLAGS} -o $@

${SERVER}: ${SERVER_OBJECTS} Makefile
	${Q}${HOSTCC} ${SERVER_OBJECTS} ${SERVER_LD_FLAGS} -o $@

%.o: %.c Makefile
	@echo "  HOSTCC  $<"
	${Q}${HOSTCC} -c ${HOSTCCFLAGS} ${INCLUDE_PATHS} $< -o $@

clean:
	${Q}${RM} ${CLIENT} ${SERVER} ${STANDALONE}
	${Q}${RM} ${CLIENT_OBJECTS} ${SERVER_OBJECTS} ${STANDALONE_OBJECTS}
