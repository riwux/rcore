/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: uname [-amnrsv]");
}

static void
print_val(char *val, _Bool flag)
{
	static _Bool skip = 1;

	if (!flag)
		return;

	if (!skip)
		putchar(' ');
	skip = 0;

	fputs(val, stdout);
}

int
main(int argc, char *argv[])
{
	_Bool mflag = 0, nflag = 0, rflag = 0, sflag = 0, vflag = 0;
	int opt;
	struct utsname uts;

	while ((opt = getopt(argc, argv, "amnrsv")) != -1) {
		switch (opt) {
		case 'a':
			mflag = nflag = rflag = sflag = vflag = 1;
			break;
		case 'm':
			mflag = 1;
			break;
		case 'n':
			nflag = 1;
			break;
		case 'r':
			rflag = 1;
			break;
		case 's':
			sflag = 1;
			break;
		case 'v':
			vflag = 1;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (!(mflag || nflag || rflag || sflag || vflag))
		sflag = 1;

	if (uname(&uts))
		die(1, "uname");

	print_val(uts.sysname, sflag);
	print_val(uts.nodename, nflag);
	print_val(uts.release, rflag);
	print_val(uts.version, vflag);
	print_val(uts.machine, mflag);
	putchar('\n');
}
