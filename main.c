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
	char* arg1 = NULL;             // Argomenti per le funzioni di modifica
	char* arg2 = NULL;
	char* Filename;            // Nome del file aperto
	char** LineBuffer;         // Buffer a righe
	char** ExtraLineBuffer;    // Extra LineBuffer per la insert mode
	int LB_Size, Line = 1;     // Numero di righe e Riga selezionata
	int ELB_Size;              // Numero di righe dell'ExtraLineBuffer
	char Command;              // Selettore di comandi
	char args[ARG_SIZE] = {0}; // Argomenti addizionali ad un comando
	int counter;               // Contatore da ricordare
	int status = 0;			   // Return status

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

	fputs(BOLD YELLOW"Welcome in Texted - " RELEASE "\n", stdout);

	// MAIN LOOP
	while (1)
	{
		printf(BOLD GREEN"%s > "RESET, Filename);
		Command = getchar();

		// Handle extended ASCII Table
		if(Command < 0) {
			PAUSE();
			fprintf(stderr, RED "Invalid command\n" RESET);
			Command = '\0';
			continue;
		}

		switch (Command)
		{
		case 'p': // PRINT MODE
			// Read arguments
			fgets(args, 4, stdin);

			// Interpet arguments
			if (streq(args, "\n", 1))
				ed_print(LineBuffer, LB_Size, 0);
			else if (streq(args, "n\n", 2))
				ed_print(LineBuffer, LB_Size, 1);
			else if (streq(args, "l\n", 2))
				fputs(getLine(LineBuffer, Line), stdout);
			else if (streq(args, "ln\n", 3))
				printf("%d   %s", Line, getLine(LineBuffer, Line));
			else
			{
				fprintf(stderr, RED "Wrong syntax for the print command\n" RESET);
				PAUSE();
				continue;
			}

			if(LineBuffer)
				putchar('\n');
			break;
		
		case 'i': // INSERT MODE
			getInsertArgs(args);
			fputs("--INSERT MODE--\n", stdout);

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
				fputs("\n"RED"Unexpected error in insert!\n"RESET, stderr); // Il programma non dovrebbe main poter raggiungere questa zona
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
				perror(RED"Failed to write to the file"RESET);
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
				fputs(RED "File is empty!\n" RESET, stderr);
				PAUSE();
				break;
			}
			
			// Read arguments
			do {
				char** array[] = {&arg1, &arg2};
				status = argumentParser(0, 2, array);
			}while(0);

			// Error handling
			if(status == ED_ERRNO) {
				perror(RED"Failed to read arguments"RESET);
				Command = '\0';
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the %s command\n"RESET,
						Command == 's' ? "substitute (s)" : "embed (m)");
				Command = '\0';
			}

			if (Command == 's') {
				if(!substitute(getLinePtr(LineBuffer, Line), arg1, arg2))
					fprintf(stderr, RED "Failed to substitute\n" RESET);
			} else if (Command == 'm') {
				if(!putstr(getLinePtr(LineBuffer, Line), arg1, arg2))
					fprintf(stderr, RED "Failed to embed new token\n" RESET);
			}

			empty(Editstr, strlen(Editstr));
			break;
		
		case 'a': // ADD WORD AT LINE END
			arg2 = NULL;

			// Read and interpret argument
			do {
				char** array[] = {&arg1};
				status = argumentParser(1, 1, array); //! Don't free!!!
			}while(0);

			// Error Handling
			if(status == ED_ERRNO) {
				perror(RED "Failed to add new word at line end");
				break;
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the append (a) command\n"RESET);
				break;
			}

			putstr(getLinePtr(LineBuffer, Line), ADD_MODE, arg1);
			break;
		
		case 'l': // SET LINE
			scanf("%s", Editstr);
			counter = atoi(Editstr); // Uso counter come variabile temporanea
			if (counter > 0 && counter <= LB_Size)
				Line = counter;
			else
				fprintf(stderr, RED"Wrong line number\n"RESET);

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
		
		case 'n': // NEW LINE
			arg2 = NULL;

			// Read and interpret argument
			do {
				char** array[] = {&arg1};
				status = argumentParser(0, 1, array);
			} while(0);

			// Error Handling
			if(status == ED_ERRNO) {
				perror(RED "Failed to add new word at line end");
				break;
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the new line (n) command\n"RESET);
				break;
			}

			// Add new line
			if((status = addLine(&LineBuffer, &LB_Size, arg1, Line)))
				fprintf(stderr, RED"An error occured while trying to add a new line\n"
						ITALIC "Error code: %d\n"RESET, status);
			break;

		case 'd': // DELETE LINE
			PAUSE();
			if(delLine(&LineBuffer, &LB_Size, Line))
				fprintf(stderr, RED"An error occured while trying to remove line no. %d\n"
						"Error code: %d\n"RESET, Line, status);
			else {
				Line--;
				printf(CYAN ITALIC "New working line set to %d\n" RESET, Line);
			}
			break;

		default:
			PAUSE();
			fprintf(stderr, RED "Invalid command\n" RESET);
			break;
		}
	}

loop_exit:
	freeLineBuffer(LineBuffer, LB_Size);
	return 0;
}
