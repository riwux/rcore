/*
 * Copyright 2023-2024 Tom Schwindl <schwindl@posteo.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS
 * IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * ascii - print ASCII table or obtain ASCII value of input characters
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: ascii [-xdo] [string...]");
}

static void
print_chart(char const base)
{
	char const fmt[]    = {'|', '%', '.', (base == 'x') ? '2' : '3', base, \
	    ' ', ' ', '%', 'c', ' ', '\0'};
	char const ctrlfmt[] = {'|', '%', '.', (base == 'x') ? '2' : '3', base, \
	    ' ', '%', '-', '3', 's', '\0'};
	char const *const ctrl[] = {
		"nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
		"bs",  "ht",  "nl",  "vt",  "np",  "cr",  "so",  "si",
		"dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
		"can", "em",  "sub", "esc", "fs",  "gs",  "rs",  "us",
		"sp", "del"
	};

	for (uchar c = 0; c < 128; ++c) {
		if (c != 0 && c % 8 == 0)
			printf("|\n");
		if (c < 33)
			printf(ctrlfmt, c, ctrl[c]);
		else if (c == 127)
			printf(ctrlfmt, c, ctrl[33]);
		else
			printf(fmt, c, c);
	}
	printf("|\n");
}

static void
print_ascii_code(char const *arg, char const base)
{
	char const fmt[] = {'%', base, '\n', '\0'};

	for (; *arg; ++arg)
		printf(fmt, *arg);
}

int
main(int argc, char *argv[])
{
	char base = 'x';
	int opt;

	setup("ascii", argv);
	while ((opt = getopt(argc, argv, "xdo")) != -1) {
		switch (opt) {
		case 'x':
		case 'd':
		case 'o':
			base = opt;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		print_chart(base);
	} else {
		for (; *argv; ++argv)
			print_ascii_code(*argv, base);
	}
}
