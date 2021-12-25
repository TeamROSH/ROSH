#ifndef FS_H
#define FS_H

#define FS_EXISTS 0x552F60B2
#define FS_SECTOR 128
#define DISK_SECTOR 512

#include <stdint.h>

typedef struct Block
{
	uint8_t data[DISK_SECTOR];
} Block;

typedef struct Inode
{
	uint32_t folder;	// 0 - file, 1 - folder
	uint32_t reserved;
	uint32_t size;		// file size
	Block* block;
} Inode;

typedef struct Superblock
{
	uint32_t checksum;
	uint8_t* bitmaps;
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