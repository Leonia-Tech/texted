#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <texted/fileio.h>
#include <texted/insert.h>
#include <texted/texted.h>

volatile int temp = 0;

void set_temp() { temp = 1; }
void clr_temp() { temp = 0; }
int get_temp()  { return temp; }

// If temp is set read from /tmp
// Else read from filename
// On error errno is set

char* loadFile(char* Filename)
{
	FILE* File;
	size_t FileSize = 0;
	char* Buffer = NULL;

	if(get_temp())
		Filename = TMP_PATH;

	// Check file size
	FileSize = getFileSize(Filename);
	if(FileSize > 0) {
		// Allocate FileSize space for the Buffer
		Buffer = malloc(FileSize * sizeof(char));
		empty(Buffer, FileSize);
	} else {
		if(errno != ENOENT)
			return NULL;
	}

	// Open file
	File = fopen(Filename, "r");
	if (!File) {
		// If there is no file create it
		if(createFile() == ED_NULL_FILE_PTR) {
			return NULL;
		}
	}

	// If file is empty exit with null
	if(!Buffer) {
		fclose(File);
		return Buffer;
	}

	// Read the whole file
	if(!fread(Buffer, FileSize, sizeof(char), File))
		fputs(RED "Failed to read the File\n" RESET, stderr);

	fclose(File);
	return Buffer;
}

size_t getFileSize(char* Filename)
{
	struct stat st;
	return st.st_size *= ((stat(Filename, &st) + 1) != 0);
}

// Saves the Buffer in the File and frees the Buffer
int app_save(char* Filename, char* Buffer)
{
	FILE* File;

	if(!Buffer)
		return ED_NULL_PTR;

	File = fopen(Filename, "a");
	if (!File)
		return ED_NULL_FILE_PTR;

	fprintf(File, "%s", Buffer);
	fclose(File);

	clr_temp();
	return ED_SUCCESS;
}

int save(char* Filename, char* Buffer)
{
	FILE* File;

	File = fopen(Filename, "w");
	if (!File)
		return ED_NULL_FILE_PTR;

	if(Buffer)
		fputs(Buffer, File);
	fclose(File);

	if(get_temp()) {
		fputs(ITALIC CYAN "New file created: " RESET, stderr);
		fputs(Filename, stderr);
		fputc('\n', stderr);
		clr_temp();
	}

	return ED_SUCCESS;
}

char* genBackupName(char* Filename)
{
	char* BackupName;

	BackupName = strins(Filename, "-bkp", '.');
	if(!BackupName) {
		BackupName = malloc(strlen(Filename) + 5);
		strcpy(BackupName, Filename);
		strcat(BackupName, "-bkp");
	}

	return BackupName;
}

int backup(char* Filename)
{
	FILE* From, *To;
	char Buffer[LINE_SIZE];
	char* BackupName;

	if(get_temp())
		return ED_NULL_FILE_PTR;

	BackupName = genBackupName(Filename);

	From = fopen(Filename, "r");
	if (!From)
		return ED_NULL_FILE_PTR;
	
	To = fopen(BackupName, "w");
	if (!To)
		return ED_NULL_FILE_PTR;

	while (fgets(Buffer, LINE_SIZE, From))
		fprintf(To, "%s", Buffer);

	fclose(From);
	fclose(To);
	free(BackupName);
	return ED_SUCCESS;
}

LineBuffer_s* lbLoadFile(char* Filename)
{
	char* Buffer;
	LineBuffer_s* LineBuffer;

	Buffer = loadFile(Filename);
	if(!Buffer)
		return NULL;
	
	LineBuffer = getLineBuffer(Buffer);

	free(Buffer);
	return LineBuffer;
}