#ifndef INSERT_H
#define INSERT_H

// Insert mode
char* insert();

int getInsertArgs(char* args);

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer);

// Salva il buffer nel file risrivendolo
int save(char* Filename, char* Buffer);

void empty(char* arr, int size);

int streq(char* str1, char* str2, int size);

// Inserisce "in" in "out" prima di "ch"
char* strins(char* out, char* in, char ch);

// Crea un backup-file del file aperto
int backup(char* Filename);

#endif // INSERT_H
