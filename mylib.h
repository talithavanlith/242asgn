#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern void *ecalloc(size_t, size_t);
extern int getword(char *s, int limit, FILE *stream);

#endif
