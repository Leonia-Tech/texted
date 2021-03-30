#ifndef UNIRUN_H
#define UNIRUN_H

#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>

#define BUFFER_SIZE     0x4000
#define PATH_SIZE       0x400

char* run(char* program, char* args[], int* status);    // returns the output in a buffer
pid_t launch(char* program, char* args[], int fd);        // prints the output to fd (if fd is 0 doesn't redirect)
char** genargs(size_t size, ...);                       // generates an array of arguments from a list

#define WHICH(_program)             run("/usr/bin/which", genargs(3, "/usr/bin/which", (_program), NULL), NULL)
#define TAR_EXTRACT(_tar)           launch("/usr/bin/tar", genargs(4, "/usr/bin/tar", "-xf", (_tar), NULL), open("/dev/null", O_WRONLY))

#endif
