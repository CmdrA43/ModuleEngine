#pragma once
#include "camera.h"
#include <cmath>

// Readable version - good balance of performance and maintainability
void updateCameraBasis(Camera& camera, float yaw, float pitch, float roll) {
    // Compute forward vector from yaw/pitch
    float sinY = sinf(yaw), cosY = cosf(yaw);
    float sinP = sinf(pitch), cosP = cosf(pitch);
    
    camera.forward = vector3(
        sinY * cosP,
        sinP, 
        cosY * cosP
    );
    
    // Compute right vector
    vector3 worldUp(0.0f, 1.0f, 0.0f);
    camera.right = camera.forward.cross(worldUp).normalize();
    
    // Compute up vector
    camera.up = camera.right.cross(camera.forward).normalize();
}

void setCameraFrustum(Camera& camera, float fovXDegrees, float aspectRatio, float nearPlane, float farPlane) {
    float fovXRad = fovXDegrees * (3.14159265f / 180.0f);
    camera.tanHalfFovX = tanf(fovXRad * 0.5f);
    camera.tanHalfFovY = camera.tanHalfFovX / aspectRatio;
    camera.nearZ = nearPlane;
    camera.farZ = farPlane;
}

// Readable bounds check - good for general use
bool boundsCheck(const Camera& camera, const vector3& point) {
    // Translate to camera space
    vector3 translated = point - camera.position;
    
    // Transform to camera space
    float camX = translated.dot(camera.right);
    float camY = translated.dot(camera.up);
    float camZ = translated.dot(camera.forward);
    
    // Early rejection behind camera
    if (camZ < camera.nearZ) return false;
    
    // Calculate frustum bounds at point's depth
    float boundX = camera.tanHalfFovX * camZ;
    float boundY = camera.tanHalfFovY * camZ;
    
    // Final bounds check
    return (camZ <= camera.farZ) && 
           (fabsf(camX) <= boundX) && 
           (fabsf(camY) <= boundY);
}

// Batch version for multiple points
void boundsCheckBatch(const Camera& camera, const vector3* points, int count, bool* results) {
    for (int i = 0; i < count; ++i) {
        results[i] = boundsCheck(camera, points[i]);
    }
}
