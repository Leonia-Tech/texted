#ifndef INSERT_H
#define INSERT_H

char* insert();                             // Insert mode
int getInsertArgs(char* args);
char* strins(char* out, char* in, char ch); // Inserisce "in" in "out" prima di "ch"
int app_save(char* Filename, char* Buffer); // Salva il buffer nel file in append
int save(char* Filename, char* Buffer);     // Salva il buffer nel file risrivendolo
int backup(char* Filename);                 // Crea un backup-file del file aperto

#endif