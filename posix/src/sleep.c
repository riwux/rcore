/* See LICENSE file for copyright and license details. */
#include <limits.h>
#include <math.h>
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

static _Bool
fill_timespec(struct timespec *time, char *str)
{
	_Bool nosec = 0;
	uint_least8_t zeroes = 0;
	uint_least64_t mul   = 100000000;
	size_t len;
	char *p, *dot;

	time->tv_sec = time->tv_nsec = 0;

	dot = strchr(str, '.');
	if (str == dot)
		nosec = 1;

	if (dot && dot[1]) {
		*dot++ = '\0';
		p = dot;

		for (; *dot == '0'; ++dot)
			++zeroes;

		for (; *p; ++p) {
			if (!IS_DIGIT(*p))
				return 1;
		}

		len = strnlen(dot, 9);
		dot[len] = '\0'; /* stop at nanoseconds or earlier */
		mul /= pow(10, zeroes + len-1);

		time->tv_nsec = to_num(dot) * mul;
	}

	if (!nosec)
		time->tv_sec = to_num(str);

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

	if (fill_timespec(&time, *argv))
		die(2, "sleep: invalid number format");

	return !!nanosleep(&time, NULL);
}
