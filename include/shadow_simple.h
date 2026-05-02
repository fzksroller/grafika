#ifndef SHADOW_SIMPLE_H
#define SHADOW_SIMPLE_H

#include <GL/gl.h>
#include <stdbool.h>

void setLightDirection(float dirX, float dirY, float dirZ);

void initDefaultLightDirection(void);

void drawShadowEllipse(float x, float z, float radius, float opacity);

void drawShadowForObject(float objX, float objZ, float objY, float objRadius, float groundY);

#endif