#ifndef MODEL_H
#define MODEL_H

#include "vec3.h"
#include <GL/gl.h>

typedef struct {
    Vec3 *vertices;
    Vec2 *texcoords;
    Face *faces;
    int v_count, vt_count, f_count;
    GLuint textureID;
} Model;

#endif