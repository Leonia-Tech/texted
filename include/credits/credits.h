#ifndef CREDITS_H
#define CREDITS_H

#include <sys/types.h>

#ifndef DEBUG
    #define AUDIO_PATH  "/usr/share/texted/assets/Temple_os.mp3"
#else
    #define AUDIO_PATH  "assets/Temple_os.mp3"
#endif

pid_t mplay(char* Filepath); 

int credits();

#endif