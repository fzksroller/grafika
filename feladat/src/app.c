#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <SDL2/SDL_image.h>

// makefile gcc main.c -o program.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32

#define WIDTH 800
#define HEIGHT 600
#define SIZE 5
#define N_NUM 10

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

int isWalkable(float x, float z)
{
    int points[4][2] = {
        {(int)((x - 1.0f) / SIZE), (int)((z - 1.0f) / SIZE)},
        {(int)((x + 1.0f) / SIZE), (int)((z - 1.0f) / SIZE)},
        {(int)((x - 1.0f) / SIZE), (int)((z + 1.0f) / SIZE)},
        {(int)((x + 1.0f) / SIZE), (int)((z + 1.0f) / SIZE)}};

    for (int i = 0; i < 4; i++)
    {
        int r = points[i][1];
        int c = points[i][0];

        if (r < 0 || c < 0 || r >= N_NUM || c >= N_NUM)
            return 0;

        if (map[r][c] == 0)
            return 0;
    }

    return 1;
}

void shadowMatrix(GLfloat shadowMat[16], GLfloat ground[4], GLfloat light[4])
{
    GLfloat dot = ground[0] * light[0] +
                  ground[1] * light[1] +
                  ground[2] * light[2] +
                  ground[3] * light[3];

    shadowMat[0] = dot - light[0] * ground[0];
    shadowMat[4] = 0.0f - light[0] * ground[1];
    shadowMat[8] = 0.0f - light[0] * ground[2];
    shadowMat[12] = 0.0f - light[0] * ground[3];

    shadowMat[1] = 0.0f - light[1] * ground[0];
    shadowMat[5] = dot - light[1] * ground[1];
    shadowMat[9] = 0.0f - light[1] * ground[2];
    shadowMat[13] = 0.0f - light[1] * ground[3];

    shadowMat[2] = 0.0f - light[2] * ground[0];
    shadowMat[6] = 0.0f - light[2] * ground[1];
    shadowMat[10] = dot - light[2] * ground[2];
    shadowMat[14] = 0.0f - light[2] * ground[3];

    shadowMat[3] = 0.0f - light[3] * ground[0];
    shadowMat[7] = 0.0f - light[3] * ground[1];
    shadowMat[11] = 0.0f - light[3] * ground[2];
    shadowMat[15] = dot - light[3] * ground[3];
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

    // ===== LIGHTING =====
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_dir[] = {-1.0f, -1.0f, -1.0f, 0.0f}; // fényirány
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // anyag + színek
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // szebb megjelenés
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

    float cam_pitch = 0.3f; // kezdeti szög

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
                //cam_pitch -= event.motion.yrel * sensitivity;

                // limitálás (ne forduljon át)
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
        int col, row;

        if (keys[SDL_SCANCODE_F1])
        {
        }
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
        float offset = cam_dist * 0.707f;

        float camY = ball_y + sinf(cam_pitch) * cam_dist;
        float horizontal_dist = cosf(cam_pitch) * cam_dist;

        float camX = ball_x + sinf(cam_angle) * horizontal_dist;
        float camZ = ball_z + cosf(cam_angle) * horizontal_dist;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(
            camX, camY, camZ,
            ball_x, ball_y, ball_z,
            0, 1, 0);

        GLfloat light_dir[] = {-1.0f, -1.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

        // ===== GROUND =====
        for (int i = 0; i < N_NUM; i++)
        {
            for (int j = 0; j < N_NUM; j++)
            {
                float x = j * SIZE;
                float z = i * SIZE;
                switch (map[i][j])
                {
                case 0:
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

        // ===== LABDA =====
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

        GLfloat ground[] = {0.0f, 1.0f, 0.0f, 0.0f}; // y=0 sík
        GLfloat light[] = {-1.0f, -1.0f, -1.0f, 0.0f};

        GLfloat shadowMat[16];
        shadowMatrix(shadowMat, ground, light);

        // ===== SHADOW =====
        glDisable(GL_LIGHTING);
        glColor4f(0, 0, 0, 0.9f);

        glPushMatrix();
        glMultMatrixf(shadowMat);

        // kis offset, hogy ne villogjon
        glTranslatef(0.0f, 0.1f, 0.0f);

        // ugyanaz a transform mint a labdánál
        glTranslatef(ball_x, ball_y, ball_z);
        glRotatef(roll_x, 1, 0, 0);
        glRotatef(roll_z, 0, 0, 1);

        // rajzold újra a gömböt (árnyékként)
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