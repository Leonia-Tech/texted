#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include <texted/permissions.h>
#include <texted/texted.h>

mode_t get_file_permissions(const char* Filename)
{
	struct stat st;

	if(!~stat(Filename, &st)) {
		perror(RED "Failed to read file infos" RESET);
		return -1;
	}

	return st.st_mode;
}

char* get_file_user(const char* Filename)
{
	struct stat st;
	struct passwd* pw;

	if(!~stat(Filename, &st)) {
		perror(RED "Failed to read file infos" RESET);
		return NULL;
	}

	// Don't free
	pw = getpwuid(st.st_uid);
	if(!pw)
		return NULL;
	
	return pw->pw_name;
}

char* get_file_group(const char* Filename)
{
	struct stat st;
	struct group* gr;

	if(!~stat(Filename, &st)) {
		perror(RED "Failed to read file infos" RESET);
		return NULL;
	}

	// Don't free
	gr = getgrgid(st.st_gid);
	if(!gr)
		return NULL;
	
	return gr->gr_name;
}

char* get_string_permissions(const mode_t mode)
{
	const size_t size = 8;
	char* permissions = strdup("---------");
	char* mask = "rwxrwxrwx";

	for(size_t i = 0; i < 9; i++)
		if(mode & (1 << i))
			permissions[size - i] = mask[size - i];
	
	return permissions;
}

void ed_print_permissions(const char* Filename)
{
	printf(BOLD MAGENTA "-%s\t%s %s\t%s\n" RESET,
		   get_string_permissions(get_file_permissions(Filename)),
		   get_file_user(Filename),
		   get_file_group(Filename),
		   Filename);
}