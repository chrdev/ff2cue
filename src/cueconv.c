/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
*/
#include "cueconv.h"

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif
#include <string.h>
#include <stdlib.h>

#include "mos.h"


typedef struct FFChapter {
	char title[240];
	char artist[240];
	uint64_t num;
	uint64_t den;
	uint64_t start;
}FFChapter;

static inline uint64_t
frameCountFromFFChapter(const FFChapter* f) {
	return f->start * f->num * 75 / f->den;
}

static inline void
timeFromFrameCount(CueTrack* t, uint64_t n) {
	t->ff = (uint8_t)(n % 75);
	n /= 75;
	t->ss = (uint8_t)(n % 60);
	t->mm = (uint8_t)(n / 60);
}

static inline void
timeFromFFChapter(CueTrack* t, const FFChapter* f) {
	if (!f->den) {
		t->mm = 0;
		t->ss = 0;
		t->ff = 0;
		return;
	}
	timeFromFrameCount(t, frameCountFromFFChapter(f));
}

static void
trackFromFFChapter(CueTrack* t, const FFChapter* f) {
	timeFromFFChapter(t, f);
	mos_strcpy(t->title, sizeof(((CueTrack*)0)->title), f->title);
	mos_strcpy(t->performer, sizeof(((CueTrack*)0)->performer), f->artist);
}

static const char kTitle[] = "title=";
static const char kArtist[] = "ARTIST=";
static const char kStart[] = "START=";
static const char kTimebase[] = "TIMEBASE=";
static const char kChapter[] = "[CHAPTER]";

static inline void
removeTrailingNewline(char* text) {
	const size_t last = strlen(text) - 1;
	if (text[last] == '\n') {
		text[last] = '\0';
	}
}

static void
skipNonChapter(char* buf, int bufSz, FILE* f) {
	for (;;) {
		if (!mos_strnicmp(buf, kChapter, sizeof(kChapter) - 1)) return;
		for (char* r = fgets(buf, bufSz, f); true; r = fgets(buf, bufSz, f)) {
			if (!r) return;
			if (*buf == '[') break;
		}
	};
}

bool
cueFromFFMeta(Cue* cue, FILE* f) {
	FFChapter ff = { 0 };
	char ln[256];
	CueTrack* track = cue->t;
	for (char* r = fgets(ln, (int)sizeof(ln), f); r; r = fgets(ln, (int)sizeof(ln), f)) {
		switch (*ln) {
		case ';':
		case '#':
		case '\n':
			continue;
		case '[':
			trackFromFFChapter(track++, &ff);
			ff.title[0] = '\0';
			ff.artist[0] = '\0';
			ff.num = 1;
			ff.den = 1000000000;
			skipNonChapter(ln, (int)sizeof(ln), f);
			continue;
		}

		if (!mos_strnicmp(ln, kTitle, sizeof(kTitle) - 1)) {
			removeTrailingNewline(ln);
			mos_strcpy(ff.title, sizeof(((FFChapter*)0)->title), ln + sizeof(kTitle) - 1);
		}
		else if (!mos_strnicmp(ln, kArtist, sizeof(kArtist) - 1)) {
			removeTrailingNewline(ln);
			mos_strcpy(ff.artist, sizeof(((FFChapter*)0)->artist), ln + sizeof(kArtist) - 1);
		}
		else if (!mos_strnicmp(ln, kStart, sizeof(kStart) - 1)) {
			const char* t = ln + sizeof(kStart) - 1;
			ff.start = strtoull(t, NULL, 10);
		}
		else if (!mos_strnicmp(ln, kTimebase, sizeof(kTimebase) - 1)) {
			char* t = ln + sizeof(kTimebase) - 1;
			ff.num = strtoull(t, &t, 10);
			ff.den = strtoull(t + 1, NULL, 10);
		}
	}
	return true;
}
