#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <texted/insert.h>
#include <texted/texted.h>

// Find how many times the character ch appears in str
unsigned int strocc(const char* str, char ch)
{
	if(!str)
		return 0;
	
	unsigned int counter = 0;

	for (unsigned int i = 0; str[i] != 0; i++)
		if (str[i] == ch)
			counter++;

	return counter;
}

int streq(char* str1, char* str2, size_t size)
{
	for (size_t i = 0; i < size; i++)
		if (str1[i] != str2[i])
			return 0;

	return 1;
}

LineBuffer_s* getLineBuffer(char* Buffer)
{
	LineBuffer_s* linebuff = malloc(sizeof(LineBuffer_s));
	char* ptr;
	size_t Length = 0;

	linebuff->LB_Size = strocc(Buffer, '\n') + 1;

	// Handle NULL buffer
	if(Buffer) {
		linebuff->LineBuffer = (char**)malloc(linebuff->LB_Size * sizeof(char*));
		empty(linebuff->LineBuffer, linebuff->LB_Size);
	} else {
		linebuff->LB_Size = 0;
		linebuff->LineBuffer = NULL;

		return linebuff;
	}

	// Next line
	ptr = Buffer;
	
	for (size_t i = 0; i < linebuff->LB_Size; i++) {
		if (strchr(ptr, '\n'))
			Length = (size_t)(strchr(ptr, '\n') - ptr) + 2;
		else
			Length = strlen(ptr) + 1;

		(linebuff->LineBuffer)[i] = (char*)malloc(Length * sizeof(char));
		empty((linebuff->LineBuffer)[i], Length);

		strncpy((linebuff->LineBuffer)[i], ptr, --Length);
		ptr += Length;
	}

	return linebuff;
}

char* getLine(LineBuffer_s* linebuff, size_t Line)
{
	if(!linebuff)
		return "\n";
	if(!linebuff->LineBuffer)
		return "\n";
	return linebuff->LineBuffer[Line - 1];
}

char** getLinePtr(LineBuffer_s* linebuff, size_t Line)
{
	if(!linebuff)
		return NULL;
	else if(!linebuff->LineBuffer)
		return NULL;
	return &(linebuff->LineBuffer[Line - 1]);
}

int freeLineBuffer(LineBuffer_s* linebuff)
{
	if(!linebuff)
		return ED_NULL_PTR;
	
	if(linebuff->LineBuffer) {
		for (size_t i = 0; i < linebuff->LB_Size; i++)
			free(linebuff->LineBuffer[i]);
	}
	
	free(linebuff);
	return ED_SUCCESS;
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
	if(ptr[0])
		strcat(edit, ptr);

	// Move to *row and free temporary string
	*row = realloc(*row, size);
	strcpy(*row, edit);
	free(edit);

	return *row;
}

int putstr(char** row, const char* _before, const char* _new)
{
	if(!row || !_new || !_new[0])
		return ED_NULL_PTR;
	
	char* ptr;
	char* edit;
	size_t size = strlen(*row) + 1;
	size_t new_size = strlen(_new) + 1;
	
	// Initial check
	if(_before) {
		ptr = strstr(*row, _before);
		if(!ptr)
			return ED_NULL_PTR;
	}
	
	// Allocation of new space
	*row = realloc(*row, (size + new_size - 1) * sizeof(char));
	empty(*row + size, new_size - 1);
	size += new_size - 1;

	if(!row)
		return ED_NULL_PTR;

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
		strcat(*row, "\n");
	}
	return ED_SUCCESS;
}

size_t getLineBufferSize(LineBuffer_s* linebuff)
{
	size_t counter;

	if(!linebuff)
		return 0;
	else if(!linebuff->LineBuffer)
		return 0;
	
	counter = linebuff->LB_Size; // Null-terminator characters

	for (size_t i = 0; i < linebuff->LB_Size; i++)
		counter += strlen((linebuff->LineBuffer)[i]);

	return counter;
}

char* getBuffer(LineBuffer_s* linebuff)
{
	size_t size;
	char* Buffer;

	if(!linebuff)
		return NULL;

	size = getLineBufferSize(linebuff) + 1;
	if(!size)
		return NULL;
	
	Buffer = (char*)malloc(size * sizeof(char));

	empty(Buffer, strlen(Buffer));
	for (size_t i = 0; i < linebuff->LB_Size; i++)
		strcat(Buffer, linebuff->LineBuffer[i]);

	return Buffer;
}
