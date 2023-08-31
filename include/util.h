/* See LICENSE file for copyright and license details. */
#ifndef UTIL_H_
#define UTIL_H_

#include <sys/types.h> /* mode_t ; (s)size_t */

#define FAILURE 1
#define BUFLEN 4096

typedef struct Buf Buf;
struct Buf {
	char *data;
	size_t size;
};

Buf *buf_create(size_t);
void buf_free(Buf *);

void die(int, char *, ...);

int copy_file(int, int);
ssize_t write_all(int, void *, size_t);
ssize_t get_line(int, Buf *);

void *xmalloc(size_t);
void *xrealloc(void *, size_t);
void xwrite_all(int, void *, size_t);
void xclose(int);

#endif /* UTIL_H_ */
