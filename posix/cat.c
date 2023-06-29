/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die("usage: cat [-u] [file...]\n");
}

static void
cat(int fd)
{
	Buf *buf = buf_create(BUFSIZ);

	for (ssize_t n = get_line(fd, buf); n > 0; n = get_line(fd, buf)) {
		fputs(buf->data, stdout);
	}
}

int
main(int argc, char *argv[])
{
	int fd;
	char *arg;

	if (argc == 1) {
		cat(STDIN_FILENO);
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
		cat(fd);

		/* avoid closing stdin, stdout & stderr */
		if (fd >= 0 && fd <= 2)
			close(fd);
	}
}
