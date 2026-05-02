#include "../include/rain.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RAIN_COUNT 1000
#define RAIN_AREA 100.0f
#define RAIN_HEIGHT 20.0f

static RainDrop *raindrops = NULL;
static int rainCount = 0;

void initRain(int count)
{
    rainCount = (count > 0) ? count : RAIN_COUNT;
    raindrops = (RainDrop *)malloc(sizeof(RainDrop) * rainCount);

    srand(time(NULL));

    for (int i = 0; i < rainCount; i++)
    {
        raindrops[i].x = ((float)rand() / RAND_MAX - 0.5f) * RAIN_AREA * 2;
        raindrops[i].z = ((float)rand() / RAND_MAX - 0.5f) * RAIN_AREA * 2;
        raindrops[i].y = ((float)rand() / RAND_MAX) * RAIN_HEIGHT;
        raindrops[i].speed = 5.0f + ((float)rand() / RAND_MAX) * 10.0f;
        raindrops[i].active = true;
    }
}

void updateRain(float dt)
{
    for (int i = 0; i < rainCount; i++)
    {
        if (raindrops[i].active)
        {
            raindrops[i].y -= raindrops[i].speed * dt;

            if (raindrops[i].y < 0.0f)
            {
                raindrops[i].y = RAIN_HEIGHT;
                raindrops[i].x = ((float)rand() / RAND_MAX - 0.5f) * RAIN_AREA * 2;
                raindrops[i].z = ((float)rand() / RAND_MAX - 0.5f) * RAIN_AREA * 2;
                raindrops[i].speed = 5.0f + ((float)rand() / RAND_MAX) * 10.0f;
            }
        }
    }
}

void drawRain(void)
{
    if (!raindrops)
        return;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(1.0f);
    glColor4f(0.6f, 0.8f, 1.0f, 0.7f);

    glBegin(GL_LINES);
    for (int i = 0; i < rainCount; i++)
    {
        if (raindrops[i].active)
        {
            float len = 0.3f;
            glVertex3f(raindrops[i].x, raindrops[i].y, raindrops[i].z);
            glVertex3f(raindrops[i].x, raindrops[i].y - len, raindrops[i].z);
        }
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void cleanupRain(void)
{
    if (raindrops)
    {
        free(raindrops);
        raindrops = NULL;
    }
}