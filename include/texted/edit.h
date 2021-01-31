#ifndef EDIT_H
#define EDIT_H

char** getLineBuffer(char* Buffer, int* _Lines);        // Dividi il Buffer in righe e restituisci il numero di righe in _Lines
char* getLine(char** LineBuffer, int Line);             // Restituisce la linea specificata
char** getLinePtr(char** LineBuffer, int Line);         // Restitusice un puntatore alla riga specificata
void freeLineBuffer(char** LineBuffer, int Lines);

char* substitute(char** row, char* _old, char* _new);    // Sostituisci
char* putstr(char** row, char* _before, char* _new);     // Aggiungi dopo / _before = 0 ==> aggiungi infondo alla riga
char* editCommandInterpreter(char* arg, char** str1, char** str2);

char* getBuffer(char** LineBuffer, int Lines);           // Ricomponi il Buffer diviso in righe

#endif