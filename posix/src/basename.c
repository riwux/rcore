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

	if (argc != 2 && argc != 3)
		usage();

	base = basename(argv[1]);

	if (argc == 3) {
		suffix = (base + strlen(base)) - strlen(argv[2]);
		if (suffix != base && !strcmp(suffix, argv[2]))
			*suffix = '\0';
	}

	puts(base);
}
