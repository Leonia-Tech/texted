#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
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

char get_file_type(const mode_t mode)
{
	char* mask = "bcdp-ls";
	char type = '\0';
	uint8_t bit_field = 0;

	bit_field |= S_ISBLK(mode)  << 6
			  |  S_ISCHR(mode)  << 5
			  |  S_ISDIR(mode)  << 4
			  |  S_ISFIFO(mode) << 3
			  |  S_ISREG(mode)  << 2
			  |  S_ISLNK(mode)  << 1
			  |  S_ISSOCK(mode) << 0;
	
	for(uint8_t b = 1, i = 0; i < 7; b <<= 1, i++) {
		if(b & bit_field) {
			type = mask[i];
			break;
		}
	}
	
	return type;
}

char* get_string_permissions(const mode_t mode)
{
	const size_t size = 8;
	char* permissions = strdup("----------");
	char* mask = "rwxrwxrwx";

	permissions[0] = get_file_type(mode);
	for(size_t i = 0; i < 9; i++)
		if(mode & (1 << i))
			permissions[size + 1 - i] = mask[size - i];
	
	return permissions;
}

finfo_s* finfo(const char* Filename)
{
	char* tmp;
	finfo_s* fi = malloc(sizeof(finfo_s));
	char* fname = strdup(Filename);

	do {
		struct stat st;

		if(!~stat(Filename, &st)) {
			return NULL;
		}
	}while(0);

	fi->fi_name = strdup(Filename);
	fi->fi_permissions = get_string_permissions(get_file_permissions(Filename));
	
	if(strchr(Filename, '.')) {
		tmp = strtok(fname, ".");
		tmp = strtok(NULL, ".");
		fi->fi_extension = strdup(tmp);
	} else {
		fi->fi_extension = NULL;
	}

	// Shouldn't be freed (?)
	fi->fi_user = get_file_user(Filename);
	fi->fi_group = get_file_group(Filename);

	return fi;
}

int finfo_free(finfo_s* fi)
{
	if(!fi)
		return ED_NULL_PTR;
	
	free(fi->fi_name);
	free(fi->fi_permissions);

	if(fi->fi_extension)
		free(fi->fi_extension);

	free(fi);

	return ED_SUCCESS;
}

usr_info_s* usr_info()
{
	usr_info_s* usr = malloc(sizeof(usr_info_s));
	struct passwd* pw;
	struct group* gr;

	// Get uid (never fails)
	usr->usr_uid = geteuid();

	// Get username
	pw = getpwuid(usr->usr_uid); // Don't free
	if(!pw) {
		perror(RED "Failed to get user info about the caller" RESET);
		free(usr);
		return NULL;
	}
	usr->usr_name = strdup(pw->pw_name);

	// Get gid
	usr->usr_gid = pw->pw_gid;

	// Get group name
	gr = getgrgid(usr->usr_gid);
	if(!gr) {
		perror(RED "Failed to get group info about the caller" RESET);
		free(usr->usr_name);
		free(usr);
		return NULL;
	}
	usr->usr_group = strdup(gr->gr_name);

	return usr;
}

int usr_info_free(usr_info_s* usr)
{
	if(!usr)
		return ED_NULL_PTR;
	
	free(usr->usr_name);
	free(usr->usr_group);

	free(usr);

	return ED_SUCCESS;
}

// Return caller permission mask
mode_t get_caller_permissions_mask(char* Filename)
{
	mode_t user_mask = 0;
	mode_t file_mask;
	usr_info_s* user;
	finfo_s* file;

	user = usr_info();
	file = finfo(Filename);
	if(!file)
		return -1;

	// Check if user is file user
	if(streq(user->usr_name, file->fi_user, MIN(strlen(user->usr_name), strlen(file->fi_name))))
		user_mask |= 0700;
	else if(streq(user->usr_group, file->fi_group, MIN(strlen(user->usr_group), strlen(file->fi_group))))
		user_mask |= 0070;
	else
		user_mask |= 0007;
	
	file_mask = get_file_permissions(Filename);
	if(!~file_mask)
		return -1;

	return user_mask & file_mask;
}

usr_perm_e get_user_permissions(char* Filename)
{
	const mode_t READ 		= 0444;
	const mode_t WRITE 		= 0222;
	const mode_t EXECUTE 	= 0111;

	usr_perm_e permissions = 0;
	mode_t caller_permission_mask = get_caller_permissions_mask(Filename);
	if(!~caller_permission_mask)
		return -1;

	if(caller_permission_mask & READ)
		permissions |= RD_PERM;
	
	if(caller_permission_mask & WRITE)
		permissions |= WR_PERM;
	
	if(caller_permission_mask & EXECUTE)
		permissions |= EX_PERM;
	
	return permissions;
}

char* get_user_permission_color(char* Filename)
{
	usr_perm_e permissions = get_user_permissions(Filename);

	if(permissions == (RD_PERM | WR_PERM | EX_PERM))
		return RED "(!) " GREEN;
	else if(permissions == (RD_PERM | WR_PERM))
		return GREEN;
	else if(permissions == WR_PERM)
		return RED "(write only) " GREEN;
	else if(permissions == RD_PERM)
		return RED "(read only) " GREEN;
	else
		return RESET;
}