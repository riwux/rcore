# See LICENSE file for copyright and license details.
.POSIX:
.SUFFIXES:
.SUFFIXES: .c

include config.mak

POSIX_DIR     = posix
MISC_DIR      = misc

POSIX_SRC_DIR = $(POSIX_DIR)/src
MISC_SRC_DIR  = $(MISC_DIR)/src

POSIX_MAN_DIR = $(POSIX_DIR)/man
MISC_MAN_DIR  = $(MISC_DIR)/man

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
           $(POSIX_SRC_DIR)/false.c\
           $(POSIX_SRC_DIR)/rmdir.c\
           $(POSIX_SRC_DIR)/sleep.c\
           $(POSIX_SRC_DIR)/tee.c\
           $(POSIX_SRC_DIR)/true.c\
           $(POSIX_SRC_DIR)/tty.c\
           $(POSIX_SRC_DIR)/unlink.c

MISC_SRC =\
          $(MISC_SRC_DIR)/clear.c\
          $(MISC_SRC_DIR)/yes.c

LIBUTIL_SRC =\
             $(LIBUTIL_DIR)/buf.c\
             $(LIBUTIL_DIR)/io.c\
             $(LIBUTIL_DIR)/mem.c\
             $(LIBUTIL_DIR)/num.c

LIBUTIL_OBJ = $(LIBUTIL_SRC:.c=.o)

POSIX_BIN = $(POSIX_SRC:.c=)
MISC_BIN  = $(MISC_SRC:.c=)
LIBUTIL   = $(LIBUTIL_DIR)/libutil.a


all: lib posix misc $(HDR)

install: all
	@mkdir -p $(BINDIR)
	cp -f $(POSIX_BIN) $(MISC_BIN) $(BINDIR)

uninstall:
	@printf "Removing $(POSIX_BIN) $(MISC_BIN)\n"
	@for i in $(POSIX_BIN) $(MISC_BIN); do rm -f $(BINDIR)/$$(basename $$i); done

# Libraries
lib: $(LIBUTIL)

$(LIBUTIL):
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(LIBUTIL_SRC)
	mv *.o $(LIBUTIL_DIR)
	$(AR) $(ARFLAGS) $(LIBUTIL) $(LIBUTIL_OBJ)


# Binaries
posix: $(POSIX_BIN)
misc: $(MISC_BIN)

.c:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBUTIL) $(LDFLAGS)

clean:
	rm -f $(POSIX_BIN) $(MISC_BIN) $(LIBUTIL) $(LIBUTIL_OBJ)

.PHONY: all install uninstall posix misc lib clean
