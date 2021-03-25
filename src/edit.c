#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <texted/insert.h>
#include <texted/texted.h>

// Trova quante volte il caratter ch compare in str
int strocc(const char* str, char ch)
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

char* substitute(char** row, const char* _old, const char* _new)
{
	
	char* ptr;		// Pointer to _old in row
	char* edit;		// Temporary string
	size_t size;	// New size

	if(!(ptr = strstr(*row, _old)))
		return NULL;
	
	if(_new == NULL)
		_new = "";

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

char* putstr(char** row, const char* _before, const char* _new)
{
	if(!row || !_new || !_new[0])
		return NULL;
	
	char* ptr;
	char* edit;
	size_t size;
	
	// Initial check
	if(_before) {
		ptr = strstr(*row, _before);
		if(!ptr)
			return NULL;
	}
	
	// Allocation of new space
	size = strlen(*row) + strlen(_new);
	*row = realloc(*row, ++size * sizeof(char));
	if(!row)
		return NULL;

	if (_before) {
		// After realloc ptr points to a non-valid location
		ptr = strstr(*row, _before);
		
		edit = (char*)malloc(size * sizeof(char));
		empty(edit, size);

		// Copy untill before
		strncpy(edit, *row, (size_t)(ptr - *row));

		// Add new
		strcat(edit, _new);

		// Copy after before
		strcat(edit, ptr);

		// Put in row
		strcpy(*row, edit);
		free(edit);
	} else {
		(*row)[strlen(*row) - 1] = '\0';
		strcat(*row, _new); // If before is NULL we use this function to concatenate
		(*row)[strlen(*row)] = '\n';
	}
	return *row;
}

int getTokens(char* arg, size_t size, char** toks[])
{
	// Arguments check
	if(size < 2 && arg && toks && arg[0])
		return ED_FUNCTION_ERROR;
	
	// Syntax check
	if(strocc(arg, '/') != size) {
		*toks[0] = *toks[1] = NULL;
		return ED_WRONG_SYNTAX;
	}		

	// Extract tokens
	*toks[0] = strtok(arg, "/");

	for(size_t i = 1; i < size; i++)
		*toks[i] = strtok(NULL, "/");

	return ED_SUCCESS;
}

int argumentParser(int del_new_line, size_t args_number, char** argument[])
{
	size_t s = 0;
	char* tmp = NULL;
	char* nl;
	
	// Read
	if(!~getline(&tmp, &s, stdin)){
		return ED_ERRNO;
	}

	// Delete newline if any (needed)
	if(del_new_line) {
		nl = strchr(tmp, '\n');
		if(nl)
			nl[0] = '\0';
	}
	
	// Check syntax
	if(tmp[0] != '/' || (args_number == 1 ? (strocc(tmp+1, '/') > args_number) : (strocc(tmp+1, '/') != args_number)))
		return ED_WRONG_SYNTAX;
	
	// Extract token
	(*argument)[0] = strtok(tmp+1, "/");
	for(size_t i = 1; i < args_number; ++i)
		(*argument)[i] = strtok(NULL, "/");
	
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
