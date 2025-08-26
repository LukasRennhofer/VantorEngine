/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RHI/OpenGL/VRHI_OpenGLDevice.hpp>
#include <RHI/OpenGL/VRHI_OpenGLShader.hpp>
#include <RHI/OpenGL/VRHI_OpenGLTexture.hpp>
#include <RHI/OpenGL/VRHI_OpenGLMesh.hpp>
#include <RHI/OpenGL/VRHI_OpenGLBuffer.hpp>
#include <RHI/OpenGL/VRHI_OpenGLRenderTarget.hpp>

#include <iostream>

namespace VE::Internal::RHI
{

OpenGLDevice::OpenGLDevice()
    : m_initialized(false)
{
}

OpenGLDevice::~OpenGLDevice()
{
    if (m_initialized)
    {
        Shutdown();
    }
}

bool OpenGLDevice::Initialize()
{
    if (m_initialized)
        return true;

    // Initialize GLAD
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Graphics Card: " << glGetString(GL_RENDERER) << std::endl;

    SetupDefaultState();
    m_initialized = true;
    return true;
}

void OpenGLDevice::Shutdown()
{
    if (!m_initialized)
        return;

    m_currentShader.reset();
    m_currentRenderTarget.reset();
    m_boundTextures.clear();
    m_initialized = false;
}

void OpenGLDevice::Present()
{
    glFlush();
}

void OpenGLDevice::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::shared_ptr<IRHIShader> OpenGLDevice::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
    return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
}

std::shared_ptr<IRHITexture> OpenGLDevice::CreateTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data, ETextureType type, uint32_t depth)
{
    return std::make_shared<OpenGLTexture>(width, height, format, data, type, depth);
}

std::shared_ptr<IRHIMesh> OpenGLDevice::CreateMesh(const void* vertexData, uint32_t vertexSize, const void* indexData, uint32_t indexCount, const VVertexLayout& layout)
{
    return std::make_shared<OpenGLMesh>(vertexData, vertexSize, indexData, indexCount, layout);
}

std::shared_ptr<IRHIBuffer> OpenGLDevice::CreateBuffer(ERHIBufferType type, uint32_t size, const void* data)
{
    return std::make_shared<OpenGLBuffer>(type, size, data);
}

std::shared_ptr<IRHIRenderTarget> OpenGLDevice::CreateRenderTarget(uint32_t width, uint32_t height, uint32_t samples)
{
    return std::make_shared<OpenGLRenderTarget>(width, height, samples);
}

void OpenGLDevice::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLDevice::BindShader(std::shared_ptr<IRHIShader> shader)
{
    if (shader != m_currentShader)
    {
        m_currentShader = shader;
        if (shader)
        {
            shader->Use();
        }
    }
}

void OpenGLDevice::BindTexture(std::shared_ptr<IRHITexture> texture, uint32_t slot)
{
    auto it = m_boundTextures.find(slot);
    if (it == m_boundTextures.end() || it->second != texture)
    {
        m_boundTextures[slot] = texture;
        if (texture)
        {
            texture->Bind(slot);
        }
    }
}

void OpenGLDevice::BindRenderTarget(std::shared_ptr<IRHIRenderTarget> renderTarget)
{
    if (renderTarget != m_currentRenderTarget)
    {
        m_currentRenderTarget = renderTarget;
        if (renderTarget)
        {
            renderTarget->Bind();
        }
        else
        {
            // Bind default framebuffer (backbuffer)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
}

void OpenGLDevice::DrawMesh(std::shared_ptr<IRHIMesh> mesh)
{
    if (mesh)
    {
        mesh->Bind();
        mesh->Draw();
    }
}

// TODO: Only Use OpenGLTexture Function in here!
GLenum OpenGLDevice::RHIFormatToGL(ERHIFormat format)
{
    // Use the same mapping as OpenGLTexture::RHIFormatToTextureFormat
    switch (format)
    {
        case ERHIFormat::R8_UNORM: return GL_R8;
        case ERHIFormat::R8_SNORM: return GL_R8_SNORM;
        case ERHIFormat::R8_UINT: return GL_R8UI;
        case ERHIFormat::R8_SINT: return GL_R8I;
        case ERHIFormat::R8G8_UNORM: return GL_RG8;
        case ERHIFormat::R8G8_SNORM: return GL_RG8_SNORM;
        case ERHIFormat::R8G8_UINT: return GL_RG8UI;
        case ERHIFormat::R8G8_SINT: return GL_RG8I;
        case ERHIFormat::R8G8B8_UNORM: return GL_RGB8;
        case ERHIFormat::R8G8B8_SNORM: return GL_RGB8_SNORM;
        case ERHIFormat::R8G8B8_UINT: return GL_RGB8UI;
        case ERHIFormat::R8G8B8_SINT: return GL_RGB8I;
        case ERHIFormat::R8G8B8A8_UNORM: return GL_RGBA8;
        case ERHIFormat::R8G8B8A8_SNORM: return GL_RGBA8_SNORM;
        case ERHIFormat::R8G8B8A8_UINT: return GL_RGBA8UI;
        case ERHIFormat::R8G8B8A8_SINT: return GL_RGBA8I;
        case ERHIFormat::R8G8B8A8_SRGB: return GL_SRGB8_ALPHA8;

        case ERHIFormat::R16_UNORM: return GL_R16;
        case ERHIFormat::R16_SNORM: return GL_R16_SNORM;
        case ERHIFormat::R16_UINT: return GL_R16UI;
        case ERHIFormat::R16_SINT: return GL_R16I;
        case ERHIFormat::R16_FLOAT: return GL_R16F;
        case ERHIFormat::R16G16_UNORM: return GL_RG16;
        case ERHIFormat::R16G16_SNORM: return GL_RG16_SNORM;
        case ERHIFormat::R16G16_UINT: return GL_RG16UI;
        case ERHIFormat::R16G16_SINT: return GL_RG16I;
        case ERHIFormat::R16G16_FLOAT: return GL_RG16F;
        case ERHIFormat::R16G16B16A16_UNORM: return GL_RGBA16;
        case ERHIFormat::R16G16B16A16_SNORM: return GL_RGBA16_SNORM;
        case ERHIFormat::R16G16B16A16_UINT: return GL_RGBA16UI;
        case ERHIFormat::R16G16B16A16_SINT: return GL_RGBA16I;
        case ERHIFormat::R16G16B16A16_FLOAT: return GL_RGBA16F;

        case ERHIFormat::R32_UINT: return GL_R32UI;
        case ERHIFormat::R32_SINT: return GL_R32I;
        case ERHIFormat::R32_FLOAT: return GL_R32F;
        case ERHIFormat::R32G32_UINT: return GL_RG32UI;
        case ERHIFormat::R32G32_SINT: return GL_RG32I;
        case ERHIFormat::R32G32_FLOAT: return GL_RG32F;
        case ERHIFormat::R32G32B32_UINT: return GL_RGB32UI;
        case ERHIFormat::R32G32B32_SINT: return GL_RGB32I;
        case ERHIFormat::R32G32B32_FLOAT: return GL_RGB32F;
        case ERHIFormat::R32G32B32A32_UINT: return GL_RGBA32UI;
        case ERHIFormat::R32G32B32A32_SINT: return GL_RGBA32I;
        case ERHIFormat::R32G32B32A32_FLOAT: return GL_RGBA32F;

        case ERHIFormat::B8G8R8A8_UNORM: return GL_RGBA8;
        case ERHIFormat::B8G8R8A8_SRGB: return GL_SRGB8_ALPHA8;

        case ERHIFormat::D16_UNORM: return GL_DEPTH_COMPONENT16;
        case ERHIFormat::D24_UNORM_S8_UINT: return GL_DEPTH24_STENCIL8;
        case ERHIFormat::D32_FLOAT: return GL_DEPTH_COMPONENT32F;
        case ERHIFormat::D32_FLOAT_S8X24_UINT: return GL_DEPTH32F_STENCIL8;
        case ERHIFormat::S8_UINT: return GL_STENCIL_INDEX8;

        case ERHIFormat::R8_SRGB: return GL_SRGB8;
        case ERHIFormat::R8G8_SRGB: return GL_SRGB8;
        case ERHIFormat::R8G8B8_SRGB: return GL_SRGB8;

        // Typeless and compressed formats are not directly supported in OpenGL, return a default or error
        case ERHIFormat::R8_TYPELESS:
        case ERHIFormat::R8G8_TYPELESS:
        case ERHIFormat::R8G8B8_TYPELESS:
        case ERHIFormat::R8G8B8A8_TYPELESS:
        case ERHIFormat::R16_TYPELESS:
        case ERHIFormat::R16G16_TYPELESS:
        case ERHIFormat::R16G16B16A16_TYPELESS:
        case ERHIFormat::R32_TYPELESS:
        case ERHIFormat::R32G32_TYPELESS:
        case ERHIFormat::R32G32B32_TYPELESS:
        case ERHIFormat::R32G32B32A32_TYPELESS:
        case ERHIFormat::D24_UNORM_S8_UINT_TYPELESS:
        case ERHIFormat::D32_FLOAT_TYPELESS:
        case ERHIFormat::D32_FLOAT_S8X24_UINT_TYPELESS:
            return GL_NONE; // Not supported

        // Compressed formats (BCn) are not supported in core OpenGL, require extensions
        case ERHIFormat::BC1_UNORM:
        case ERHIFormat::BC1_SRGB:
        case ERHIFormat::BC2_UNORM:
        case ERHIFormat::BC2_SRGB:
        case ERHIFormat::BC3_UNORM:
        case ERHIFormat::BC3_SRGB:
        case ERHIFormat::BC4_UNORM:
        case ERHIFormat::BC4_SNORM:
        case ERHIFormat::BC5_UNORM:
        case ERHIFormat::BC5_SNORM:
        case ERHIFormat::BC6H_UF16:
        case ERHIFormat::BC6H_SF16:
        case ERHIFormat::BC7_UNORM:
        case ERHIFormat::BC7_SRGB:
            return GL_NONE; // Not supported in core

        default:
            return GL_RGBA8;
    }
}

GLenum OpenGLDevice::BufferTypeToGL(ERHIBufferType type)
{
    switch (type)
    {
        case ERHIBufferType::Vertex: return GL_ARRAY_BUFFER;
        case ERHIBufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
        case ERHIBufferType::Uniform: return GL_UNIFORM_BUFFER;
        case ERHIBufferType::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
        default: return GL_ARRAY_BUFFER;
    }
}

void OpenGLDevice::SetupDefaultState()
{
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

} // namespace VE::Internal::RHI
