# PERMISSIONS.H DOCUMENTATION 

## INFORMATIONS 

``` C 
typedef struct {
    char* fi_name;
    char* fi_permissions;
    char* fi_user;
    char* fi_group;
    char* fi_extension;
}finfo_s;
``` 
Informations about the file 

## --------------------------------------------------

``` C
typedef struct {
    char* usr_name;
    uid_t usr_uid;
    char* usr_group;
    gid_t usr_gid;
} usr_info_s;
``` 
Informations about the user

## finfio_s STRUCTURE 

``` C 
finfo_s* finfo(const char* Filename);
``` 
Initialize a finfo_s structure

## -----------------------------------------------

``` C
int finfo_free(finfo_s* fi);
```
Free a finfo_s structure

## usr_usr_info STRUCTURE 

``` C 
usr_info_s* usr_info();
```
Initialize a usr_info_s structure

## ----------------------------------------------

```C 
int usr_info_free(usr_info_s* usr);
```

## PERMISSIONS 

```C 
typedef enum {
    EX_PERM = 1 << 0,
    WR_PERM = 1 << 1,
    RD_PERM = 1 << 2
}usr_perm_e;
```
Use bitwise and (&) to check permission availability

## -----------------------------------------------

```C 
mode_t get_caller_permissions_mask(char* Filename);
```
Get the caller mask based on file ownership

## ------------------------------------------------

``` C
usr_perm_e get_user_permissions(char* Filename);
```
Get the user permissions on the file based on its mask

## ------------------------------------------------

``` C
char* get_user_permission_color(char* Filename);
```
Get the color associated with the permissions

## RETRIEVE FUNCTIONS

```C
mode_t get_file_permissions(const char* Filename);
```
Retrieve octal file permissions

## ----------------------------------------------

```C 
char get_file_type(const mode_t mode);
```
Retrieve char file type
 
## ----------------------------------------------

```C
char* get_string_permissions(const mode_t mode);
```
Retrieve string file permissions

## GET FUNCTIONS

```C
char* get_file_user(const char* Filename);
```
 Get the user of the file

 ## ---------------------------------------------

 ```C
 char* get_file_group(const char* Filename);
```
Get the group of the file
