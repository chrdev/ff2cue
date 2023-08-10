/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
*/
#pragma once

#include <stdbool.h>

#include "cue.h"


bool
cueFromFFMeta(Cue* cue, FILE* f);
