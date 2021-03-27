#ifndef TEXTED_INSERT_H
#define TEXTED_INSERT_H

#include <string.h>

// Insert mode
char* insert();

int getInsertArgs(char* args);

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer);

//  Saves the buffer in the file by rewriting it
int save(char* Filename, char* Buffer);

// Insert "in" into "out" before "ch".
char* strins(char* out, char* in, char ch);

// Generates the backup file name
char* genBackupName(char* Filename);

//  Creates a backup file of the open file
int backup(char* Filename);

// Adds a new line before "Position".
int addLine(char*** LineBuffer, size_t* Lines, char* NewLine, int Position);

// Delete the "Del" line
int delLine(char*** LineBuffer, size_t* Lines, int Del);

#endif // TEXTED_INSERT_H
