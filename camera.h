#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

struct vector3 {
    float x, y, z;
    
    vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    // Basic vector operations
    vector3 operator-(const vector3& other) const {
        return vector3(x - other.x, y - other.y, z - other.z);
    }
    
    vector3 operator+(const vector3& other) const {
        return vector3(x + other.x, y + other.y, z + other.z);
    }
    
    vector3 operator*(float scalar) const {
        return vector3(x * scalar, y * scalar, z * scalar);
    }
    
    float dot(const vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    vector3 cross(const vector3& other) const {
        return vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z, 
            x * other.y - y * other.x
        );
    }
    
    float lengthSquared() const {
        return x*x + y*y + z*z;
    }

    //add to modEngMath file
    inline float Q_rsqrt(float number) {
      long i;
      float x2, y;
      const float threehalfs = 1.5F;

      x2 = number * 0.5F;
      y = number;
      i = *(long*)&y;
      i = 0x5f3759df - (i >> 1);
      y = *(float*)&i;
      y = y * (threehalfs - (x2 * y * y));
      return y;
    }

    void fastNormalize(vector3& vec) {
      float lengthSq = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
      float invLength = Q_rsqrt(lengthSq);
      vec.x *= invLength;
      vec.y *= invLength; 
      vec.z *= invLength;
    }
};

struct Camera {
    vector3 position;
    vector3 right, up, forward;  // Precomputed basis vectors
    float tanHalfFovX, tanHalfFovY;
    float nearZ, farZ;
};

// Function declarations
void updateCameraBasis(Camera& camera, float yaw, float pitch, float roll = 0.0f);
void setCameraFrustum(Camera& camera, float fovXDegrees, float aspectRatio, float nearPlane, float farPlane);
bool boundsCheck(const Camera& camera, const vector3& point);
void boundsCheckBatch(const Camera& camera, const vector3* points, int count, bool* results);

// Ultra-optimized versions for critical paths
bool boundsCheckUltra(const Camera& camera, const vector3& point);
void boundsCheckBatchUltra(const Camera& camera, const vector3* points, int count, bool* results);

#endif
