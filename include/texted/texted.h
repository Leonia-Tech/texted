#ifndef TEXTED_TEXTED_H
#define TEXTED_TEXTED_H

#include <aio.h>
#include <stddef.h>

#include <string.h>

#include <texted/permissions.h>

#define LINE_SIZE           500
#define ARG_SIZE            6
#define ARGS_NUM			2
#define ED_ARG_SZ           50
#define ADD_MODE            (NULL)

#include <texted/paths.h>

// Error codes
#define ED_SUCCESS          0x00
#define ED_NULL_FILE_PTR    0x01
#define ED_INVALID_COMMAND  0x02
#define ED_BAD_LINE_FORMAT  0x03
#define ED_BUFFER_OVERFLOW  0x04
#define ED_WRONG_SYNTAX		0x05
#define ED_FUNCTION_ERROR	0x06
#define ED_ERRNO			0x07
#define ED_NULL_PTR			0x08
#define ED_MEMORY_ERROR		0x09

// Color definitions
#define RED     	"\x1b[31m"
#define GREEN   	"\x1b[32m"
#define YELLOW  	"\x1b[33m"
#define BLUE    	"\x1b[34m"
#define MAGENTA 	"\x1b[35m"
#define CYAN    	"\x1b[36m"
#define RESET 	  	"\x1b[0m"

//Other effects
#define BOLD		"\x1b[1m"
#define ITALIC		"\x1b[3m"
#define UNDERLINE	"\x1b[3m"

// Macros
#define PAUSE()			   		for(char c; (c = getchar()) != '\n';)
#define MIN(_a, _b)		    	((_a) < (_b) ? _a : _b)
#define empty(_buffer, _size)   memset((_buffer), 0, (_size))


// Version control
#define VERSION				"1.5.1"
#ifndef	DEBUG
	#define	RELEASE			"release "VERSION
#else
	#define RELEASE			"debug-version "VERSION
#endif

typedef struct{
	char*	raw_command;
	char 	command;
	char**	args;
}commans_s;

// Print the list of commands
void display_help();

// Try to create new file
int createFile();

// Read and interpret single argument command
int argumentParser(char* raw_arg, size_t args_number, char** argument[]);

#ifdef DEBUG
	int lineBufferIntegrity(char** LineBuffer, size_t Size);
#endif

#endif // TEXTED_TEXTED_H
