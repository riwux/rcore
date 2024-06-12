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
 * base32 - encode/decode input with base32 and print it to stdout
 * ref: https://datatracker.ietf.org/doc/html/rfc4648
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: base32 [-d] [file]");
}

int
main(int argc, char *argv[])
{
	bool dflag = false;
	int opt, n, i, s;
	uint64_t val = 0;
	char buf[5];
	char const *const table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	FILE *fp;

	setup("base32", argv);
	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt) {
		case 'd':
			dflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 1)
		usage();
	fp = (argc == 0 || !strcmp(*argv, "-")) ? stdin : x_fopen(*argv, "r");

	if (dflag) {
		/* TODO: implement decoding */
		return 1;
	}

	while ((n = fread(&buf, sizeof (char), 5, fp)) > 0) {
		for (i = 0, s = 32; i < n; s -= 8, ++i)
			val |= (uint64_t)buf[i] << s;
		n = (n<4 ? n+2 : n+3);
		for (i = 0, s = 35; i < n; s -= 5, ++i)
			putchar(table[((val >> s) & 31)]); /* 2^5-1=31 extract 5 bits */
		val = 0;
	}
	for (i=8-i%8; i!=8 && i>0; --i)
		putchar('=');
	putchar('\n');
}
