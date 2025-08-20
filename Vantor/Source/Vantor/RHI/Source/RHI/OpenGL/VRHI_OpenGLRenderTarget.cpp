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

namespace Vantor::RHI
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

void OpenGLRenderTarget::AttachTexture(EAttachmentType type, std::shared_ptr<IRHITexture> texture, uint32_t mipLevel, uint32_t layer)
{
    if (!texture)
    {
        DetachTexture(type);
        return;
    }

    Bind();
    
    // Cast to OpenGL texture to get the handle
    auto openglTexture = std::static_pointer_cast<OpenGLTexture>(texture);
    if (!openglTexture)
    {
        std::cerr << "Error: Texture is not an OpenGL texture" << std::endl;
        return;
    }

    GLenum attachment = AttachmentTypeToGL(type);
    
    // Handle color attachments separately to manage multiple render targets
    if (type == EAttachmentType::Color)
    {
        attachment = GetColorAttachment(m_colorAttachmentCount);
        m_colorAttachmentCount++;
    }
    
    // Attach the texture
    if (layer > 0)
    {
        // For array textures or cubemaps
        glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, openglTexture->GetHandle(), mipLevel, layer);
    }
    else
    {
        // For regular 2D textures
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, openglTexture->GetHandle(), mipLevel);
    }
    
    // Store attachment info
    VRenderTargetAttachment attachmentInfo;
    attachmentInfo.type = type;
    attachmentInfo.texture = texture;
    attachmentInfo.mipLevel = mipLevel;
    attachmentInfo.layer = layer;
    m_attachments[type] = attachmentInfo;
    
    UpdateDrawBuffers();
    CheckFramebufferStatus();
}

void OpenGLRenderTarget::DetachTexture(EAttachmentType type)
{
    auto it = m_attachments.find(type);
    if (it == m_attachments.end())
        return;
    
    Bind();
    
    GLenum attachment = AttachmentTypeToGL(type);
    if (type == EAttachmentType::Color)
    {
        // Find which color attachment this was
        for (uint32_t i = 0; i < m_colorAttachmentCount; ++i)
        {
            attachment = GetColorAttachment(i);
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
        }
        m_colorAttachmentCount = 0;
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
    }
    
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
        std::cerr << "Framebuffer not complete! Status: " << std::hex << status << std::endl;
        
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
    
    // Add all color attachments to draw buffers
    for (const auto& [type, attachment] : m_attachments)
    {
        if (type == EAttachmentType::Color)
        {
            m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(m_drawBuffers.size()));
        }
    }
}

} // namespace Vantor::RHI
