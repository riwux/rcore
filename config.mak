# See LICENSE file for copyright and license details.
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

CC       = cc
CFLAGS   = -std=c99 -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude -D_POSIX_C_SOURCE=200809L
LDFLAGS  = -Llib/libutil -lutil -s

AR      = ar
ARFLAGS = rcs
