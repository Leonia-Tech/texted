#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>

#include <pthread.h>

#include <credits/unirun.h>
#include <credits/credits.h>

/* 
    Credits for 100th commit 
    Happy birthday texted! do u want a cake?
*/

void* quit_routine()
{
    pid_t pid = 0;

    pid = mplay(AUDIO_PATH);

    for(char c = 0; (c = getch()) && c != 'q';);
    endwin();
    kill(pid, SIGKILL);
    exit(0);
}

int credits(){
    int nullfd;
    pthread_t quit;

    char* creds[10] = {"**************************************************************************\n",
                       "*\t\t\t\tTexted 1.5\t\t\t\t *\n",
                       "* Developed by:\t\t\t\t\t\t\t\t *\n",
                       "* Giovanni Zaccaria\tC code and installer\t\t\t\t *\n",
                       "* Sebastiano Barezzi    C code and installer\t\t\t\t *\n",
                       "* Francesco Pallara\tPython and C code\t\t\t\t *\n",
                       "* Alberto Bella\t\tDocumentation and C code\t\t\t *\n",
                       "* Andrea Minasi\t\tSegmentation Error: (core dump)\t\t\t *\n",
                       "*\t\t\t\t\t\t\t\t\t *\n",
                       "**************************************************************************"};

    if(!~(nullfd = open("/dev/null", O_WRONLY))) {
        perror("Couldn't redirect stderror!");
        return -1;
    } else {
        if(!~dup2(nullfd, STDERR_FILENO)) {
            perror("Couldn't redirect stderror!");
            return -1;
        }
    }

    initscr();
    refresh();
    pthread_create(&quit, NULL, &quit_routine, NULL);

    for(int i = 0; i < 10; i++) {
        usleep(1000000);
        addstr(creds[i]);
        refresh();
    }

    addstr("\n\nPress q to exit...");
    refresh();
    
    pthread_join(quit, NULL);

    return 0;
}

pid_t mplay(char* Filepath)
{
	pid_t status = 0;
	int nullcf = open("/dev/null", O_WRONLY);
	if(!~nullcf){
		return -1;
	}
	status = launch("mpv", genargs(4, "/usr/bin/mpv", "--no-video", Filepath, NULL), nullcf);
	close(nullcf);
	return status;
}