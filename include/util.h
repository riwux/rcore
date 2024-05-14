/* See LICENSE file for copyright and license details. */
#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h> /* provide */
#include <stdint.h>   /* types   */

#define NELEM(a) (sizeof (a) / sizeof *(a))

typedef unsigned char uchar;

/* io.c */
extern void warn(char const *, ...);
extern void die(int, char const *, ...);

extern int      fcopy(FILE *, FILE *);
extern ssize_t  non_block_fread(char *, size_t, FILE *);
extern FILE    *x_fopen(char const *, char const *);
extern void     x_fclose(FILE *);

/* mem.c */
extern void *x_malloc(size_t, size_t);
extern void *x_realloc(void *, size_t, size_t);

/* num.c */
#define OCT 8
#define DEC 10
#define HEX 16

extern int64_t  x_to_num(char const *, int);
extern uint64_t x_to_unum(char const *, int);

static inline bool
overflow_mul(size_t a, size_t b)
{
	return (a > 0 && b > SIZE_MAX / a);
}

static inline bool
is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static inline bool
is_hexdigit(char c)
{
	return (is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

static inline bool
is_octdigit(char c)
{
	return (c >= '0' && c <= '7');
}

/* lib.c */
extern char   unescape(char const *);
extern mode_t x_parsemode(char const *, mode_t);
extern mode_t get_umask(void);

#endif /* UTIL_H_ */
