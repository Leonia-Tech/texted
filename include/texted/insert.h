#ifndef TEXTED_INSERT_H
#define TEXTED_INSERT_H

#include <string.h>

// Empty a buffer with zeroes
#define empty(_buffer, _size)   memset((_buffer), 0, (_size))

// Insert mode
char* insert();

int getInsertArgs(char* args);

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer);

// Salva il buffer nel file risrivendolo
int save(char* Filename, char* Buffer);

int streq(char* str1, char* str2, int size);

// Inserisce "in" in "out" prima di "ch"
char* strins(char* out, char* in, char ch);

// Crea un backup-file del file aperto
int backup(char* Filename);

#endif // TEXTED_INSERT_H
