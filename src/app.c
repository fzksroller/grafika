#include "../include/app.h"
#include "../include/collision.h"
#include "../include/model_loader.h"
#include "../include/rain.h"
#include "../include/shadow_simple.h"
#include "../include/math3d.h"
#include "../include/texture.h"
#include "../include/game.h"
#include <math.h>
#include <stdio.h>

static GLuint help_texture;
static GLuint texture;
static Model *woodBox;
static GLuint bg_texture;

void setGlobalResources(GLuint tex, GLuint helpTex, Model *box, GLuint bgTex)
{
    texture = tex;
    help_texture = helpTex;
    woodBox = box;
    bg_texture = bgTex;
}

void initGame(GameState *state)
{
    state->x = 7.5f;
    state->y = 1.0f;
    state->z = 8.0f;
    state->speed = 5.0f;
    state->roll_x = 0.0f;
    state->roll_z = 0.0f;
    state->cam_angle = 0.0f;
    state->cam_pitch = 0.3f;
    state->ball_speed = 5.0f;
    state->enable_rain = true;
    state->enable_shadow = true;
    state->show_help = 0;
    state->mouse_on_off = 1;

    initMapUpdater();
    updateMapRandomlyBox();
    initRain(1500);
    initDefaultLightDirection();
}

void handleInput(GameState *state, const Uint8 *keys, float dt)
{
    float distance = state->ball_speed * dt * 3;
    float dir_x = sinf(state->cam_angle);
    float dir_z = cosf(state->cam_angle);
    float new_x, new_z;

    if (keys[SDL_SCANCODE_W])
    {
        new_x = state->x - dir_x * distance;
        new_z = state->z - dir_z * distance;
        if (isWalkable(new_x, new_z))
        {
            state->x = new_x;
            state->z = new_z;
            state->roll_x -= distance * 50.0f;
        }
        if (isaTrap(new_x, new_z))
        {
            state->x = 7.5f;
            state->z = 8.0f;
        }
    }
    if (keys[SDL_SCANCODE_S])
    {
        new_x = state->x + dir_x * distance;
        new_z = state->z + dir_z * distance;
        if (isWalkable(new_x, new_z))
        {
            state->x = new_x;
            state->z = new_z;
            state->roll_x += distance * 50.0f;
        }
        if (isaTrap(new_x, new_z))
        {
            state->x = 7.5f;
            state->z = 8.0f;
        }
    }
    if (keys[SDL_SCANCODE_A])
    {
        new_x = state->x - dir_z * distance;
        new_z = state->z + dir_x * distance;
        if (isWalkable(new_x, new_z))
        {
            state->x = new_x;
            state->z = new_z;
            state->roll_z -= distance * 50.0f;
        }
        if (isaTrap(new_x, new_z))
        {
            state->x = 7.5f;
            state->z = 8.0f;
        }
    }
    if (keys[SDL_SCANCODE_D])
    {
        new_x = state->x + dir_z * distance;
        new_z = state->z - dir_x * distance;
        if (isWalkable(new_x, new_z))
        {
            state->x = new_x;
            state->z = new_z;
            state->roll_z += distance * 50.0f;
        }
        if (isaTrap(new_x, new_z))
        {
            state->x = 7.5f;
            state->z = 8.0f;
        }
    }
    if (keys[SDL_SCANCODE_SPACE])
    {
        state->x = 7.5f;
        state->z = 8.0f;
    }

    float rot_speed = 2.0f;
    if (keys[SDL_SCANCODE_Q])
    {
        state->cam_angle += rot_speed * dt;
    }
    if (keys[SDL_SCANCODE_E])
    {
        state->cam_angle -= rot_speed * dt;
    }
}

void handleMouseMotion(GameState *state, int xrel, int yrel)
{
    if (state->mouse_on_off)
    {
        float sensitivity = 0.005f;
        state->cam_angle -= xrel * sensitivity;
        state->cam_pitch -= yrel * sensitivity;
        if (state->cam_pitch > 1.5f)
            state->cam_pitch = 1.5f;
        if (state->cam_pitch < -1.5f)
            state->cam_pitch = -1.5f;
    }
}

void handleKeyPress(GameState *state, int scancode)
{
    if (scancode == SDL_SCANCODE_ESCAPE)
    {
        state->mouse_on_off = !state->mouse_on_off;
        SDL_SetRelativeMouseMode(state->mouse_on_off ? SDL_TRUE : SDL_FALSE);
    }
    if (scancode == SDL_SCANCODE_R)
    {
        state->enable_rain = !state->enable_rain;
        printf("Rain: %s\n", state->enable_rain ? "ON" : "OFF");
    }
    if (scancode == SDL_SCANCODE_T)
    {
        state->enable_shadow = !state->enable_shadow;
        printf("Shadow: %s\n", state->enable_shadow ? "ON" : "OFF");
    }
    if (scancode == SDL_SCANCODE_F1)
    {
        state->show_help = !state->show_help;
    }
    if (scancode == SDL_SCANCODE_F2)
    {
        updateMapRandomlyBox();
        printf("Boxes regenerated!\n");
    }
}

void getCameraPosition(GameState *state, float cam_dist, float *camX, float *camY, float *camZ)
{
    *camY = state->y + sinf(state->cam_pitch) * cam_dist;
    float horizontal_dist = cosf(state->cam_pitch) * cam_dist;
    *camX = state->x + sinf(state->cam_angle) * horizontal_dist;
    *camZ = state->z + cosf(state->cam_angle) * horizontal_dist;
}

void updateCameraCollision(GameState *state, float *camX, float *camY, float *camZ)
{
    float camera_radius = 0.5f;
    float cam_dist = 5.0f;

    if (isCameraCollision(*camX, *camZ, *camY, camera_radius))
    {
        for (float dist = cam_dist; dist > 1.0f; dist -= 0.5f)
        {
            getCameraPosition(state, dist, camX, camY, camZ);
            if (!isCameraCollision(*camX, *camZ, *camY, camera_radius))
            {
                break;
            }
        }
    }
}

void updateGame(GameState *state, float dt)
{
    updateMapRandomlyTrap(dt);

    if (isaTrap(state->x, state->z))
    {
        state->x = 7.5f;
        state->z = 8.0f;
    }

    if (state->enable_rain)
    {
        updateRain(dt);
    }
}

void renderGround(void)
{
    for (int i = 0; i < N_NUM; i++)
    {
        for (int j = 0; j < N_NUM; j++)
        {
            float x = j * SIZE;
            float z = i * SIZE;

            glPushMatrix();

            switch (map[i][j])
            {
            case 0:
            case 4:
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glColor4f(0.0f, 1.0f, 0.0f, 0.6f);
                glVertex3f(x, 0, z);
                glVertex3f(x + SIZE, 0, z);
                glVertex3f(x + SIZE, 0, z + SIZE);
                glVertex3f(x, 0, z + SIZE);
                glEnd();
                break;

            case 1:
            case 2:
                if (woodBox)
                {
                    glTranslatef(j * SIZE + (SIZE / 2.0f), 2.5f, i * SIZE + (SIZE / 2.0f));
                    glScalef(2.5f, 2.5f, 2.5f);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, woodBox->textureID);
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

                    glBegin(GL_TRIANGLES);
                    for (int f = 0; f < woodBox->f_count; f++)
                    {
                        int v0_idx = woodBox->faces[f].v[0];
                        int v1_idx = woodBox->faces[f].v[1];
                        int v2_idx = woodBox->faces[f].v[2];

                        float ux = woodBox->vertices[v1_idx].x - woodBox->vertices[v0_idx].x;
                        float uy = woodBox->vertices[v1_idx].y - woodBox->vertices[v0_idx].y;
                        float uz = woodBox->vertices[v1_idx].z - woodBox->vertices[v0_idx].z;
                        float vx = woodBox->vertices[v2_idx].x - woodBox->vertices[v0_idx].x;
                        float vy = woodBox->vertices[v2_idx].y - woodBox->vertices[v0_idx].y;
                        float vz = woodBox->vertices[v2_idx].z - woodBox->vertices[v0_idx].z;

                        float nx = uy * vz - uz * vy;
                        float ny = uz * vx - ux * vz;
                        float nz = ux * vy - uy * vx;

                        float len = sqrtf(nx * nx + ny * ny + nz * nz);
                        if (len > 0.0001f)
                        {
                            nx /= len;
                            ny /= len;
                            nz /= len;
                        }

                        glNormal3f(nx, ny, nz);

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
                }
                glPopMatrix();
                glPushMatrix();
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glColor4f(1.0f, 0.0f, 0.0f, 0.6f);
                glVertex3f(x, 0, z);
                glVertex3f(x + SIZE, 0, z);
                glVertex3f(x + SIZE, 0, z + SIZE);
                glVertex3f(x, 0, z + SIZE);
                glEnd();
                break;

            case 3:
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
                glVertex3f(x, 0, z);
                glVertex3f(x + SIZE, 0, z);
                glVertex3f(x + SIZE, 0, z + SIZE);
                glVertex3f(x, 0, z + SIZE);
                glEnd();
                break;

            case 5:
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
                glVertex3f(x, 0, z);
                glVertex3f(x + SIZE, 0, z);
                glVertex3f(x + SIZE, 0, z + SIZE);
                glVertex3f(x, 0, z + SIZE);
                glEnd();
                break;
            }

            glPopMatrix();
        }
    }
}

void renderScene(GameState *state, float camX, float camY, float camZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    lookAt(camX, camY, camZ, state->x, state->y, state->z, 0, 1, 0);

    drawBackground();

    GLfloat light0_pos[] = {10.0f, 20.0f, 10.0f, 1.0f};
    GLfloat light1_pos[] = {0.0f, -5.0f, 0.0f, 1.0f};
    GLfloat light2_pos[] = {-5.0f, 15.0f, -8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);

    renderGround();

    if (state->enable_shadow)
    {
        drawShadowForObject(state->x, state->z, state->y, 1.0f, 0.0f);
    }

    glPushMatrix();
    glTranslatef(state->x, state->y, state->z);
    glRotatef(state->roll_x, 1, 0, 0);
    glRotatef(state->roll_z, 0, 0, 1);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

    drawSphere(1.0f, 32, 32);

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    if (state->enable_rain)
    {
        drawRain();
    }
}

int checkWinCondition(GameState *state)
{
    int gridX = (int)(state->x / SIZE);
    int gridZ = (int)(state->z / SIZE);

    if (gridX >= 0 && gridX < N_NUM && gridZ >= 0 && gridZ < N_NUM)
    {
        if (map[gridZ][gridX] == 5)
        {
            return 1;
        }
    }
    return 0;
}

void cleanupGame(void)
{
    cleanupRain();
}

void drawBackground(void)
{
    if (!bg_texture) return;
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bg_texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    float mapSize = N_NUM * SIZE;
    float startX = -mapSize / 5.0f;
    float startZ = mapSize / 5.0f;
    float yPos = -30.0f;
    glPushMatrix();
    glTranslatef(0.0f, yPos, 0.0f);
    static float rotationAngle = 0.0f;
    rotationAngle += 0.2f;
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(startX, yPos, startZ);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(startX + mapSize, yPos, startZ);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(startX + mapSize, yPos, startZ + mapSize);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(startX, yPos, startZ + mapSize);
    glEnd();
    
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}