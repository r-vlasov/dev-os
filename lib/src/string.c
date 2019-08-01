#include "../../lib/headers/string.h"

/*	The memchr() function locates the first occurrence of (char)value in initial count characters	*/

void*	memchr	(const void* memory, int value, size_t count)
{
	unsigned char *res = (unsigned char*)memory;
	while (count--)
	{
		if (*res != (unsigned char) value)
			res++;
		else
			return res;
	}
	return NULL;
}

extern void tty_write_address(uint32_t);
/*	The mrmcmp() function returns an integer greater than, equal to, or less than zero, accordingly as the object pointed to by str1 is greater than, equal to, or less than the object pointed to by str2	*/
int	memcmp (const void* str1, const void* str2, size_t count)
{
	const char* ch1 = (const char*) str1;
	const char* ch2 = (const char*) str2;
	while (count--)
	{
		if ( ch1[count] == ch2[count])
		{
			continue;
		}
		else
		{
			return ch2[count] - ch1[count];
		}
	}
	return 0x0;
}

/*	The memcpy() function shall copy the first n bytes pointed to by src to the buffer pointed to by dest	*/
void* 	memcpy(void* dest, const void* src, size_t count)
{
	char *d = dest;
	const char *s = src;
	while (count--)
		*d++ = *s++;
	return dest;
}

/*	The memmove() function shall copy the first count butes pointed to by src to the buffer pointed to by dest. Source and destination may overlap	*/
void*	memmove(void* dest, const void* src, size_t count)
{
	unsigned char tmp[count];
	memcpy(tmp, src, count);
	memcpy(dest, tmp, count);
	return dest;
}

/* 	The memset() function copies the value of c into each of the first count characters of the object pointed to by str	*/
void* 	memset	(void* str, int c, size_t count)
{
	unsigned char* res = str;
	while (count--)
		*res++ = (unsigned char) c;
	return str;
}


/*
 *
 *
 */

/*	The strcpy() shall copy the null terminated string pointed to by src to the memory pointed to by dest	*/
char* 	strcpy	(char* dest, const char* src)
{
	char* result = dest;
	while (*src != '\0')
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';
	return result;
}

/*	String Length	*/
size_t 	strlen	(const char* s)
{
	size_t size = 0;
	int i = 0;
	while (s[i] != '\0')
	{
		size++;
	}
	return size + 1;
}

/*	Reverse the string	*/
void	reverse	(char* s)
{
	char ch;
	int i1 = 0;
	int i2 = strlen(s) - 1;
	for (i1 = 0, i2 = strlen(s) - 1; i1 < i2; i1++, i2--)
	{
		ch = s[i1];
		s[i1] = s[i2];
		s[i2] = ch;
	}
}
