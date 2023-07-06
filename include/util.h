/* See LICENSE file for copyright and license details. */
#ifndef UTIL_H_
#define UTIL_H_

#include <sys/types.h> /* mode_t ; (s)size_t */

#define TOP_DOWN 0
#define BOTTOM_UP 1

typedef struct Buf Buf;
struct Buf {
	char *data;
	size_t size;
};

Buf *buf_create(size_t);

void die(char *);
void tell(char *);

ssize_t get_line(int, Buf *);

void *xmalloc(size_t);
void *xrealloc(void *, size_t);

#endif /* UTIL_H_ */
