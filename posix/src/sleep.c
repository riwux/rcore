/* See LICENSE file for copyright and license details. */
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: sleep time");
}

static int
parse_time(struct timespec *time, char const *str)
{
	bool nosec   = false;
	uint64_t mul = 1000000000;
	char *dot;
	char const *p;

	time->tv_sec  = 0;
	time->tv_nsec = 0;
	dot = strchr(str, '.');
	if (str == dot)
		nosec = true;

	if (dot && dot[1]) {
		*dot++ = '\0';
		p = dot;
		for (size_t i = 0; p[i]; ++i) {
			if (!is_digit(p[i]))
				return -1;
			else if (i < 9 && *dot++)
				mul /= 10;
		}
		*dot = '\0';
		time->tv_nsec = x_to_num(p, DEC) * mul;
	}
	if (!nosec)
		time->tv_sec = x_to_num(str, DEC);

	return 0;
}

int
main(int argc, char *argv[])
{
	int opt;
	struct timespec time;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage();

	if (parse_time(&time, *argv))
		die(2, "sleep: invalid number format");

	return !!nanosleep(&time, NULL);
}
