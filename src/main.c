#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "../include/vec3.h"
#include "../include/model.h"
#include "../include/texture.h"
#include "../include/model_loader.h"
#include "../include/collision.h"
#include "../include/game.h"
#include "../include/math3d.h"
#include "../include/rain.h"
#include "../include/shadow_simple.h"
#include "../include/app.h"

int map[N_NUM][N_NUM] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 4, 4, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

SDL_Window *window = NULL;
SDL_GLContext context;
GLuint texture;
Model *woodBox = NULL;
GLuint help_texture;
GLuint bg_texture;

void initOpenGL(void)
{
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    GLfloat global_ambient[] = {2.0f, 2.0f, 2.0f, 3.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    GLfloat light0_position[] = {10.0f, 20.0f, 10.0f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_ambient[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat light0_specular[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    GLfloat light1_position[] = {0.0f, -5.0f, 0.0f, 1.0f};
    GLfloat light1_diffuse[] = {0.6f, 0.6f, 0.7f, 1.0f};
    GLfloat light1_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    GLfloat light2_position[] = {-5.0f, 15.0f, -8.0f, 1.0f};
    GLfloat light2_diffuse[] = {0.5f, 0.5f, 0.6f, 1.0f};
    GLfloat light2_ambient[] = {0.3f, 0.3f, 0.4f, 1.0f};

    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat material_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat material_shininess[] = {32.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);

    GLfloat material_emission[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void loadResources(void)
{
    texture = loadTexture("./assets/texture.jpg");
    help_texture = loadTexture("./assets/help.png");
    bg_texture = loadTexture("./assets/bg.png");

    woodBox = loadOBJ("./assets/wood_box.obj");
    if (woodBox)
    {
        woodBox->textureID = loadTexture("./assets/wood_box_texture.png");
        if (woodBox->textureID == 0)
        {
            printf("Failed to load wood box texture!\n");
        }
    }

    setGlobalResources(texture, help_texture, woodBox, bg_texture);
}

void cleanupResources(void)
{
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &help_texture);

    if (woodBox)
    {
        glDeleteTextures(1, &(woodBox->textureID));
        freeModel(woodBox);
    }
}

void renderHelpScreen(GameState *state)
{
    if (!state->show_help)
        return;

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, help_texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(WIDTH, 0);
    glTexCoord2f(1, 1);
    glVertex2f(WIDTH, HEIGHT);
    glTexCoord2f(0, 1);
    glVertex2f(0, HEIGHT);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    window = SDL_CreateWindow(
        "Fazekas Roland Istvan - Impossible Labyrinth",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        printf("Window creation failed!\n");
        return 1;
    }

    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        printf("OpenGL context creation failed!\n");
        return 1;
    }

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    initOpenGL();
    loadResources();

    GameState gameState;
    initGame(&gameState);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(70.0, (double)WIDTH / HEIGHT, 0.1, 100.0);

    SDL_Event event;
    int running = 1;
    Uint32 lastTime = SDL_GetTicks();
    int win = 0;

    while (running)
    {
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        if (dt > 0.033f)
            dt = 0.033f;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                int w = event.window.data1;
                int h = event.window.data2;

                glViewport(0, 0, w, h);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                perspective(70.0, (double)w / h, 0.1, 100.0);
            }

            if (event.type == SDL_MOUSEMOTION && !win)
            {
                handleMouseMotion(&gameState, event.motion.xrel, event.motion.yrel);
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (win && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    win = 0;
                    gameState.show_help = 0;
                    initGame(&gameState);
                    GLuint new_help = loadTexture("./assets/help.png");
                    glDeleteTextures(1, &help_texture);
                    help_texture = new_help;
                    setGlobalResources(texture, help_texture, woodBox, bg_texture);
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    gameState.mouse_on_off = 1;
                }
                else if (!win)
                {
                    handleKeyPress(&gameState, event.key.keysym.scancode);
                }
            }
        }

        if (!win)
        {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);

            handleInput(&gameState, keys, dt);
            updateGame(&gameState, dt);

            if (checkWinCondition(&gameState))
            {
                win = 1;
                gameState.show_help = 1;
                GLuint win_texture = loadTexture("./assets/win.png");
                if (win_texture)
                {
                    glDeleteTextures(1, &help_texture);
                    help_texture = win_texture;
                    setGlobalResources(texture, help_texture, woodBox, bg_texture);
                }
                SDL_SetRelativeMouseMode(SDL_FALSE);
                gameState.mouse_on_off = 0;
            }

            float cam_dist = 5.0f;
            float camX, camY, camZ;
            getCameraPosition(&gameState, cam_dist, &camX, &camY, &camZ);
            updateCameraCollision(&gameState, &camX, &camY, &camZ);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderScene(&gameState, camX, camY, camZ);
            renderHelpScreen(&gameState);
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            lookAt(0, 5, 10, 0, 0, 0, 0, 1, 0);

            renderHelpScreen(&gameState);
        }

        SDL_GL_SwapWindow(window);
    }

    cleanupResources();
    cleanupGame();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}