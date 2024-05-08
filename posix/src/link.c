/* See LICENSE file for copyright and license details. */
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: link old new");
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

	if (argc != 2)
		usage();

	if (link(argv[0], argv[1]))
		die(1, "link: cannot create link '%s' to '%s':", argv[1], argv[0]);
}
