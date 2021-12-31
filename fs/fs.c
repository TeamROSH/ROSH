#include "fs.h"
#include "../kernel/ports/ata_pio.h"
#include "../libc/memory.h"

uint8_t buffer[512];
Superblock s;
Superblock* superblock = &s;

void init_fs()
{
	read_sectors((uint32_t)buffer, FS_SECTOR, 1);		// read Superblock
	Superblock* temp = (Superblock*)buffer;
	if (temp->checksum == FS_EXISTS)		// if fs exists
	{
		memcpy(superblock, buffer, sizeof(Superblock));
	}
	else
	{
		temp->inodes_num = DISK_SECTOR;
		temp->blocks_num = DISK_SECTOR * 7;
		temp->inode_size = sizeof(Inode);
		temp->block_size = sizeof(Block);
		temp->bitmaps = FS_SECTOR + 1;
		temp->inodes = temp->bitmaps + 1;
		temp->blocks = temp->inodes + temp->inodes_num / (DISK_SECTOR / temp->inode_size);

		write_sectors(buffer, FS_SECTOR, 1);		// write new Superblock
		memcpy(superblock, buffer, sizeof(Superblock));

		for (int i = 0; i < DISK_SECTOR; i++)
			buffer[i] = 0;
		write_sectors(buffer, superblock->bitmaps, 1);		// init empty bitmap
	}
}

int take_inode()
{
	read_sectors(buffer, superblock->bitmaps, 1);		// read bitmap
	int found = -1;
	for (int i = 0; i < superblock->inodes_num; i++)		// for every inode
	{
		uint8_t bit = buffer[i / 8] << (i % 8);		// get its bit in bitmap
		if (bit % 2 == 0)		// if not taken
		{
			found = i;
			uint8_t temp = 1;
			temp <<= (i % 8);
			buffer[i / 8] |= temp;		// take it
			break;
		}
	}
	write_sectors(buffer, superblock->bitmaps, 1);		// write bitmap back
	return found;
}

int take_block()
{
	read_sectors(buffer, superblock->bitmaps, 1);		// read bitmap
	int found = -1;
	for (int i = 0; i < superblock->blocks_num; i++)		// for every inode
	{
		uint8_t bit = buffer[64 + i / 8] << (i % 8);		// get its bit in bitmap
		if (bit % 2 == 0)		// if not taken
		{
			found = i;
			uint8_t temp = 1;
			temp <<= (i % 8);
			buffer[64 + i / 8] |= temp;		// take it
			break;
		}
	}
	write_sectors(buffer, superblock->bitmaps, 1);		// write bitmap back
	return found;
}

void create_folder(char* path)
{
	int inode_num = take_inode();
	if (inode_num == -1)
		return;

	read_sectors(buffer, superblock->inodes + inode_num / (DISK_SECTOR / temp->inode_size), 1);

	Inode* inode = (Inode*)(buffer + (inode_num % (DISK_SECTOR / temp->inode_size)) * superblock->inode_size);
	inode->folder = 1;
	inode->size = 0;
	
	int block_num = take_block();
	if (block_num == -1)
	{
		// TODO: release inode
		return;
	}
	inode->block = block_num;

	write_sectors(buffer, superblock->inodes + inode_num / (DISK_SECTOR / temp->inode_size), 1);
}

void create_file(char* path)
{
	int inode_num = take_inode();
	if (inode_num == -1)
		return;

	read_sectors(buffer, superblock->inodes + inode_num / (DISK_SECTOR / temp->inode_size), 1);

	Inode* inode = (Inode*)(buffer + (inode_num % (DISK_SECTOR / temp->inode_size)) * superblock->inode_size);
	inode->folder = 0;
	inode->size = 0;
	
	int block_num = take_block();
	if (block_num == -1)
	{
		// TODO: release inode
		return;
	}
	inode->block = block_num;

	write_sectors(buffer, superblock->inodes + inode_num / (DISK_SECTOR / temp->inode_size), 1);
}