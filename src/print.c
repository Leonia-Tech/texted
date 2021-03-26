#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>

#include <texted/insert.h>
#include <texted/texted.h>

// https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c

char* load(char* Filename)
{
	FILE* File;
	struct stat st;
	char Temp[LINE_SIZE] = {0};
	char* Buffer = NULL;

	if(!~stat(Filename, &st)) {
		if(errno != ENOENT) {
			perror(RED"Failed to read file infos"RESET);
			exit(-1);
		}
	} else {
		// Check file size
		if(st.st_size > 0) {
			Buffer = malloc(st.st_size * sizeof(char));
			empty(Buffer, st.st_size);
		}
	}

	// Open file or create it
	File = fopen(Filename, "r");
	if (!File) {
		File = fopen(Filename, "w");
		fclose(File);
		return NULL;
	}

	// If file is empty exit with null
	if(!Buffer) {
		fclose(File);
		return Buffer;
	}

	// Read the whole file
	while (fgets(Temp, LINE_SIZE, File))
		strcat(Buffer, Temp);

	fclose(File);
	return Buffer;
}

void ed_print(char** LineBuffer, int Lines, int LineNum)
{
	if (LineNum)
		LineNum = 1;

	for (int i = 0; i < Lines; i++)
	{
		if (LineNum)
			printf("%d   ", LineNum++); 	  // First prints the line, than increments the number
		fputs(LineBuffer[i], stdout);         // Line ends with \n, except the last one which ends with 0x00
	}
}

void display_help()
{
	fputs(BOLD BLUE "--HELP--\n\n"

		   "p:\t\tprint\n"
		   "-l\t\tcurrent line\n"
		   "-n\t\twith line numbers\n"
		   "-ln\t\t-l + -n\n\n"

		   "i:\t\tinsert mode\n"
		   "-w\t\tsave after quitting insert mode\n"
		   "esc:\t\texit insert mode\n\n"

		   "w:\t\tsave\n"
		   "x:\t\tsave and exit\n"
		   "b:\t\tgenerate backup file\n"
		   "q:\t\tquit\n\n"

		   "s:\t\tsubstitute word (ed syntax)\n"
		   "m:\t\tadd word before ... (ed syntax)\n"
		   "a:\t\tappend at the end of the edit line\n"
		   "l:\t\tset edit line\n\n"

		   "n:\t\tadd a new line before the edit line\n"
		   "d:\t\tremove the edit line\n"RESET, stdout);
}
