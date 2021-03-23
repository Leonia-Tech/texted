#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <texted/texted.h>
#include <texted/insert.h>

char* insert()
{
	size_t size = 200;
	const int INC = 100; // Increment
	char* Buffer = malloc(size * sizeof(char));
	size_t counter = 0;

	empty(Buffer, strlen(Buffer));

	// Chiedi caratteri finché non è premuto ESC
	for(char c = 0; (c = getchar()) != '\e'; ++counter) {
		Buffer[counter] = c;

		// Se il Buffer supera size espandi di INC il Buffer
		if (counter >= size){
			Buffer = realloc(Buffer, size + INC);
			empty(&Buffer[size], strlen(&Buffer[size]));
			size += INC;
		}
	}

	if(counter < 0)
	{
		free(Buffer);
		return NULL;
	}

	return Buffer;
}

int getInsertArgs(char* args)
{
	args[0] = getchar();
	if (args[0] != '\n')
	{
		getchar();
		if (args[0] != 'w')
			return ED_INVALID_COMMAND;
	}

	for (int i = 0; i < ARG_SIZE; i++)
		if (args[i] == '\n')
			args[i] = 0x00;

	return ED_SUCCESS;
}

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer)
{
	FILE* File;

	File = fopen(Filename, "a");
	if (!File)
		return ED_NULL_FILE_PTR;

	fprintf(File, "%s", Buffer);
	fclose(File);
	return ED_SUCCESS;
}

int save(char* Filename, char* Buffer)
{
	FILE* File;

	File = fopen(Filename, "w");
	if (!File)
		return ED_NULL_FILE_PTR;

	fprintf(File, "%s", Buffer);
	fclose(File);
	return ED_SUCCESS;
}

int streq(char* str1, char* str2, int size)
{
	for (int i = 0; i < size; i++)
		if (str1[i] != str2[i])
			return 0;

	return 1;
}

// Inserisce "in" in "out" prima di "ch"
char* strins(char* out, char* in, char ch)
{
	int size = strlen(out) + strlen(in);
	char* newStr = (char*)malloc(size * sizeof(char));
	char* AfterPoint;
	int BeforePoint = 0;
	empty(newStr, size);

	AfterPoint = strchr(out, ch);                   // Trova la prima ricorrenza di ch
	BeforePoint = strlen(out) - strlen(AfterPoint); // Stabilisce la lunghezza di out prima del punto
	strncpy(newStr, out, BeforePoint);              // Copia tutto quello che c'è prima del punto
	strcat(newStr, in);                             // Aggiunge in nel punto deiderato
	strcat(newStr, AfterPoint);                     // Aggiunge il resto
	return newStr;
}

int backup(char* Filename)
{
	FILE* From, *To;
	char Buffer[LINE_SIZE];

	From = fopen(Filename, "r");
	if (!From)
		return ED_NULL_FILE_PTR;

	To = fopen(strins(Filename, "-bkp", '.'), "w");
	if (!To)
		return ED_NULL_FILE_PTR;

	while (fgets(Buffer, LINE_SIZE, From))
		fprintf(To, "%s", Buffer);

	fclose(From);
	fclose(To);
	return ED_SUCCESS;
}
