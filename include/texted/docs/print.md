# PINRT.H DOCUMENTATION 

## LOAD FUNCTION

```C
char* load(char* Filename);
```
Load a file into RAM

## PRINT FUNCTIONS

```C
void ed_print(char** LineBuffer, int Lines, int LineNum);
```
Print the whole file

## -------------------------------------------

```C
void ed_print_permissions(const char* Filename);
```
rint file permissions