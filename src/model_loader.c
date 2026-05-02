#include "../include/model_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Model *loadOBJ(const char *path)
{

    FILE *file = fopen(path, "r");
    if (!file)
        return NULL;

    Model *m = (Model *)calloc(1, sizeof(Model));

    char line[256];

    while (fgets(line, sizeof(line), file))
    {

        if (strncmp(line, "v ", 2) == 0)
            m->v_count++;

        else if (strncmp(line, "vt ", 3) == 0)
            m->vt_count++;

        else if (strncmp(line, "f ", 2) == 0)
            m->f_count += 2;
    }

    m->vertices = malloc(sizeof(Vec3) * m->v_count);
    m->texcoords = malloc(sizeof(Vec2) * m->vt_count);
    m->faces = malloc(sizeof(Face) * m->f_count);

    rewind(file);

    int vc = 0;
    int vtc = 0;
    int fc = 0;

    while (fgets(line, sizeof(line), file))
    {

        if (strncmp(line, "v ", 2) == 0)
        {

            sscanf(line,
                   "v %f %f %f",
                   &m->vertices[vc].x,
                   &m->vertices[vc].y,
                   &m->vertices[vc].z);

            vc++;
        }

        else if (strncmp(line, "vt ", 3) == 0)
        {

            sscanf(line,
                   "vt %f %f",
                   &m->texcoords[vtc].u,
                   &m->texcoords[vtc].v);

            m->texcoords[vtc].v = 1.0f - m->texcoords[vtc].v;

            vtc++;
        }

        else if (strncmp(line, "f ", 2) == 0)
        {

            int v[4];
            int vt[4];
            int vn[4];

            int matches = sscanf(line,
                                 "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                                 &v[0], &vt[0], &vn[0],
                                 &v[1], &vt[1], &vn[1],
                                 &v[2], &vt[2], &vn[2],
                                 &v[3], &vt[3], &vn[3]);

            if (matches == 12)
            {

                m->faces[fc].v[0] = v[0] - 1;
                m->faces[fc].v[1] = v[1] - 1;
                m->faces[fc].v[2] = v[2] - 1;

                m->faces[fc].vt[0] = vt[0] - 1;
                m->faces[fc].vt[1] = vt[1] - 1;
                m->faces[fc].vt[2] = vt[2] - 1;

                fc++;

                m->faces[fc].v[0] = v[0] - 1;
                m->faces[fc].v[1] = v[2] - 1;
                m->faces[fc].v[2] = v[3] - 1;

                m->faces[fc].vt[0] = vt[0] - 1;
                m->faces[fc].vt[1] = vt[2] - 1;
                m->faces[fc].vt[2] = vt[3] - 1;

                fc++;
            }
        }
    }

    m->f_count = fc;

    fclose(file);

    return m;
}

void freeModel(Model *model)
{
    if (model != NULL)
    {
        free(model->vertices);
        free(model->texcoords);
        free(model->faces);
        free(model);
    }
}