#include "fs.h"
#include "../kernel/memory/memorylayout.h"

Superblock* superblock = (Superblock*)TEMP_FS;

void init_fs()
{
	superblock->inodes_num = 64;
	superblock->blocks_num = 64;
	superblock->inode_size = sizeof(Inode);
	superblock->block_size = 512;
	superblock->inode_bitmap = (void*)(TEMP_FS + sizeof(Superblock));
	superblock->block_bitmap = (void*)(superblock->inode_bitmap + superblock->inodes_num / 8);
	superblock->inodes = (void*)(superblock->block_bitmap + superblock->blocks_num / 8);
	superblock->blocks = (void*)(superblock->inodes + superblock->inodes_num * superblock->inode_size);
}