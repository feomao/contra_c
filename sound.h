#ifndef SOUND_H
#define SOUND_H


#include <stdlib.h>
#include <stdio.h>

#include "sdl/include/SDL_mixer.h"

#define SOUNDFMT AUDIO_U8
#define SOUNDCHANS 1
#define CHUNKSIZ 1024
#define SOUNDRATE 22050
#define SILENT_DATA 0x80
#define MAXBGM 6

struct Musicdata{
	char name[60];
	char* buf;
	int size;
	unsigned int reset;
};

int initSound(int format, int chans, int rate, int chunksiz);
void shutdownSound();

void open_bgm(char*filename, int index);
void play_bgm(int index);
void stop_bgm();
void pause_bgm();
void resume_bgm();

extern int g_bgmplaying;

#endif

