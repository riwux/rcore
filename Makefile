# See LICENSE file for copyright and license details.
.POSIX:
.SUFFIXES:
.SUFFIXES: .c

include config.mak

POSIX_DIR = posix
EXTRA_DIR = extra

POSIX_SRC_DIR = $(POSIX_DIR)/src
EXTRA_SRC_DIR = $(EXTRA_DIR)/src

POSIX_MAN_DIR = $(POSIX_DIR)/man
EXTRA_MAN_DIR = $(EXTRA_DIR)/man

INCLUDE_DIR   = include
LIB_DIR       = lib
LIBUTIL_DIR   = $(LIB_DIR)/libutil


HDR =\
     $(INCLUDE_DIR)/util.h

POSIX_SRC =\
           $(POSIX_SRC_DIR)/basename.c\
           $(POSIX_SRC_DIR)/cat.c\
           $(POSIX_SRC_DIR)/dirname.c\
           $(POSIX_SRC_DIR)/echo.c\
           $(POSIX_SRC_DIR)/env.c\
           $(POSIX_SRC_DIR)/false.c\
           $(POSIX_SRC_DIR)/link.c\
           $(POSIX_SRC_DIR)/mesg.c\
           $(POSIX_SRC_DIR)/rmdir.c\
           $(POSIX_SRC_DIR)/sleep.c\
           $(POSIX_SRC_DIR)/tee.c\
           $(POSIX_SRC_DIR)/true.c\
           $(POSIX_SRC_DIR)/tty.c\
           $(POSIX_SRC_DIR)/uname.c\
           $(POSIX_SRC_DIR)/unlink.c

EXTRA_SRC =\
          $(EXTRA_SRC_DIR)/ascii.c\
          $(EXTRA_SRC_DIR)/base32.c\
          $(EXTRA_SRC_DIR)/base64.c\
          $(EXTRA_SRC_DIR)/clear.c\
          $(EXTRA_SRC_DIR)/hostname.c\
          $(EXTRA_SRC_DIR)/printenv.c\
          $(EXTRA_SRC_DIR)/yes.c

LIBUTIL_SRC =\
             $(LIBUTIL_DIR)/io.c\
             $(LIBUTIL_DIR)/lib.c\
             $(LIBUTIL_DIR)/mem.c\
             $(LIBUTIL_DIR)/num.c

POSIX_BIN   = $(POSIX_SRC:.c=)
EXTRA_BIN   = $(EXTRA_SRC:.c=)
LIBUTIL     = $(LIBUTIL_DIR)/libutil.a
LIBUTIL_OBJ = $(LIBUTIL_SRC:.c=.o)


all: lib posix extra $(HDR)

install: all
	@mkdir -p $(BINDIR)
	cp -f $(POSIX_BIN) $(EXTRA_BIN) $(BINDIR)

uninstall:
	@printf "Removing $(POSIX_BIN) $(EXTRA_BIN)\n"
	@for i in $(POSIX_BIN) $(EXTRA_BIN); do rm -f $(BINDIR)/$$(basename $$i); done

lib: $(LIBUTIL)
posix: $(LIBUTIL) $(POSIX_BIN)
extra: $(LIBUTIL) $(EXTRA_BIN)

$(LIBUTIL): $(LIBUTIL_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(LIBUTIL_SRC)
	mv *.o $(LIBUTIL_DIR)
	$(AR) $(ARFLAGS) $(LIBUTIL) $(LIBUTIL_OBJ)

.c:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBUTIL) $(LDFLAGS)

clean:
	rm -f $(POSIX_BIN) $(EXTRA_BIN) $(LIBUTIL) $(LIBUTIL_OBJ)

.PHONY: all install uninstall posix extra lib clean
