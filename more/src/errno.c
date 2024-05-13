/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "util.h"

struct Errno {
	char const *name;
	unsigned    code;
} errno_table[] = {
	{.name = "EPERM",           .code = EPERM          },
	{.name = "ENOENT",          .code = ENOENT         },
	{.name = "ESRCH",           .code = ESRCH          },
	{.name = "EINTR",           .code = EINTR          },
	{.name = "EIO",             .code = EIO            },
	{.name = "ENXIO",           .code = ENXIO          },
	{.name = "E2BIG",           .code = E2BIG          },
	{.name = "ENOEXEC",         .code = ENOEXEC        },
	{.name = "EBADF",           .code = EBADF          },
	{.name = "ECHILD",          .code = ECHILD         },
	{.name = "EAGAIN",          .code = EAGAIN         },
	{.name = "ENOMEM",          .code = ENOMEM         },
	{.name = "EACCES",          .code = EACCES         },
	{.name = "EFAULT",          .code = EFAULT         },
	{.name = "ENOTBLK",         .code = ENOTBLK        },
	{.name = "EBUSY",           .code = EBUSY          },
	{.name = "EEXIST",          .code = EEXIST         },
	{.name = "EXDEV",           .code = EXDEV          },
	{.name = "ENODEV",          .code = ENODEV         },
	{.name = "ENOTDIR",         .code = ENOTDIR        },
	{.name = "EISDIR",          .code = EISDIR         },
	{.name = "EINVAL",          .code = EINVAL         },
	{.name = "ENFILE",          .code = ENFILE         },
	{.name = "EMFILE",          .code = EMFILE         },
	{.name = "ENOTTY",          .code = ENOTTY         },
	{.name = "ETXTBSY",         .code = ETXTBSY        },
	{.name = "EFBIG",           .code = EFBIG          },
	{.name = "ENOSPC",          .code = ENOSPC         },
	{.name = "ESPIPE",          .code = ESPIPE         },
	{.name = "EROFS",           .code = EROFS          },
	{.name = "EMLINK",          .code = EMLINK         },
	{.name = "EPIPE",           .code = EPIPE          },
	{.name = "EDOM",            .code = EDOM           },
	{.name = "ERANGE",          .code = ERANGE         },
	{.name = "EDEADLK",         .code = EDEADLK        },
	{.name = "ENAMETOOLONG",    .code = ENAMETOOLONG   },
	{.name = "ENOLCK",          .code = ENOLCK         },
	{.name = "ENOSYS",          .code = ENOSYS         },
	{.name = "ENOTEMPTY",       .code = ENOTEMPTY      },
	{.name = "ELOOP",           .code = ELOOP          },
	{.name = "EWOULDBLOCK",     .code = EWOULDBLOCK    },
	{.name = "ENOMSG",          .code = ENOMSG         },
	{.name = "EIDRM",           .code = EIDRM          },
	{.name = "ECHRNG",          .code = ECHRNG         },
	{.name = "EL2NSYNC",        .code = EL2NSYNC       },
	{.name = "EL3HLT",          .code = EL3HLT         },
	{.name = "EL3RST",          .code = EL3RST         },
	{.name = "ELNRNG",          .code = ELNRNG         },
	{.name = "EUNATCH",         .code = EUNATCH        },
	{.name = "ENOCSI",          .code = ENOCSI         },
	{.name = "EL2HLT",          .code = EL2HLT         },
	{.name = "EBADE",           .code = EBADE          },
	{.name = "EBADR",           .code = EBADR          },
	{.name = "EXFULL",          .code = EXFULL         },
	{.name = "ENOANO",          .code = ENOANO         },
	{.name = "EBADRQC",         .code = EBADRQC        },
	{.name = "EBADSLT",         .code = EBADSLT        },
	{.name = "EDEADLOCK",       .code = EDEADLOCK      },
	{.name = "EBFONT",          .code = EBFONT         },
	{.name = "ENOSTR",          .code = ENOSTR         },
	{.name = "ENODATA",         .code = ENODATA        },
	{.name = "ETIME",           .code = ETIME          },
	{.name = "ENOSR",           .code = ENOSR          },
	{.name = "ENONET",          .code = ENONET         },
	{.name = "ENOPKG",          .code = ENOPKG         },
	{.name = "EREMOTE",         .code = EREMOTE        },
	{.name = "ENOLINK",         .code = ENOLINK        },
	{.name = "EADV",            .code = EADV           },
	{.name = "ESRMNT",          .code = ESRMNT         },
	{.name = "ECOMM",           .code = ECOMM          },
	{.name = "EPROTO",          .code = EPROTO         },
	{.name = "EMULTIHOP",       .code = EMULTIHOP      },
	{.name = "EDOTDOT",         .code = EDOTDOT        },
	{.name = "EBADMSG",         .code = EBADMSG        },
	{.name = "EOVERFLOW",       .code = EOVERFLOW      },
	{.name = "ENOTUNIQ",        .code = ENOTUNIQ       },
	{.name = "EBADFD",          .code = EBADFD         },
	{.name = "EREMCHG",         .code = EREMCHG        },
	{.name = "ELIBACC",         .code = ELIBACC        },
	{.name = "ELIBBAD",         .code = ELIBBAD        },
	{.name = "ELIBSCN",         .code = ELIBSCN        },
	{.name = "ELIBMAX",         .code = ELIBMAX        },
	{.name = "ELIBEXEC",        .code = ELIBEXEC       },
	{.name = "EILSEQ",          .code = EILSEQ         },
	{.name = "ERESTART",        .code = ERESTART       },
	{.name = "ESTRPIPE",        .code = ESTRPIPE       },
	{.name = "EUSERS",          .code = EUSERS         },
	{.name = "ENOTSOCK",        .code = ENOTSOCK       },
	{.name = "EDESTADDRREQ",    .code = EDESTADDRREQ   },
	{.name = "EMSGSIZE",        .code = EMSGSIZE       },
	{.name = "EPROTOTYPE",      .code = EPROTOTYPE     },
	{.name = "ENOPROTOOPT",     .code = ENOPROTOOPT    },
	{.name = "EPROTONOSUPPORT", .code = EPROTONOSUPPORT},
	{.name = "ESOCKTNOSUPPORT", .code = ESOCKTNOSUPPORT},
	{.name = "EOPNOTSUPP",      .code = EOPNOTSUPP     },
	{.name = "ENOTSUP",         .code = ENOTSUP        },
	{.name = "EPFNOSUPPORT",    .code = EPFNOSUPPORT   },
	{.name = "EAFNOSUPPORT",    .code = EAFNOSUPPORT   },
	{.name = "EADDRINUSE",      .code = EADDRINUSE     },
	{.name = "EADDRNOTAVAIL",   .code = EADDRNOTAVAIL  },
	{.name = "ENETDOWN",        .code = ENETDOWN       },
	{.name = "ENETUNREACH",     .code = ENETUNREACH    },
	{.name = "ENETRESET",       .code = ENETRESET      },
	{.name = "ECONNABORTED",    .code = ECONNABORTED   },
	{.name = "ECONNRESET",      .code = ECONNRESET     },
	{.name = "ENOBUFS",         .code = ENOBUFS        },
	{.name = "EISCONN",         .code = EISCONN        },
	{.name = "ENOTCONN",        .code = ENOTCONN       },
	{.name = "ESHUTDOWN",       .code = ESHUTDOWN      },
	{.name = "ETOOMANYREFS",    .code = ETOOMANYREFS   },
	{.name = "ETIMEDOUT",       .code = ETIMEDOUT      },
	{.name = "ECONNREFUSED",    .code = ECONNREFUSED   },
	{.name = "EHOSTDOWN",       .code = EHOSTDOWN      },
	{.name = "EHOSTUNREACH",    .code = EHOSTUNREACH   },
	{.name = "EALREADY",        .code = EALREADY       },
	{.name = "EINPROGRESS",     .code = EINPROGRESS    },
	{.name = "ESTALE",          .code = ESTALE         },
	{.name = "EUCLEAN",         .code = EUCLEAN        },
	{.name = "ENOTNAM",         .code = ENOTNAM        },
	{.name = "ENAVAIL",         .code = ENAVAIL        },
	{.name = "EISNAM",          .code = EISNAM         },
	{.name = "EREMOTEIO",       .code = EREMOTEIO      },
	{.name = "EDQUOT",          .code = EDQUOT         },
	{.name = "ENOMEDIUM",       .code = ENOMEDIUM      },
	{.name = "EMEDIUMTYPE",     .code = EMEDIUMTYPE    },
	{.name = "ECANCELED",       .code = ECANCELED      },
	{.name = "ENOKEY",          .code = ENOKEY         },
	{.name = "EKEYEXPIRED",     .code = EKEYEXPIRED    },
	{.name = "EKEYREVOKED",     .code = EKEYREVOKED    },
	{.name = "EKEYREJECTED",    .code = EKEYREJECTED   },
	{.name = "EOWNERDEAD",      .code = EOWNERDEAD     },
	{.name = "ENOTRECOVERABLE", .code = ENOTRECOVERABLE},
	{.name = "ERFKILL",         .code = ERFKILL        },
	{.name = "EHWPOISON",       .code = EHWPOISON      }
};

static void
usage(void)
{
	die(1, "usage: errno name-or-code...\n"
	       "       errno -s word...\n"
	       "       errno -l");
}

static void
print_errno(int i)
{
	printf("%s %d %s\n", errno_table[i].name, errno_table[i].code, \
	    strerror(errno_table[i].code));
}

static void
list_errno(void)
{
	for (unsigned i = 0; i < NELEM(errno_table); ++i)
		print_errno(i);
}

static int
find_errno(char *noc)
{
	unsigned idx;

	/* A code is provided. */
	if ((idx = strtol(noc, NULL, DEC)) < NELEM(errno_table) && idx >= 1) {
		for (unsigned i = 0; i < NELEM(errno_table); ++i) {
			if (errno_table[i].code == idx) {
				print_errno(i);
				return 0;
			}
		}
	} else { /* A name is provided. */
		for (unsigned i = 0; i < NELEM(errno_table); ++i) {
			if (!strcasecmp(errno_table[i].name, noc)) {
				print_errno(i);
				return 0;
			}
		}
	}

	return -1;
}

static int
match_errno_desc(char *keys[])
{
	bool found = false;

	for (unsigned i = 0; i < NELEM(errno_table); ++i) {
		for (char **k = keys; *k; ++k) {
			if (strstr(strerror(errno_table[i].code), *k)) {
				found = true;
			} else {
				found = false;
				break;
			}
		}
		if (found)
			print_errno(i);
	}

	return found ? 0 : -1;
}

int
main(int argc, char *argv[])
{
	bool lflag = false;
	bool sflag = false;
	int ret    = 0;
	int opt;

	while ((opt = getopt(argc, argv, "ls:")) != -1) {
		switch (opt) {
		case 'l':
			lflag = true;
			sflag = false;
			break;
		case 's':
			lflag = false;
			sflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (!lflag && !sflag && argc == 0)
		usage();

	if (lflag) {
		list_errno();
	} else if (sflag) {
		if (match_errno_desc(--argv))
			ret = 1;
	} else {
		for (; *argv; ++argv) {
			if (find_errno(*argv))
				ret = 1;
		}
	}

	return ret;
}
