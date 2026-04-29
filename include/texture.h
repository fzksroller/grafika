#ifndef TEXTURE_H
#define TEXTURE_H
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#include <GL/gl.h>

GLuint loadTexture(const char *path);
char* getTexturePathFromMTL(const char* mtlPath);

#endif