#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <texted/texted.h>
#include <texted/insert.h>

char* insert()
{
	size_t size = 200;
	const size_t INC = 100; // Increment
	char* Buffer = malloc(size * sizeof(char));
	size_t counter = 0;

	empty(Buffer, size);

	// Ask for characters until ESC is pressed
	for(char c = 0; (c = getchar()) != '\e'; ++counter) {
		Buffer[counter] = c;

		// If the buffer exceeds the size, expand the buffer by INC.
		if (counter >= size - 1){
			Buffer = realloc(Buffer, (size + INC) * sizeof(char));
			if(!Buffer)
				return NULL;
			empty(&Buffer[size], INC);
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

// Insert 'in' into 'out' before 'ch'.
char* strins(char* out, char* in, char ch)
{
	size_t size;
	char* newStr;
	char* AfterPoint;
	size_t BeforePoint = 0;

	size = strlen(out) + strlen(in) + 1;
	newStr = (char*)malloc(size * sizeof(char));
	empty(newStr, size);

	// Find the first occurrence of ch
	AfterPoint = strchr(out, ch);

	if(AfterPoint) {
		// Copy everything before the point
		BeforePoint = AfterPoint - out;
		strncpy(newStr, out, BeforePoint);
		
		// Adds "in" at the desired location
		strcat(newStr, in);

		// Adds the remaining
		strcat(newStr, AfterPoint);
	} else {
		return NULL;
	}

	return newStr;
}

char* genBackupName(char* Filename)
{
	char* BackupName;

	BackupName = strins(Filename, "-bkp", '.');
	if(!BackupName) {
		BackupName = malloc(strlen(Filename) + 5);
		strcpy(BackupName, Filename);
		strcat(BackupName, "-bkp");
	}

	return BackupName;
}

int backup(char* Filename)
{
	FILE* From, *To;
	char Buffer[LINE_SIZE];
	char* BackupName;

	BackupName = genBackupName(Filename);

	From = fopen(Filename, "r");
	if (!From)
		return ED_NULL_FILE_PTR;
	
	To = fopen(BackupName, "w");
	if (!To)
		return ED_NULL_FILE_PTR;

	while (fgets(Buffer, LINE_SIZE, From))
		fprintf(To, "%s", Buffer);

	fclose(From);
	fclose(To);
	free(BackupName);
	return ED_SUCCESS;
}

// Adds Newline in before Position (Line count starts from 1)
int addLine(char*** LineBuffer, size_t* Lines, char* NewLine, int Position)
{
    char** NewLineBuffer;
    int counter;
	int NewLen = strlen(NewLine);

	// Heap Allocation 
	NewLine = strdup(NewLine);

    if(!NewLine || (NewLen && NewLine[NewLen - 1] != '\n'))
        return ED_BAD_LINE_FORMAT;
    
    if(Position < 1 || Position > *Lines)
        return ED_BUFFER_OVERFLOW;
    
	// Initializing new LineBuffer
    NewLineBuffer = (char**)malloc(++(*Lines) * sizeof(char*));

	// Copy lines before Position
    for(counter = 0; counter < Position - 1; counter++)
        NewLineBuffer[counter] = (*LineBuffer)[counter];
	
	// Add new line
    NewLineBuffer[counter] = NewLine;

	// Copy lines after Position
    for(; counter < *Lines - 1; counter++)
        NewLineBuffer[counter+1] = (*LineBuffer)[counter];
    
    *LineBuffer = NewLineBuffer;
    return ED_SUCCESS;
}

// Delete Line number Del (Line count starts from 1)
int delLine(char*** LineBuffer, size_t* Lines, int Del)
{
    int Last = 0;
	int Len = 0;
	char** NewLineBuffer;

    if(Del < 1 || Del > *Lines)
        return ED_BUFFER_OVERFLOW;

    if(Del == *Lines)
        ++Last;
    
	// Delete Line number Del
    free((*LineBuffer)[--Del]);
    (*LineBuffer)[Del] = NULL;

	// If we're in the last line we need to delete the space
	// In the line before
    if(--Del < 0) // Check if there is a line before
        Last = 0;
	
    if((Len = strlen((*LineBuffer)[Del])) && Last) {
		(*LineBuffer)[Del][Len - 1] = '\0';
	}

	NewLineBuffer = malloc((*Lines - 1) * sizeof(char*));

	// Copy LineBuffer to NewLineBuffer
	for(int i = 0, n = 0; i < (*Lines); i++)
		if((*LineBuffer)[i])
			NewLineBuffer[n++] = (*LineBuffer)[i];
	
	*LineBuffer = NewLineBuffer;
	(*Lines)--;
    
    return ED_SUCCESS;
}