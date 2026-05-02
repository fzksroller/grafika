#ifndef SHADOW_SIMPLE_H
#define SHADOW_SIMPLE_H

#include <GL/gl.h>
#include <stdbool.h>

// Fény irányának beállítása (x, y, z) - alapértelmezett: felülről 45 fok
void setLightDirection(float dirX, float dirY, float dirZ);

// Alapértelmezett fényirány inicializálása (felülről 45 fok)
void initDefaultLightDirection(void);

// Ellipszis alakú árnyék rajzolása a megadott pozícióban
void drawShadowEllipse(float x, float z, float radius, float opacity);

// Objektum alá árnyék rajzolása a fény irányának figyelembevételével
void drawShadowForObject(float objX, float objZ, float objY, float objRadius, float groundY);

#endif