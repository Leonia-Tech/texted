#ifndef TEXTED_FILEIO_H
#define TEXTED_FILEIO_H

#include <texted/edit.h>

// Load a file into RAM
char* loadFile(char* Filename);

// Get the size of the File in bytes
ssize_t getFileSize(char* Filename);

// Load a File into a LineBuffer
LineBuffer_s* lbLoadFile(char* Filename);

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer);

//  Saves the buffer in the file by rewriting it
int save(char* Filename, char* Buffer);

// Generates the backup file name
char* genBackupName(char* Filename);

// Creates a backup file of the open file
int backup(char* Filename);

// "temp" indicates if the file is created just temporarely
void set_temp();
void clr_temp();
int get_temp();

#endif
