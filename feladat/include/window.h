#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_TITLE "xd.."
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;

int isRunning;

int window_init();

void window_startloop();

void window_destroy();