
#include "../include/math3d.h"

void perspective(float fovy, float aspect, float zNear, float zFar) {
    float f = 1.0f / tanf(fovy * 3.14159265f / 360.0f);
    float nz = zNear - zFar;
    
    GLfloat matrix[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar + zNear) / nz, -1,
        0, 0, (2 * zFar * zNear) / nz, 0
    };
    
    glMultMatrixf(matrix);
}

void lookAt(float eyeX, float eyeY, float eyeZ,
                   float centerX, float centerY, float centerZ,
                   float upX, float upY, float upZ) {
    
    float forwardX = centerX - eyeX;
    float forwardY = centerY - eyeY;
    float forwardZ = centerZ - eyeZ;
    
    float len = sqrtf(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
    forwardX /= len;
    forwardY /= len;
    forwardZ /= len;
    
    float sideX = forwardY * upZ - forwardZ * upY;
    float sideY = forwardZ * upX - forwardX * upZ;
    float sideZ = forwardX * upY - forwardY * upX;
    
    len = sqrtf(sideX*sideX + sideY*sideY + sideZ*sideZ);
    sideX /= len;
    sideY /= len;
    sideZ /= len;
    
    float upX2 = sideY * forwardZ - sideZ * forwardY;
    float upY2 = sideZ * forwardX - sideX * forwardZ;
    float upZ2 = sideX * forwardY - sideY * forwardX;
    
    GLfloat matrix[16] = {
        sideX, upX2, -forwardX, 0,
        sideY, upY2, -forwardY, 0,
        sideZ, upZ2, -forwardZ, 0,
        0, 0, 0, 1
    };
    
    glMultMatrixf(matrix);
    glTranslatef(-eyeX, -eyeY, -eyeZ);
}

void drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i <= stacks; i++) {
        float theta1 = 3.14159265f * i / stacks;
        float theta2 = 3.14159265f * (i + 1) / stacks;
        
        float y1 = radius * cosf(theta1);
        float y2 = radius * cosf(theta2);
        float r1 = radius * sinf(theta1);
        float r2 = radius * sinf(theta2);
        
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; j++) {
            float phi = 2 * 3.14159265f * j / slices;
            float x1 = r1 * cosf(phi);
            float z1 = r1 * sinf(phi);
            float x2 = r2 * cosf(phi);
            float z2 = r2 * sinf(phi);
            
            glNormal3f(x1 / radius, y1 / radius, z1 / radius);
            glTexCoord2f((float)j / slices, (float)i / stacks);
            glVertex3f(x1, y1, z1);
            
            glNormal3f(x2 / radius, y2 / radius, z2 / radius);
            glTexCoord2f((float)j / slices, (float)(i + 1) / stacks);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
}