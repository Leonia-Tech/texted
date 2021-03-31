#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

		   "p:\t\tprint\n"
		   "+l\t\tcurrent line\n"
		   "+n\t\twith line numbers\n"
		   "+ln\t\t-l + -n\n"
		   "+ -p\t\tprint permissions\n\n"

		   "i:\t\tinsert mode\n"
		   "+w\t\tsave after quitting insert mode\n"
		   "esc:\t\texit insert mode\n\n"

		   "w:\t\tsave\n"
		   "x:\t\tsave and exit\n"
		   "b:\t\tgenerate backup file\n"
		   "q:\t\tquit\n\n"

		   "s:\t\tsubstitute word (ed syntax)\n"
		   "m:\t\tadd word before ... (ed syntax)\n"
		   "a:\t\tappend at the end of the edit line\n"
		   "l:\t\tset edit line\n\n"

		   "n:\t\tadd a new line before the edit line\n"
		   "d:\t\tremove the edit line\n"RESET, stdout);
}

int argumentParser(char* raw_arg, int del_new_line, size_t args_number, char** argument[])
{
	char* tmp;
	char* nl;
	
	tmp = strdup(raw_arg);

	// Delete newline if any (needed)
	if(del_new_line) {
		nl = strchr(tmp, '\n');
		if(nl)
			nl[0] = '\0';
	}
	
	// Check syntax
	if(tmp[0] != '/' || (args_number == 1 ? (strocc(tmp+1, '/') > args_number) : (strocc(tmp+1, '/') != args_number)))
		return ED_WRONG_SYNTAX;
	
	// Extract token
	(*argument)[0] = strtok(tmp+1, "/");
	for(size_t i = 1; i < args_number; ++i) {
		(*argument)[i] = strtok(NULL, "/");
		if((*argument)[i][0] == '\n')
			(*argument)[i] = "";
	}
	
	return ED_SUCCESS;
}

int lineBufferIntegrity(char** LineBuffer, size_t LB_Size)
{
	for(size_t i = 0; i < LB_Size; i++)
		if(LineBuffer[i] == NULL)
			return 0;
	
	return 1;
}