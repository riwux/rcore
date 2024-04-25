/* See LICENSE file for copyright and license details. */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static void
echo_unescape(char const *str)
{
	char c;
	int i;
	char num[4];

	while ((c = *str)) {
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
				for (i=0; i<2 && is_hexdigit(*str); ++i, ++str)
					num[i] = *str;
				c = to_num(num, HEX);
				break;
			case '0':
				for (i=0; i<4 && is_octdigit(*str); ++i, ++str)
					num[i] = *str;
				c = to_num(num, OCT);
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
	(void) argc;

	bool nflag = false;
	bool eflag = false;
	char *arg;

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
				nflag = eflag = false;
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
