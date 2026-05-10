#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "model.h"

typedef struct {
    float x, y, z;
    float speed;
    float roll_x, roll_z;
    float cam_angle, cam_pitch;
    float ball_speed;
    bool enable_rain;
    bool enable_shadow;
    int show_help;
    int mouse_on_off;
} GameState;

void initGame(GameState* state);
void updateGame(GameState* state, float dt);
void handleInput(GameState* state, const Uint8* keys, float dt);
void handleMouseMotion(GameState* state, int xrel, int yrel);
void handleKeyPress(GameState* state, int scancode);
void renderScene(GameState* state, float camX, float camY, float camZ);
void getCameraPosition(GameState* state, float cam_dist, float* camX, float* camY, float* camZ);
void cleanupGame(void);
int checkWinCondition(GameState* state);
void updateCameraCollision(GameState* state, float* camX, float* camY, float* camZ);
void setGlobalResources(GLuint tex, GLuint helpTex, Model* box, GLuint bgTex);
void drawBackground(void);

#endif