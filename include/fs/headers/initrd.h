#ifndef INITRD_H
#define INITRD_H

#include "vfs.h"
#include "../../../lib/headers/stdlib.h"


typedef struct initrd_header
{
	uint32_t 		magic;
	uint32_t		length;

} __attribute__((packed)) file_system_header_t;




typedef struct initrd_file_header
{
	unsigned long	magic_file;		//	magic-file
	char		name[64];		//	filename
	unsigned long 	number;
	unsigned long	parent;
	unsigned long 	size;
	short		stat;
	unsigned long	nextfile;
} file_t;



	fs_node_t	*initrd_init(uint32_t location);


file_system_header_t 	*initrd_header;
file_t			*file;
fs_node_t		*initrd_root;
fs_node_t		*initrd_dev;
fs_node_t		*root_nodes;
fs_node_t		*dev_nodes;
int			nroot_nodes;

#endif

