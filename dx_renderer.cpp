// dx_renderer.cpp
#include "dx_renderer.h"

bool DXRenderingSystem::Initialize() {
    // Create device and swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc,
        &swapChain, &device, nullptr, &deviceContext
    );

    if (FAILED(hr)) return false;

    // Create render target view
    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    // Create depth stencil buffer
    ID3D11Texture2D* depthStencilBuffer;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    device->CreateTexture2D(&depthDesc, nullptr, &depthStencilBuffer);
    device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);
    depthStencilBuffer->Release();

    // Set render targets
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    // Set viewport
    D3D11_VIEWPORT viewport = {};
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &viewport);

    // Create rasterizer state
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.DepthClipEnable = true;
    
    device->CreateRasterizerState(&rasterDesc, &rasterizerState);
    deviceContext->RSSetState(rasterizerState);

    // Compile shaders and create buffers
    return CompileShaders() && CreateBuffers();
}

bool DXRenderingSystem::CompileShaders() {
    // Simple vertex shader
    const char* vsSource = R"(
        cbuffer ConstantBuffer : register(b0) {
            float4x4 worldViewProj;
        }

        struct VS_IN {
            float3 pos : POSITION;
            float3 color : COLOR;
        };

        struct VS_OUT {
            float4 pos : SV_POSITION;
            float3 color : COLOR;
        };

        VS_OUT main(VS_IN input) {
            VS_OUT output;
            output.pos = mul(float4(input.pos, 1.0f), worldViewProj);
            output.color = input.color;
            return output;
        }
    )";

    // Simple pixel shader
    const char* psSource = R"(
        struct PS_IN {
            float4 pos : SV_POSITION;
            float3 color : COLOR;
        };

        float4 main(PS_IN input) : SV_TARGET {
            return float4(input.color, 1.0f);
        }
    )";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    // Compile vertex shader
    HRESULT hr = D3DCompile(vsSource, strlen(vsSource), nullptr, nullptr, nullptr,
                           "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return false;
    }

    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
                              nullptr, &vertexShader);

    // Compile pixel shader
    hr = D3DCompile(psSource, strlen(psSource), nullptr, nullptr, nullptr,
                   "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return false;
    }

    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
                             nullptr, &pixelShader);

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(),
                             vsBlob->GetBufferSize(), &inputLayout);

    vsBlob->Release();
    psBlob->Release();

    return true;
}

bool DXRenderingSystem::CreateBuffers() {
    // Create constant buffer
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = device->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
    return SUCCEEDED(hr);
}

void DXRenderingSystem::UpdateConstantBuffer(const Camera& camera) {
    // Calculate matrices
    XMMATRIX view = XMMatrixLookAtLH(
        XMVectorSet(camera.position.x, camera.position.y, camera.position.z, 0.0f),
        XMVectorSet(camera.position.x + camera.forward.x, 
                   camera.position.y + camera.forward.y, 
                   camera.position.z + camera.forward.z, 0.0f),
        XMVectorSet(camera.up.x, camera.up.y, camera.up.z, 0.0f)
    );

    XMMATRIX projection = XMMatrixPerspectiveFovLH(
        2.0f * atan(camera.tanHalfFovY), // FOV Y
        (float)width / height,            // Aspect ratio
        camera.nearZ, camera.farZ         // Near/far planes
    );

    XMMATRIX worldViewProj = XMMatrixIdentity() * view * projection;

    // Update constant buffer
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    
    ConstantBuffer* constants = (ConstantBuffer*)mappedResource.pData;
    constants->worldViewProj = XMMatrixTranspose(worldViewProj); // HLSL uses column-major
    
    deviceContext->Unmap(constantBuffer, 0);
}

std::vector<DXRenderingSystem::Vertex> DXRenderingSystem::ConvertMeshToVertices(const Mesh& mesh) {
    std::vector<Vertex> vertices;
    
    for (const auto& triangle : mesh.triangles) {
        // Convert each vertex in the triangle
        vertices.push_back({
            XMFLOAT3(triangle.v0.position.x, triangle.v0.position.y, triangle.v0.position.z),
            XMFLOAT3(triangle.v0.color.r / 255.0f, triangle.v0.color.g / 255.0f, triangle.v0.color.b / 255.0f)
        });
        vertices.push_back({
            XMFLOAT3(triangle.v1.position.x, triangle.v1.position.y, triangle.v1.position.z),
            XMFLOAT3(triangle.v1.color.r / 255.0f, triangle.v1.color.g / 255.0f, triangle.v1.color.b / 255.0f)
        });
        vertices.push_back({
            XMFLOAT3(triangle.v2.position.x, triangle.v2.position.y, triangle.v2.position.z),
            XMFLOAT3(triangle.v2.color.r / 255.0f, triangle.v2.color.g / 255.0f, triangle.v2.color.b / 255.0f)
        });
    }
    
    return vertices;
}

void DXRenderingSystem::RenderFrame(const std::vector<Mesh>& meshes, const Camera& camera) {
    // Clear render target
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Set shaders and input layout
    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);
    deviceContext->IASetInputLayout(inputLayout);

    // Update constant buffer
    UpdateConstantBuffer(camera);
    deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

    // Render each mesh
    for (const auto& mesh : meshes) {
        auto vertices = ConvertMeshToVertices(mesh);
        
        if (!vertices.empty()) {
            // Create vertex buffer for this mesh
            D3D11_BUFFER_DESC vbDesc = {};
            vbDesc.ByteWidth = sizeof(Vertex) * vertices.size();
            vbDesc.Usage = D3D11_USAGE_DEFAULT;
            vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            
            D3D11_SUBRESOURCE_DATA vbData = {};
            vbData.pSysMem = vertices.data();
            
            ID3D11Buffer* meshVertexBuffer;
            device->CreateBuffer(&vbDesc, &vbData, &meshVertexBuffer);

            // Set vertex buffer
            UINT stride = sizeof(Vertex);
            UINT offset = 0;
            deviceContext->IASetVertexBuffers(0, 1, &meshVertexBuffer, &stride, &offset);
            deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            // Draw
            deviceContext->Draw(vertices.size(), 0);

            // Clean up
            meshVertexBuffer->Release();
        }
    }

    // Present
    swapChain->Present(1, 0); // VSync enabled
}

void DXRenderingSystem::Resize(int newWidth, int newHeight) {
    if (swapChain) {
        width = newWidth;
        height = newHeight;
        
        // Release old views
        if (renderTargetView) renderTargetView->Release();
        if (depthStencilView) depthStencilView->Release();
        
        // Resize swap chain
        swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        
        // Recreate render target view
        ID3D11Texture2D* backBuffer;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
        backBuffer->Release();
        
        // Recreate depth stencil view
        ID3D11Texture2D* depthBuffer;
        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = width;
        depthDesc.Height = height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.SampleDesc.Quality = 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        
        device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
        device->CreateDepthStencilView(depthBuffer, nullptr, &depthStencilView);
        depthBuffer->Release();
        
        // Reset render targets
        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
        
        // Update viewport
        D3D11_VIEWPORT viewport = {};
        viewport.Width = (float)width;
        viewport.Height = (float)height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        deviceContext->RSSetViewports(1, &viewport);
    }
}

void DXRenderingSystem::Cleanup() {
    if (rasterizerState) rasterizerState->Release();
    if (constantBuffer) constantBuffer->Release();
    if (inputLayout) inputLayout->Release();
    if (vertexShader) vertexShader->Release();
    if (pixelShader) pixelShader->Release();
    if (depthStencilView) depthStencilView->Release();
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
}
