#ifndef FS_H
#define FS_H

#include <stdint.h>

typedef struct Inode
{
	int8_t folder : 1;	// 0 - file, 1 - folder
	int32_t size;		// file size
	void* block;
} Inode;


typedef struct Superblock
{
	void* inode_bitmap;
	void* block_bitmap;
	void* inodes;
	void* blocks;
	int32_t inodes_num;
	int32_t blocks_num;
	int32_t inode_size;
	int32_t block_size;
} Superblock;

/*
	init a temporary file system
*/
void* init_fs();

#endif