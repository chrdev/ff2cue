/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
*/
#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mos.h"
#include "cueconv.h"


static void
help(void) {
	mos_printf(
		mos_t("FFMetadata to cuesheet converter V0.90 0BSD\n")
		mos_t("ff2cue [-b] [-y] INPUT OUTPUT [cue_audio_file]\n")
		mos_t("  -b: Write UTF-8 BOM\n")
		mos_t("  -y: Overwrite output\n")
		mos_t("  INPUT: Use - for stdin\n")
		mos_t("  OUTPUT: Use - for stdout\n")
		mos_t("Example: ffmpeg -i INPUT -f ffmetadata - | ff2cue - output.cue -b -y \"live show.flac\"\n")
	);
}

typedef struct Cmd {
	const mos_char* src;
	const mos_char* dst;
	const mos_char* wav;
	bool bom;
	bool overwrite;
	bool ok;
}Cmd;

static void
parseCmd(Cmd* cmd, int c, mos_char** v) {
	memset(cmd, 0, sizeof(*cmd));
	for (int i = 1; i < c; ++i) {
		if (v[i][0] == mos_t('-') || v[i][0] == mos_t('/')) {
			switch (v[i][1]) {
			case mos_t('b'):
			case mos_t('B'):
				cmd->bom = true;
				break;
			case mos_t('y'):
			case mos_t('Y'):
				cmd->overwrite = true;
				break;
			case mos_t('\0'):
				if (!cmd->src) cmd->src = mos_t("");
				else if (!cmd->dst) cmd->dst = mos_t("");
				else return;
				break;
			}
		}
		else {
			if (!cmd->src) {
				cmd->src = v[i];
			}
			else if (!cmd->dst) {
				cmd->dst = v[i];
			}
			else if (!cmd->wav) {
				cmd->wav = v[i];
			}
			else {
				// Too many path-like args, return with (cmd->ok = false)
				return;
			}
		}
	}
	cmd->ok = cmd->src && cmd->dst;
}

static inline void
writeBom(FILE* f) {
	mos_fprintf(f, "\xEF\xBB\xBF");
}

static bool
save(const mos_char* fn, const Cue* cue, bool bom, bool overwrite) {
	FILE* f = stdout;
	if (fn[0]) {
		mos_fopen(f, fn, overwrite ? mos_t("w") : mos_t("wx"));
		if (!f) return false;
	}
	if (bom) writeBom(f);
	cueSave(f, cue);
	fclose(f);
	return true;
}

enum {
	kErrOk,
	kErrArg,
	kErrMem,
	kErrRead,
	kErrConv,
	kErrSave,
};

int
mos_main{
	Cmd cmd;
	parseCmd(&cmd, argc, argv);
	if (!cmd.ok) {
		help();
		return kErrArg;
	}

	Cue* cue = calloc(1, sizeof(Cue));
	if (!cue) return kErrMem;

	FILE* f = stdin;
	if (cmd.src[0]) {
		mos_fopen(f, cmd.src, mos_t("r"));
		if (!f) return kErrRead;
	}
	if (!cueFromFFMeta(cue, f)) return kErrConv;
	fclose(f);

	cue->wavFn = mos_utf8(cmd.wav);
	if (!save(cmd.dst, cue, cmd.bom, cmd.overwrite)) return kErrSave;

	return kErrOk;
}
