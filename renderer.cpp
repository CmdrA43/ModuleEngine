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

bool boundsCheckUltra(const Camera& camera, const vector3& point) {
    // Manual expansion for maximum speed
    float dx = point.x - camera.position.x;
    float dy = point.y - camera.position.y;
    float dz = point.z - camera.position.z;
    
    // Manual dot products
    float camZ = dx * camera.forward.x + dy * camera.forward.y + dz * camera.forward.z;
    if (camZ < camera.nearZ) return false;
    
    float camX = dx * camera.right.x + dy * camera.right.y + dz * camera.right.z;
    float camY = dx * camera.up.x + dy * camera.up.y + dz * camera.up.z;
    
    float boundX = camera.tanHalfFovX * camZ;
    float boundY = camera.tanHalfFovY * camZ;
    
    return (camZ <= camera.farZ) && 
           (fabsf(camX) <= boundX) && 
           (fabsf(camY) <= boundY);
}

// Ultra-optimized batch processing
void boundsCheckBatchUltra(const Camera& camera, const vector3* points, int count, bool* results) {
    // Pre-load camera values (helps compiler optimization)
    const vector3& pos = camera.position;
    const vector3& right = camera.right;
    const vector3& up = camera.up;
    const vector3& forward = camera.forward;
    const float nearZ = camera.nearZ;
    const float farZ = camera.farZ;
    const float tanHalfFovX = camera.tanHalfFovX;
    const float tanHalfFovY = camera.tanHalfFovY;
    
    for (int i = 0; i < count; ++i) {
        const vector3& point = points[i];
        float dx = point.x - pos.x;
        float dy = point.y - pos.y;
        float dz = point.z - pos.z;
        
        float camZ = dx * forward.x + dy * forward.y + dz * forward.z;
        if (camZ < nearZ) {
            results[i] = false;
            continue;
        }
        
        float camX = dx * right.x + dy * right.y + dz * right.z;
        float camY = dx * up.x + dy * up.y + dz * up.z;
        
        float boundX = tanHalfFovX * camZ;
        float boundY = tanHalfFovY * camZ;
        
        results[i] = (camZ <= farZ) && 
                    (fabsf(camX) <= boundX) && 
                    (fabsf(camY) <= boundY);
    }
}
