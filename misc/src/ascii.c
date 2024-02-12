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
print_table(int base)
{
	char *fmt, *escfmt;
	char *ascii[] = {
		"nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
		"bs",  "ht",  "nl",  "vt",  "np",  "cr",  "so",  "si",
		"dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
		"can", "em",  "sub", "esc", "fs",  "gs",  "rs",  "us", "sp",
		[127] = "del"
	};

	switch (base) {
	case DEC:
		fmt = "|%.3d  %c ";
		escfmt = "|%.3d %-3s";
		break;
	case OCT:
		fmt = "|%.3o  %c ";
		escfmt = "|%.3o %-3s";
		break;
	case HEX:
		fmt = "|%.2x  %c ";
		escfmt = "|%.2x %-3s";
		break;
	}

	for (uchar c = 0; c < 128; ++c) {
		if (c != 0 && c % 8 == 0)
			printf("|\n");
		if (c < 33 || c == 127)
			printf(escfmt, c, ascii[c]);
		else
			printf(fmt, c, c);
	}
	printf("|\n");
}

static void
convert(char *argv[], int base)
{
	char *fmt;

	switch (base) {
	case DEC:
		fmt = "%d\n";
		break;
	case OCT:
		fmt = "%o\n";
		break;
	case HEX:
		fmt = "%x\n";
		break;
	}

	for (; *argv; ++argv) {
		for (; **argv; ++(*argv))
			printf(fmt, **argv);
	}
}

int
main(int argc, char *argv[])
{
	int opt;
	int base  = HEX;

	while ((opt = getopt(argc, argv, "xdo")) != -1) {
		switch (opt) {
		case 'x':
			base = HEX;
			break;
		case 'd':
			base = DEC;
			break;
		case 'o':
			base = OCT;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		print_table(base);
	else
		convert(argv, base);

}
