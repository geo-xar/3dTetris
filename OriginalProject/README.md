# 3dTetris

## Intro

The game works explicitly with the keyboard.

The size of the grid is fixed.

Volumes are generated randomly.

The colour of the volumes is picked based on their size.
Yellow volumes are the smallest ones, blue ones are of a medium size,
red ones are the largest ones.

Red volumes have a larger weight than blue ones.
Blue volumes have a larger weight than yellow ones.
Volumes with a larger weight destroy the ones with less weight.

The game contains an easter egg...

## Keyboard controls

 - `ESC`     -> exit game
 - `S` , `s` -> start/pause game
 - `Ì` , `m` -> switch camera
 - `J` , `j` -> left camera rotation
 - `L` , `l` -> right camera rotation
 - `I` , `i` -> zoom in
 - `K` , `k` -> zoom out
 - `+`       -> increase game speed
 - `-`       -> decrease game speed
 - Move volumes around the grid using left, right, up and down arrows
 - `x`       -> rotate volume x axis
 - `c`       -> rotate volume y axis
 - `v`       -> rotate volume z axis

## Executable

Pre-built executable is compiled for Windows platform.
It utilises `glut32.dll` shipped together.
Antivirus could detect both executable
and `glut32.dll` as potential threats...

Tested on both Windows 7 & Windows 10.

## Limitations

The user is generally not allowed to move volumes through other ones,
this mechanism might not always work as expected.

The game cannot be restarted since it is over.

Honestly a lot more might exist as this game was only tested manually...

## Play

- Download and extract `3dTetrisBinaries.tar`
- Run `3dTetris.exe`

Please note that `glut32.dll` is expected to be
in the same directory where the executable is.