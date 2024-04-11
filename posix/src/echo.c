/* See LICENSE file for copyright and license details. */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static void
echo_unescape(char *str)
{
	bool cesc;
	size_t len;

	cesc = false;
	len  = unescape_num(str);

	for (size_t i = 0; i < len; ++i) {
		if (str[i] == '\\' && str[i+1] == 'c') {
			len = i;
			cesc = true;
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
