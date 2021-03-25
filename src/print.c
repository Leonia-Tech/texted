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
			printf("%d   ", LineNum++); 	  // Prima stampa e poi incrementa
		fputs(LineBuffer[i], stdout);         // La riga termina con \n, tranne la finale che termina con 0x00
	}
}

void display_help()
{
	fputs(BOLD BLUE "--HELP--\n\n"

		   "p:\t\tstampa\n"
		   "-l\t\triga corrente\n"
		   "-n\t\tcon numeri di riga\n"
		   "-ln\t\t-l + -n\n\n"

		   "i:\t\tinsert mode\n"
		   "-w\t\tsalva dopo l'uscita dalla insert mode\n"
		   "esc:\t\tesci dalla insert mode\n\n"

		   "w:\t\tsalva\n"
		   "x:\t\tsalva ed esci\n"
		   "b:\t\tcrea il file di backup\n"
		   "q:\t\tesci\n\n"

		   "s:\t\tsosttuisci parola (ed syntax)\n"
		   "m:\t\tinserisci parola prima di ... (ed syntax)\n"
		   "a:\t\tinserisci a fine riga\n"
		   "l:\t\tsetta la riga di modifica\n\n"

		   "n:\t\taggiungi riga prima della riga di mofifica\n"
		   "d:\t\trimuovi la riga di modifica\n"RESET, stdout);
}
