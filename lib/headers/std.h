#ifndef STD_H
#define STD_H
	
	/* 	NULL PTR	*/
#define	NULL	((void*)0)


	/*	Minimum and Maximum	*/
#define min(s1,s2) (((s1) > (s2))?(s2):(s1))
#define max(s1,s2) (((s1) > (s2))?(s1):(s2))

/*	Standart types 		*/


typedef unsigned char		uint8_t;
typedef signed char		int8_t;
typedef unsigned short		uint16_t;
typedef signed short		int16_t;
typedef	unsigned long		uint32_t;
typedef signed long		int32_t;
typedef	unsigned long long	uint64_t;
typedef signed long long	int64_t;


typedef	uint64_t	size_t;
#endif

