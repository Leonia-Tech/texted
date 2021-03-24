#ifndef TEXTED_TEXTED_H
#define TEXTED_TEXTED_H

#include <aio.h>

#define LINE_SIZE           500
#define ARG_SIZE            4
#define ED_ARG_SZ           50
#define ADD_MODE            "\n"

// Error codes
#define ED_SUCCESS          0x00
#define ED_NULL_FILE_PTR    0x01
#define ED_INVALID_COMMAND  0x02
#define ED_BAD_LINE_FORMAT  0x03
#define ED_BUFFER_OVERFLOW  0x04
#define ED_WRONG_SYNTAX		0x05

// Color definitions
#define RED     "\033[31m"
#define RESET   "\033[0m"

// Macros
#define PAUSE()			    for(char c; (c = getchar()) != '\n';)
#define MIN(_a, _b)		    ((_a) < (_b) ? _a : _b)
#define empty(_buffer, _size)   memset((_buffer), 0, (_size))


// Version control
#ifndef	DEBUG
	#define	RELEASE			"release 1.1"
#else
	#define RELEASE			"debug-version"
#endif

// Confronta due stringhe fino a size
int streq(char* str1, char* str2, size_t size);

// Stampa la lista dei comandi
void display_help();

#endif // TEXTED_TEXTED_H
