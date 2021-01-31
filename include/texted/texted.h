#ifndef COMMANDS_H
#define COMMANDS_H

#define LINE_SIZE 500
#define ARG_SIZE 4
#define ED_ARG_SZ 50
#define ADD_MODE "\n"

#define ED_SUCCESS 0x00
#define ED_NULL_FILE_PTR 0x01
#define ED_INVALID_COMMAND 0x02

void empty(char* arr, int size);
int streq(char* str1, char* str2, int size);
void display_help();

#include <texted/insert.h>
#include <texted/print.h>
#include <texted/edit.h>

#endif // COMMANDS_H
