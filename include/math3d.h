#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>
#include <GL/gl.h>

void perspective(float fovy, float aspect, float zNear, float zFar);
void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
void drawSphere(float radius, int slices, int stacks);

#endif