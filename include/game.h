#ifndef GAME_H
#define GAME_H

#include "model.h"

#define WIDTH 800
#define HEIGHT 600
#define SIZE 5
#define N_NUM 10

extern int map[N_NUM][N_NUM];
extern SDL_Window *window;
extern SDL_GLContext context;
extern GLuint texture;
extern Model* woodBox;

#endif