#ifndef TEXTED_PATHS_H
#define TEXTED_PATHS_H

#define	TMP_PATH			"/tmp/xie6Bei3"

#ifdef  DEBUG
    #define HIGHLIGHTER_PATH        "src/highlighter/parser.py"
    #define AUDIO_PATH              "assets/Temple_os.mp3"
#else
    #define AUDIO_PATH              "/usr/share/texted/assets/Temple_os.mp3"
    #ifdef  ARCH_LINUX
        #define HIGHLIGHTER_PATH    "/usr/lib/texted/highlighter/parser.py"
    #else
        #define HIGHLIGHTER_PATH    "/usr/libexec/texted/highlighter/parser.py"
    #endif
#endif

#endif
