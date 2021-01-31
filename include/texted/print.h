#ifndef TEXTED_PRINT_H
#define TEXTED_PRINT_H

// Carica un file nella RAM
char* load(char* Filename);

// Trova gli argomenti del comando p
int getPrintArgs(char* args);

// Stampa l'intero file
void ed_print(char** LineBuffer, int Lines, int LineNum);

// Stampa la lista dei comandi
void display_help();

#endif // TEXTED_PRINT_H
