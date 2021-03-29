#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <texted/edit.h>
#include <texted/print.h>
#include <texted/insert.h>
#include <texted/fileio.h>
#include <texted/texted.h>

int main(int argc, char* argv[])
{
	char* Buffer;					// Continuous buffer
	char* Filename;					// Name of open file
	LineBuffer_s* LineBuffer;		// Array of lines
	LineBuffer_s* ExtraLineBuffer;	// Array of lines
	int Line = 1;					// Selected row
	commans_s Command;				// Command
	int counter;					// Global counter
	int status = 0;					// Return status
	usr_perm_e permissions;			// Operations we can perform on this file

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
	
	// Try to create file
	permissions = get_user_permissions(Filename);
	if((status = createFile(Filename, permissions))) {
		return status;
	}

	if(!(permissions & RD_PERM)) {
		fputs(RED "Failed to read the file: Permission denied!\n" RESET, stderr);
		return -1;
	}

	// Load the file in the LineBuffer
	Buffer = load(Filename);
	if(Buffer == (char*)0x1) {
		fputs(RED "Unexpected error while trying to load the file\n" RESET, stderr);
		return ED_NULL_FILE_PTR;
	}
	LineBuffer = getLineBuffer(Buffer);
	free(Buffer);
	Buffer = NULL;

	// Initialize command handler
	Command.args = calloc(ARGS_NUM, sizeof(char*));

	fputs(BOLD YELLOW"Welcome in Texted - " RELEASE "\n", stdout);

	// MAIN LOOP
	while (1)
	{
		printf(BOLD "%s%s > "RESET,
			   get_temp() ? get_user_permission_color(TMP_PATH) : get_user_permission_color(Filename),
			   Filename);
		
		Command.command = getchar();

		// Handle extended ASCII Table
		if(Command.command < 0) {
			PAUSE();
			fprintf(stderr, RED "Invalid command\n" RESET);
			Command.command = '\0';
			continue;
		}

		switch (Command.command)
		{
		case 'p': // PRINT MODE
			// Read arguments
			Command.args[0] = malloc(ARG_SIZE);
			fgets(Command.args[0], ARG_SIZE - 1, stdin);

			// Interpet arguments
			if (streq(Command.args[0], "\n", 1)) {
				ed_print(LineBuffer, 0);
			} else if (streq(Command.args[0], "n\n", 2)) {
				ed_print(LineBuffer, 1);
			} else if (streq(Command.args[0], "l\n", 2)) {
				fputs(getLine(LineBuffer, Line), stdout);

				// Newline coherence
				if(Line != LineBuffer->LB_Size)
					goto exit_print;
			} else if (streq(Command.args[0], "ln\n", 3)) {
				printf("%d   %s", Line, getLine(LineBuffer, Line));

				// Newline coherence
				if(Line != LineBuffer->LB_Size)
					goto exit_print;
			}
			else if(streq(Command.args[0], " -p\n", 4)) {
				ed_print_permissions(Filename);
				goto exit_print;
			}
			else
			{
				fprintf(stderr, RED "Wrong syntax for the print command\n" RESET);
				if(!(strlen(Command.args[0]) < ARG_SIZE))
					PAUSE();
				goto exit_print;
			}

			if(LineBuffer->LineBuffer)
				putchar('\n');
		
		exit_print:
			free(Command.args[0]);
			break;
		
		case 'i': // INSERT MODE

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				PAUSE();
				break;
			}

			Command.args[0] = malloc(ARG_SIZE);
			fgets(Command.args[0], ARG_SIZE - 1, stdin);

			if (streq(Command.args[0], "\n", 1)) // Start writing from the next to the last line.
			{
				fputs("--INSERT MODE--\n", stdout);
				Buffer = insert(); // Only write something if Buffer is not empty.
				if(!Buffer) {
					fputs(RED "Not enough memory\n" RESET, stderr);
					free(Command.args[0]);
					break;
				}

				// Set up the temporary LineBuffer
				if(!LineBuffer->LineBuffer && !LineBuffer->LB_Size) {
					LineBuffer = getLineBuffer(Buffer);
					ExtraLineBuffer = NULL;
					free(Command.args[0]);
					PAUSE();
					break;
				} else {
					ExtraLineBuffer = getLineBuffer(Buffer);
					--(LineBuffer->LB_Size);
				}

				// Concatenation of the first string of the temporary buffer with the last string of the LineBuffer
				LineBuffer->LineBuffer = realloc(LineBuffer, (LineBuffer->LB_Size + ExtraLineBuffer->LB_Size) * sizeof(char*));
				(LineBuffer->LineBuffer)[LineBuffer->LB_Size] = realloc((LineBuffer->LineBuffer)[LineBuffer->LB_Size],
											              strlen((LineBuffer->LineBuffer)[LineBuffer->LB_Size])  + 
														  strlen((ExtraLineBuffer->LineBuffer)[0]) + 1);
				strcat((LineBuffer->LineBuffer)[LineBuffer->LB_Size], (ExtraLineBuffer->LineBuffer)[0]);

				// Other strings
				for (int i = 1; i < ExtraLineBuffer->LB_Size; i++) {
					(LineBuffer->LineBuffer)[LineBuffer->LB_Size + i] = strdup((ExtraLineBuffer->LineBuffer)[i]);
					strcpy((LineBuffer->LineBuffer)[LineBuffer->LB_Size + i], (ExtraLineBuffer->LineBuffer)[i]);
				}

				LineBuffer->LB_Size += ExtraLineBuffer->LB_Size;
				freeLineBuffer(ExtraLineBuffer);
			}
			else if (streq(Command.args[0], "w\n", 2)) // Start writing from the end of the last line.
			{
				fputs("--INSERT MODE--\n", stdout);
				Buffer = insert(); // Only write something if Buffer is not empty.
				if(!Buffer) {
					fputs(RED "Not enough memory\n" RESET, stderr);
					free(Command.args[0]);
					break;
				}

				app_save(Filename, Buffer);
				printf("Added %lu bytes\n", strlen(Buffer));
				freeLineBuffer(LineBuffer);
				free(Buffer);

				Buffer = load(Filename);
				LineBuffer = getLineBuffer(Buffer);
			}
			else
			{
				fputs(RED"Wrong syntax for the insert (i) command\n"RESET, stderr);
			}

			PAUSE();
			
			if(!Buffer) {
				free(Buffer);
				Buffer = NULL;
			}
			free(Command.args[0]);
			break;
		
		case 'w': // SAVE
		case 'x': // SAVE AND EXIT

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				PAUSE();
				break;
			}

			getchar();
			Buffer = getBuffer(LineBuffer);
			if(save(Filename, Buffer) == ED_NULL_FILE_PTR) {
				perror(RED"Failed to write to the file"RESET);
				free(Buffer);
				Buffer = NULL;
				break;
			} else {
				printf("Written %lu bytes\n", strlen(Buffer));
				free(Buffer);
				Buffer = NULL;
				if (Command.command == 'x')
					goto loop_exit;
			}
			break;
		
		case 's': // SUBSTITUTE WORD
		case 'm': // ADD WORD AFTER

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				PAUSE();
				break;
			}

			// Handle NULL LineBuffer
			if(!LineBuffer){
				fputs(RED "File is empty!\n" RESET, stderr);
				PAUSE();
				break;
			}
			
			// Read arguments
			status = argumentParser(0, 2, &(Command.args));

			// Error handling
			if(status == ED_ERRNO) {
				perror(RED"Failed to read arguments"RESET);
				Command.command = '\0';
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the %s command\n"RESET,
						Command.command == 's' ? "substitute (s)" : "embed (m)");
				Command.command = '\0';
			}

			if (Command.command == 's') {
				if(!substitute(getLinePtr(LineBuffer, Line), Command.args[0], Command.args[1]))
					fprintf(stderr, RED "Failed to substitute\n" RESET);
			} else if (Command.command == 'm') {
				if(!putstr(getLinePtr(LineBuffer, Line), Command.args[0], Command.args[1]))
					fprintf(stderr, RED "Failed to embed new token\n" RESET);
			}

			break;
		
		case 'a': // ADD WORD AT LINE END

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				PAUSE();
				break;
			}

			Command.args[1] = NULL;

			// Read and interpret argument
			status = argumentParser(1, 1, &(Command.args));

			// Error Handling
			if(status == ED_ERRNO) {
				perror(RED "Failed to add new word at line end");
				break;
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the append (a) command\n"RESET);
				break;
			}

			putstr(getLinePtr(LineBuffer, Line), ADD_MODE, Command.args[0]);
			break;
		
		case 'l': // SET LINE
			Command.args[0] = malloc(ARG_SIZE);
			fgets(Command.args[0], ARG_SIZE, stdin);

			// "counter" as temporary variable
			counter = atoi(Command.args[0]);
			if (counter > 0 && counter <= LineBuffer->LB_Size)
				Line = counter;
			else
				fprintf(stderr, RED"Wrong line number\n"RESET);

			if(!strchr(Command.args[0], '\n'))
				PAUSE();
			break;
		
		case 'q': // EXIT
			getchar();
			goto loop_exit;
			break;
		
		case 'b': // GET BACKUP
			if(backup(Filename))
				printf(ITALIC CYAN "Backup file generated: %s\n", genBackupName(Filename));
			else
				fprintf(stderr, RED "Failed to create a backup of %s: No such file or directory\n", Filename);
			getchar();
			break;
		
		case 'h': // PRINT HELP
			display_help();
			getchar();
			break;
		
		case 'n': // NEW LINE

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				PAUSE();
				break;
			}

			Command.args[1] = NULL;

			// Read and interpret argument
			status = argumentParser(0, 1, &(Command.args));

			// Error Handling
			if(status == ED_ERRNO) {
				perror(RED "Failed to add new word at line end");
				break;
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the new line (n) command\n"RESET);
				break;
			}

			// Add new line
			if((status = addLine(&(LineBuffer->LineBuffer), &(LineBuffer->LB_Size), Command.args[0], Line)))
				fprintf(stderr, RED"An error occured while trying to add a new line\n"
						ITALIC "Error code: %d\n"RESET, status);
			break;

		case 'd': // DELETE LINE

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				PAUSE();
				break;
			}

			PAUSE();
			if(delLine(&(LineBuffer->LineBuffer), &(LineBuffer->LB_Size), Line))
				fprintf(stderr, RED"An error occured while trying to remove line no. %d\n"
						"Error code: %d\n"RESET, Line, status);
			else {
				if(Line != 1) {
					Line--;
					printf(CYAN ITALIC "New working line set to %d\n" RESET, Line);
				}
				else
					fputs(CYAN ITALIC "Line 2 became line 1\n" RESET, stdout);

				if(LineBuffer->LB_Size == 0) {
					free(LineBuffer);
					LineBuffer = NULL;
				}
			}
			break;

		default:
			if(Command.command != '\n')
				PAUSE();
			fprintf(stderr, RED "Invalid command\n" RESET);
			break;
		}
	}

loop_exit:
	remove(TMP_PATH);
	free(Command.args);
	freeLineBuffer(LineBuffer);
	return 0;
}
