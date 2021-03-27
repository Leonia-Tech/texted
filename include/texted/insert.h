#ifndef TEXTED_INSERT_H
#define TEXTED_INSERT_H

#include <string.h>

// Insert mode
char* insert();

// Insert 
int getInsertArgs(char* args);

// Insert "in" into "out" before "ch".
char* strins(char* out, char* in, char ch);

// Adds a new line before "Position".
int addLine(char*** LineBuffer, size_t* Lines, char* NewLine, int Position);

// Delete the "Del" line
int delLine(char*** LineBuffer, size_t* Lines, int Del);

#endif // TEXTED_INSERT_H
