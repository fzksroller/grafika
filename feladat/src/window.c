#include "window.h"

int sdl_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        printf("Error");
        return 1;
    }
    return 0;
}

int window_init() {
    if (sdl_init())
    {
        return 1;
    }
    
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);

    isRunning = true;
    return 0;
}

void window_destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    isRunning = false;

    return;
}

void window_loop() {

    SDL_Event Event;
    while (SDL_PollEvent(&Event)>0)
    {
        switch (Event.type)
        {
        case SDL_QUIT:
            window_destroy();
            break;
        
        default:
            break;
        }
    }

    return;
    
}

void window_startloop(){
    while (isRunning)
    {
        window_loop();
    }
    return;
}