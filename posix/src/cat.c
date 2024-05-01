/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
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
	int fd, opt;

	while ((opt = getopt(argc, argv, "u")) != -1) {
		switch (opt) {
		case 'u':
			/* Since stdio isn't used, no buffering is going on anyway. */
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		if (copy_file(STDOUT_FILENO, STDIN_FILENO))
			die(1, "copy_file:");
		return 0;
	}

	for (; *argv; ++argv) {
		if (!strcmp(*argv, "-")) {
			fd = STDIN_FILENO;
		} else if ((fd = open(*argv, O_RDONLY)) < 0) {
			die(1, "open: %s:", *argv);
		}
		if (copy_file(STDOUT_FILENO, fd))
			die(1, "copy_file:");

		/* avoid closing stdin, stdout & stderr */
		if (fd > 2)
			x_close(fd);
	}
}
