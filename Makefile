# See LICENSE file for copyright and license details.
.POSIX:
.SUFFIXES:
.SUFFIXES: .c

include config.mak

POSIX_DIR   = posix
INCLUDE_DIR = include
LIB_DIR     = lib
LIBUTIL_DIR = $(LIB_DIR)/libutil

HDR =\
     $(INCLUDE_DIR)/util.h

POSIX_SRC =\
           $(POSIX_DIR)/basename.c\
           $(POSIX_DIR)/cat.c

LIBUTIL_SRC =\
             $(LIBUTIL_DIR)/alloc.c\
             $(LIBUTIL_DIR)/buf.c\
             $(LIBUTIL_DIR)/error.c\
             $(LIBUTIL_DIR)/io.c

LIBUTIL_OBJ = $(LIBUTIL_SRC:.c=.o)

POSIX_BIN = $(POSIX_SRC:.c=)
LIBUTIL   = $(LIBUTIL_DIR)/libutil.a


all: lib posix $(HDR)

# Libraries
lib: libutil

libutil:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(LIBUTIL_SRC)
	$(MV) *.o $(LIBUTIL_DIR)
	$(AR) $(ARFLAGS) $(LIBUTIL) $(LIBUTIL_OBJ)


# Binaries
posix: libutil $(POSIX_BIN)

.c:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBUTIL)

clean:
	rm -f $(POSIX_BIN) $(LIBUTIL) $(LIBUTIL_OBJ)

.PHONY: all posix lib libutil clean
