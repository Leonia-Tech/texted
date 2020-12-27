#ifndef INSERT_H
#define INSERT_H

#define ED_SUCCESS          0x00
#define ED_NULL_FILE_PTR    0x01

char* insert();
char* strins(char* out, char* in, char ch);
int save(char* Filename, char* Buffer);
int backup(char* Filename);

#endif