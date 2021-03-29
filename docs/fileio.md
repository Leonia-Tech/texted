# FILEIO.H DOCUMENTATION 

## BUFFER FUNCTIONS 

``` C 
int app_save(char* Filename, char* Buffer);
``` 
Saves the Buffer in the File and frees the Buffer

## --------------------------------------------------

``` C
int save(char* Filename, char* Buffer);
``` 
 Saves the buffer in the file by rewriting it

 ##  BACKUP FUNCTIONS 

 ```C 
char* genBackupName(char* Filename);
``` 
Generates the backup file name

## -------------------------------------------------

``` C 
int backup(char* Filename);
``` 
Creates a backup file of the open file


