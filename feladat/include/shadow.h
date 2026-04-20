#ifndef SHADOW_H
#define SHADOW_H

#include <GL/gl.h>

void shadowMatrix(GLfloat shadowMat[16], GLfloat ground[4], GLfloat light[4]);

#endif