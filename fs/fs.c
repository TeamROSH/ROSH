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

	for (int i = 0; i < superblock->inodes_num / 8; i++)
		superblock->inode_bitmap[i] = 0;
	for (int i = 0; i < superblock->blocks_num / 8; i++)
		superblock->block_bitmap[i] = 0;
}

int take_inode()
{
	int found = -1;
	for (int i = 0; i < superblock->inodes_num; i++)
	{
		uint8_t bit = superblock->inodes_num[i / 8] >> (i % 8);
		if (bit % 2 == 0)
		{
			found = i;
			uint8_t temp = 1;
			temp >>= (i % 8);
			superblock->inodes_num[i / 8] |= temp;
		}
	}
	return found;
}

int take_block()
{
	int found = -1;
	for (int i = 0; i < superblock->blocks_num; i++)
	{
		uint8_t bit = superblock->blocks_num[i / 8] >> (i % 8);
		if (bit % 2 == 0)
		{
			found = i;
			uint8_t temp = 1;
			temp >>= (i % 8);
			superblock->blocks_num[i / 8] |= temp;
		}
	}
	return found;
}

void create_folder()
{
	int inode_num = take_inode();
	if (inode_num == -1)
		return;
	Inode* inode = (Inode*)(superblock->inodes + inode_num);
	inode->folder = 1;
	inode->size = 0;
	
	int block_num = take_block();
	if (block_num == -1)
		return;
	inode->block = (Block*)(superblock->blocks + block_num);
}

void create_file(char* name)
{
	int inode_num = take_inode();
	if (inode_num == -1)
		return;
	Inode* inode = (Inode*)(superblock->inodes + inode_num);
	inode->folder = 0;
	inode->size = 0;
	
	int block_num = take_block();
	if (block_num == -1)
		return;
	inode->block = (Block*)(superblock->blocks + block_num);
}