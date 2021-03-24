#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <texted/insert.h>
#include <texted/texted.h>

// Trova quante volte il caratter ch compare in str
int strocc(char* str, char ch)
{
	if(!str)
		return 0;
	
	int counter = 0;

	for (int i = 0; str[i] != 0; i++)
		if (str[i] == ch)
			counter++;

	return counter;
}

char** getLineBuffer(char* Buffer, int* Lines)
{
	int ln = strocc(Buffer, '\n') + 1;
	char** LineBuffer;
	char* ptr;
	size_t Length;

	// Handle NULL buffer
	if(Buffer) {
		LineBuffer = (char**)malloc(ln * sizeof(char*));
		empty(LineBuffer, ln);
	} else {
		*Lines = 0;
		return NULL;
	}

	// Next line
	ptr = Buffer;
	
	for (int i = 0; i < ln; i++) {
		if (strchr(ptr, '\n'))
			Length = (size_t)(strchr(ptr, '\n') - ptr) + 2;
		else
			Length = strlen(ptr) + 1;

		LineBuffer[i] = (char*)malloc(Length * sizeof(char));
		empty(LineBuffer[i], Length);

		strncpy(LineBuffer[i], ptr, --Length);
		ptr += Length;
	}

	*Lines = ln;
	return LineBuffer;
}

char* getLine(char** LineBuffer, int Line)
{
	if(!LineBuffer)
		return "\n";
	return LineBuffer[Line - 1];
}

char** getLinePtr(char** LineBuffer, int Line)
{
	if(!LineBuffer)
		return NULL;
	return &LineBuffer[Line - 1];
}

void freeLineBuffer(char** LineBuffer, int Lines)
{
	for (int i = 0; i < Lines; i++)
		free(LineBuffer[i]);
}

char* substitute(char** row, char* _old, char* _new)
{
	
	char* ptr;		// Pointer to _old in row
	char* edit;		// Temporary string
	size_t size;	// New size

	if(!(ptr = strstr(*row, _old)))
		return NULL;

	size = strlen(*row) - strlen(_old) + strlen(_new);
	edit = (char*)malloc(size * sizeof(char));

	if(!row)
		return NULL;

	empty(edit, size);

	// Copy untill old
	strncpy(edit, *row, (int)(ptr - *row));

	// Add new
	strcat(edit, _new);

	// Add after old
	ptr += strlen(_old);
	strcat(edit, ptr);

	// Move to *row and free temporary string
	*row = realloc(*row, size);
	strcpy(*row, edit);
	free(edit);

	return *row;
}

char* putstr(char** row, char* _before, char* _new)
{
	if(!row)
		return NULL;
	
	char* ptr;
	char* edit;
	int size = strlen(*row) + strlen(_new);
	*row = realloc(*row, ++size * sizeof(char));

	if (!_new && !_new[0])
		return NULL;

	if (_before[0])
	{
		ptr = strstr(*row, _before);
		edit = (char*)malloc(size * sizeof(char));

		empty(edit, size);

		strncpy(edit, *row, (int)(ptr - *row));
		strcat(edit, _new);
		strcat(edit, ptr);
		strcpy(*row, edit);
		free(edit);
	}
	else
		strcat(*row, _new);

	return *row;
}

int getTokens(char* arg, size_t size, char** toks[2])
{
	// Syntax check
	if(strocc(arg, '/') < 2) {
		*toks[0] = *toks[1] = NULL;
		return ED_WRONG_SYNTAX;
	}		

	// Extract tokens
	arg++;
	*toks[0] = strtok(arg, "/");

	for(size_t i = 1; i < size; i++)
		*toks[i] = strtok(NULL, "/");

	return ED_SUCCESS;
}

int getLineBufferSize(char** LineBuffer, int Lines)
{
	int counter = 0;
	for (int i = 0; i < Lines; i++)
		counter += strlen(LineBuffer[i]);

	return counter;
}

char* getBuffer(char** LineBuffer, int Lines)
{
	int size = getLineBufferSize(LineBuffer, Lines) + 1;
	char* Buffer = (char*)malloc(size * sizeof(char));

	empty(Buffer, strlen(Buffer));
	for (int i = 0; i < Lines; i++)
		strcat(Buffer, LineBuffer[i]);

	return Buffer;
}
