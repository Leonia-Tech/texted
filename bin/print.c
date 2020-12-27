#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "../include/commands.h"
#include "../include/print.h"

// https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c

char* load(char* Filename)
{
    FILE* File;
    struct stat st;
    stat(Filename, &st);
    char Temp[LINE_SIZE] = {0};
    char* Buffer = (char*)malloc(st.st_size*sizeof(char));
    empty(Buffer, st.st_size);

    File = fopen(Filename, "r");
    while(fgets(Temp, LINE_SIZE, File))
        strcat(Buffer, Temp);

    fclose(File);
    return Buffer;
}

char* getFirstLine(char* Buffer)
{
    char* str = (char*)malloc(LINE_SIZE*sizeof(char)); // alloca memoria per la linea
    int counter = 0;

    empty(str, LINE_SIZE); // Pulisci la memoria allocata
    while(Buffer[counter] != '\n') counter++;
    str = strncpy(str, Buffer, counter);
    str[counter] = '\0';
    return str;
}

char* getLine(char* Buffer, int Line)
{
    char* str = Buffer;
    Line--;

    for(int i = 0; i < Line && (str = strchr(str, '\n')); i++ ) str = &str[1];
    str = getFirstLine(str);
    return str;
}