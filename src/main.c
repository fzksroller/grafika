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
Model *woodBox = NULL;

void calculateNormal(float x1, float y1, float z1, 
                     float x2, float y2, float z2, 
                     float x3, float y3, float z3) {
    float ux = x2 - x1;
    float uy = y2 - y1;
    float uz = z2 - z1;
    
    float vx = x3 - x1;
    float vy = y3 - y1;
    float vz = z3 - z1;
    
    float nx = uy * vz - uz * vy;
    float ny = uz * vx - ux * vz;
    float nz = ux * vy - uy * vx;
    
    float len = sqrtf(nx * nx + ny * ny + nz * nz);
    if (len > 0.0001f) {
        nx /= len;
        ny /= len;
        nz /= len;
    }
    
    glNormal3f(nx, ny, nz);
}

void drawGroundQuad(float x, float z, float r, float g, float b, float a, bool lighting_enabled) {
    if (!lighting_enabled) {
        glDisable(GL_LIGHTING);
    }
    if (lighting_enabled) {
        glNormal3f(0, 1, 0);
    }
    glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glVertex3f(x, 0, z);
    glVertex3f(x + SIZE, 0, z);
    glVertex3f(x + SIZE, 0, z + SIZE);
    glVertex3f(x, 0, z + SIZE);
    glEnd();

    if (lighting_enabled) {
        glNormal3f(0, -1, 0);
    }
    glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glVertex3f(x, 0, z);
    glVertex3f(x, 0, z + SIZE);
    glVertex3f(x + SIZE, 0, z + SIZE);
    glVertex3f(x + SIZE, 0, z);
    glEnd();
    
    if (!lighting_enabled) {
        glEnable(GL_LIGHTING);
    }
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
        "FRI - beadando - Rain & Shadows",
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

    GLuint help_texture;
    int show_help = 0;
    help_texture = loadTexture("./assets/help.png");

    woodBox = loadOBJ("./assets/wood_box.obj");
    if (woodBox)
    {
        woodBox->textureID = loadTexture("./assets/wood_box_texture.png");
        if (woodBox->textureID == 0)
        {
            printf("Failed to load wood box texture!\n");
        }
    }

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(70.0, (double)WIDTH / HEIGHT, 0.1, 100.0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    initDefaultLightDirection();

    SDL_Event event;
    int running = 1;
    int mouse_on_off = 1;
    float ball_x = 7.5f, ball_y = 1.0f, ball_z = 8.0f;
    float ball_speed = 5.0f;
    float roll_x = 0.0f, roll_z = 0.0f;
    float cam_angle = 0.0f;
    float cam_pitch = 0.3f;
    
    bool enable_rain = true;
    bool enable_shadow = true;

    Uint32 lastTime = SDL_GetTicks();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    initRain(1500);

    while (running)
    {
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;
        
        if (dt > 0.033f) dt = 0.033f;

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
            if (event.type == SDL_MOUSEMOTION && mouse_on_off)
            {
                float sensitivity = 0.005f;
                cam_angle -= event.motion.xrel * sensitivity;
                cam_pitch -= event.motion.yrel * sensitivity;
                if (cam_pitch > 1.5f)
                    cam_pitch = 1.5f;
                if (cam_pitch < -1.5f)
                    cam_pitch = -1.5f;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    mouse_on_off = !mouse_on_off;
                    SDL_SetRelativeMouseMode(mouse_on_off ? SDL_TRUE : SDL_FALSE);
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_R)
                {
                    enable_rain = !enable_rain;
                    printf("Rain: %s\n", enable_rain ? "ON" : "OFF");
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_T)
                {
                    enable_shadow = !enable_shadow;
                    printf("Shadow: %s\n", enable_shadow ? "ON" : "OFF");
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_F1)
                {
                    show_help = !show_help;
                }
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
        lookAt(camX, camY, camZ, ball_x, ball_y, ball_z, 0, 1, 0);

        GLfloat light0_pos[] = {10.0f, 20.0f, 10.0f, 1.0f};
        GLfloat light1_pos[] = {0.0f, -5.0f, 0.0f, 1.0f};
        GLfloat light2_pos[] = {-5.0f, 15.0f, -8.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
        glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);

        for (int i = 0; i < N_NUM; i++)
        {
            for (int j = 0; j < N_NUM; j++)
            {
                float x = j * SIZE;
                float z = i * SIZE;
                switch (map[i][j])
                {
                case 0:
                    if (woodBox)
                    {
                        glPushMatrix();
                        glTranslatef(j * SIZE + (SIZE / 2.0f), 1.001f, i * SIZE + (SIZE / 2.0f));
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, woodBox->textureID);
                        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                        
                        glBegin(GL_TRIANGLES);
                        for (int f = 0; f < woodBox->f_count; f++)
                        {
                            int v0_idx = woodBox->faces[f].v[0];
                            int v1_idx = woodBox->faces[f].v[1];
                            int v2_idx = woodBox->faces[f].v[2];
                            
                            calculateNormal(
                                woodBox->vertices[v0_idx].x,
                                woodBox->vertices[v0_idx].y,
                                woodBox->vertices[v0_idx].z,
                                woodBox->vertices[v1_idx].x,
                                woodBox->vertices[v1_idx].y,
                                woodBox->vertices[v1_idx].z,
                                woodBox->vertices[v2_idx].x,
                                woodBox->vertices[v2_idx].y,
                                woodBox->vertices[v2_idx].z
                            );
                            
                            for (int v = 0; v < 3; v++)
                            {
                                int vt_idx = woodBox->faces[f].vt[v];
                                int v_idx = woodBox->faces[f].v[v];

                                glTexCoord2f(
                                    woodBox->texcoords[vt_idx].u,
                                    woodBox->texcoords[vt_idx].v);

                                glVertex3f(
                                    woodBox->vertices[v_idx].x,
                                    woodBox->vertices[v_idx].y,
                                    woodBox->vertices[v_idx].z);
                            }
                        }
                        glEnd();
                        glPopMatrix();
                        drawGroundQuad(x, z, 1.0f, 0.0f, 0.0f, 0.6f, true);
                    }
                    break;
                    
                case 1:
                    drawGroundQuad(x, z, 0.0f, 1.0f, 0.0f, 0.6f, true);
                    break;
                    
                case 2:
                    drawGroundQuad(x, z, 1.0f, 0.918f, 0.0f, 0.5f, false);
                    break;
                }
            }
        }

        if (enable_shadow) {
            drawShadowForObject(ball_x, ball_z, ball_y, 1.0f, 0.0f);
        }

        glPushMatrix();
        glTranslatef(ball_x, ball_y, ball_z);
        glRotatef(roll_x, 1, 0, 0);
        glRotatef(roll_z, 0, 0, 1);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        
        drawSphere(1.0f, 32, 32);
        
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        if (enable_rain) {
            updateRain(dt);
            drawRain();
        }

        if (show_help)
        {
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

        SDL_GL_SwapWindow(window);
    }
    
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &help_texture);

    if (woodBox)
    {
        glDeleteTextures(1, &(woodBox->textureID));
        freeModel(woodBox);
    }
    
    cleanupRain();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}