/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: mkdir [-p] [-m mode] dir...");
}

static int
mkpath(char *path)
{
	int ret = 0;
	char *p = path;
	char *s = path;
	char *last;
	struct stat st;

	/* Skip the initial <slash> of an absolute path. */
	if (path[0] == '/')
		++s;

	/* The path has to always be terminated by a trailing <slash>. */
	last = &path[strlen(path)];
	if (*(last-1) != '/')
		*last = '/';

	while (ret != -1 && s < last && (p = strchr(s, '/'))) {
		*p = '\0';
		s  = p + 1;
		errno = 0;
		if (mkdir(path, 0) && errno != EEXIST) {
			warn("mkdir: cannot create '%s':", path);
			ret = -1;
			goto reset;
		}
		/* Already existing pathname components keep their mode. */
		if (errno != EEXIST &&
		    chmod(path, (S_IWUSR | S_IXUSR | ~get_umask()) & 0777)) {
			warn("mkdir: chmod: cannot change permission of '%s':", path);
			ret = -1;
			goto reset;
		}
		if (!stat(path, &st) && !S_ISDIR(st.st_mode)) {
			warn("mkdir: cannot create '%s': Not a directory", path);
			ret = -1;
		}
reset:
		*p = '/';
	}
	*last = '\0';

	return ret;
}

int
main(int argc, char *argv[])
{
	bool pflag = false;
	int opt;
	int ret = 0;
	mode_t mode = (S_IRWXU | S_IRWXG | S_IRWXO) & ~get_umask();

	while ((opt = getopt(argc, argv, "pm:")) != -1) {
		switch (opt) {
		case 'p':
			pflag = true;
			break;
		case 'm':
			mode = x_parsemode(optarg, (S_IRWXU | S_IRWXG | S_IRWXO));
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	for (; *argv; ++argv) {
		if (pflag) {
			if (mkpath(*argv)) {
				ret = 1;
				continue;
			}
		} else if (mkdir(*argv, mode)) {
			warn("mkdir: cannot create '%s':", *argv);
			ret = 1;
			continue;
		}
		if (chmod(*argv, mode))
			warn("mkdir: chmod: cannot change permission of '%s':", *argv);
	}

	return ret;
}
