#ifndef COMMANDS_H
#define COMMANDS_H

#define LINE_SIZE 200

typedef struct
{
	char Instruction;
	char** argv;
	int argc;
} Command;

void empty(char* arr, int size);

#endif // COMMANDS_H
