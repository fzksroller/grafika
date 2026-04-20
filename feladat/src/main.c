#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "../include/vec3.h"
#include "../include/model.h"
#include "../include/texture.h"
#include "../include/model_loader.h"
#include "../include/collision.h"
#include "../include/shadow.h"
#include "../include/game.h"

// makefile gcc main.c texture.c model_loader.c collision.c shadow.c -o program.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32

int map[N_NUM][N_NUM] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

SDL_Window *window = NULL;
SDL_GLContext context;
GLuint texture;
Model* woodBox = NULL;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    window = SDL_CreateWindow(
        "FRI - beadando",
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
    texture = loadTexture("./assets/texture.jpg");

    woodBox = loadOBJ("./assets/wood_box.obj");
    if (woodBox) {
        woodBox->textureID = loadTexture("./assets/wood_box_texture.png");
        if (woodBox->textureID == 0) {
            printf("Failed to load wood box texture!\n");
        }
    }   

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_dir[] = {-1.0f, -1.0f, -1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)WIDTH / HEIGHT, 0.1, 100.0);

    glClearColor(0.9f, 1.0f, 0.9f, 0.7f);

    SDL_Event event;
    int running = 1;

    float ball_x = 7.5f, ball_y = 1.0f, ball_z = 8.0f;
    float ball_speed = 5.0f;
    float roll_x = 0.0f, roll_z = 0.0f;
    float cam_angle = 0.0f;
    float cam_pitch = 0.3f;

    Uint32 lastTime = SDL_GetTicks();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    while (running)
    {
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

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
                gluPerspective(70.0, (double)w / h, 0.1, 100.0);
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                float sensitivity = 0.005f;
                cam_angle -= event.motion.xrel * sensitivity;
                if (cam_pitch > 1.5f)
                    cam_pitch = 1.5f;
                if (cam_pitch < -1.5f)
                    cam_pitch = -1.5f;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float distance = ball_speed * dt * 3;
        float dir_x = sinf(cam_angle);
        float dir_z = cosf(cam_angle);
        float new_x, new_z;

        if (keys[SDL_SCANCODE_W])
        {
            new_x = ball_x - dir_x * distance;
            new_z = ball_z - dir_z * distance;
            if (isWalkable(new_x, new_z))
            {
                ball_x = new_x;
                ball_z = new_z;
                roll_x -= distance * 50.0f;
            }
        }
        if (keys[SDL_SCANCODE_S])
        {
            new_x = ball_x + dir_x * distance;
            new_z = ball_z + dir_z * distance;
            if (isWalkable(new_x, new_z))
            {
                ball_x = new_x;
                ball_z = new_z;
                roll_x += distance * 50.0f;
            }
        }
        if (keys[SDL_SCANCODE_A])
        {
            new_x = ball_x - dir_z * distance;
            new_z = ball_z + dir_x * distance;
            if (isWalkable(new_x, new_z))
            {
                ball_x = new_x;
                ball_z = new_z;
                roll_z -= distance * 50.0f;
            }
        }
        if (keys[SDL_SCANCODE_D])
        {
            new_x = ball_x + dir_z * distance;
            new_z = ball_z - dir_x * distance;
            if (isWalkable(new_x, new_z))
            {
                ball_x = new_x;
                ball_z = new_z;
                roll_z += distance * 50.0f;
            }
        }
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }

        float rot_speed = 2.0f;
        if (keys[SDL_SCANCODE_Q])
        {
            cam_angle += rot_speed * dt;
        }
        if (keys[SDL_SCANCODE_E])
        {
            cam_angle -= rot_speed * dt;
        }

        float cam_dist = 7.0f;
        float camY = ball_y + sinf(cam_pitch) * cam_dist;
        float horizontal_dist = cosf(cam_pitch) * cam_dist;
        float camX = ball_x + sinf(cam_angle) * horizontal_dist;
        float camZ = ball_z + cosf(cam_angle) * horizontal_dist;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camX, camY, camZ, ball_x, ball_y, ball_z, 0, 1, 0);

        GLfloat light_dir2[] = {-1.0f, -1.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light_dir2);

        // Ground rendering
        for (int i = 0; i < N_NUM; i++)
        {
            for (int j = 0; j < N_NUM; j++)
            {
                float x = j * SIZE;
                float z = i * SIZE;
                switch (map[i][j])
                {
                case 0:
                    if (woodBox) {
                        glPushMatrix();
                        glTranslatef(j * SIZE + (SIZE / 2.0f), 1.0f, i * SIZE + (SIZE / 2.0f));
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, woodBox->textureID);
                        glColor3f(1.0f, 1.0f, 1.0f);
                        glBegin(GL_TRIANGLES);
                        for (int f = 0; f < woodBox->f_count; f++) {
                            for (int v = 0; v < 3; v++) {
                                int vt_idx = woodBox->faces[f].vt[v] - 1;
                                int v_idx = woodBox->faces[f].v[v] - 1;
                                glTexCoord2f(woodBox->texcoords[vt_idx].u, woodBox->texcoords[vt_idx].v);
                                glVertex3f(woodBox->vertices[v_idx].x, woodBox->vertices[v_idx].y, woodBox->vertices[v_idx].z);
                            }
                        }
                        glEnd();
                        glPopMatrix();
                    }
                    break;
                case 1:
                    glNormal3f(0, 1, 0);
                    glBegin(GL_QUADS);
                    glColor4f(1.0f, 0.0f, 0.0f, 0.2f);
                    glVertex3f(x, 0, z);
                    glVertex3f(x + SIZE, 0, z);
                    glVertex3f(x + SIZE, 0, z + SIZE);
                    glVertex3f(x, 0, z + SIZE);
                    glEnd();
                    break;
                case 2:
                    glNormal3f(0, 1, 0);
                    glBegin(GL_QUADS);
                    glColor4f(0.6f, 0.6f, 0.6f, 0.3f);
                    glVertex3f(x, 0, z);
                    glVertex3f(x + SIZE, 0, z);
                    glVertex3f(x + SIZE, 0, z + SIZE);
                    glVertex3f(x, 0, z + SIZE);
                    glEnd();
                    break;
                }
            }
        }

        // Ball rendering
        glPushMatrix();
        glTranslatef(ball_x, ball_y, ball_z);
        glRotatef(roll_x, 1, 0, 0);
        glRotatef(roll_z, 0, 0, 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glColor3f(0.9f, 0.2f, 0.0f);
        GLUquadric *quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);
        gluSphere(quad, 1.0, 32, 32);
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        // Shadow rendering
        GLfloat ground[] = {0.0f, 1.0f, 0.0f, 0.0f};
        GLfloat light[] = {-1.0f, -1.0f, -1.0f, 0.0f};
        GLfloat shadowMat[16];
        shadowMatrix(shadowMat, ground, light);

        glDisable(GL_LIGHTING);
        glColor4f(0, 0, 0, 0.9f);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        glTranslatef(0.0f, 0.1f, 0.0f);
        glTranslatef(ball_x, ball_y, ball_z);
        glRotatef(roll_x, 1, 0, 0);
        glRotatef(roll_z, 0, 0, 1);
        GLUquadric *quadl = gluNewQuadric();
        gluSphere(quadl, 1.0, 32, 32);
        gluDeleteQuadric(quadl);
        glPopMatrix();
        glEnable(GL_LIGHTING);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}