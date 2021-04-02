#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <texted/edit.h>
#include <texted/fileio.h>
#include <texted/texted.h>

int createFile()
{
	const int ITERATIONS = 5;
	const int MICROSECONDS = 10000;
	usr_perm_e permissions = ERR_PERM;
	int status = ED_SUCCESS;

	for(int i = 0; i < ITERATIONS && permissions == ERR_PERM; i++) {

		FILE* File = fopen(TMP_PATH, "w");
		if(!File) {
			perror(RED "Failed to create file\n" RESET);
			clr_temp();
			status = ED_NULL_FILE_PTR;
			usleep(MICROSECONDS);
		} else {
			set_temp();
			fclose(File);
		}
		permissions = get_user_permissions(TMP_PATH);
		if(~permissions)
			status = ED_SUCCESS;
	}

    return status;
}

void display_help()
{
	fputs(BOLD BLUE "--HELP--\n\n"

		   "p\t\tprint\n"
		   "pl\t\tprint current line (see l for changing the current line)\n"
		   "pn\t\tprint with line numbers\n"
		   "pn <num>\t\t print line <num> without changing the current line\n"
		   "pln\t\tpl + pn\n"
		   "p -p\t\tprint permissions\n\n"

		   "i\t\tinsert mode\n"
		   "iw\t\tsave after quitting insert mode\n"
		   "esc\t\texit insert mode\n\n"

		   "w\t\tsave\n"
		   "x\t\tsave and exit\n"
		   "b\t\tgenerate backup file\n"
		   "q\t\tquit\n\n"

		   "s\t\tsubstitute word (ed syntax)\n"
		   "m\t\tadd word before ... (ed syntax)\n"
		   "a\t\tappend at the end of the current line\n"
		   "l <num>\t\tset current line to <num>\n\n"

		   "!\t\trun bash command\n\n"

		   "n\t\tadd a new line before the current line\n"
		   "d\t\tremove the current line\n"RESET, stdout);
}

int argumentParser(char* raw_arg, size_t args_number, char** argument[])
{
	char* tmp;
	
	tmp = strdup(raw_arg);
	if(!tmp) {
		return ED_ERRNO;
	}
	
	// Check syntax
	if(tmp[0] != '/' || (args_number == 1 ? (strocc(tmp+1, '/') > args_number) : (strocc(tmp+1, '/') != args_number))) {
		free(tmp);
		return ED_WRONG_SYNTAX;
	}
	
	// Extract token
	(*argument)[0] = strtok(tmp+1, "/");
	for(size_t i = 1; i < args_number; ++i)
		(*argument)[i] = strtok(NULL, "/");
	
	return ED_SUCCESS;
}

int lineBufferIntegrity(char** LineBuffer, size_t LB_Size)
{
	for(size_t i = 0; i < LB_Size; i++)
		if(LineBuffer[i] == NULL)
			return 0;
	
	return 1;
}