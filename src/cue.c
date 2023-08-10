/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
*/
#include "cue.h"
#include "mos.h"


static void
cueSaveTrack0(FILE* f, const CueTrack* t, const char* wavFn) {
	mos_fprintf(f, "FILE \"%s\" WAVE\n", wavFn ? wavFn : "");
	if (*t->title) {
		mos_fprintf(f, "TITLE \"%s\"\n", t->title);
	}
	if (*t->performer) {
		mos_fprintf(f, "PERFORMER \"%s\"\n", t->performer);
	}
}

void
cueSave(FILE* f, const Cue* cue) {
	cueSaveTrack0(f, cue->t, cue->wavFn);
	for (int i = 1; i < 100 && cue->t[i].title[0]; ++i) {
		const CueTrack* t = cue->t + i;
		mos_fprintf(f, "TRACK %d AUDIO\n", i);
		if (*t->title) {
			mos_fprintf(f, "\tTITLE \"%s\"\n", t->title);
		}
		if (*t->performer) {
			mos_fprintf(f, "\tPERFORMER \"%s\"\n",t->performer);
		}
		mos_fprintf(f, "\tINDEX 01 %02hhu:%02hhu:%02hhu\n", t->mm, t->ss, t->ff);
	}
}
