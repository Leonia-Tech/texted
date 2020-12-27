#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/insert.h"
#include "../include/commands.h"

char* insert()
{
    int size = 100;
    const int INC = 50; // Increment
    char* Buffer = (char*)malloc(size * sizeof(char));
    int counter = 0;

    // Chiedi caratteri finché non è premuto ESC
    while(Buffer[counter-2] != '\e')
    {
        Buffer[counter] = getchar();

        // Se il Buffer supera Size espandi di INC il Buffer
        if(counter >= size)
        {
            Buffer = (char*)realloc(Buffer, size += INC);
        }
        counter++;
    }
    Buffer[counter-2] = '\0';

    return Buffer;
}

// Saves the Buffer in the File and frees the Buffer
int save(char* Filename, char* Buffer)
{
    FILE* File;

    File = fopen(Filename, "a");
    if(!File)
        return ED_NULL_FILE_PTR;
    
    fprintf(File, "%s", Buffer);
    fclose(File);
    free(Buffer);
    return ED_SUCCESS;
}

void empty(char* arr, int size)
{
    for(int i = 0; i < size; i++)
        arr[i] = 0;
}

// Inserisce "in" in "out" prima di "ch"
char* strins(char* out, char* in, char ch)
{
    int size = strlen(out) + strlen(in);
    char* newStr = (char*)malloc(size*sizeof(char));
    char* AfterPoint;
    int BeforePoint = 0;
    empty(newStr, size);

    AfterPoint = strchr(out, ch);                       // Trova la prima ricorrenza di ch
    BeforePoint = strlen(out) - strlen(AfterPoint);     // Stabilisce la lunghezza di out prima del punto
    strncpy(newStr, out, BeforePoint);                  // Copia tutto quello che c'è prima del punto
    strcat(newStr, in);                                 // Aggiunge in nel punto deiderato
    strcat(newStr, AfterPoint);                         // Aggiunge il resto
    return newStr;
}

int backup(char* Filename)
{
    FILE* From, *To;
    char Buffer[LINE_SIZE];
    
    From = fopen(Filename, "r");
    if(!From)
        return ED_NULL_FILE_PTR;
    
    To = fopen(strins(Filename, "-bkp", '.'), "w");
    if(!To)
        return ED_NULL_FILE_PTR;

    while(fgets(Buffer, LINE_SIZE, From))
        fprintf(To, "%s", Buffer);
    
    fclose(From);
    fclose(To);
    return ED_SUCCESS;
}