#!/bin/bash
windres -O coff -i nakedheretic.rc -o nakedheretic.res
gcc -O2 -DWIN32 -D_WIN32 -DWINDOWS -DWIN32_LEAN_AND_MEAN *.c -o NakedHeretic.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lm -mwindows nakedheretic.res


