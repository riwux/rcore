/* See LICENSE file for copyright and license details. */
#ifndef UTIL_H_
#define UTIL_H_

#include <sys/types.h> /* provide */
#include <stdint.h>   /* types   */

#define BUFCAP 4096

typedef unsigned char uchar;

/* io.c */
extern void eprintf(char *, ...);
extern void die(int, char *, ...);

extern int copy_file(int, int);
extern ssize_t write_all(int, char *, size_t);
extern void xwrite_all(int, char *, size_t);
extern void xclose(int);

/* mem.c */
extern void *xmalloc(size_t, size_t);
extern void *xrealloc(void *, size_t, size_t);

/* num.c */
#define OCT 8
#define DEC 10
#define HEX 16

static inline _Bool overflow_mul(size_t a, size_t b)
{
	return (a > 0 && b > SIZE_MAX / a);
}

static inline _Bool is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static inline _Bool is_hexdigit(char c)
{
	return (is_digit(c) || (c >= 'a' && c <= 'f'));
}

static inline _Bool is_octdigit(char c)
{
	return (c >= '0' && c <= '7');
}

extern int_least64_t to_num(char *, int);
extern uint_least64_t to_unum(char *, int);

/* lib.c */
extern char   unescape(char *);
extern size_t unescape_num(char *);

#endif /* UTIL_H_ */
