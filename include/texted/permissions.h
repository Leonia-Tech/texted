#ifndef TEXTED_PERMISSIONS_H
#define TEXTED_PERMISSIONS_H

#include <sys/stat.h>

// Informations about the file
typedef struct {
    char* fi_name;
    char* fi_permissions;
    char* fi_user;
    char* fi_group;
    char* fi_extension;
}finfo_s;

// Initialize a finfo_s structure
finfo_s* finfo(const char* Filename);

// Free a finfo_s structure
void finfo_free(finfo_s* fi);

/* -------------------------------- */

// Informations about the user
typedef struct {
    char* usr_name;
    uid_t usr_uid;
    char* usr_group;
    gid_t usr_gid;
} usr_info_s;

// Initialize a usr_info_s structure
usr_info_s* usr_info();

// Free a usr_info_s structure
int usr_info_free(usr_info_s* usr);

/* -------------------------------- */

// Retrieve octal file permissions
mode_t get_file_permissions(const char* Filename);

// Retrieve char file type
char get_file_type(const mode_t mode);

// Retrieve string file permissions
char* get_string_permissions(const mode_t mode);

// Get the user of the file
char* get_file_user(const char* Filename);

// Get the group of the file
char* get_file_group(const char* Filename);

// Print file info
void ed_print_permissions(const char* Filename);

#endif // TEXTED_PERMISSIONS_H