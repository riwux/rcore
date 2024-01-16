/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(2, "usage: tty");
}

int
main(int argc, char *argv[])
{
	int opt;
	char *tname;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}

	if (argc != 1)
		usage();

	tname = ttyname(STDIN_FILENO);
	puts(tname ? tname : "not a tty");

	return !tname;
}
