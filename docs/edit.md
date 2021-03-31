# EDIT.H DOCUMENTATION 
This library is ment for manipulate buffers, lineBuffers and strings 

## STRINGS AND CHARACTERS FUNCTIONS 
``` C 
int strocc(const char* str, char ch);
``` 
 This function finds how many times the character "ch" appears in the "str" 

##  ---------------------------------------------------------

``` C 
int streq(char* str1, char* str2, size_t size);
``` 
 Function to compare strings up to "size"  

## BUFFER FUNCTIONS
``` C 
char** getLineBuffer(char* Buffer, size_t* _Lines);
``` 
Function that divides "Buffer" into lines and return the number of lines inside "lines_" 

## --------------------------------------------------------

``` C 
char* getBuffer(char** LineBuffer, int Lines);
``` 
Funtion that recompose buffer into rows format  
## -------------------------------------------------------

```C 
char* getLine(char** LineBuffer, size_t Line);
``` 
Function that return "line" -th line address  

## ------------------------------------------------------

```C 
char** getLinePtr(char** LineBuffer, size_t Line);
``` 
 Function that returns a "limeBuffer" pointer into the "line" -th line  

## -----------------------------------------------------

``` C 
void freeLineBuffer(char** LineBuffer, size_t Lines);
``` 

Void function that frees a dynamically allocated line buffer 

## -----------------------------------------------------

```C
int getLineBufferSize(char** LineBuffer, int Lines);
``` 

Get "LineBuffer" size in bytes 

## SUBSTITUDE FUNCTIONS

``` C 
int substitute(char** row, const char* _old, const char* _new);
``` 

Subtitute "_old" with "_new" 

## ----------------------------------------------------

``` C 
int putstr(char** row, const char* _before, const char* _new);
``` 

Add before "_before"If "_before" is null, then add at the end of the line