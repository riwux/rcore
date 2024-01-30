/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

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
				/* TODO: implement \0xxx escape sequences */
			default:
				--str;
				break;
			}
		}
		*p++ = *str++;
	}
	*p = '\0';

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
		printf("%s", (eflag ? unescape(*argv) : *argv));

		if (*++argv)
			putchar(' ');
	}

	if (!nflag)
		putchar('\n');
}
