/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: logname");
}

int
main(int argc, char *argv[])
{
	int opt;
	char *name;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 0)
		usage();

	if (!(name = getlogin()))
		die(1, "logname: getlogin:");
	puts(name);
}
