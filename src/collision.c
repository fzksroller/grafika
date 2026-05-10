#include "../include/collision.h"

#define SIZE 5
#define N_NUM 20

float timer = 0.0f;

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

        if (map[r][c] == 1 || map[r][c] == 2)
            return false;
    }

    return true;
}

bool isaTrap(float x, float z)
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
            return true;

        if (map[r][c] == 3)
            return true;
    }

    return false;
}

void initMapUpdater(void)
{
    srand(time(NULL));
    timer = 3.0f;
}

void updateMapRandomlyBox(void)
{
    for (int i = 0; i < N_NUM; i++)
    {
        for (int j = 0; j < N_NUM; j++)
        {
            if (map[i][j] == 1 || map[i][j] == 2)
            {
                map[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < N_NUM; i++)
    {
        for (int j = 0; j < N_NUM; j++)
        {
            if (map[i][j] == 0)
            {
                int random = rand() % 100;
                if (random < 25)
                {
                    map[i][j] = (rand() % 2) + 1;
                }
            }
        }
    }

    for (int i = 0; i < N_NUM; i++)
    {
        map[i][0] = 1;
        map[i][N_NUM - 1] = 1;
        map[0][i] = 1;
        map[N_NUM - 1][i] = 1;
    }
}
void updateMapRandomlyTrap(float dt)
{
    timer += dt;
    int random;

    if (timer >= 3.0f)
    {
        timer = 0.0f;

        for (int i = 0; i < N_NUM; i++)
        {
            for (int j = 0; j < N_NUM; j++)
            {
                if (map[i][j] == 0)
                {
                    random = rand() % 100;
                    if (random < 50)
                    {
                        map[i][j] = (rand() % 2) * 3;
                    }
                }
                else if (map[i][j] == 3)
                {
                    random = rand() % 100;
                    if (random < 90)
                    {
                        map[i][j] = 0;
                    }
                }
            }
        }

        for (int i = 0; i < N_NUM; i++)
        {
            map[i][0] = 1;
            map[i][N_NUM - 1] = 1;
            map[0][i] = 1;
            map[N_NUM - 1][i] = 1;
        }
    }
}

int isCameraCollision(float camX, float camZ, float camY, float radius)
{
    int gridX = (int)(camX / SIZE);
    int gridZ = (int)(camZ / SIZE);

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int x = gridX + i;
            int z = gridZ + j;

            if (x >= 0 && x < N_NUM && z >= 0 && z < N_NUM)
            {
                if (map[z][x] == 1 || map[z][x] == 2)
                {
                    float centerX = x * SIZE + SIZE / 2.0f;
                    float centerZ = z * SIZE + SIZE / 2.0f;
                    float centerY = 2.5f;

                    float dx = camX - centerX;
                    float dz = camZ - centerZ;
                    float dy = camY - centerY;

                    float dist = sqrtf(dx * dx + dy * dy + dz * dz);

                    if (dist < radius + 1.5f)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}