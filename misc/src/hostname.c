/* See LICENSE file for copyright and license details. */
#define _BSD_SOURCE /* sethostname(2) */

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: hostname [name]");
}

int
main(int argc, char *argv[])
{
	int opt;
	char hostname[HOST_NAME_MAX+1];

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	switch (argc) {
	case 0:
		if (gethostname(hostname, HOST_NAME_MAX))
			die(1, "gethostname:");
		puts(hostname);
		break;
	case 1:
		if (sethostname(*argv, strlen(*argv)))
			die(1, "sethostname:");
		break;
	default:
		usage();
		break;
	}
}
