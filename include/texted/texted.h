#ifndef TEXTED_TEXTED_H
#define TEXTED_TEXTED_H

#define LINE_SIZE           500
#define ARG_SIZE            4
#define ED_ARG_SZ           50
#define ADD_MODE            "\n"

// Error codes
#define ED_SUCCESS          0x00
#define ED_NULL_FILE_PTR    0x01
#define ED_INVALID_COMMAND  0x02

// Macros
#define PAUSE()			    for(char c; (c = getchar()) != '\n';)
#define MIN(_a, _b)		    ((_a) < (_b) ? _a : _b)

// Version control
#ifndef	DEBUG
	#define	RELEASE			"release 1.1"
#else
	#define RELEASE			"debug-version"
#endif


#endif // TEXTED_TEXTED_H
