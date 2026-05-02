#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 5
#define N_NUM 20

int map[N_NUM][N_NUM];

bool isWalkable(float x, float z);
bool isaTrap(float x, float z);
void updateMapRandomlyTrap(float dt);
void updateMapRandomlyBox(void);
void initMapUpdater(void);
int isCameraCollision(float camX, float camZ, float camY, float radius);

#endif