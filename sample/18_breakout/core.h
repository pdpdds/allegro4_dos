/*
SAMPLE BRICKBREAKER GAME
Developed by Chris DeLeon
August 2009
For part of the Free Newsletter Vol. 5 of
www.GameDevLessons.com

Instructions: Click to reset the ball, move mouse to position paddle

This code is written with clarity and understandability to a beginner in mind.
It is not intended to represent the most optimized or most organized way
that the code could possibly be prepared.

Allegro is required to compile this application. Find instructions here:
http://gamedevlessons.com/lessons/letter5.html
(All software needed to compile this, for Mac/PC/Linux, is free!)

Files that are part of this project should be:
  core.cpp
  core.h
  main.cpp

Feel free to make changes, experiment, and distribute. Please leave this
comment at the top of the file, since it points back to my instructional
resources. My mission is to help people - your help in this is appreciated!
*/

#include <allegro.h>

// the next variable is declared in core.cpp
// by listing it here as "extern" we can reference
// that same variable from main.cpp, which knows about
// it because this core.h is included at the top.
extern BITMAP *screenBuffer;
// see where updateScreen is called in core.cpp for more
// information about the role of the screen buffer

// the actual functions listed below are defined in
// core.cpp - just putting the declaration here with
// a semicolin after it is a "function prototype" and
// lets the compiler know they're legitmate when we
// call them from main.cpp, even though they aren't
// defined in that file.

//sets up ALlegri for graphics, input
void techInit();
//copies screenBuffer to actual screen 
void updateScreen();
