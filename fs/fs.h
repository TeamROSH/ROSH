#ifndef FS_H
#define FS_H

#include <stdint.h>

typedef struct Block
{
	uint8_t data[512];
} Block;

typedef struct Inode
{
	uint8_t folder : 1;	// 0 - file, 1 - folder
	uint32_t size;		// file size
	Block* block;
} Inode;

typedef struct Superblock
{
	uint8_t* inode_bitmap;
	uint8_t* block_bitmap;
	Inode* inodes;
	Block* blocks;
	uint32_t inodes_num;
	uint32_t blocks_num;
	uint32_t inode_size;
	uint32_t block_size;
} Superblock;

/*
	init a temporary file system
*/
void init_fs();

/*
	create a folder
*/
void create_folder(char* path);

/*
	create a file
*/
void create_file(char* name);

#endif