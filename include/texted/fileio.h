#ifndef TEXTED_FILEIO_H
#define TEXTED_FILEIO_H

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer);

//  Saves the buffer in the file by rewriting it
int save(char* Filename, char* Buffer);

// Generates the backup file name
char* genBackupName(char* Filename);

//  Creates a backup file of the open file
int backup(char* Filename);

#endif