/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: cat [-u] [file...]");
}

int
main(int argc, char *argv[])
{
	int opt;
	FILE *fp;

	while ((opt = getopt(argc, argv, "u")) != -1) {
		switch (opt) {
		case 'u':
			/* Default behavior. */
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		if (fcopy(stdout, stdin))
			die(1, "cat: fcopy:");
		return 0;
	}

	for (; *argv; ++argv) {
		fp = (!strcmp(*argv, "-")) ? stdin : x_fopen(*argv, "r");
		if (fcopy(stdout, fp))
			die(1, "cat: fcopy:");
		if (fp != stdin && fp != stdout && fp != stderr)
			x_fclose(fp);
	}
}
