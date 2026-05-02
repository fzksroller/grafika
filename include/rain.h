#ifndef RAIN_H
#define RAIN_H

#include <GL/gl.h>
#include <stdbool.h>

typedef struct
{
    float x, y, z;
    float speed;
    bool active;
} RainDrop;

void initRain(int count);
void updateRain(float dt);
void drawRain(void);
void cleanupRain(void);

#endif