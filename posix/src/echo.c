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
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/echo.html
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static void
echo_unescape(char const *str)
{
	char num[4];

	for (char c; (c = *str);) {
		if (*str == '\\' && str[1]) {
			num[0] = num[1] = num[2] = num[3] = 0;
			switch (*++str) {
			case 'c':
				exit(0);
				break;
			case 'x':
				if (!is_hexdigit(*++str)) {
					--str;
					break;
				}
				for (int i = 0; i < 2 && is_hexdigit(*str); ++i, ++str)
					num[i] = *str;
				c = x_to_num(num, HEX);
				break;
			case '0':
				for (int i = 0; i < 4 && is_octdigit(*str); ++i, ++str)
					num[i] = *str;
				c = x_to_num(num, OCT);
				break;
			case 'a': case 'b': case 'f': case 'n':
			case 'r': case 't': case 'v': case '\\':
				c = unescape(str++);
				break;
			default:
				c = '\\';
				break;
			}
		} else {
			++str;
		}
		putchar(c);
	}
}

int
main(int argc, char *argv[])
{
	(void)argc;

	bool nflag = false;
	bool eflag = false;
	char const *arg;

	x_setup("echo", argv);
	while ((arg = *++argv) && *arg == '-') {
		while (*++arg) {
			switch (*arg) {
			case 'n':
				nflag = true;
				break;
			case 'e':
				eflag = true;
				break;
			case 'E':
				eflag = false;
				break;
			default:
				goto echo;
				break;
			}
		}
	}

echo:
	while (*argv) {
		if (eflag)
			echo_unescape(*argv);
		else
			fputs(*argv, stdout);
		if (*++argv)
			putchar(' ');
	}
	if (!nflag)
		putchar('\n');
}
