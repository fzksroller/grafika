#include "../include/shadow_simple.h"
#include <math.h>

// Statikus változók a fényirány tárolására
static float lightDirX = 1.0f;
static float lightDirY = -1.0f;
static float lightDirZ = 0.0f;

void initDefaultLightDirection(void) {
    // Fény iránya: felülről, 45 fokos szögben
    lightDirX = 1.0f;
    lightDirY = -1.0f;
    lightDirZ = 0.0f;
    
    // Normalizálás
    float len = sqrtf(lightDirX * lightDirX + lightDirY * lightDirY + lightDirZ * lightDirZ);
    if (len > 0.01f) {
        lightDirX /= len;
        lightDirY /= len;
        lightDirZ /= len;
    }
}

void setLightDirection(float dirX, float dirY, float dirZ) {
    lightDirX = dirX;
    lightDirY = dirY;
    lightDirZ = dirZ;
    
    // Normalizálás
    float len = sqrtf(lightDirX * lightDirX + lightDirY * lightDirY + lightDirZ * lightDirZ);
    if (len > 0.01f) {
        lightDirX /= len;
        lightDirY /= len;
        lightDirZ /= len;
    }
}

void drawShadowEllipse(float x, float z, float radius, float opacity) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(0.0f, 0.0f, 0.0f, opacity * 0.6f);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, 0.02f, z);
    
    int segments = 32;
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159265f * i / segments;
        float dx = cosf(angle) * radius;
        float dz = sinf(angle) * radius;
        
        // A fény irányába megnyújtjuk az árnyékot
        float stretchFactor = 1.0f;
        if (fabs(lightDirY) > 0.01f) {
            float horizontalLen = sqrtf(lightDirX * lightDirX + lightDirZ * lightDirZ);
            stretchFactor = 1.0f + horizontalLen / fabs(lightDirY);
            if (stretchFactor > 2.5f) stretchFactor = 2.5f;
        }
        
        float finalDx = dx;
        float finalDz = dz;
        
        if (fabs(lightDirX) > 0.01f) {
            finalDx = dx * (1.0f + fabs(lightDirX) * stretchFactor * 0.5f);
        }
        if (fabs(lightDirZ) > 0.01f) {
            finalDz = dz * (1.0f + fabs(lightDirZ) * stretchFactor * 0.5f);
        }
        
        glVertex3f(x + finalDx, 0.02f, z + finalDz);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void drawShadowForObject(float objX, float objZ, float objY, float objRadius, float groundY) {
    float shadowX = objX;
    float shadowZ = objZ;
    float shadowRadius = objRadius;
    float opacity = 0.5f;
    
    float heightAboveGround = objY - groundY;
    
    if (heightAboveGround > 0.01f && fabs(lightDirY) > 0.01f) {
        float t = heightAboveGround / fabs(lightDirY);
        float offsetX = lightDirX * t;
        float offsetZ = lightDirZ * t;
        
        shadowX += offsetX;
        shadowZ += offsetZ;
        
        float scaleFactor = 1.0f + heightAboveGround * 0.3f;
        float horizontalComponent = sqrtf(lightDirX * lightDirX + lightDirZ * lightDirZ);
        scaleFactor += horizontalComponent * heightAboveGround * 0.2f;
        
        shadowRadius = objRadius * scaleFactor;
        
        opacity = 0.5f / (1.0f + heightAboveGround * 0.5f);
        if (opacity > 0.6f) opacity = 0.6f;
        if (opacity < 0.2f) opacity = 0.2f;
        
        if (fabs(lightDirY) > 0.7f) {
            opacity *= 1.3f;
            if (opacity > 0.7f) opacity = 0.7f;
        }
    }
    
    drawShadowEllipse(shadowX, shadowZ, shadowRadius, opacity);
}