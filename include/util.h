/* See LICENSE file for copyright and license details. */
#ifndef UTIL_H_
#define UTIL_H_

#include <sys/types.h> /* provide */
#include <stdint.h>   /* types   */

/* buf.c */
#define BUFLEN 4096

struct Buf {
	char *data;
	size_t size;
};

struct Buf *buf_create(size_t);
void buf_free(struct Buf *);


/* io.c */
void die(int, char *, ...);

int copy_file(int, int);
ssize_t write_all(int, void *, size_t);
void xwrite_all(int, void *, size_t);
void xclose(int);
ssize_t get_line(int, struct Buf *);


/* mem.c */
void *xmalloc(size_t);
void *xrealloc(void *, size_t);


/* num.c */
#define IS_DIGIT(c) (c >= 0x30 && c <= 0x39)

int_least64_t to_num(char *);
uint_least64_t to_unum(char *);

#endif /* UTIL_H_ */
