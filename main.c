#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <texted/edit.h>
#include <texted/print.h>
#include <texted/insert.h>
#include <texted/texted.h>

int main(int argc, char* argv[])
{
	char* Buffer;                  // Buffer continuo
	char Editstr[ED_ARG_SZ] = {0}; // Stringa da cui ricavare gli argomenti per le funzioni di modifica
	char* arg1;                    // Argomenti per le funzioni di modifica
	char* arg2;
	char* Filename;            // Nome del file aperto
	char** LineBuffer;         // Buffer a righe
	char** ExtraLineBuffer;    // Extra LineBuffer per la insert mode
	int LB_Size, Line = 1;     // Numero di righe e Riga selezionata
	int ELB_Size;              // Numero di righe dell'ExtraLineBuffer
	char Command;              // Selettore di comandi
	char args[ARG_SIZE] = {0}; // Argomenti addizionali ad un comando
	int counter;               // Contatore da ricordare

	// LOADING
	if (argc <= 1)
		Filename = "a.txt";
	else if (!streq(argv[1], "--help", 7) && !streq(argv[1], "-h", 3))
		Filename = argv[1];
	else
	{
		display_help();
		return 0;
	}

	//! Permission handling
	Buffer = load(Filename);
	LineBuffer = getLineBuffer(Buffer, &LB_Size);
	free(Buffer);

	printf("Welcome in Texted - " RELEASE "\n");

	// MAIN LOOP
	while (1)
	{
		printf("%s > ", Filename);
		Command = getchar();

		switch (Command)
		{
		case 'p': // PRINT MODE
			// Read arguments
			fgets(args, 3, stdin);

			// Interpet arguments
			if (streq(args, "\n", 1))
				ed_print(LineBuffer, LB_Size, 0);
			else if (streq(args, "n\n", 2))
				ed_print(LineBuffer, LB_Size, 1);
			else if (streq(args, "l\n", 2))
				printf(getLine(LineBuffer, Line));
			else if (streq(args, "ln\n", 3))
				printf("%d\t%s", Line, getLine(LineBuffer, Line));
			else
			{
				fprintf(stderr, "\nWrong syntax for the print command\n");
				continue;
			}

			if(LineBuffer)
				putchar('\n');
			break;
		case 'i': // INSERT MODE
			getInsertArgs(args);
			printf("--INSERT MODE--\n");

			Buffer = insert(); // Scrivi qualcosa solo se Buffer non è vuoto.
			if (!Buffer)
			{
				free(Buffer);
				continue;
			}
			PAUSE();

			if (streq(args, "", 1)) // Inizia a scrivere dalla riga successiva all'ultima.
			{
				// Prepara il LineBuffer temporaneo
				if(!LineBuffer && !LB_Size) {
					LineBuffer = getLineBuffer(Buffer, &LB_Size);
					break;
				} else {
					ExtraLineBuffer = getLineBuffer(Buffer, &ELB_Size);
					--LB_Size;
				}

				// Concatenazione dell'a prima stringa del Buffer temporaneo con l'ultima del LineBuffer
				LineBuffer = realloc(LineBuffer, (LB_Size + ELB_Size) * sizeof(char*));
				LineBuffer[LB_Size] = realloc(LineBuffer[LB_Size],
											  strlen(LineBuffer[LB_Size]) + strlen(ExtraLineBuffer[0]) + 1);
				strcat(LineBuffer[LB_Size], ExtraLineBuffer[0]);

				// Altre stringhe
				for (int i = 1; i < ELB_Size; i++) {
					LineBuffer[LB_Size + i] = strdup(ExtraLineBuffer[i]);
					strcpy(LineBuffer[LB_Size + i], ExtraLineBuffer[i]);
				}

				freeLineBuffer(ExtraLineBuffer, ELB_Size);
				LB_Size += ELB_Size;
			}
			else if (streq(args, "w", 2)) // Inizia a scrivere dalla fine dell'ultima riga.
			{
				app_save(Filename, Buffer);
				printf("Added %lu bytes\n", strlen(Buffer));
				freeLineBuffer(LineBuffer, LB_Size);
				free(Buffer);
				Buffer = load(Filename);
				LineBuffer = getLineBuffer(Buffer, &LB_Size);
				if(!Buffer)
					free(Buffer);
			}
			else
			{
				fprintf(stderr, "\nUnexpected error in insert!\n"); // Il programma non dovrebbe main poter raggiungere questa zona
				free(Buffer);
				freeLineBuffer(LineBuffer, LB_Size);
				exit(1);
			}
			// PAUSE();
			free(Buffer);
			break;
		case 'w': // SAVE
		case 'x': // SAVE AND EXIT
			getchar();
			Buffer = getBuffer(LineBuffer, LB_Size);
			if(save(Filename, Buffer) == ED_NULL_FILE_PTR) {
				perror("Failed to write to the file");
				free(Buffer);
				break;
			} else {
				printf("Written %lu bytes\n", strlen(Buffer));
				free(Buffer);
				if (Command == 'x')
					goto loop_exit;
			}
			break;
		case 's': // SUBSTITUTE WORD
		case 'm': // ADD WORD AFTER

			// Handle NULL LineBuffer
			if(!LineBuffer){
				fprintf(stderr, "File is empty!\n");
				PAUSE();
				break;
			}
			
			// Read arguments
			do {
				size_t s = 0;
				char* nl;
				char* st = NULL;
				getline(&st, &s, stdin);
				nl = strchr(st, '\n');
				if(nl)
					nl[0] = '\0';
				strncpy(Editstr, st, MIN(ED_ARG_SZ, s));
			} while(0);

			// Interpret arguments
			do {
				char* s = editCommandInterpreter(Editstr, &arg1, &arg2);
				if(s)
					memcpy(args, s, ARG_SIZE);
				else
					empty(args, ARG_SIZE);
			} while(0);

			if (Command == 's')
				substitute(getLinePtr(LineBuffer, Line), arg1, arg2);
			else if (Command == 'm')
				putstr(getLinePtr(LineBuffer, Line), arg1, arg2);

			free(arg1);
			free(arg2);
			empty(Editstr, strlen(Editstr));
			break;
		case 'a': // ADD WORD AT LINE END
			getchar();
			arg1 = (char*)malloc(ARG_SIZE);
			arg2 = NULL;
			empty(arg1, strlen(arg1));

			do {
				size_t s = ARG_SIZE;
				char* nl;
				getline(&arg1, &s, stdin);
				nl = strchr(arg1, '\n');
				if(nl)
					nl[0] = '\0';
			} while(0);

			putstr(getLinePtr(LineBuffer, Line), ADD_MODE, arg1);
			free(arg1);
			break;
		case 'l': // SET LINE
			scanf("%s", Editstr);
			counter = atoi(Editstr); // Uso counter come variabile temporanea
			if (counter > 0 && counter <= LB_Size)
				Line = counter;

			PAUSE();
			break;
		case 'q': // EXIT
			getchar();
			goto loop_exit;
			break;
		case 'b': // GET BACKUP
			backup(Filename);
			getchar();
			break;
		case 'h': // PRINT HELP
			display_help();
			getchar();
			break;
		default:
			getchar();
			fprintf(stderr, "Invalid command\n");
			break;
		}
	}

loop_exit:
	freeLineBuffer(LineBuffer, LB_Size);
	return 0;
}
