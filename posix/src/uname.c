/* See LICENSE file for copyright and license details. */
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: uname [-amnrsv]");
}

static void
print_val(char *val, bool flag)
{
	static bool skip = true;

	if (!flag)
		return;

	if (!skip)
		putchar(' ');
	skip = false;

	fputs(val, stdout);
}

int
main(int argc, char *argv[])
{
	bool mflag, nflag, rflag, sflag, vflag;
	int opt;
	struct utsname uts;

	mflag = nflag = false;
	rflag = sflag = vflag = false;

	while ((opt = getopt(argc, argv, "amnrsv")) != -1) {
		switch (opt) {
		case 'a':
			mflag = nflag = rflag = sflag = vflag = true;
			break;
		case 'm':
			mflag = true;
			break;
		case 'n':
			nflag = true;
			break;
		case 'r':
			rflag = true;
			break;
		case 's':
			sflag = true;
			break;
		case 'v':
			vflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (!(mflag || nflag || rflag || sflag || vflag))
		sflag = true;

	if (uname(&uts))
		die(1, "uname");

	print_val(uts.sysname, sflag);
	print_val(uts.nodename, nflag);
	print_val(uts.release, rflag);
	print_val(uts.version, vflag);
	print_val(uts.machine, mflag);
	putchar('\n');
}
