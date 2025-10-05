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

namespace VE::Internal::RHI
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
    // 8-bit formats
    R8_UNORM,
    R8_SNORM,
    R8_UINT,
    R8_SINT,
    R8G8_UNORM,
    R8G8_SNORM,
    R8G8_UINT,
    R8G8_SINT,
    R8G8B8_UNORM,
    R8G8B8_SNORM,
    R8G8B8_UINT,
    R8G8B8_SINT,
    R8G8B8A8_UNORM,
    R8G8B8A8_SNORM,
    R8G8B8A8_UINT,
    R8G8B8A8_SINT,
    R8G8B8A8_SRGB,

    // 16-bit formats
    R16_UNORM,
    R16_SNORM,
    R16_UINT,
    R16_SINT,
    R16_FLOAT,
    R16G16_UNORM,
    R16G16_SNORM,
    R16G16_UINT,
    R16G16_SINT,
    R16G16_FLOAT,
    R16G16B16A16_UNORM,
    R16G16B16A16_SNORM,
    R16G16B16A16_UINT,
    R16G16B16A16_SINT,
    R16G16B16A16_FLOAT,

    // 32-bit formats
    R32_UINT,
    R32_SINT,
    R32_FLOAT,
    R32G32_UINT,
    R32G32_SINT,
    R32G32_FLOAT,
    R32G32B32_UINT,
    R32G32B32_SINT,
    R32G32B32_FLOAT,
    R32G32B32A32_UINT,
    R32G32B32A32_SINT,
    R32G32B32A32_FLOAT,

    // Packed formats
    B8G8R8A8_UNORM,
    B8G8R8A8_SRGB,

    // Depth/Stencil
    D16_UNORM,
    D24_UNORM_S8_UINT,
    D32_FLOAT,
    D32_FLOAT_S8X24_UINT,
    S8_UINT,

    // sRGB
    R8_SRGB,
    R8G8_SRGB,
    R8G8B8_SRGB,

    // Typeless (for resource views)
    R8_TYPELESS,
    R8G8_TYPELESS,
    R8G8B8_TYPELESS,
    R8G8B8A8_TYPELESS,
    R16_TYPELESS,
    R16G16_TYPELESS,
    R16G16B16A16_TYPELESS,
    R32_TYPELESS,
    R32G32_TYPELESS,
    R32G32B32_TYPELESS,
    R32G32B32A32_TYPELESS,
    D24_UNORM_S8_UINT_TYPELESS,
    D32_FLOAT_TYPELESS,
    D32_FLOAT_S8X24_UINT_TYPELESS,

    // Compressed (examples)
    BC1_UNORM,
    BC1_SRGB,
    BC2_UNORM,
    BC2_SRGB,
    BC3_UNORM,
    BC3_SRGB,
    BC4_UNORM,
    BC4_SNORM,
    BC5_UNORM,
    BC5_SNORM,
    BC6H_UF16,
    BC6H_SF16,
    BC7_UNORM,
    BC7_SRGB
};

enum class ERHIBufferType
{
    Vertex,
    Index,
    Uniform,
    ShaderStorage
};

enum class ETextureType
{
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube
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
    VE::Internal::Core::Container::TVector<VVertexAttribute> attributes;
    uint32_t stride;
};

struct VRenderCommand {
    IRHIMesh* mesh;
    // TODO: Add Material and Transform
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
    virtual std::shared_ptr<IRHITexture> CreateTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data = nullptr, ETextureType type = ETextureType::Texture2D, uint32_t depth = 1) = 0;
    virtual std::shared_ptr<IRHIMesh> CreateMesh(const void* vertexData, uint32_t vertexSize, const void* indexData, uint32_t indexCount, const VVertexLayout& layout) = 0;
    virtual std::shared_ptr<IRHIBuffer> CreateBuffer(ERHIBufferType type, uint32_t size, const void* data = nullptr) = 0;
    virtual std::shared_ptr<IRHIRenderTarget> CreateRenderTarget(uint32_t width, uint32_t height, uint32_t samples = 1) = 0;

    // Rendering
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    // virtual void BindShader(std::shared_ptr<IRHIShader> shader) = 0;
    // virtual void BindTexture(std::shared_ptr<IRHITexture> texture, uint32_t slot = 0) = 0;
    // virtual void BindRenderTarget(std::shared_ptr<IRHIRenderTarget> renderTarget) = 0;
    // virtual void DrawMesh(std::shared_ptr<IRHIMesh> mesh) = 0;
};

} // namespace VE::Internal::RHI
