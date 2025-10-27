#ifndef DX_RENDERER_H
#define DX_RENDERER_H

#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "render_system.h"  // Your existing rendering system

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class DXRenderingSystem {
private:
    // DirectX components
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    ID3D11DepthStencilView* depthStencilView = nullptr;
    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11PixelShader* pixelShader = nullptr;
    ID3D11InputLayout* inputLayout = nullptr;
    ID3D11Buffer* vertexBuffer = nullptr;
    ID3D11Buffer* indexBuffer = nullptr;
    ID3D11Buffer* constantBuffer = nullptr;
    ID3D11RasterizerState* rasterizerState = nullptr;

    // Window handle
    HWND hwnd;
    int width, height;

    // Shader structures
    struct Vertex {
        XMFLOAT3 position;
        XMFLOAT3 color;
    };

    struct ConstantBuffer {
        XMMATRIX worldViewProj;
    };

public:
    DXRenderingSystem(HWND window, int w, int h) : hwnd(window), width(w), height(h) {}
    ~DXRenderingSystem() { Cleanup(); }

    bool Initialize();
    void Cleanup();
    void RenderFrame(const std::vector<Mesh>& meshes, const Camera& camera);
    void Resize(int newWidth, int newHeight);

private:
    bool CompileShaders();
    bool CreateBuffers();
    void UpdateConstantBuffer(const Camera& camera);
    std::vector<Vertex> ConvertMeshToVertices(const Mesh& mesh);
};

#endif
