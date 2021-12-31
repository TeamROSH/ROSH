#include "fs.h"
#include "../kernel/ports/ata_pio.h"
#include "../libc/memory.h"
#include "../libc/string.h"

uint8_t disk_buffer[DISK_SECTOR + 1];
Superblock s;
Superblock* superblock = &s;
char dir[200] = {0};

int getPath(char* path);
int take_inode();
int take_block();
void release_inode(int inode);
void release_block(int block);
int followPath(char* path_parts, int size, int prev);
Inode* getInode(int inode_num);
Block* getBlock(int block_num);
void writeInode(int inode_num);
void writeBlock(int block_num);
void addInodeToFolder(char* path, int inode_num);

void init_fs()
{
	read_sectors((uint32_t)disk_buffer, FS_SECTOR, 1);		// read Superblock
	Superblock* temp = (Superblock*)disk_buffer;
	dir[0] = '/';
	if (temp->checksum == FS_EXISTS)		// if fs exists
	{
		memcpy(superblock, disk_buffer, sizeof(Superblock));
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

		write_sectors((uint32_t)disk_buffer, FS_SECTOR, 1);		// write new Superblock
		memcpy(superblock, disk_buffer, sizeof(Superblock));

		for (int i = 0; i < DISK_SECTOR; i++)
			disk_buffer[i] = 0;
		write_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// init empty bitmap

		create_folder("/");
	}
}

int take_inode()
{
	read_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// read bitmap
	int found = -1;
	for (int i = 0; i < superblock->inodes_num; i++)		// for every inode
	{
		uint8_t bit = disk_buffer[i / 8] << (i % 8);		// get its bit in bitmap
		if (bit % 2 == 0)		// if not taken
		{
			found = i;
			uint8_t temp = 1;
			temp <<= (i % 8);
			disk_buffer[i / 8] |= temp;		// take it
			break;
		}
	}
	write_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// write bitmap back
	return found;
}

void release_inode(int inode)
{
	read_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// read bitmap
	uint8_t bit = disk_buffer[inode / 8] << (inode % 8);		// get its bit in bitmap
	if (bit % 2 == 1)		// if taken
	{
		uint8_t temp = 1;
		temp <<= (inode % 8);
		temp ^= 0xFF;
		disk_buffer[inode / 8] &= temp;		// release it
	}
	write_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// write bitmap back
}

void release_block(int block)
{
	read_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// read bitmap
	uint8_t bit = disk_buffer[64 + block / 8] << (block % 8);		// get its bit in bitmap
	if (bit % 2 == 1)		// if taken
	{
		uint8_t temp = 1;
		temp <<= (block % 8);
		temp ^= 0xFF;
		disk_buffer[64 + block / 8] &= temp;		// release it
	}
	write_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// write bitmap back
}

int take_block()
{
	read_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// read bitmap
	int found = -1;
	for (int i = 0; i < superblock->blocks_num; i++)		// for every inode
	{
		uint8_t bit = disk_buffer[64 + i / 8] << (i % 8);		// get its bit in bitmap
		if (bit % 2 == 0)		// if not taken
		{
			found = i;
			uint8_t temp = 1;
			temp <<= (i % 8);
			disk_buffer[64 + i / 8] |= temp;		// take it
			break;
		}
	}
	write_sectors((uint32_t)disk_buffer, superblock->bitmaps, 1);		// write bitmap back
	return found;
}

void create_folder(char* path)
{
	int inode_num = take_inode();		// allocate inode
	if (inode_num == -1)
		return;

	Inode* inode = getInode(inode_num);
	inode->folder = 1;
	inode->size = 0;
	
	int block_num = take_block();		// allocate block
	if (block_num == -1)
	{
		release_inode(inode_num);
		return;
	}
	inode->block = block_num;

	writeInode(inode_num);

	addInodeToFolder(path, inode_num);
}

void create_file(char* path)
{
	int inode_num = take_inode();
	if (inode_num == -1)
		return;

	Inode* inode = getInode(inode_num);
	inode->folder = 0;
	inode->size = 0;
	
	int block_num = take_block();
	if (block_num == -1)
	{
		release_inode(inode_num);
		return;
	}
	inode->block = block_num;

	writeInode(inode_num);

	addInodeToFolder(path, inode_num);
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
	int strlen_real = strlen(real);
	int counter = split(real + 1, '/');
	memcpy(path, real + 1, strlen_real - 1);
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
	
	Inode* inode = getInode(prev);
	int data_size = inode->size;

	if (!inode->folder)		// if file - return it
		return prev;

	getBlock(inode->block);

	disk_buffer[data_size] = 0;
	int lines = split(disk_buffer, '\n');
	for (int i = 0; i < lines; i++)
	{
		const char* line = getArg(disk_buffer, lines, i);		// compare every line
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

Inode* getInode(int inode_num)
{
	read_sectors((uint32_t)disk_buffer, superblock->inodes + inode_num / (DISK_SECTOR / superblock->inode_size), 1);		// get inode
	return (Inode*)(disk_buffer + (inode_num % (DISK_SECTOR / superblock->inode_size)) * superblock->inode_size);
}

Block* getBlock(int block_num)
{
	read_sectors((uint32_t)disk_buffer, superblock->blocks + block_num, 1);		// get inode
	return (Block*)(disk_buffer);
}

void writeInode(int inode_num)
{
	write_sectors((uint32_t)disk_buffer, superblock->inodes + inode_num / (DISK_SECTOR / superblock->inode_size), 1);
}

void writeBlock(int block_num)
{
	write_sectors((uint32_t)disk_buffer, superblock->blocks + block_num, 1);
}

void addInodeToFolder(char* path, int inode_num)
{
	char temp[400] = {0};
	memcpy(temp, path, strlen(path));
	int parts = getPath(temp);		// split path
	int prev = followPath(temp, parts - 1, 0);		// get containing dir inode
	if (prev == -1)
		return;
	Inode* inode = getInode(prev);

	const char* name = getArg(temp, parts, parts - 1);		// update containing dir size
	int data_size = inode->size;
	char inode_str[3] = {0};
	itoa(inode_num, inode_str);
	inode->size += strlen(name) + 1 + strlen(inode_str) + 1;
	int dir_block = inode->block;
	writeInode(prev);
	
	getBlock(dir_block);			// update block data
	memcpy(disk_buffer + data_size, name, strlen(name));
	data_size += strlen(name);
	disk_buffer[data_size] = ',';
	data_size++;
	memcpy(disk_buffer + data_size, inode_str, strlen(inode_str));
	data_size += strlen(inode_str);
	disk_buffer[data_size] = '\n';
	data_size++;
	writeBlock(dir_block);
}