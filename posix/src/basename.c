/* See LICENSE file for copyright and license details. */
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: basename path [suffix]");
}

int
main(int argc, char *argv[])
{
	int opt;
	char *base, *suffix;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1 && argc != 2)
		usage();

	base = basename(argv[0]);
	if (argc == 2) {
		suffix = (base + strlen(base)) - strlen(argv[1]);
		if (suffix != base && !strcmp(suffix, argv[1]))
			*suffix = '\0';
	}

	puts(base);
}
