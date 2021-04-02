#include <stdio.h>
#include <stdlib.h>
#include <texted/texted.h>
#include <texted/insert.h>

// Only write something if Buffer is not empty.
char* insert()
{
	size_t size = 200;
	const size_t INC = 100; // Increment
	char* Buffer = malloc(size * sizeof(char));
	size_t counter = 0;

	empty(Buffer, size);

	// Ask for characters until ESC is pressed
	char c;
	while((c = getchar()) != ESC && c != EOF) {
		Buffer[counter++] = c;

		// If the buffer exceeds the size, expand the buffer by INC.
		if (counter >= size - 1){
			char* temp = realloc(Buffer, (size + INC) * sizeof(char));
			if(!temp) {
				// It's better to always return a valid pointer
				// In this case there is never an error and the handling on the caller's side is easier.
				*Buffer = 0;
				return Buffer;
			}
			Buffer = temp;
			size += INC;
		}
	}

	Buffer[counter] = 0;
	return Buffer;
}

LineBuffer_s* concatenateLineBuffer(LineBuffer_s* lb1, LineBuffer_s* lb2)
{
	// Set up the temporary LineBuffer
	if(!lb1->LineBuffer && !lb1->LB_Size) {
		return NULL;
	}

	--(lb1->LB_Size);

	// Concatenation of the first string of the temporary buffer with the last string of the LineBuffer
	do {
		char** temp = realloc(lb1->LineBuffer, (lb1->LB_Size + lb2->LB_Size) * sizeof(char*));
		if(!temp)
			return NULL;
		lb1->LineBuffer = temp;
	}while(0);

	do {
		char* temp = realloc((lb1->LineBuffer)[lb1->LB_Size],
													strlen((lb1->LineBuffer)[lb1->LB_Size])  + 
													strlen((lb2->LineBuffer)[0]) + 1);
		if(!temp) {
			free(lb1->LineBuffer);
			return NULL;
		}
		(lb1->LineBuffer)[lb1->LB_Size] = temp;
	}while(0);
	
	strcat((lb1->LineBuffer)[lb1->LB_Size], (lb2->LineBuffer)[0]);

	// Other strings
	for (size_t i = 1; i < lb2->LB_Size; i++) {
		(lb1->LineBuffer)[lb1->LB_Size + i] = strdup((lb2->LineBuffer)[i]);
		strcpy((lb1->LineBuffer)[lb1->LB_Size + i], (lb2->LineBuffer)[i]);
	}

	lb1->LB_Size += lb2->LB_Size;

	return lb1;
}

LineBuffer_s* concatenateBuffer(LineBuffer_s* LineBuffer, char* Buffer)
{
	LineBuffer_s* ExtraLineBuffer;

	ExtraLineBuffer = getLineBuffer(Buffer);
	if(!ExtraLineBuffer)
		return NULL;

	// If LineBuffer isn't void, concatenate the new text
	LineBuffer = concatenateLineBuffer(LineBuffer, ExtraLineBuffer);
	
	freeLineBuffer(ExtraLineBuffer);
	free(ExtraLineBuffer);
	return LineBuffer;
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
		free(newStr);
		return NULL;
	}

	return newStr;
}

// Adds Newline in before Position (Line count starts from 1)
int addLine(LineBuffer_s** LineBuffer, char* NewLine, size_t Position)
{
    char** NewLineBuffer;
    size_t counter;
	int NewLen;

	if(!NewLine)
		return ED_NULL_PTR;
	
	if(!*LineBuffer) {
		if(NewLine[0]) {
			*LineBuffer = getLineBuffer(NewLine);
			return ED_SUCCESS;
		}
		else
			return ED_NULL_PTR;
	}

	// Heap Allocation 
	NewLine = strdup(NewLine);
	NewLen = strlen(NewLine);

	// Add newline
	char* temp = realloc(NewLine, NewLen + 2);
	if(!temp) {
		free(NewLine);
		return ED_NULL_PTR;
	}
	NewLine = temp;

	NewLine[NewLen] = '\n';
	NewLine[NewLen+1] = '\0';

	// Error handling
    if(!NewLine)
        return ED_BAD_LINE_FORMAT;
    
    if(Position < 1 || Position > (*LineBuffer)->LB_Size) {
		free(NewLine);
        return ED_BUFFER_OVERFLOW;
	}
    
	// Initializing new LineBuffer
	NewLineBuffer = (char**)malloc(++((*LineBuffer)->LB_Size) * sizeof(char*));

	// Copy lines before Position
    for(counter = 0; counter < Position - 1; counter++)
        NewLineBuffer[counter] = ((*LineBuffer)->LineBuffer)[counter];
	
	// Add new line
    NewLineBuffer[counter] = NewLine;

	// Copy lines after Position
    for(; counter < ((*LineBuffer)->LB_Size) - 1; counter++)
        NewLineBuffer[counter+1] = ((*LineBuffer)->LineBuffer)[counter];
    
    (*LineBuffer)->LineBuffer = NewLineBuffer;
    return ED_SUCCESS;
}

// Delete Line number Del (Line count starts from 1)
//! Bloated
int delLine(LineBuffer_s** LineBuffer, size_t Del)
{
    int Last = 0;
	int Len = 0;
	const size_t UNDERFLOW = (size_t)(-1);
	char** NewLineBuffer;

	if(!LineBuffer)
		return ED_NULL_PTR;

	// Wrong line number
    if(Del < 1 || Del > (*LineBuffer)->LB_Size)
        return ED_BUFFER_OVERFLOW;

	// Handling last line
    if(Del == (*LineBuffer)->LB_Size)
	{
		// If only one line, deallocate the LineBuffer
		if((*LineBuffer)->LB_Size == 1) {
			free(*LineBuffer);
			*LineBuffer = NULL;
			return ED_SUCCESS;
		} else {
			++Last;
		}
	}

	// Delete Line number Del
    free((*LineBuffer)->LineBuffer[--Del]);
    (*LineBuffer)->LineBuffer[Del] = NULL;

	// Check if there is a line before
	if(--Del == UNDERFLOW)
		Last = 0;
	
	// If we're in the last line and there is a line before
	// Delete the newline in the line before
	if(Last && (Len = strlen((*LineBuffer)->LineBuffer[Del]))) {
		(*LineBuffer)->LineBuffer[Del][Len - 1] = '\0';
	}

	NewLineBuffer = malloc((*LineBuffer)->LB_Size - 1 * sizeof(char*));

	// Copy LineBuffer to NewLineBuffer
	for(size_t i = 0, n = 0; i < (*LineBuffer)->LB_Size; i++)
		if((*LineBuffer)->LineBuffer[i])
			NewLineBuffer[n++] = (*LineBuffer)->LineBuffer[i];
	
	(*LineBuffer)->LineBuffer = NewLineBuffer;
	(*LineBuffer)->LB_Size--;
    
    return ED_SUCCESS;
}