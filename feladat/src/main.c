#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>


SDL_Surface *screen = NULL;

/* makefile
gcc main.c -o program.exe -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32
*/
int main (int argc, char *argv[]){ // main 

    (void)argc;
    (void)argv;

    // sdl inicializálás
    SDL_Init(SDL_INIT_VIDEO);

    //opengl attributumok
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                    SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *Window = NULL;


    //sdl ablak létrehozás attribúrumokkal
    Window = SDL_CreateWindow("", 0, 0, 0, 0,  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowSize(Window, 800, 600);
    SDL_SetWindowTitle(Window, "Fazekas Roland István - Számítógépi grafika");
    
    SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    screen=SDL_GetWindowSurface(Window);
    Uint32 color = SDL_MapRGB(screen->format, 113, 113, 113);
    SDL_FillRect(screen, NULL, color);
    SDL_UpdateWindowSurface(Window);

    //sdl ablak ikon létrehozás
    SDL_Surface *icon;
    icon = SDL_LoadBMP("../assets/icon.bmp");
   if (icon == NULL)
   {
    printf("%s", SDL_GetError());
   }else{
     SDL_SetWindowIcon(Window, icon);
    SDL_FreeSurface(icon);
   }

   SDL_ShowWindow(Window);
   


    //opengl context létrehozás

    SDL_GLContext context = SDL_GL_CreateContext(Window);

    // opengl inicializálás
    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 800.0/600.0, 0.1, 100.0);

    SDL_Event event;

    int running = 1;

    float angle = 0.0f;

    //game loop
    while (running)
    {
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            
            if (event.type == SDL_WINDOWEVENT)
            {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                int w = event.window.data1;
                int h = event.window.data2;

                glViewport(0, 0, w, h);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(70.0, (double)w/h, 0.1, 100.0);
                }
            }  
        }       
        
            // --- RAJZOLÁS ---
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // kamera hátra
        glTranslatef(0.0f, 0.0f, -5.0f);

        // forgatás
        angle += 0.5f;
        glRotatef(angle, 1.0f, 1.0f, 0.0f);

        // gömb
        GLUquadric *quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluSphere(quad, 1.0, 32, 32);
        gluDeleteQuadric(quad);

        // kirajzolás
        SDL_GL_SwapWindow(Window);
    }

    //ablak törlése
    SDL_DestroyWindow(Window);
    SDL_Quit();

    return EXIT_SUCCESS;
    
}