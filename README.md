# ff2cue

FFMetadata to cuesheet converter, 0BSD

Usage:
> ff2cue [-b] [-y] INPUT OUTPUT [cue_audio_file]
>  -b: Write UTF-8 BOM
>  -y: Overwrite output
>  INPUT: Use - for stdin
>  OUTPUT: Use - for stdout
> Example: ffmpeg -i INPUT -f ffmetadata - | ff2cue - output.cue -b -y "live show.flac"
