#ifndef TEXTED_PERMISSIONS_H
#define TEXTED_PERMISSIONS_H

#include <sys/stat.h>

// Retrieve octal file permissions
mode_t get_file_permissions(const char* Filename);

// Retrieve string file permissions
char* get_string_permissions(const mode_t mode);

// Get the user of the file
char* get_file_user(const char* Filename);

// Get the group of the file
char* get_file_group(const char* Filename);

// Print file info
void ed_print_permissions(const char* Filename);

#endif // TEXTED_PERMISSIONS_H