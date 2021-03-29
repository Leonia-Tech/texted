# TEXTED.H DOCUMENTATION 

```C
void display_help();
```
Print list of commands

## ----------------------------------------------

```C
int createFile(char* Filename, usr_perm_e permissions);
```
Try to create new file if it doesn't exist

## ----------------------------------------------

```C
int argumentParser(int del_new_line, size_t args_number, char** argument[]);
```
Read and interpret single argument command