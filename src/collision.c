#include "../include/collision.h"

#define SIZE 5
#define N_NUM 10

bool isWalkable(float x, float z)
{
    int points[4][2] = {
        {(int)((x - 1.0f) / SIZE), (int)((z - 1.0f) / SIZE)},
        {(int)((x + 1.0f) / SIZE), (int)((z - 1.0f) / SIZE)},
        {(int)((x - 1.0f) / SIZE), (int)((z + 1.0f) / SIZE)},
        {(int)((x + 1.0f) / SIZE), (int)((z + 1.0f) / SIZE)}};

    for (int i = 0; i < 4; i++)
    {
        int r = points[i][1];
        int c = points[i][0];

        if (r < 0 || c < 0 || r >= N_NUM || c >= N_NUM)
            return false;

        if (map[r][c] == 0)
            return false;
    }

    return true;
}