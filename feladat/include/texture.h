#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

GLuint loadTexture(const char *path);
char* getTexturePathFromMTL(const char* mtlPath);

#endif