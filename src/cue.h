/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
*/
#pragma once
#include <stdint.h>
#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif
#include <stdio.h>


typedef struct CueTrack {
	char title[240];
	char performer[240];
	uint8_t mm;
	uint8_t ss;
	uint8_t ff;
}CueTrack;

typedef struct Cue {
	CueTrack t[100];
	const char* wavFn;
}Cue;


void
cueSave(FILE* f, const Cue* cue);
