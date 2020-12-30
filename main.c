#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/texted.h"

int main(int argc, char* argv[])
{
    char* Buffer;               // Buffer continuo
    char Editstr[ED_ARG_SZ] = {0}; // Stringa da cui ricavare gli argomenti per le funzioni di modifica
    char* arg1;                 // Argomenti per le funzioni di modifica
    char* arg2;
    char* Filename;             // Nome del file aperto
    char** LineBuffer;          // Buffer a righe
    char** ExtraLineBuffer;     // Extra LineBuffer per la insert mode
    int LB_Size, Line = 1;      // Numero di righe e Riga selezionata
    int ELB_Size;               // Numero di righe dell'ExtraLineBuffer
    int Status;                 // Controllo degli errori
    char Command;               // Selettore di comandi
    char args[ARG_SIZE] = {0};  // Argomenti addizionali ad un comando
    int counter;                // Contatore da ricordare

    // LOADING
    if(argc <= 1)
        Filename = "a.txt";
    else if(!streq(argv[1], "--help", 7) && !streq(argv[1], "-h", 3))
        Filename = argv[1];
    else
    {
        display_help();
        return 0;
    }
    
    
    Buffer = load(Filename);
    LineBuffer = getLineBuffer(Buffer, &LB_Size);
    free(Buffer);
    
    printf("Welcome in Texted - release 1.0\n");
    
    // MAIN LOOP
    while(1)
    {
        printf("%s > ", Filename);
        Command = getchar();

        if(Command == 'p') // PRINT MODE
        {
            Status = getPrintArgs(args);
            if(Status != ED_SUCCESS) {
                fprintf(stderr, "Invalid command\n");
                continue;
            }

            if(streq(args, "", 1))
                ed_print(LineBuffer, LB_Size, 0);
            else if(streq(args, "n", 2))
                ed_print(LineBuffer, LB_Size, 1);
            else if(streq(args, "l", 2))
                printf(getLine(LineBuffer, Line));
            else if(streq(args, "ln", 3))
                printf("%d\t%s", Line, getLine(LineBuffer, Line));
            else {
                fprintf(stderr, "\nUnexpected error in print!\n");    // Il programma non dovrebbe main poter raggiungere questa zona
                freeLineBuffer(LineBuffer, LB_Size);
                exit(1);
            }
            putchar('\n');
        }
        else if(Command == 'i') // INSERT MODE
        {
            getInsertArgs(args);
            printf("--INSERT MODE--\n");

            Buffer = insert(); // Scrivi qualcosa solo se Buffer non è vuoto.
            if(streq(Buffer, "", 1)) {
                free(Buffer);
                continue;
            }

            if(streq(args, "", 1)) // Inizia a scrivere dalla riga successiva all'ultima.
            {
                ExtraLineBuffer = getLineBuffer(Buffer, &ELB_Size);
                LineBuffer = realloc(LineBuffer, (LB_Size + ELB_Size) * sizeof(char*));
                strcat(LineBuffer[LB_Size-1], ExtraLineBuffer[0]);
                for(int i = 1; i < ELB_Size; i++)
                    LineBuffer[LB_Size+i-1] = ExtraLineBuffer[i];

                free(ExtraLineBuffer);
                LB_Size += ELB_Size - 1;
            }
            else if(streq(args, "w", 2)) // Inizia a scrivere dalla fine dell'ultima riga.
            {
                app_save(Filename, Buffer);
                printf("Added %d bytes\n", strlen(Buffer));
                free(LineBuffer);
                free(Buffer);
                Buffer = load(Filename);
                LineBuffer = getLineBuffer(Buffer, &LB_Size);
            }
            else {
                fprintf(stderr, "\nUnexpected error in insert!\n");    // Il programma non dovrebbe main poter raggiungere questa zona
                free(Buffer);
                freeLineBuffer(LineBuffer, LB_Size);
                exit(1);
            }
            free(Buffer);
        }
        else if(Command == 'w' || Command == 'x') // SAVE
        {
            getchar();
            Buffer = getBuffer(LineBuffer, LB_Size);
            save(Filename, Buffer);
            printf("Written %d bytes\n", strlen(Buffer));
            free(Buffer);
            if(Command == 'x')
                break;
        }
        else if(Command == 's' || Command == 'm')
        {
            for(counter = 0; (Editstr[counter] = getchar()) != '\n'; counter++);
            Editstr[counter] = '\0';

            memcpy(args, editCommandInterpreter(Editstr, &arg1, &arg2), ARG_SIZE);
            
            if(Command == 's')
                substitute(getLinePtr(LineBuffer, Line), arg1, arg2);
            else if(Command == 'm')
                putstr(getLinePtr(LineBuffer, Line), arg1, arg2);

            free(arg1);
            free(arg2);
            empty(Editstr, strlen(Editstr));
        }
        else if(Command == 'a')
        {
            getchar();
            arg1 = (char*)malloc(ARG_SIZE);
            empty(arg1, strlen(arg1));
            for(counter = 0; (arg1[counter] = getchar()) != '\n'; counter++);
            arg1[counter] = '\0';
            putstr(getLinePtr(LineBuffer, Line), ADD_MODE, arg1);
            arg2 = NULL;
            free(arg1);
        }
        else if(Command == 'l') // SET LINE
        {
            scanf("%s", Editstr);
            counter = atoi(Editstr); // Uso counter come variabile temporanea
            if(counter <= LB_Size)
                Line = counter;

            getchar();
        }
        else if(Command == 'q') // QUIT
        {
            getchar();
            break;
        }
        else if(Command == 'b') // GET BACKUP
        {
            backup(Filename);
            getchar();
            continue;
        }
        else if(Command == 'h') // PRINT HELP
        {
            display_help();
            getchar();
        }
        else
        {
            getchar();
            fprintf(stderr, "Invalid command\n");
            continue;
        }
    }

    freeLineBuffer(LineBuffer, LB_Size);
    return 0;
}