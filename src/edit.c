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
	LineBuffer_s* linebuff;
	char* ptr;
	size_t Length = 0;

	if(!(Buffer && *Buffer))
		return NULL;
	
	linebuff = malloc(sizeof(LineBuffer_s));
	if(!linebuff)
		return NULL;
	
	linebuff->LB_Size = strocc(Buffer, '\n') + 1;

	// Handle NULL buffer
	if(Buffer && Buffer[0]) {
		linebuff->LineBuffer = (char**)malloc(linebuff->LB_Size * sizeof(char*));
		if(!linebuff->LineBuffer) {
			free(linebuff);
			return NULL;
		}
		empty(linebuff->LineBuffer, linebuff->LB_Size);
	} else {
		free(linebuff);
		return NULL;
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
		return "";
	if(!linebuff->LineBuffer)
		return "";
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

	if(linebuff->LineBuffer)
		free(linebuff->LineBuffer);
	return ED_SUCCESS;
}


int substitute(char** str, char* orig, char* rep)
{
	char* buffer = NULL;
	char *p;

	// Is 'orig' even in 'str'?
	if(!(p = strstr(*str, orig)))
		return ED_WRONG_SYNTAX;
	
	char* temp = malloc(strlen(*str) + strlen(rep) - strlen(orig) + 1);
	if(!temp)
		return ED_MEMORY_ERROR;
	buffer = temp;

	// Copy characters from 'str' start to 'orig' st$
	strncpy(buffer, *str, p-(*str));
	buffer[p-(*str)] = '\0';

	sprintf(buffer+(p-(*str)), "%s%s", rep, p+strlen(orig));

	*str = buffer;
	return ED_SUCCESS;
}

int putstr(char** row, const char* _before, const char* _new)
{
	if(!row || !_new || !_new[0])
		return ED_NULL_PTR;
	
	char* ptr;
	char* edit;
	char last;
	size_t size;
	size_t new_size;

	size = strlen(*row) + 1;
	new_size = strlen(_new) + 1;
	
	// Initial check
	if(_before) {
		ptr = strstr(*row, _before);
		if(!ptr)
			return ED_NULL_PTR;
	}
	
	// Allocation of new space
	char* temp = realloc(*row, (size + new_size - 1) * sizeof(char));
	if(!temp)
		return ED_NULL_PTR;
	
	*row = temp;
	
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
		
		// Remove newline if any
		size -= new_size;
		last = (*row)[size - 1];
		(*row)[size - 1] *= (('\n' - last) != 0);

		// If before is NULL we use this function to concatenate
		strcat(*row, _new);

		if(last == '\n')
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
	char* Buffer = NULL;

	if(!linebuff)
		return NULL;

	size = getLineBufferSize(linebuff) + 1;
	if(!size)
		return NULL;
	
	char* temp = (char*)malloc(size * sizeof(char));
	if(!temp)
		return NULL;
	
	empty(temp, size * sizeof(char));
	Buffer = temp;

	for (size_t i = 0; i < linebuff->LB_Size; i++)
		strcat(Buffer, linebuff->LineBuffer[i]);

	return Buffer;
}
