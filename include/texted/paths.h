#ifndef TEXTED_PATHS_H
#define TEXTED_PATHS_H

#define TMPDIR              "/tmp"
#define	TMP_PATH			TMPDIR "/xie6Bei3"

#ifdef  DEBUG
    #define HIGHLIGHTER_PATH        "src/highlighter/parser.py"
    #define AUDIO_PATH              "assets/Temple_os.mp3"
#else
    #define HIGHLIGHTER_PATH        "/usr/lib/texted/highlighter/parser.py"
    #define AUDIO_PATH              "/usr/share/texted/assets/Temple_os.mp3"
#endif

#endif
