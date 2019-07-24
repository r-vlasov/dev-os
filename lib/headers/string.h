#ifndef STRING_H
#define STRING_H


#include "std.h"

/*
 * Prototypes of the ANSI STANDART C LIBRARY STRING FUNCTIONS
 */
void*  	memchr	(const void*, int, size_t);
int 	memcmp	(const void*, const void*, size_t);
void* 	memcpy	(void*, const void*, size_t);
void* 	memmove	(void*, const void*, size_t);
void* 	memset	(void*, int, size_t);

char* 	strcpy	(char*, const char*);
size_t 	strlen	(const char*);

#endif
