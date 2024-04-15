/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: mkfifo [-m mode] file...");
}

int
main(int argc, char *argv[])
{
	int opt;
	mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;

	while ((opt = getopt(argc, argv, "m:")) != -1) {
		switch (opt) {
		case 'm':
			mode = parse_mode(optarg);
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	for (; *argv; ++argv) {
		if (mkfifo(*argv, mode))
			eprintf("mkfifo: cannot create fifo '%s': %s\n", *argv, \
			    strerror(errno));
	}
}
