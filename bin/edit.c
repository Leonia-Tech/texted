#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <texted/texted.h>

// Trova quante volte il caratter ch compare in str
int strocc(char* str, char ch)
{
	int counter = 0;

	for (int i = 0; str[i] != 0; i++)
		if (str[i] == ch)
			counter++;

	return counter;
}

char** getLineBuffer(char* Buffer, int* _Lines)
{
	int Lines = strocc(Buffer, '\n') + 1;
	char** LineBuffer = (char**)malloc(Lines * sizeof(char*));
	char* ptr = Buffer;
	int Length;

	for (int i = 0; i < Lines; i++)
	{
		if (strchr(ptr, '\n'))
			Length = (int)(strchr(ptr, '\n') - ptr) + 2;
		else
			Length = strlen(ptr) + 1;

		LineBuffer[i] = (char*)malloc(Length * sizeof(char));
		empty(LineBuffer[i], Length);
		strncpy(LineBuffer[i], ptr, --Length);
		ptr = &ptr[Length];
	}
	*_Lines = Lines;
	return LineBuffer;
}

char* getLine(char** LineBuffer, int Line) { return LineBuffer[Line - 1]; }
char** getLinePtr(char** LineBuffer, int Line) { return &LineBuffer[Line - 1]; }

void freeLineBuffer(char** LineBuffer, int Lines)
{
	for (int i = 0; i < Lines; i++)
		free(LineBuffer[i]);
}

char* substitute(char** row, char* _old, char* _new)
{
	char* ptr = strstr(*row, _old);
	int size = strlen(*row) - strlen(_old) + strlen(_new);
	char* edit = (char*)malloc(size * sizeof(char));

	empty(edit, size);
	strncpy(edit, *row, (int)(ptr - *row));
	strcat(edit, _new);
	ptr = &ptr[strlen(_old)];
	strcat(edit, ptr);
	*row = realloc(*row, size);
	strcpy(*row, edit);
	free(edit);
	return *row;
}

char* putstr(char** row, char* _before, char* _new)
{
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

char* editCommandInterpreter(char* arg, char** _str1, char** _str2)
{
	int i;
	char* str1;
	char* str2;

	// arg è del tipo "/.../.../..."
	str1 = (char*)malloc(ED_ARG_SZ);
	str2 = (char*)malloc(ED_ARG_SZ);

	empty(str1, strlen(str1));
	empty(str2, strlen(str2));
	arg = &arg[1];

	for (i = 0; arg[i] != '/'; i++)
		str1[i] = arg[i];
	str1[i] = '\0';

	arg = strchr(arg, '/');
	arg = &arg[1];

	for (i = 0; arg[i] != '/'; i++)
		str2[i] = arg[i];
	str2[i] = '\0';

	arg = strchr(arg, '/');
	*_str1 = str1;
	*_str2 = str2;

	return &arg[1];
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
