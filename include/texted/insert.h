#ifndef TEXTED_INSERT_H
#define TEXTED_INSERT_H

#include <string.h>

// Insert mode
char* insert();

int getInsertArgs(char* args);

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer);

// Salva il buffer nel file risrivendolo
int save(char* Filename, char* Buffer);

// Inserisce "in" in "out" prima di "ch"
char* strins(char* out, char* in, char ch);

// Genera il nome del file di backup
char* genBackupName(char* Filename);

// Crea un backup-file del file aperto
int backup(char* Filename);

// Aggiunge una nuova riga prima di "Position"
int addLine(char*** LineBuffer, size_t* Lines, char* NewLine, int Position);

// Elimina la riga "Del"
int delLine(char*** LineBuffer, size_t* Lines, int Del);

#endif // TEXTED_INSERT_H
