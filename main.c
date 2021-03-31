#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <texted/edit.h>
#include <texted/print.h>
#include <texted/insert.h>
#include <texted/fileio.h>
#include <texted/texted.h>

#include <credits/credits.h>

int main(int argc, char* argv[])
{
	char* Buffer = NULL;					// Continuous buffer
	char* Prompt = NULL;
	char* Filename;							// Name of open file
	LineBuffer_s* LineBuffer;				// Array of lines
	size_t Line = 1;						// Selected row
	commans_s Command;						// Command
	size_t counter;							// Global counter
	int status = 0;							// Return status
	usr_perm_e permissions;					// Operations we can perform on this file

	// LOADING
	if (argc <= 1) {
		Filename = "a.txt";
	} else if(streq(argv[1], "--help", 7) || streq(argv[1], "-h", 3)) {
		display_help();
		return 0;
	} else if(streq(argv[1], "-v", 3) || streq(argv[1], "--version", 10) ) {
		fputs(BOLD RED " / \\--------------, \n"
			  RED " \\_,|             | \t" BLUE "TextEd\n"
			  RED "    |    TextEd   | \t" BLUE "Version: " VERSION "\n"
			  RED "    |  ,------------\n"
			  RED "    \\_/___________/\n" RESET, stdout);
		return 0;
	}
	
	#ifndef ANDROID
		else if(streq(argv[1], "--credits", 10)) {
			return credits();
	#endif
	
	} else {
		Filename = argv[1];
	}
	
	// Try to create file
	permissions = get_user_permissions(Filename);
	if(permissions == ERR_PERM &&
	   (status = createFile(Filename, permissions)))
	{
		return status;
	}

	if(!(permissions & RD_PERM)) {
		fputs(RED "Failed to read the file: Permission denied!\n" RESET, stderr);
		return -1;
	}

	// Load the file in the LineBuffer
	LineBuffer = LbLoadFile(Filename);

	// Initialize command handler
	Command.args = calloc(ARGS_NUM, sizeof(char*));
	Command.raw_command = malloc(ED_ARG_SZ);

	//Initialize Prompt
	Prompt = malloc(strlen(Filename) + 30);

	fputs(BOLD YELLOW"Welcome in Texted - " RELEASE RESET"\n", stdout);

	// MAIN LOOP
	for(;;)
	{
		sprintf(Prompt, BOLD "%s%s > "RESET,
			    get_temp() ? get_user_permission_color(TMP_PATH) : get_user_permission_color(Filename),
			    Filename);
		
		Command.raw_command = readline(Prompt);
		add_history(Command.raw_command);
		
		Command.command = Command.raw_command[0];

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
			Command.args[0] = strdup(Command.raw_command + 1);

			// Interpet arguments
			if (streq(Command.args[0], "", 1)) {
				ed_print(LineBuffer, 0);
			} else if (streq(Command.args[0], "n", 2)) {
				ed_print(LineBuffer, 1);
			} else if (streq(Command.args[0], "l", 2)) {
				fputs(getLine(LineBuffer, Line), stdout);

				// Newline coherence
				if(LineBuffer && Line != LineBuffer->LB_Size)
					goto exit_print;
			} else if (streq(Command.args[0], "ln", 3)) {
				if(LineBuffer)
					printf("%lu   %s", Line, getLine(LineBuffer, Line));

				// Newline coherence
				if(LineBuffer && Line != LineBuffer->LB_Size)
					goto exit_print;
			} else if(streq(Command.args[0], " -p", 4)) {
				ed_print_permissions(Filename);
				goto exit_print;
			} else {
				fprintf(stderr, RED "Wrong syntax for the print command\n" RESET);
				goto exit_print;
			}

			if((LineBuffer) && (LineBuffer->LineBuffer))
				putchar('\n');
		
		exit_print:
			free(Command.args[0]);
			break;
		
		case 'i': // INSERT MODE

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				break;
			}

			Command.args[0] = strdup(Command.raw_command + 1);

			if (streq(Command.args[0], "", 1)) // Write in RAM
			{
				// Load Buffer
				fputs("--INSERT MODE--\n", stdout);
				Buffer = insert();
				if(!Buffer) {
					goto exit_insert;
				}

				// Make LineBuffer from Buffer
				if(!LineBuffer)
					LineBuffer = getLineBuffer(Buffer);
				else
					LineBuffer = concatenateBuffer(LineBuffer, Buffer);
				
			}
			else if (streq(Command.args[0], "w", 2)) // Write directly to file
			{
				// Load Buffer
				fputs("--INSERT MODE--\n", stdout);
				Buffer = insert();
				if(!Buffer) {
					goto exit_insert;
				}

				// Append to file
				app_save(Filename, Buffer);
				printf("Added %lu bytes\n", strlen(Buffer));
				freeLineBuffer(LineBuffer);
				free(Buffer);
				Buffer = NULL;

				// Reload LineBuffer
				LineBuffer = LbLoadFile(Filename);
			}
			else
			{
				Buffer = NULL;
				fputs(RED"Wrong syntax for the insert (i) command\n"RESET, stderr);
			}

		exit_insert:
			PAUSE();
			if(Buffer) {
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
				break;
			}

			Buffer = getBuffer(LineBuffer);

			status = save(Filename, Buffer);
			if(status == ED_NULL_FILE_PTR || status == ED_NULL_PTR) {
				if(errno)
					perror(RED"Failed to write to the file"RESET);
				else
					fputs(RED"Failed to write to the file\n"RESET, stderr);
				
				if(Buffer)
					free(Buffer);
				Buffer = NULL;
				break;
			} else {
				printf("Written %lu bytes\n", Buffer ? strlen(Buffer) : 0U);

				if(Buffer)
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
				break;
			}

			// Handle NULL LineBuffer
			if(!LineBuffer){
				fputs(RED "File is empty!\n" RESET, stderr);
				break;
			}
			
			// Read arguments
			status = argumentParser(Command.raw_command + 1, 0, 2, &(Command.args));

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
				if(substitute(getLinePtr(LineBuffer, Line), Command.args[0], Command.args[1]))
					fprintf(stderr, RED "Failed to substitute\n" RESET);
			} else if (Command.command == 'm') {
				if(putstr(getLinePtr(LineBuffer, Line), Command.args[0], Command.args[1]))
					fprintf(stderr, RED "Failed to embed new token\n" RESET);
			}

			break;
		
		case 'a': // ADD WORD AT LINE END

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				break;
			}

			Command.args[1] = NULL;

			// Read and interpret argument
			status = argumentParser(Command.raw_command + 1, 1, 1, &(Command.args));

			// Error Handling
			if(status == ED_ERRNO) {
				perror(RED "Failed to add new word at line end");
				break;
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the append (a) command\n"RESET);
				break;
			}

			if(putstr(getLinePtr(LineBuffer, Line), ADD_MODE, Command.args[0]))
				fputs(RED "Failed to append string\n" RESET, stderr);
			break;
		
		case 'l': // SET LINE
			Command.args[0] = strdup(Command.raw_command + 1);

			// "counter" as temporary variable
			counter = strtoul(Command.args[0], NULL, 10);
			if (counter != 0 && counter < ULONG_MAX && LineBuffer && counter <= LineBuffer->LB_Size)
				Line = counter;
			else
				fprintf(stderr, RED"Wrong line number\n"RESET);
			
			free(Command.args[0]);
			break;
		
		case 'q': // EXIT
			goto loop_exit;
			break;
		
		case 'b': // GET BACKUP
			if(!backup(Filename))
				printf(ITALIC CYAN "Backup file generated: %s\n" RESET, genBackupName(Filename));
			else
				fprintf(stderr, RED "Failed to create a backup of %s: No such file or directory\n", Filename);
			break;
		
		case 'h': // PRINT HELP
			if(!Command.raw_command[1])
				display_help();
			else
				fputs(RED "Wrong syntax for the help (h) command.\n"
						  "Type h for help\n" RESET, stderr);
			break;
		
		case 'n': // NEW LINE

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				break;
			}

			Command.args[1] = NULL;

			// Read and interpret argument
			status = argumentParser(Command.raw_command + 1, 0, 1, &(Command.args));

			// Error Handling
			if(status == ED_ERRNO) {
				perror(RED "Failed to add new word at line end");
				break;
			} else if(status) {
				fprintf(stderr, RED"Wrong syntax for the new line (n) command\n"RESET);
				break;
			}

			// Add new line
			if((status = addLine(&LineBuffer, Command.args[0], Line)))
				fprintf(stderr, RED"An error occured while trying to add a new line\n"
						ITALIC "Error code: %d\n"RESET, status);
			break;

		case 'd': // DELETE LINE

			// Permission handling
			if(!(permissions & WR_PERM)) {
				fputs(RED "You don't have write permissions on this file!\n" RESET, stderr);
				break;
			}

			PAUSE();
			if(delLine(&LineBuffer, Line))
				fprintf(stderr, RED"An error occured while trying to remove line no. %lu\n"
						"Error code: %d\n"RESET, Line, status);
			else {
				// branchless if(Line != 1) Line--;
				Line -= (Line > 1);
				printf(CYAN ITALIC "New working line set to %lu\n" RESET, Line);

			}
			break;
		
		case '!':
			system(Command.raw_command + 1);
			break;

		default:
			if(Command.command)
				fprintf(stderr, RED "Invalid command\n" RESET);
			break;
		}
	}

loop_exit:
	remove(TMP_PATH);
	free(Prompt);
	free(Command.raw_command);
	free(Command.args);
	freeLineBuffer(LineBuffer);
	return 0;
}
