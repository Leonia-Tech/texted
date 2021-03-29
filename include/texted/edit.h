#ifndef TEXTED_EDIT_H
#define TEXTED_EDIT_H

/*
    Here we put all the functions to manipulate buffers, LineBuffers and strings
*/

#ifndef ANDROID
    #include <aio.h>
#else
    #include <stdio.h>
#endif

// Find how many times the character "ch" appears in "str"
int strocc(const char* str, char ch);

// Compare two strings up to "size"
int streq(char* str1, char* str2, size_t size);

/* ------------------------------------------------- */

typedef struct{
    char** LineBuffer;
    size_t LB_Size;
}LineBuffer_s;

// Divide "Buffer" into lines and return the number of lines inside "_Lines"
LineBuffer_s* getLineBuffer(char* Buffer);

// Recompose buffer devided into rows
char* getBuffer(LineBuffer_s* linebuff);

// Return the "Line"-th line address
char* getLine(LineBuffer_s* linebuff, size_t Line);

// Return a "LineBuffer" pointer to the "Line"-th line
char** getLinePtr(LineBuffer_s* linebuff, size_t Line);

// Frees a dynamically allocated line buffer
int freeLineBuffer(LineBuffer_s* linebuff);

// Get "LineBuffer" size in bytes
int getLineBufferSize(LineBuffer_s* linebuff);

/* ------------------------------------------------- */

// Substitute "_old" with "_new"
char* substitute(char** row, const char* _old, const char* _new);

// Add before "_before"
// If "_before" is null, then add at the end of the line
char* putstr(char** row, const char* _before, const char* _new);

#endif // TEXTED_EDIT_H
