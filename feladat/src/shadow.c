#include "../include/shadow.h"

void shadowMatrix(GLfloat shadowMat[16], GLfloat ground[4], GLfloat light[4])
{
    GLfloat dot = ground[0] * light[0] +
                  ground[1] * light[1] +
                  ground[2] * light[2] +
                  ground[3] * light[3];

    shadowMat[0] = dot - light[0] * ground[0];
    shadowMat[1] = -light[1] * ground[0];
    shadowMat[2] = -light[2] * ground[0];
    shadowMat[3] = -light[3] * ground[0];

    shadowMat[4] = -light[0] * ground[1];
    shadowMat[5] = dot - light[1] * ground[1];
    shadowMat[6] = -light[2] * ground[1];
    shadowMat[7] = -light[3] * ground[1];

    shadowMat[8] = -light[0] * ground[2];
    shadowMat[9] = -light[1] * ground[2];
    shadowMat[10] = dot - light[2] * ground[2];
    shadowMat[11] = -light[3] * ground[2];

    shadowMat[12] = -light[0] * ground[3];
    shadowMat[13] = -light[1] * ground[3];
    shadowMat[14] = -light[2] * ground[3];
    shadowMat[15] = dot - light[3] * ground[3];
}