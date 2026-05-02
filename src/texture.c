#include "../include/texture.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_image.h>

GLuint loadTexture(const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        printf("Texture load error: %s\n", IMG_GetError());
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, mode,
                 surface->w, surface->h, 0,
                 mode, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    return tex;
}

char *getTexturePathFromMTL(const char *mtlPath)
{
    FILE *file = fopen(mtlPath, "r");
    if (!file)
        return NULL;

    static char textureFileName[256];
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "map_Kd ", 7) == 0)
        {
            sscanf(line, "map_Kd %s", textureFileName);
            fclose(file);
            return textureFileName;
        }
    }

    fclose(file);
    return NULL;
}