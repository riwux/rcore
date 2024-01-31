/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: unlink file");
}

int
main(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage();

	if (unlink(*argv))
		die(1, "unlink: '%s':", *argv);
}
