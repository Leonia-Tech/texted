#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <texted/texted.h>

#include "texted.h"

// https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c

char* load(char* Filename)
{
	FILE* File;
	struct stat st;
	stat(Filename, &st);
	char Temp[LINE_SIZE] = {0};
	char* Buffer = (char*)malloc(st.st_size * sizeof(char));
	empty(Buffer, st.st_size);

	File = fopen(Filename, "r");
	if (!File)
		File = fopen(Filename, "w+");

	while (fgets(Temp, LINE_SIZE, File))
		strcat(Buffer, Temp);

	fclose(File);
	return Buffer;
}

int getPrintArgs(char* args) // ! for n get s error
{
	empty(args, ARG_SIZE);
	args[0] = getchar();
	if (args[0] != '\n')
	{
		if (args[0] == 'n') // pn
		{
			args[1] = getchar(); // '\n' expected
			if (args[1] != '\n')
			{
				return ED_INVALID_COMMAND;
			}
		}
		else if (args[0] == 'l') // pl-
		{
			args[1] = getchar();
			if (args[1] == 'n') // pln
			{
				args[2] = getchar(); // '\n' expected
				if (args[2] != '\n')
				{
					return ED_INVALID_COMMAND;
				}
			}
			else if (args[1] != '\n')
				return ED_INVALID_COMMAND;
		} // pl
		else
			return ED_INVALID_COMMAND;
	} // p

	for (int i = 0; i < ARG_SIZE; i++)
		if (args[i] == '\n')
			args[i] = 0;

	return ED_SUCCESS;
}

void ed_print(char** LineBuffer, int Lines, int LineNum)
{
	if (LineNum)
		LineNum = 1;

	for (int i = 0; i < Lines; i++)
	{
		if (LineNum)
			printf("%d\t", LineNum++); // Prima stampa e poi incrementa
		printf(LineBuffer[i]);         // La riga termina con \n, tranne la finale che termina con 0x00
	}
}

void display_help()
{
	printf("--HELP--\n\n"
		   "p:\t\tstampa\n"
		   "-l\t\triga corrente\n"
		   "-n\t\tcon numeri di riga\n"
		   "-ln\t\t-l + -n\n\n"
		   "i:\t\tinsert mode\n"
		   "-w\t\tsalva dopo l'uscita dalla insert mode\n"
		   "esc:\t\tesci dalla innsert mode\n\n"
		   "w:\t\tsalva\n"
		   "x:\t\tsalva ed esci\n"
		   "b:\t\tcrea il file di backup\n"
		   "q:\t\tesci\n\n"
		   "s:\t\tsosttuisci parola (ed syntax)\n"
		   "m:\t\tinserisci parola dopo ... (ed syntax)\n"
		   "a:\t\tinserisci a fine riga\n"
		   "l:\t\tsetta la riga di modifica\n");
}
