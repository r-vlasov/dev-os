#include "../headers/vfs.h"


/* Virtual file system */

fs_node_t *fs_root = 0;


/* */
uint32_t 	read_fs(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	if (node->read)
	{
		uint32_t ret = node->read(node, offset, size, buffer);
		return ret;
	}
	else
		return 0;
}


/* */
uint32_t 	write_fs(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	if (node->write)
	{
		uint32_t ret = node->write(node, offset, size, buffer);
		return ret;
	}
	else
		return 0;
}


/* */
void 		open_fs(fs_node_t* node)
{
	if (node->open)
		node->open(node);
}


/* */
void 		close_fs(fs_node_t* node)
{
	if (node->close) 
		node->close(node);
}


/* */
struct dirent	*readdir_fs(fs_node_t *node, uint32_t index)
{
	if ( (node->flags & 0x07) == FS_DIRECTORY && node->readdir != 0 )
		return node->readdir(node, index);	
	else
		return 0;
}


/* */
fs_node_t	*finddir_fs(fs_node_t *node, char *name)
{
	if ( (node->flags & 0x07) == FS_DIRECTORY && node->finddir != 0 )
	{
		return node->finddir(node, name);
	}
	else
	{
		return 0;
	}
}

