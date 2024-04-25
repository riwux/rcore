/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: ascii [-xdo] [string...]");
}

static void
print_chart(char base)
{
	char const fmt[] = {'|', '%', '.', (base == 'x' ? '2' : '3'), base, \
	' ', ' ', '%', 'c', ' ', '\0'};
	char const ctlfmt[] = {'|', '%', '.', (base == 'x' ? '2' : '3'), base, \
	' ', '%', '-', '3', 's', '\0'};
	char const *ascii[] = {
		"nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
		"bs",  "ht",  "nl",  "vt",  "np",  "cr",  "so",  "si",
		"dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
		"can", "em",  "sub", "esc", "fs",  "gs",  "rs",  "us",
		"sp", "del"
	};

	for (uchar c = 0; c < 128; ++c) {
		if (c != 0 && c % 8 == 0)
			printf("|\n");
		if (c < 33)
			printf(ctlfmt, c, ascii[c]);
		else if (c == 127)
			printf(ctlfmt, c, ascii[33]);
		else
			printf(fmt, c, c);
	}
	printf("|\n");
}

static void
print_ascii_code(char const *arg, char base)
{
	char fmt[] = {'%', base, '\n', '\0'};

	for (; *arg; ++arg)
		printf(fmt, *arg);
}

int
main(int argc, char *argv[])
{
	char base = 'x';
	int opt;

	while ((opt = getopt(argc, argv, "xdo")) != -1) {
		switch (opt) {
		case 'x':
		case 'd':
		case 'o':
			base = opt;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		print_chart(base);
	} else {
		for (; *argv; ++argv)
			print_ascii_code(*argv, base);
	}

}
