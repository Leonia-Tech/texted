#ifndef TEXTED_PERMISSIONS_H
#define TEXTED_PERMISSIONS_H

#include <sys/stat.h>
#include <stdint.h>

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
int finfo_free(finfo_s* fi);

/* -------------------------------- */

// Informations about the user
typedef struct {
    char* usr_name;
    uid_t usr_uid;
    char* usr_group;
    gid_t usr_gid;
} usr_info_s;

// Initialize a usr_info_s structure
// usr_info_s* usr_info();

char* get_extension(const char* Filename);

// Free a usr_info_s structure
// int usr_info_free(usr_info_s* usr);

// Free user info about the caller (always run if you run a function which uses usr_info())
int caller_user_info_free();

// Notice that the usr_info are taken once the program is run for the first time

/* -------------------------------- */

// Use bitwise and (&) to check permission availability
typedef enum {
    EX_PERM = 1 << 0,
    WR_PERM = 1 << 1,
    RD_PERM = 1 << 2,

    ERR_PERM = -1
}usr_perm_e;

// Get the caller mask based on file ownership
mode_t get_caller_permissions_mask(char* Filename);

// Get the user permissions on the file based on its mask
usr_perm_e get_user_permissions(char* Filename);

// Get the color associated with the permissions
char* get_user_permission_color(char* Filename);

// max size of permission_color
#define USER_PERMISSION_COLOR_SIZE 30

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

#endif // TEXTED_PERMISSIONS_H
