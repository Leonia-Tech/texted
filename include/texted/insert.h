#ifndef TEXTED_INSERT_H
#define TEXTED_INSERT_H

// aio.h in edit.h
#include <texted/edit.h>
#include <string.h>

#define ESC     (char)(0x1b)

// Insert mode
char* insert();

// Concatenate a LineBuffer to another
LineBuffer_s* concatenateLineBuffer(LineBuffer_s* lb1, LineBuffer_s* lb2);

// Insert 
int getInsertArgs(char* args);

// Insert "in" into "out" before "ch".
char* strins(char* out, char* in, char ch);

// Adds a new line before "Position".
int addLine(char*** LineBuffer, size_t* Lines, char* NewLine, size_t Position);

// Delete the "Del" line
int delLine(LineBuffer_s* LineBuffer, size_t Del);

#endif // TEXTED_INSERT_H
