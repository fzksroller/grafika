#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "model.h"

Model* loadOBJ(const char* path);
void freeModel(Model* model);

#endif