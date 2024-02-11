/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


static size_t len;

static char *
unescape(char *str)
{
	size_t digs;
	char *p   = str;
	char *ret = str;

	char escape[] = {
		['a'] = '\a',
		['b'] = '\b',
		['f'] = '\f',
		['n'] = '\n',
		['r'] = '\r',
		['t'] = '\t',
		['v'] = '\v',
		['\\'] = '\\',
	};

	while (*str) {
		if (*str == '\\') {
			switch (*++str) {
			case '\\': /* unescape <backslash> characters i.e. \\ becomes \ */
			case 'a':
			case 'b':
			case 'f':
			case 'n':
			case 'r':
			case 't':
			case 'v':
				*p++ = escape[(uchar) *str++];
				continue;
			case 'c':
				fwrite(ret, sizeof(char), p - ret, stdout);
				exit(0);
				break;
			case '0':
				++str;
				if (!*str || !is_digit(*str)) {
					*p++ = '\0';
					continue;
				}
				*p++ = (char) to_num_base(str, OCT);
				digs = count_digits(str);
				str += (digs < 3) ? digs : 3;
				continue;
			default:
				--str;
				break;
			}
		}
		*p++ = *str++;
	}
	len = p - ret;

	return ret;
}

int
main(int argc, char *argv[])
{
	_Bool nflag = 0;
	_Bool eflag = 0;
	char *arg;

	while ((arg = *++argv) && *arg == '-') {
		while (*++arg) {
			switch (*arg) {
			case 'n':
				nflag = 1;
				break;
			case 'e':
				eflag = 1;
				break;
			case 'E':
				eflag = 0;
				break;
			default:
				nflag = eflag = 0;
				goto echo;
				break;
			}
		}
	}

echo:
	while (*argv) {
		len = strlen(*argv);
		arg = eflag ? unescape(*argv) : *argv;
		fwrite(arg, sizeof(char), len, stdout);

		if (*++argv)
			putchar(' ');
	}

	if (!nflag)
		putchar('\n');
}
