#ifndef TEXTED_PRINT_H
#define TEXTED_PRINT_H

// Carica un file nella RAM
char* load(char* Filename);

// Trova gli argomenti del comando p
int getPrintArgs(char* args);

// Stampa l'intero file
void ed_print(char** LineBuffer, int Lines, int LineNum);

// Stampa i permessi del file
void ed_print_permissions(const char* Filename);

#endif // TEXTED_PRINT_H
