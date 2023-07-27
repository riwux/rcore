/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die("usage: cat [-u] [file...]\n");
}

int
main(int argc, char *argv[])
{
	int fd;
	char *arg;

	if (argc == 1) {
		if (output_file(STDIN_FILENO))
			die("output_file:");
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
			die("open:");
		}
		if (output_file(fd))
			die("output_file:");

		/* avoid closing stdin, stdout & stderr */
		if (fd > 2)
			close(fd);
	}
}
