/* See LICENSE file for copyright and license details. */
#include <libgen.h>
#include <stdio.h>

#include "util.h"

static void
usage(void)
{
	die(FAILURE, "usage: dirname path");
}

int
main(int argc, char *argv[])
{
	if (argc != 2)
		usage();

	puts(dirname(argv[1]));
}
