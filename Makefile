# See LICENSE file for copyright and license details.
.POSIX:
.SUFFIXES:
.SUFFIXES: .c

include config.mak

POSIX_DIR     = posix
POSIX_SRC_DIR = $(POSIX_DIR)/src
POSIX_MAN_DIR = $(POSIX_DIR)/man
INCLUDE_DIR   = include
LIB_DIR       = lib
LIBUTIL_DIR   = $(LIB_DIR)/libutil

HDR =\
     $(INCLUDE_DIR)/util.h

POSIX_SRC =\
           $(POSIX_SRC_DIR)/basename.c\
           $(POSIX_SRC_DIR)/cat.c\
           $(POSIX_SRC_DIR)/dirname.c\
           $(POSIX_SRC_DIR)/false.c\
           $(POSIX_SRC_DIR)/tee.c\
           $(POSIX_SRC_DIR)/true.c\
           $(POSIX_SRC_DIR)/tty.c

LIBUTIL_SRC =\
             $(LIBUTIL_DIR)/buf.c\
             $(LIBUTIL_DIR)/util.c\
             $(LIBUTIL_DIR)/xutil.c

LIBUTIL_OBJ = $(LIBUTIL_SRC:.c=.o)

POSIX_BIN = $(POSIX_SRC:.c=)
LIBUTIL   = $(LIBUTIL_DIR)/libutil.a


all: lib posix $(HDR)

install: all
	@mkdir -p $(BINDIR)
	cp -f $(POSIX_BIN) $(BINDIR)

uninstall:
	@printf "Removing $(POSIX_BIN)\n"
	@for i in $(POSIX_BIN); do rm -f $(BINDIR)/$$(basename $$i); done

# Libraries
lib: $(LIBUTIL)

$(LIBUTIL):
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(LIBUTIL_SRC)
	mv *.o $(LIBUTIL_DIR)
	$(AR) $(ARFLAGS) $(LIBUTIL) $(LIBUTIL_OBJ)


# Binaries
posix: $(POSIX_BIN)

.c:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBUTIL) $(LDFLAGS)

clean:
	rm -f $(POSIX_BIN) $(LIBUTIL) $(LIBUTIL_OBJ)

.PHONY: all install uninstall posix lib clean
