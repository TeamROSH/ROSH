#include "stdlib.h"

void ushutdown(void)
{
	syscall(G_SYSTEM, F_SHUTDOWN, 0, 0);
}

void uputc(char c)
{
	uint32_t params[1];
	params[0] = (uint32_t)c;
	syscall(G_OUTPUT, F_PUTC, params, 1);
}

void uputs(const char* str)
{
	uint32_t params[1];
	params[0] = (uint32_t)str;
	syscall(G_OUTPUT, F_PUTS, params, 1);
}

void uputi(int n)
{
	uint32_t params[1];
	params[0] = (uint32_t)n;
	syscall(G_OUTPUT, F_PUTI, params, 1);
}

void usetColor(char fore, char back)
{
	uint32_t params[2];
	params[0] = (uint32_t)fore;
	params[1] = (uint32_t)back;
	syscall(G_OUTPUT, F_COLOR, params, 2);
}

char ugetchar()
{
	char c = 0;
	uint32_t params[1];
	params[0] = (uint32_t)&c;
	syscall(G_INPUT, F_GETCHAR, params, 1);
	return c;
}

void ubflush()
{
	syscall(G_INPUT, F_BFLUSH, 0, 0);
}

void ugetline(char* pStr, int size)
{
	uint32_t params[2];
	params[0] = (uint32_t)&pStr;
	params[1] = (uint32_t)size;
	syscall(G_INPUT, F_GETLINE, params, 2);
}

void ucreate_folder(char* path)
{
	uint32_t params[1];
	params[0] = (uint32_t)path;
	syscall(G_FS, F_CREATE_D, params, 1);
}

void ucreate_file(char* path)
{
	uint32_t params[1];
	params[0] = (uint32_t)path;
	syscall(G_FS, F_CREATE_F, params, 1);
}

void udelete_file(char* path)
{
	uint32_t params[1];
	params[0] = (uint32_t)path;
	syscall(G_FS, F_FS_DELETE, params, 1);
}

int ufile_type(char* path)
{
	int res = 0;
	uint32_t params[2];
	params[0] = (uint32_t)path;
	params[1] = (uint32_t)&res;
	syscall(G_FS, F_FS_TYPE, params, 2);
	return res;
}

int uread_file(char* path, char* res)
{
	int size = 0;
	uint32_t params[3];
	params[0] = (uint32_t)path;
	params[1] = (uint32_t)&res;
	params[2] = (uint32_t)&size;
	syscall(G_FS, F_FS_READ, params, 3);
	return size;
}

void uwrite_file(char* path, const char* data, int size, int append)
{
	uint32_t params[4];
	params[0] = (uint32_t)path;
	params[1] = (uint32_t)data;
	params[2] = (uint32_t)size;
	params[3] = (uint32_t)append;
	syscall(G_FS, F_FS_WRITE, params, 4);
}

int ufile_size(char* path)
{
	int res = 0;
	uint32_t params[2];
	params[0] = (uint32_t)path;
	params[1] = (uint32_t)&res;
	syscall(G_FS, F_FS_SIZE, params, 2);
	return res;
}

void unew_process(char* path)
{
	int res = 0;
	uint32_t params[1];
	params[0] = (uint32_t)path;
	syscall(G_PROCESS, F_NEW_PROC, params, 1);
}

void usleep(uint32_t ms)
{
	uint32_t params[1];
	params[0] = (uint32_t)ms;
	syscall(G_PROCESS, F_SLEEP, params, 1);
}

void unet_info()
{
	syscall(G_NET, F_NET_INFO, 0, 0);
}

void unet_arp(uint32_t ip)
{
	uint32_t params[1];
	params[0] = (uint32_t)ip;
	syscall(G_NET, F_NET_ARP, params, 1);
}

void unet_send(uint32_t ip, char* msg)
{
	uint32_t params[2];
	params[0] = (uint32_t)ip;
	params[2] = (uint32_t)msg;
	syscall(G_NET, F_NET_ROSH, params, 2);
}