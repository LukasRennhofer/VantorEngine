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

namespace Vantor::RHI
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
    // This would typically be handled by the window/context manager
    // For now, we just flush the OpenGL commands
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

std::shared_ptr<IRHITexture> OpenGLDevice::CreateTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data)
{
    return std::make_shared<OpenGLTexture>(width, height, format, data);
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

GLenum OpenGLDevice::RHIFormatToGL(ERHIFormat format)
{
    switch (format)
    {
        case ERHIFormat::R8G8B8A8_UNORM: return GL_RGBA8;
        case ERHIFormat::R8G8B8_UNORM: return GL_RGB8;
        case ERHIFormat::R32G32B32_FLOAT: return GL_RGB32F;
        case ERHIFormat::R32G32_FLOAT: return GL_RG32F;
        case ERHIFormat::R32_FLOAT: return GL_R32F;
        case ERHIFormat::D24_UNORM_S8_UINT: return GL_DEPTH24_STENCIL8;
        default: return GL_RGBA8;
    }
}

GLenum OpenGLDevice::BufferTypeToGL(ERHIBufferType type)
{
    switch (type)
    {
        case ERHIBufferType::Vertex: return GL_ARRAY_BUFFER;
        case ERHIBufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
        case ERHIBufferType::Uniform: return GL_UNIFORM_BUFFER;
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

} // namespace Lab
