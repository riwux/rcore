/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(FAILURE, "usage: cat [-u] [file...]");
}

int
main(int argc, char *argv[])
{
	int fd;
	char *arg;

	if (argc == 1) {
		if (copy_file(STDOUT_FILENO, STDIN_FILENO))
			die(FAILURE, "copy_file:");
		return 0;
	}

	while (*++argv) {
		arg = *argv;
		if (arg[0] == '-') {
			switch (arg[1]) {
			case 'u':
				break;
			case '\0':
				fd = STDIN_FILENO;
				break;
			default:
				usage();
				break;
			}
		} else if ((fd = open(*argv, O_RDONLY)) < 0) {
			die(FAILURE, "open: %s:", *argv);
		}
		if (copy_file(STDOUT_FILENO, fd))
			die(FAILURE, "copy_file:");

		/* avoid closing stdin, stdout & stderr */
		if (fd > 2)
			xclose(fd);
	}
}
