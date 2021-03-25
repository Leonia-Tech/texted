#ifndef TEXTED_EDIT_H
#define TEXTED_EDIT_H

// Trova quante volte il caratter ch compare in str
int strocc(const char* str, char ch);

// Dividi il Buffer in righe e restituisci il numero di righe in _Lines
char** getLineBuffer(char* Buffer, int* _Lines);

// Restituisce la linea specificata
char* getLine(char** LineBuffer, int Line);

// Restitusice un puntatore alla riga specificata
char** getLinePtr(char** LineBuffer, int Line);

void freeLineBuffer(char** LineBuffer, int Lines);

// Sostituisci
char* substitute(char** row, const char* _old, const char* _new);

// Aggiungi dopo / _before = 0 ==> aggiungi infondo alla riga
char* putstr(char** row, const char* _before, const char* _new);

// Extract tokens for the substitute function
int getTokens(char* arg, size_t size, char** toks[]);

// Read and interpret single argument command
int argumentParser(char** argument);    //! Improve for n arguments commands

int getLineBufferSize(char** LineBuffer, int Lines);

// Ricomponi il Buffer diviso in righe
char* getBuffer(char** LineBuffer, int Lines);

#endif // TEXTED_EDIT_H
