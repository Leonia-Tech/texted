#include <stdio.h>
#include <unistd.h>
#include <texted/texted.h>

int createFile(char* Filename, usr_perm_e permissions)
{
	const int ITERATIONS = 5;
	const int MICROSECONDS = 10000;

	for(int i = 0; i < ITERATIONS && permissions == -1; i++) {
		if(!~permissions) {
			FILE* File = fopen(Filename, "w");
			if(!File) {
				perror(RED "Failed to create file\n" RESET);
				return ED_NULL_FILE_PTR;
			}
			fclose(File);
		}
		fputs(ITALIC CYAN "New file created: " RESET, stderr);
		fputs(Filename, stderr);
		fputc('\n', stderr);
		permissions = get_user_permissions(Filename);
		usleep(MICROSECONDS);
	}

    return ED_SUCCESS;
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

int argumentParser(int del_new_line, size_t args_number, char** argument[])
{
	size_t s = 0;
	char* tmp = NULL;
	char* nl;
	
	// Read
	if(!~getline(&tmp, &s, stdin)){
		return ED_ERRNO;
	}

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