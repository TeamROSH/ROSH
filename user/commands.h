#ifndef COMMANDS_H
#define COMMANDS_H

typedef void (*command)(char* argv, int argc);

void echo(char* argv, int argc);

#endif