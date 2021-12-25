#include "fs.h"
#include "../kernel/ports/ata_pio.h"
#include "../libc/memory.h"

uint8_t buffer[512];
Superblock superblock;

void init_fs()
{
	read_sectors((uint32_t)buffer, FS_SECTOR, 1);		// read Superblock
	Superblock* temp = (Superblock*)buffer;
	if (temp->checksum != FS_EXISTS)		// if fs exists
	{
		temp->inodes_num = DISK_SECTOR;
		temp->blocks_num = DISK_SECTOR * 7;
		temp->inode_size = sizeof(Inode);
		temp->block_size = sizeof(Block);
		temp->bitmaps = FS_SECTOR + 1;
		temp->inodes = temp->bitmaps + 1;
		temp->blocks = temp->inodes + temp->inodes_num / (DISK_SECTOR / temp->inode_size);

		for (int i = 0; i < (temp->inodes_num + temp->blocks_num) / 8; i++)
			temp->bitmaps[i] = 0;

		write_sectors(buffer, FS_SECTOR, 1);		// write new Superblock
	}
	memcpy(&superblock, buffer, sizeof(Superblock));
}

int take_inode()
{
	int found = -1;
	for (int i = 0; i < superblock->inodes_num; i++)
	{
		uint8_t bit = superblock->inode_bitmap[i / 8] >> (i % 8);
		if (bit % 2 == 0)
		{
			found = i;
			uint8_t temp = 1;
			temp >>= (i % 8);
			superblock->inode_bitmap[i / 8] |= temp;
		}
	}
	return found;
}

int take_block()
{
	int found = -1;
	for (int i = 0; i < superblock->blocks_num; i++)
	{
		uint8_t bit = superblock->block_bitmap[i / 8] >> (i % 8);
		if (bit % 2 == 0)
		{
			found = i;
			uint8_t temp = 1;
			temp >>= (i % 8);
			superblock->block_bitmap[i / 8] |= temp;
		}
	}
	return found;
}

void create_folder(char* path)
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