/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


static void
echo_unescape(char *str)
{
	_Bool cesc;
	size_t len;

	cesc = 0;
	len  = unescape_num(str);

	for (size_t i = 0; i < len; ++i) {
		if (str[i] == '\\' && str[i+1] == 'c') {
			len = i;
			cesc = 1;
		}
	}
	fwrite(str, len, sizeof(char), stdout);

	if (cesc)
		exit(0);
}

int
main(int argc, char *argv[])
{
	(void) argc;

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
