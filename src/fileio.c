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

char* loadFile(char* Filename)
{
	FILE* File;
	struct stat st;
	char Temp[LINE_SIZE] = {0};
	char* Buffer = NULL;

	if(get_temp())
		Filename = TMP_PATH;

	if(!~stat(Filename, &st)) {
		if(errno != ENOENT) {
			perror(RED"Failed to read file infos"RESET);
			return NULL;
		}
	} else {
		// Check file size
		if(st.st_size > 0) {
			Buffer = malloc(st.st_size * sizeof(char));
			empty(Buffer, st.st_size);
		}
	}

	// Open file or create it
	File = fopen(Filename, "r");
	if (!File) {
		File = fopen(TMP_PATH, "w");
		if(!File) // Errno is set
			return NULL;
		fclose(File);
        errno = 0;
		return NULL;
	}

	// If file is empty exit with null
	if(!Buffer) {
		fclose(File);
		return Buffer;
	}

	// Read the whole file
	while (fgets(Temp, LINE_SIZE, File))
		strcat(Buffer, Temp);

	fclose(File);
	return Buffer;
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

	if(!Buffer)
		return ED_NULL_PTR;

	File = fopen(Filename, "w");
	if (!File)
		return ED_NULL_FILE_PTR;

	fprintf(File, "%s", Buffer);
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

LineBuffer_s* LbLoadFile(char* Filename)
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