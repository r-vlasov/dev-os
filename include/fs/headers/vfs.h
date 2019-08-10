#ifndef FS_H
#define FS_H


#include "../../../lib/headers/stdlib.h"

#define PATH_DOT "."
#define PATH_DOWN ".."


#define FS_FILE		0x01
#define	FS_DIRECTORY	0x02
#define	FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		0x05
#define	FS_SYMLINK	0x06
#define FS_MOUNTPOINT	0x07


/* File types */
#define _IFFILE		0x1200
#define _IFDIR		0x1201
#define _IFCHR		0x1202
#define _IFBLK		0x1203
#define _IFPIPE		0x1204
#define _IFSLINK	0x1205
#define _IFSOCK		0x1206
#define _IFFIFO		0x1207




/* Define the prototypes for our r/w functions */
// define function's pointers //
	typedef uint32_t 	(*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
	typedef uint32_t 	(*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
	typedef void 		(*open_type_t)(struct fs_node*);
	typedef void		(*close_type_t)(struct fs_node*);
	typedef struct dirent*	(*readdir_type_t)(struct fs_node*, uint32_t);
	typedef struct fs_node*	(*finddir_type_t)(struct fs_node*, char* name);



typedef struct fs_node
{
	char 			name[64];
	uint32_t		mask;
	uint32_t		flags;
	uint32_t		inode;
	uint32_t		length;
	
	struct fs_node		*ptr;
	
	uint32_t		offset;
	
	read_type_t		read;
	write_type_t		write;
	open_type_t		open;
	close_type_t		close;
	readdir_type_t		readdir;
	finddir_type_t		finddir;

} fs_node_t;


struct dirent {
	uint32_t 		ino;		// i-node number
	char 			name[64];	// filename
};

/* file system's root */
	fs_node_t	*fs_root;

	


uint32_t	read_fs		(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t	write_fs	(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void		open_fs		(fs_node_t *node);
void		close_fs	(fs_node_t *node);
struct dirent*	readdir_fs	(fs_node_t *node, uint32_t index);
fs_node_t*	finddir_fs	(fs_node_t *node, char *name);

#endif
