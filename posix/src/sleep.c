/*
 * Copyright 2023-2024 Tom Schwindl <schwindl@posteo.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS
 * IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sleep.html
 */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: sleep time");
}

static int
parse_time(struct timespec *const time, char *const str)
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

	setup("sleep", argv);
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
		die(2, "%s: invalid number format", prog_);

	return !!nanosleep(&time, NULL);
}
