/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RHI/OpenGL/VRHI_OpenGLTexture.hpp>

#include <iostream>

namespace Vantor::RHI
{

OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data)
    : m_texture(0), m_width(width), m_height(height), m_currentSlot(0)
{
    // Convert RHI format to texture format
    switch (format)
    {
        case ERHIFormat::R8G8B8A8_UNORM: m_format = ETextureFormat::RGBA8; break;
        case ERHIFormat::R8G8B8_UNORM: m_format = ETextureFormat::RGB8; break;
        case ERHIFormat::R32G32B32_FLOAT: m_format = ETextureFormat::RGB32F; break;
        default: m_format = ETextureFormat::RGBA8; break;
    }
    
    CreateTexture(data);
}

OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, ETextureFormat format, const void* data)
    : m_texture(0), m_width(width), m_height(height), m_format(format), m_currentSlot(0)
{
    CreateTexture(data);
}

OpenGLTexture::~OpenGLTexture()
{
    if (m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);
    }
}

void OpenGLTexture::Bind(uint32_t slot)
{
    m_currentSlot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void OpenGLTexture::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_currentSlot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::UpdateData(const void* data, uint32_t width, uint32_t height)
{
    if (width != m_width || height != m_height)
    {
        m_width = width;
        m_height = height;
        
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGL(m_format), 
                     m_width, m_height, 0, GetPixelFormat(), GetPixelType(), data);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, 
                        GetPixelFormat(), GetPixelType(), data);
    }
}

void OpenGLTexture::SetFilter(ETextureFilter minFilter, ETextureFilter magFilter)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGL(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGL(magFilter));
}

void OpenGLTexture::SetWrap(ETextureWrap wrapS, ETextureWrap wrapT)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToGL(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToGL(wrapT));
}

void OpenGLTexture::CreateTexture(const void* data)
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    // Set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGL(m_format), 
                 m_width, m_height, 0, GetPixelFormat(), GetPixelType(), data);
    
    // Set default filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Generate mipmaps if data is provided
    if (data != nullptr)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLenum OpenGLTexture::GetPixelFormat() const
{
    switch (m_format)
    {
        case ETextureFormat::RGB8:
        case ETextureFormat::RGB16F:
        case ETextureFormat::RGB32F:
            return GL_RGB;
        case ETextureFormat::RGBA8:
        case ETextureFormat::RGBA16F:
        case ETextureFormat::RGBA32F:
            return GL_RGBA;
        case ETextureFormat::Depth24Stencil8:
            return GL_DEPTH_STENCIL;
        default:
            return GL_RGBA;
    }
}

GLenum OpenGLTexture::GetPixelType() const
{
    switch (m_format)
    {
        case ETextureFormat::RGB8:
        case ETextureFormat::RGBA8:
            return GL_UNSIGNED_BYTE;
        case ETextureFormat::RGB16F:
        case ETextureFormat::RGBA16F:
            return GL_HALF_FLOAT;
        case ETextureFormat::RGB32F:
        case ETextureFormat::RGBA32F:
            return GL_FLOAT;
        case ETextureFormat::Depth24Stencil8:
            return GL_UNSIGNED_INT_24_8;
        default:
            return GL_UNSIGNED_BYTE;
    }
}

GLenum OpenGLTexture::TextureFormatToGL(ETextureFormat format)
{
    switch (format)
    {
        case ETextureFormat::RGB8: return GL_RGB8;
        case ETextureFormat::RGBA8: return GL_RGBA8;
        case ETextureFormat::RGB16F: return GL_RGB16F;
        case ETextureFormat::RGBA16F: return GL_RGBA16F;
        case ETextureFormat::RGB32F: return GL_RGB32F;
        case ETextureFormat::RGBA32F: return GL_RGBA32F;
        case ETextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
        default: return GL_RGBA8;
    }
}

GLenum OpenGLTexture::RHIFormatToTextureFormat(ERHIFormat format)
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

GLenum OpenGLTexture::TextureFilterToGL(ETextureFilter filter)
{
    switch (filter)
    {
        case ETextureFilter::Nearest: return GL_NEAREST;
        case ETextureFilter::Linear: return GL_LINEAR;
        default: return GL_LINEAR;
    }
}

GLenum OpenGLTexture::TextureWrapToGL(ETextureWrap wrap)
{
    switch (wrap)
    {
        case ETextureWrap::Repeat: return GL_REPEAT;
        case ETextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case ETextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
        default: return GL_REPEAT;
    }
}

} // namespace Vantor::RHI
