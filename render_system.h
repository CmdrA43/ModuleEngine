// render_system.h
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <cmath>
#include <vector>
#include <cstdint>
#include <algorithm>

// vector3 with all necessary operations
struct vector3 {
    float x, y, z;
    
    vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    vector3 operator-(const vector3& other) const { return vector3(x - other.x, y - other.y, z - other.z); }
    vector3 operator+(const vector3& other) const { return vector3(x + other.x, y + other.y, z + other.z); }
    vector3 operator*(float scalar) const { return vector3(x * scalar, y * scalar, z * scalar); }
    
    float dot(const vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    
    vector3 cross(const vector3& other) const {
        return vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
    
    float lengthSquared() const { return x*x + y*y + z*z; }
    
    vector3 normalize() const {
        float lenSq = lengthSquared();
        if (lenSq > 0) {
            float invLen = 1.0f / std::sqrt(lenSq);
            return vector3(x * invLen, y * invLen, z * invLen);
        }
        return *this;
    }
};

// Color structure
struct Color {
    uint8_t r, g, b, a;
    Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
};

// Camera with precomputed basis vectors
struct Camera {
    vector3 position;
    vector3 right, up, forward;
    float tanHalfFovX, tanHalfFovY;
    float nearZ, farZ;
    
    Camera() : position(0,0,0), right(1,0,0), up(0,1,0), forward(0,0,1), 
               tanHalfFovX(0.577f), tanHalfFovY(0.577f), nearZ(0.1f), farZ(1000.0f) {}
    
    void updateBasis(float yaw, float pitch, float roll = 0.0f) {
        float sinY = sinf(yaw), cosY = cosf(yaw);
        float sinP = sinf(pitch), cosP = cosf(pitch);
        
        forward = vector3(sinY * cosP, sinP, cosY * cosP);
        right = forward.cross(vector3(0,1,0)).normalize();
        up = right.cross(forward).normalize();
    }
    
    void setFrustum(float fovXDegrees, float aspectRatio, float nearPlane, float farPlane) {
        float fovXRad = fovXDegrees * (3.14159265f / 180.0f);
        tanHalfFovX = tanf(fovXRad * 0.5f);
        tanHalfFovY = tanHalfFovX / aspectRatio;
        nearZ = nearPlane;
        farZ = farPlane;
    }
    
    vector3 worldToCameraSpace(const vector3& worldPoint) const {
        vector3 translated = worldPoint - position;
        return vector3(translated.dot(right), translated.dot(up), translated.dot(forward));
    }
    
    bool boundsCheck(const vector3& point) const {
        vector3 cameraSpace = worldToCameraSpace(point);
        if (cameraSpace.z < nearZ) return false;
        
        float boundX = tanHalfFovX * cameraSpace.z;
        float boundY = tanHalfFovY * cameraSpace.z;
        
        return (cameraSpace.z <= farZ) && 
               (fabsf(cameraSpace.x) <= boundX) && 
               (fabsf(cameraSpace.y) <= boundY);
    }
};

// Frame buffer with color and depth
struct FrameBuffer {
    int width, height;
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;
    
    FrameBuffer(int w, int h) : width(w), height(h) {
        colorBuffer.resize(w * h, Color(0, 0, 0, 255));
        depthBuffer.resize(w * h, 1.0f);
    }
    
    void clear(Color clearColor = Color(0, 0, 0, 255), float depth = 1.0f) {
        std::fill(colorBuffer.begin(), colorBuffer.end(), clearColor);
        std::fill(depthBuffer.begin(), depthBuffer.end(), depth);
    }
    
    void setPixel(int x, int y, const Color& color, float depth) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int index = y * width + x;
            if (depth < depthBuffer[index]) {
                colorBuffer[index] = color;
                depthBuffer[index] = depth;
            }
        }
    }
};

// Vertex with position and color
struct Vertex {
    vector3 position;
    Color color;
    Vertex(const vector3& pos = vector3(), const Color& col = Color()) : position(pos), color(col) {}
};

// Triangle face
struct Triangle {
    Vertex v0, v1, v2;
    Color color;
    Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& col = Color()) 
        : v0(a), v1(b), v2(c), color(col) {}
};

// 3D Mesh
struct Mesh {
    std::vector<Triangle> triangles;
    Color baseColor;
    
    Mesh(const Color& col = Color()) : baseColor(col) {}
    
    void addTriangle(const Vertex& a, const Vertex& b, const Vertex& c) {
        triangles.emplace_back(a, b, c, baseColor);
    }
    
    void addTriangle(const Triangle& tri) {
        triangles.push_back(tri);
    }
};

// Main rendering system
class RenderSystem {
private:
    FrameBuffer& frameBuffer;
    Camera& camera;
    
    float edgeFunction(const vector3& a, const vector3& b, const vector3& c) {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }
    
    Color interpolateColor(const Color& c0, const Color& c1, const Color& c2, float alpha, float beta, float gamma) {
        return Color(
            (uint8_t)(alpha * c0.r + beta * c1.r + gamma * c2.r),
            (uint8_t)(alpha * c0.g + beta * c1.g + gamma * c2.g),
            (uint8_t)(alpha * c0.b + beta * c1.b + gamma * c2.b),
            (uint8_t)(alpha * c0.a + beta * c1.a + gamma * c2.a)
        );
    }
    
    vector3 projectToScreen(const vector3& worldPoint) {
        vector3 cameraSpace = camera.worldToCameraSpace(worldPoint);
        if (cameraSpace.z <= 0) return vector3(-1, -1, -1);
        
        float screenX = (cameraSpace.x / (camera.tanHalfFovX * cameraSpace.z) + 1.0f) * 0.5f * frameBuffer.width;
        float screenY = (1.0f - (cameraSpace.y / (camera.tanHalfFovY * cameraSpace.z))) * 0.5f * frameBuffer.height;
        return vector3(screenX, screenY, cameraSpace.z);
    }

public:
    RenderSystem(FrameBuffer& fb, Camera& cam) : frameBuffer(fb), camera(cam) {}
    
    void rasterizeTriangle(const Triangle& tri) {
        vector3 screen0 = projectToScreen(tri.v0.position);
        vector3 screen1 = projectToScreen(tri.v1.position);
        vector3 screen2 = projectToScreen(tri.v2.position);
        
        if (screen0.z <= 0 || screen1.z <= 0 || screen2.z <= 0) return;
        
        int minX = std::max(0, (int)std::floor(std::min(screen0.x, std::min(screen1.x, screen2.x))));
        int maxX = std::min(frameBuffer.width - 1, (int)std::ceil(std::max(screen0.x, std::max(screen1.x, screen2.x))));
        int minY = std::max(0, (int)std::floor(std::min(screen0.y, std::min(screen1.y, screen2.y))));
        int maxY = std::min(frameBuffer.height - 1, (int)std::ceil(std::max(screen0.y, std::max(screen1.y, screen2.y))));
        
        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                vector3 pixelPos(x + 0.5f, y + 0.5f, 0);
                
                float alpha = edgeFunction(screen1, screen2, pixelPos);
                float beta = edgeFunction(screen2, screen0, pixelPos);
                float gamma = edgeFunction(screen0, screen1, pixelPos);
                
                if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                    float totalArea = alpha + beta + gamma;
                    alpha /= totalArea;
                    beta /= totalArea;
                    gamma /= totalArea;
                    
                    float depth = alpha * screen0.z + beta * screen1.z + gamma * screen2.z;
                    Color pixelColor = interpolateColor(tri.v0.color, tri.v1.color, tri.v2.color, alpha, beta, gamma);
                    frameBuffer.setPixel(x, y, pixelColor, depth);
                }
            }
        }
    }
    
    void renderMesh(const Mesh& mesh) {
        for (const auto& tri : mesh.triangles) {
            rasterizeTriangle(tri);
        }
    }
    
    void renderScene(const std::vector<Mesh>& meshes) {
        for (const auto& mesh : meshes) {
            renderMesh(mesh);
        }
    }
};

// Mesh generators for common shapes
namespace MeshGenerators {
    Mesh createCube(float size = 1.0f, const Color& color = Color(255, 0, 0)) {
        Mesh cube(color);
        float halfSize = size * 0.5f;
        
        std::vector<Vertex> vertices = {
            Vertex(vector3(-halfSize, -halfSize, halfSize), color),
            Vertex(vector3(halfSize, -halfSize, halfSize), color),
            Vertex(vector3(halfSize, halfSize, halfSize), color),
            Vertex(vector3(-halfSize, halfSize, halfSize), color),
            Vertex(vector3(-halfSize, -halfSize, -halfSize), color),
            Vertex(vector3(-halfSize, halfSize, -halfSize), color),
            Vertex(vector3(halfSize, halfSize, -halfSize), color),
            Vertex(vector3(halfSize, -halfSize, -halfSize), color),
            Vertex(vector3(-halfSize, halfSize, -halfSize), color),
            Vertex(vector3(-halfSize, halfSize, halfSize), color),
            Vertex(vector3(halfSize, halfSize, halfSize), color),
            Vertex(vector3(halfSize, halfSize, -halfSize), color),
            Vertex(vector3(-halfSize, -halfSize, -halfSize), color),
            Vertex(vector3(halfSize, -halfSize, -halfSize), color),
            Vertex(vector3(halfSize, -halfSize, halfSize), color),
            Vertex(vector3(-halfSize, -halfSize, halfSize), color),
            Vertex(vector3(halfSize, -halfSize, -halfSize), color),
            Vertex(vector3(halfSize, halfSize, -halfSize), color),
            Vertex(vector3(halfSize, halfSize, halfSize), color),
            Vertex(vector3(halfSize, -halfSize, halfSize), color),
            Vertex(vector3(-halfSize, -halfSize, -halfSize), color),
            Vertex(vector3(-halfSize, -halfSize, halfSize), color),
            Vertex(vector3(-halfSize, halfSize, halfSize), color),
            Vertex(vector3(-halfSize, halfSize, -halfSize), color)
        };
        
        int faces[] = {0,1,2,0,2,3,4,5,6,4,6,7,8,9,10,8,10,11,12,13,14,12,14,15,16,17,18,16,18,19,20,21,22,20,22,23};
        
        for (int i = 0; i < 12; ++i) {
            cube.addTriangle(vertices[faces[i*3]], vertices[faces[i*3+1]], vertices[faces[i*3+2]]);
        }
        
        return cube;
    }
    
    Mesh createPyramid(float baseSize = 1.0f, float height = 1.0f, const Color& color = Color(0, 255, 0)) {
        Mesh pyramid(color);
        float halfBase = baseSize * 0.5f;
        
        Vertex tip(vector3(0, height, 0), color);
        Vertex base0(vector3(-halfBase, 0, -halfBase), color);
        Vertex base1(vector3(halfBase, 0, -halfBase), color);
        Vertex base2(vector3(halfBase, 0, halfBase), color);
        Vertex base3(vector3(-halfBase, 0, halfBase), color);
        
        pyramid.addTriangle(base0, base1, tip);
        pyramid.addTriangle(base1, base2, tip);
        pyramid.addTriangle(base2, base3, tip);
        pyramid.addTriangle(base3, base0, tip);
        pyramid.addTriangle(base0, base2, base1);
        pyramid.addTriangle(base0, base3, base2);
        
        return pyramid;
    }
}

#endif
