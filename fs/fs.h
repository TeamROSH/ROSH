#ifndef FS_H
#define FS_H

#include <stdint.h>

typedef struct inode
{
	int8_t folder : 1;	// 0 - file, 1 - folder
	int32_t size;		// file size
	int8_t* block;
} inode;


typedef struct superblock
{
	int8_t* inode_bitmap;
	int8_t* block_bitmap;
	inode* inodes;
	int8_t* blocks;
	int32_t inodes_num;
	int32_t blocks_num;
	int32_t inode_size;
	int32_t block_size;
} superblock;

#endif