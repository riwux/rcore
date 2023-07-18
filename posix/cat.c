/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
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
	ssize_t n;
	Buf *buf = buf_create(BUFLEN);

	while ((n = read(fd, buf->data, buf->size)) > 0)
		xwrite_all(STDOUT_FILENO, buf->data, n);
	if (n == -1)
		die("read:");

	buf_free(buf);
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
		if (fd > 2)
			close(fd);
	}
}
