/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: clear");
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

	if (argc > 1)
		usage();

	fputs("\x1b[2J\x1b[H", stdout);
}
