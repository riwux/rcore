/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


static size_t len;

static inline int_least32_t
count_digits(char *str)
{
	int_least32_t n = 0;

	for (int i = 3; i && IS_DIGIT(*str); ++str, ++n, --i)
		;

	return n;
}

static char *
unescape(char *str)
{
	char *p   = str;
	char *ret = str;

	char escape[] = {
		['a'] = '\a',
		['b'] = '\b',
		['c'] = '\0',
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
			case 'c':
			case 'f':
			case 'n':
			case 'r':
			case 't':
			case 'v':
				*p++ = escape[(unsigned char) *str++];
				continue;
			case '0':
				++str;
				if (!*str || (!IS_DIGIT(*str) && *str)) {
					*p++ = '\0';
					continue;
				}
				*p++ = (char) to_num_base(str, OCT);
				str += count_digits(str);
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
