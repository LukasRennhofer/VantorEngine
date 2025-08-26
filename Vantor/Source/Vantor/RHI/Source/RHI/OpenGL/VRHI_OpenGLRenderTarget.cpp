/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RHI/OpenGL/VRHI_OpenGLRenderTarget.hpp>
#include <RHI/OpenGL/VRHI_OpenGLTexture.hpp>

#include <iostream>
#include <cassert>
#include <Shared/glad/glad.h>

namespace VE::Internal::RHI
{

uint32_t OpenGLRenderTarget::s_boundFramebuffer = 0;

OpenGLRenderTarget::OpenGLRenderTarget(uint32_t width, uint32_t height, uint32_t samples)
    : m_width(width)
    , m_height(height)
    , m_samples(samples)
    , m_colorAttachmentCount(0)
{
    glGenFramebuffers(1, &m_framebuffer);
    assert(m_framebuffer != 0);
}

OpenGLRenderTarget::~OpenGLRenderTarget()
{
    if (m_framebuffer != 0)
    {
        if (s_boundFramebuffer == m_framebuffer)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            s_boundFramebuffer = 0;
        }
        glDeleteFramebuffers(1, &m_framebuffer);
    }
}

void OpenGLRenderTarget::Bind()
{
    if (s_boundFramebuffer != m_framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        s_boundFramebuffer = m_framebuffer;
        
        // Set draw buffers if we have color attachments
        if (!m_drawBuffers.empty())
        {
            glDrawBuffers(static_cast<GLsizei>(m_drawBuffers.size()), m_drawBuffers.data());
        }
        else
        {
            // No color attachments, disable color drawing
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
    }
}

void OpenGLRenderTarget::Unbind()
{
    if (s_boundFramebuffer != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        s_boundFramebuffer = 0;
    }
}

bool OpenGLRenderTarget::IsComplete() const
{
    GLuint previousFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&previousFramebuffer));
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
    
    return status == GL_FRAMEBUFFER_COMPLETE;
}

void OpenGLRenderTarget::AttachTexture(EAttachmentType type,
                                       std::shared_ptr<IRHITexture> texture,
                                       uint32_t mipLevel,
                                       uint32_t layer)
{
    if (!texture)
    {
        DetachTexture(type);
        return;
    }

    Bind();

    auto openglTexture = std::static_pointer_cast<OpenGLTexture>(texture);
    if (!openglTexture)
    {
        std::cerr << "Error: Texture is not an OpenGL texture" << std::endl;
        return;
    }

    GLenum attachment = 0;

    if (type == EAttachmentType::Color)
    {
        uint32_t colorIndex = m_colorAttachmentCount;
        attachment = GL_COLOR_ATTACHMENT0 + colorIndex;

        switch (openglTexture->GetType())
        {
            case ETextureType::Texture2D:
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, openglTexture->GetHandle(), mipLevel);
                break;

            case ETextureType::Texture3D:
            case ETextureType::TextureCube:
                glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, openglTexture->GetHandle(), mipLevel, layer);
                break;

            case ETextureType::Texture1D:
                glFramebufferTexture1D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_1D, openglTexture->GetHandle(), mipLevel);
                break;
        }

        if (colorIndex >= m_colorAttachments.size())
            m_colorAttachments.resize(colorIndex + 1);
        m_colorAttachments[colorIndex] = {type, texture, mipLevel, layer};

        ++m_colorAttachmentCount;
        UpdateDrawBuffers();
    }
    else if (type == EAttachmentType::Depth)
    {
        attachment = GL_DEPTH_ATTACHMENT;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, openglTexture->GetHandle(), mipLevel);
    }
    else if (type == EAttachmentType::DepthStencil)
    {
        attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, openglTexture->GetHandle(), mipLevel);
    }

    VRenderTargetAttachment info;
    info.type = type;
    info.texture = texture;
    info.mipLevel = mipLevel;
    info.layer = layer;
    m_attachments[type] = info;

    CheckFramebufferStatus();
}

void OpenGLRenderTarget::DetachTexture(EAttachmentType type)
{
    auto it = m_attachments.find(type);
    if (it == m_attachments.end())
        return;

    Bind();

    if (type == EAttachmentType::Color)
    {
        // Find and clear the correct color attachment slot
        for (uint32_t i = 0; i < m_colorAttachments.size(); ++i)
        {
            if (m_colorAttachments[i].type == type && m_colorAttachments[i].texture)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
                m_colorAttachments[i].texture = nullptr;
            }
        }
        // Recount color attachments
        m_colorAttachmentCount = 0;
        for (const auto& att : m_colorAttachments)
            if (att.texture) ++m_colorAttachmentCount;
    }
    else if (type == EAttachmentType::Depth)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
    else if (type == EAttachmentType::DepthStencil)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

    m_attachments.erase(it);
    UpdateDrawBuffers();
}

std::shared_ptr<IRHITexture> OpenGLRenderTarget::GetAttachment(EAttachmentType type) const
{
    auto it = m_attachments.find(type);
    if (it != m_attachments.end())
    {
        return it->second.texture;
    }
    return nullptr;
}

void OpenGLRenderTarget::Clear(float r, float g, float b, float a)
{
    Bind();
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderTarget::ClearDepth(float depth)
{
    Bind();
    glClearDepth(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderTarget::ClearStencil(int32_t stencil)
{
    Bind();
    glClearStencil(stencil);
    glClear(GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderTarget::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    Bind();
    glViewport(x, y, width, height);
}

GLenum OpenGLRenderTarget::AttachmentTypeToGL(EAttachmentType type)
{
    switch (type)
    {
        case EAttachmentType::Color:        return GL_COLOR_ATTACHMENT0;
        case EAttachmentType::Depth:        return GL_DEPTH_ATTACHMENT;
        case EAttachmentType::Stencil:      return GL_STENCIL_ATTACHMENT;
        case EAttachmentType::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
        default:                           return GL_COLOR_ATTACHMENT0;
    }
}

GLenum OpenGLRenderTarget::GetColorAttachment(uint32_t index)
{
    return GL_COLOR_ATTACHMENT0 + index;
}

void OpenGLRenderTarget::CheckFramebufferStatus() const
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer not complete! Status: 0x" << std::hex << status << std::endl;
        switch (status)
        {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
                break;
            default:
                std::cerr << "Unknown framebuffer error" << std::endl;
                break;
        }
    }
}

void OpenGLRenderTarget::UpdateDrawBuffers()
{
    m_drawBuffers.clear();
    for (uint32_t i = 0; i < m_colorAttachments.size(); ++i) {
        if (m_colorAttachments[i].texture) {
            m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
    }

    if (!m_drawBuffers.empty()) {
        glDrawBuffers(static_cast<GLsizei>(m_drawBuffers.size()), m_drawBuffers.data());
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
}


} // namespace VE::Internal::RHI
