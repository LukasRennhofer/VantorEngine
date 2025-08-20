/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Core/Container/VCO_Vector.hpp>

#include <memory>
#include <string>
#include <cstdint>

namespace Vantor::RHI
{

// Forward declarations
class IRHIShader;
class IRHITexture;
class IRHIMesh;
class IRHIBuffer;
class IRHIRenderTarget;

enum class EGraphicsAPI {
    OPENGL
};

enum class ERHIFormat
{
    R8G8B8A8_UNORM,
    R8G8B8_UNORM,
    R32G32B32_FLOAT,
    R32G32_FLOAT,
    R32_FLOAT,
    D24_UNORM_S8_UINT
};

enum class ERHIBufferType
{
    Vertex,
    Index,
    Uniform,
    ShaderStorage
};

enum class ERHIShaderType
{
    Vertex,
    Fragment,
    Geometry,
    Compute
};

struct VVertexAttribute
{
    uint32_t location;
    ERHIFormat format;
    uint32_t offset;
};

struct VVertexLayout
{
    Vantor::Core::Container::TVector<VVertexAttribute> attributes;
    uint32_t stride;
};

class IRHIDevice
{
public:
    virtual ~IRHIDevice() = default;

    // Device management
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void Present() = 0;
    virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;

    // Resource creation
    virtual std::shared_ptr<IRHIShader> CreateShader(const std::string& vertexSource, const std::string& fragmentSource) = 0;
    virtual std::shared_ptr<IRHITexture> CreateTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data = nullptr) = 0;
    virtual std::shared_ptr<IRHIMesh> CreateMesh(const void* vertexData, uint32_t vertexSize, const void* indexData, uint32_t indexCount, const VVertexLayout& layout) = 0;
    virtual std::shared_ptr<IRHIBuffer> CreateBuffer(ERHIBufferType type, uint32_t size, const void* data = nullptr) = 0;
    virtual std::shared_ptr<IRHIRenderTarget> CreateRenderTarget(uint32_t width, uint32_t height, uint32_t samples = 1) = 0;

    // Rendering
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void BindShader(std::shared_ptr<IRHIShader> shader) = 0;
    virtual void BindTexture(std::shared_ptr<IRHITexture> texture, uint32_t slot = 0) = 0;
    virtual void BindRenderTarget(std::shared_ptr<IRHIRenderTarget> renderTarget) = 0;
    virtual void DrawMesh(std::shared_ptr<IRHIMesh> mesh) = 0;
};

} // namespace Vantor::RHI
