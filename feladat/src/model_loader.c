#include "../include/model_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Model* loadOBJ(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return NULL;

    Model* m = (Model*)calloc(1, sizeof(Model));
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) m->v_count++;
        else if (strncmp(line, "vt ", 3) == 0) m->vt_count++;
        else if (strncmp(line, "f ", 2) == 0) m->f_count++;
    }

    m->vertices = malloc(sizeof(Vec3) * m->v_count);
    m->texcoords = malloc(sizeof(Vec2) * m->vt_count);
    m->faces = malloc(sizeof(Face) * m->f_count);

    rewind(file);
    int vc = 0, vtc = 0, fc = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line, "v %f %f %f", &m->vertices[vc].x, &m->vertices[vc].y, &m->vertices[vc].z);
            vc++;
        }
        
        if (strncmp(line, "vt ", 3) == 0) {
            sscanf(line, "vt %f %f", &m->texcoords[vtc].u, &m->texcoords[vtc].v);
            m->texcoords[vtc].v = 1.0f - m->texcoords[vtc].v;
            vtc++;
        }

        if (strncmp(line, "f ", 2) == 0) {
            int matches = sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", 
                &m->faces[fc].v[0], &m->faces[fc].vt[0],
                &m->faces[fc].v[1], &m->faces[fc].vt[1],
                &m->faces[fc].v[2], &m->faces[fc].vt[2]);

            if (matches < 6) {
                sscanf(line, "f %d/%d %d/%d %d/%d", 
                    &m->faces[fc].v[0], &m->faces[fc].vt[0],
                    &m->faces[fc].v[1], &m->faces[fc].vt[1],
                    &m->faces[fc].v[2], &m->faces[fc].vt[2]);
            }
            fc++;
        }
    }
    fclose(file);
    return m;
}