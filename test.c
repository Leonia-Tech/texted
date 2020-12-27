#include <stdio.h>
#include <stdlib.h>
#include "include/commands.h"
#include "include/insert.h"
#include "include/print.h"
#include <time.h>

int main(int argc, char* argv[])
{
    char* Buffer;
    char* Filename;
    int line;

    Buffer = insert();
    if(argc > 1)
        Filename = argv[1];
    else
        Filename = "a.txt";
    
    save(Filename, Buffer);
    Buffer = load(Filename);

    printf("Line: ");
    scanf("%d", &line);
    Buffer = getLine(Buffer, line);
    puts(Buffer);
    free(Buffer);
    return 0;
}
