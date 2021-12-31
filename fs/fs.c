#include "fs.h"
#include "../kernel/ports/ata_pio.h"
#include "../libc/memory.h"
#include "../libc/string.h"

uint8_t buffer[DISK_SECTOR + 1];
Superblock s;
Superblock* superblock = &s;
char dir[200] = {0};

void getPath(char* path);
int take_inode();
int take_block();
void release_inode(int inode);
void release_block(int block);
int followPath(char* path_parts, int size);

void init_fs()
{
	read_sectors((uint32_t)buffer, FS_SECTOR, 1);		// read Superblock
	Superblock* temp = (Superblock*)buffer;
	dir[0] = '/';
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

void release_inode(int inode)
{
	read_sectors(buffer, superblock->bitmaps, 1);		// read bitmap
	uint8_t bit = buffer[inode / 8] << (inode % 8);		// get its bit in bitmap
	if (bit % 2 == 1)		// if taken
	{
		uint8_t temp = 1;
		temp <<= (inode % 8);
		temp ^= 0xFF;
		buffer[inode / 8] &= temp;		// release it
	}
	write_sectors(buffer, superblock->bitmaps, 1);		// write bitmap back
}

void release_block(int block)
{
	read_sectors(buffer, superblock->bitmaps, 1);		// read bitmap
	uint8_t bit = buffer[64 + block / 8] << (block % 8);		// get its bit in bitmap
	if (bit % 2 == 1)		// if taken
	{
		uint8_t temp = 1;
		temp <<= (block % 8);
		temp ^= 0xFF;
		buffer[64 + block / 8] &= temp;		// release it
	}
	write_sectors(buffer, superblock->bitmaps, 1);		// write bitmap back
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
		release_inode(inode_num);
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
		release_inode(inode_num);
		return;
	}
	inode->block = block_num;

	write_sectors(buffer, superblock->inodes + inode_num / (DISK_SECTOR / temp->inode_size), 1);
}

/*
	get path parts
	@param path: path to split and return value (size >= 400)
	@returns number of parts
*/
int getPath(char* path)
{
	char real[400] = {0};
	if (path[0] == '/')		// connect paths
	{
		memcpy(real, path + 1, strlen(path) - 1);
	}
	else
	{
		int strlen_dir = strlen(dir);
		memcpy(real, dir + 1, strlen_dir - 1);
		real[strlen_dir - 1] = '/';
		memcpy(real + strlen_dir, path, strlen(path));
	}
	int counter = split(real, '/');
	memcpy(path, real, strlen_real + 1);
	return counter;
}

/*
	get last inode in path
	@param path_parts: the path
	@param size: number of parts in path
	@param prev: prev node. set to 0.
	@returns last inode
*/
int followPath(char* path_parts, int size, int prev)
{
	if (size == 0)		// inode 0 is "/" folder
		return prev;
	
	read_sectors(buffer, superblock->inodes + prev / (DISK_SECTOR / temp->inode_size), 1);		// get inode
	Inode* inode = (Inode*)(buffer + (prev % (DISK_SECTOR / temp->inode_size)) * superblock->inode_size);
	int data_size = inode->size;

	if (!inode->folder)		// if file - return it
		return prev;

	read_sectors(buffer, superblock->blocks + inode->block);		// get block

	buffer[data_size] = 0;
	int lines = split(buffer, '\n');
	for (int i = 0; i < lines; i++)
	{
		const char* line = getArg(buffer, lines, i);		// compare every line
		int j = 0, flag = 1;
		while(line[j] != ',' && flag)
		{
			flag = line[j] == path_parts[j];
			j++;
		}
		if (flag && path_parts[j] == 0)		// if equal
		{
			return followPath(path_parts + strlen(path_parts) + 1, size - 1, atoi(line + j + 1));
		}
	}
	return -1;
}