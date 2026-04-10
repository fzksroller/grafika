#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600

int map[5][5] = {
    {1, 0, 1, 1, 0},
    {1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1},
    {1, 0, 0, 1, 1},
    {1, 1, 1, 1, 1}};

SDL_Window *window = NULL;
SDL_GLContext context;

GLuint texture;

GLuint loadTexture(const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        printf("Texture load error: %s\n", IMG_GetError());
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, mode,
                 surface->w, surface->h, 0,
                 mode, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    return tex;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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
    texture = loadTexture("../assets/texture.jpg");

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)WIDTH / HEIGHT, 0.1, 100.0);

    // háttérszín
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    SDL_Event event;
    int running = 1;

    float ball_x = 0.0f, ball_y = 1.0f, ball_z = 0.0f;
    float ball_speed = 5.0f;

    float roll_x = 0.0f, roll_z = 0.0f;

    float cam_angle = 0.0f;

    Uint32 lastTime = SDL_GetTicks();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

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
        }

        // billentyűk
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float distance = ball_speed * dt;

        float dirX = sinf(cam_angle);
        float dirZ = cosf(cam_angle);

        // előre / hátra
        if (keys[SDL_SCANCODE_W])
        {
            ball_x -= dirX * distance;
            ball_z -= dirZ * distance;

            roll_x -= distance * 50.0f;
        }
        if (keys[SDL_SCANCODE_S])
        {
            ball_x += dirX * distance;
            ball_z += dirZ * distance;

            roll_x += distance * 50.0f;
        }

        // oldalirány (kamera jobb vektor)
        if (keys[SDL_SCANCODE_A])
        {
            ball_x -= dirZ * distance;
            ball_z += dirX * distance;

            roll_z -= distance * 50.0f;
        }
        if (keys[SDL_SCANCODE_D])
        {
            ball_x += dirZ * distance;
            ball_z -= dirX * distance;

            roll_z += distance * 50.0f;
        }

        float rot_speed = 2.0f;

        if (keys[SDL_SCANCODE_Q])
        {
            cam_angle += rot_speed * dt; // balra
        }
        if (keys[SDL_SCANCODE_E])
        {
            cam_angle -= rot_speed * dt; // jobbra
        }

        float cam_dist = 7.0f;
        float offset = cam_dist * 0.707f;

        float camX = ball_x + sinf(cam_angle) * cam_dist;
        float camZ = ball_z + cosf(cam_angle) * cam_dist;
        float camY = ball_y + offset;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // kamera beállítás
        gluLookAt(
            camX, camY, camZ,
            ball_x, ball_y, ball_z,
            0, 1, 0);

        // ===== GROUND =====
        glColor3f(0.2f, 0.8f, 0.2f);

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (map[i][j] == 0)
                    continue;

                float x = j * 25;
                float z = i * 25;

                glBegin(GL_QUADS);
                glVertex3f(x, 0, z);
                glVertex3f(x + 25, 0, z);
                glVertex3f(x + 25, 0, z + 25);
                glVertex3f(x, 0, z + 25);
                glEnd();
            }
        }

        // ===== LABDA =====
        glPushMatrix();

        glTranslatef(ball_x, ball_y, ball_z);
        glRotatef(roll_x, 1, 0, 0);
        glRotatef(roll_z, 0, 0, 1);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glColor3f(0.6f, 0.2f, 0.8f);
        GLUquadric *quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);

        gluSphere(quad, 1.0, 32, 32);

        gluDeleteQuadric(quad);

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();

        int col = (int)floorf(ball_x / 25);
        int row = (int)floorf(ball_z / 25);

        printf("Ball is on cell: row=%d col=%d\n", row, col);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}