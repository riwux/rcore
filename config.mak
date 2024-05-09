# See LICENSE file for copyright and license details.
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

CC       = cc
CPPFLAGS = -Iinclude -D_XOPEN_SOURCE=700
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Werror
LDFLAGS  = -Llib/libutil -lutil -s

AR      = ar
ARFLAGS = rcs
