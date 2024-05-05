# See LICENSE file for copyright and license details.
.POSIX:
.SUFFIXES:
.SUFFIXES: .c

include config.mak

POSIX_DIR = posix
MORE_DIR  = more

POSIX_SRC_DIR = $(POSIX_DIR)/src
MORE_SRC_DIR  = $(MORE_DIR)/src

POSIX_MAN_DIR = $(POSIX_DIR)/man
MORE_MAN_DIR  = $(MORE_DIR)/man

INCLUDE_DIR   = include
LIB_DIR       = lib
LIBUTIL_DIR   = $(LIB_DIR)/libutil

HDR =\
     $(INCLUDE_DIR)/util.h

POSIX_SRC =\
           $(POSIX_SRC_DIR)/basename.c\
           $(POSIX_SRC_DIR)/cat.c\
           $(POSIX_SRC_DIR)/cmp.c\
           $(POSIX_SRC_DIR)/dirname.c\
           $(POSIX_SRC_DIR)/echo.c\
           $(POSIX_SRC_DIR)/env.c\
           $(POSIX_SRC_DIR)/false.c\
           $(POSIX_SRC_DIR)/head.c\
           $(POSIX_SRC_DIR)/link.c\
           $(POSIX_SRC_DIR)/logname.c\
           $(POSIX_SRC_DIR)/mesg.c\
           $(POSIX_SRC_DIR)/mkdir.c\
           $(POSIX_SRC_DIR)/mkfifo.c\
           $(POSIX_SRC_DIR)/pathchk.c\
           $(POSIX_SRC_DIR)/rmdir.c\
           $(POSIX_SRC_DIR)/sleep.c\
           $(POSIX_SRC_DIR)/tee.c\
           $(POSIX_SRC_DIR)/true.c\
           $(POSIX_SRC_DIR)/tty.c\
           $(POSIX_SRC_DIR)/uname.c\
           $(POSIX_SRC_DIR)/unlink.c

MORE_SRC =\
          $(MORE_SRC_DIR)/ascii.c\
          $(MORE_SRC_DIR)/base32.c\
          $(MORE_SRC_DIR)/base64.c\
          $(MORE_SRC_DIR)/clear.c\
          $(MORE_SRC_DIR)/errno.c\
          $(MORE_SRC_DIR)/hostname.c\
          $(MORE_SRC_DIR)/printenv.c\
          $(MORE_SRC_DIR)/sync.c\
          $(MORE_SRC_DIR)/yes.c

LIBUTIL_SRC =\
             $(LIBUTIL_DIR)/io.c\
             $(LIBUTIL_DIR)/lib.c\
             $(LIBUTIL_DIR)/mem.c\
             $(LIBUTIL_DIR)/num.c

POSIX_BIN   = $(POSIX_SRC:.c=)
MORE_BIN    = $(MORE_SRC:.c=)
LIBUTIL     = $(LIBUTIL_DIR)/libutil.a
LIBUTIL_OBJ = $(LIBUTIL_SRC:.c=.o)

all: lib posix more $(HDR)

install: all
	@mkdir -p $(BINDIR)
	cp -f $(POSIX_BIN) $(MORE_BIN) $(BINDIR)

uninstall:
	@printf "Removing $(POSIX_BIN) $(MORE_BIN)\n"
	@for i in $(POSIX_BIN) $(MORE_BIN); do rm -f $(BINDIR)/$$(basename $$i); done

lib: $(LIBUTIL)
posix: lib $(POSIX_BIN)
more: lib $(MORE_BIN)

$(LIBUTIL): $(LIBUTIL_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(LIBUTIL_SRC)
	mv *.o $(LIBUTIL_DIR)
	$(AR) $(ARFLAGS) $(LIBUTIL) $(LIBUTIL_OBJ)

.c:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBUTIL) $(LDFLAGS)

clean:
	rm -f $(POSIX_BIN) $(MORE_BIN) $(LIBUTIL) $(LIBUTIL_OBJ)

.PHONY: all install uninstall posix more lib clean
