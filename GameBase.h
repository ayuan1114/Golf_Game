#include <SDL.h>

#ifndef GOLF_GAME_GAMEBASE_H
#define GOLF_GAME_GAMEBASE_H

// global variables needed across all files
const int screenH = 1000, screenW = 1000;
int frameCount = 0;
const double fps = 120;
const double velCap = 100;
const double timeInt = 30 / fps;

#endif //GOLF_GAME_GAMEBASE_H
