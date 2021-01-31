#ifndef PRINT_H
#define PRINT_H

char* load(char* Filename);								  // Carica un file nella RAM
int getPrintArgs(char* args);							  // Trova gli argomenti del comando p
void ed_print(char** LineBuffer, int Lines, int LineNum); // Stampa l'intero file

#endif // PRINT_H
