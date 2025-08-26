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

namespace VE::Internal::RHI
{

// Helper to map texture type to GL target
inline GLenum GetGLTarget(ETextureType type) {
    switch (type) {
        case ETextureType::Texture1D: return GL_TEXTURE_1D;
        case ETextureType::Texture2D: return GL_TEXTURE_2D;
        case ETextureType::Texture3D: return GL_TEXTURE_3D;
        case ETextureType::TextureCube: return GL_TEXTURE_CUBE_MAP;
        default: return GL_TEXTURE_2D;
    }
}

OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, ERHIFormat format,
                             const void* data, ETextureType type, uint32_t depth)
    : m_texture(0), m_width(width), m_height(height), m_depth(depth), m_format(format), m_type(type), m_currentSlot(0)
{
    CreateTexture(data);
}

OpenGLTexture::~OpenGLTexture()
{
    if (m_texture != 0)
        glDeleteTextures(1, &m_texture);
}

void OpenGLTexture::Bind(uint32_t slot)
{
    m_currentSlot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GetGLTarget(m_type), m_texture);
}

void OpenGLTexture::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_currentSlot);
    glBindTexture(GetGLTarget(m_type), 0);
}

void OpenGLTexture::SetFilter(ETextureFilter minFilter, ETextureFilter magFilter)
{
    glBindTexture(GetGLTarget(m_type), m_texture);
    glTexParameteri(GetGLTarget(m_type), GL_TEXTURE_MIN_FILTER, TextureFilterToGL(minFilter));
    glTexParameteri(GetGLTarget(m_type), GL_TEXTURE_MAG_FILTER, TextureFilterToGL(magFilter));
}

void OpenGLTexture::SetWrap(ETextureWrap wrapS, ETextureWrap wrapT, ETextureWrap wrapR)
{
    glBindTexture(GetGLTarget(m_type), m_texture);
    glTexParameteri(GetGLTarget(m_type), GL_TEXTURE_WRAP_S, TextureWrapToGL(wrapS));
    glTexParameteri(GetGLTarget(m_type), GL_TEXTURE_WRAP_T, TextureWrapToGL(wrapT));
    if (m_type == ETextureType::Texture3D)
        glTexParameteri(GetGLTarget(m_type), GL_TEXTURE_WRAP_R, TextureWrapToGL(wrapR));
}

void OpenGLTexture::UpdateData(const void* data, uint32_t width, uint32_t height, uint32_t depth, uint32_t face)
{
    m_width = width;
    m_height = height;
    m_depth = depth;

    glBindTexture(GetGLTarget(m_type), m_texture);

    switch (m_type) {
        case ETextureType::Texture1D:
            glTexImage1D(GL_TEXTURE_1D, 0, RHIFormatToTextureFormat(m_format), m_width, 0, GetPixelFormat(), GetPixelType(), data);
            break;
        case ETextureType::Texture2D:
            glTexImage2D(GL_TEXTURE_2D, 0, RHIFormatToTextureFormat(m_format), m_width, m_height, 0, GetPixelFormat(), GetPixelType(), data);
            break;
        case ETextureType::Texture3D:
            glTexImage3D(GL_TEXTURE_3D, 0, RHIFormatToTextureFormat(m_format), m_width, m_height, m_depth, 0, GetPixelFormat(), GetPixelType(), data);
            break;
        case ETextureType::TextureCube:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, RHIFormatToTextureFormat(m_format),
                         m_width, m_height, 0, GetPixelFormat(), GetPixelType(), data);
            break;
    }
}

void OpenGLTexture::Resize(uint32_t newWidth, uint32_t newHeight, uint32_t newDepth)
{
    m_width  = newWidth;
    m_height = newHeight;
    m_depth  = newDepth;

    glBindTexture(GetGLTarget(m_type), m_texture);

    switch (m_type)
    {
        case ETextureType::Texture1D:
            glTexImage1D(GL_TEXTURE_1D, 0, RHIFormatToTextureFormat(m_format),
                         m_width, 0, GetPixelFormat(), GetPixelType(), nullptr);
            break;

        case ETextureType::Texture2D:
            glTexImage2D(GL_TEXTURE_2D, 0, RHIFormatToTextureFormat(m_format),
                         m_width, m_height, 0, GetPixelFormat(), GetPixelType(), nullptr);
            break;

        case ETextureType::Texture3D:
            glTexImage3D(GL_TEXTURE_3D, 0, RHIFormatToTextureFormat(m_format),
                         m_width, m_height, m_depth, 0, GetPixelFormat(), GetPixelType(), nullptr);
            break;

        case ETextureType::TextureCube:
            for (uint32_t i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, RHIFormatToTextureFormat(m_format),
                             m_width, m_height, 0, GetPixelFormat(), GetPixelType(), nullptr);
            }
            break;
    }

    glBindTexture(GetGLTarget(m_type), 0);
}

void OpenGLTexture::CreateTexture(const void* data, uint32_t face)
{
    glGenTextures(1, &m_texture);
    glBindTexture(GetGLTarget(m_type), m_texture);

    GLenum internalFormat = RHIFormatToTextureFormat(m_format);

    if (m_type == ETextureType::TextureCube) {
        for (uint32_t i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat,
                         m_width, m_height, 0, GetPixelFormat(), GetPixelType(), nullptr);
        if (data)
            UpdateData(data, m_width, m_height, m_depth, face);
    } else {
        switch (m_type) {
            case ETextureType::Texture1D:
                glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, m_width, 0, GetPixelFormat(), GetPixelType(), data);
                break;
            case ETextureType::Texture2D:
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, GetPixelFormat(), GetPixelType(), data);
                break;
            case ETextureType::Texture3D:
                glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, m_width, m_height, m_depth, 0, GetPixelFormat(), GetPixelType(), data);
                break;
            default:
                break;
        }
    }

    // Default filtering and wrapping
    SetFilter(ETextureFilter::Linear, ETextureFilter::Linear);
    SetWrap(ETextureWrap::Repeat, ETextureWrap::Repeat, ETextureWrap::Repeat);

    if (data)
        glGenerateMipmap(GetGLTarget(m_type));

    glBindTexture(GetGLTarget(m_type), 0);
}

GLenum OpenGLTexture::GetPixelFormat() const
{
    switch (m_format)
    {
        // 8-bit
        case ERHIFormat::R8_UNORM:
        case ERHIFormat::R8_SNORM:
        case ERHIFormat::R8_UINT:
        case ERHIFormat::R8_SINT:
        case ERHIFormat::R8_SRGB:
            return GL_RED;

        case ERHIFormat::R8G8_UNORM:
        case ERHIFormat::R8G8_SNORM:
        case ERHIFormat::R8G8_UINT:
        case ERHIFormat::R8G8_SINT:
        case ERHIFormat::R8G8_SRGB:
            return GL_RG;

        case ERHIFormat::R8G8B8_UNORM:
        case ERHIFormat::R8G8B8_SNORM:
        case ERHIFormat::R8G8B8_UINT:
        case ERHIFormat::R8G8B8_SINT:
        case ERHIFormat::R8G8B8_SRGB:
            return GL_RGB;

        case ERHIFormat::R8G8B8A8_UNORM:
        case ERHIFormat::R8G8B8A8_SNORM:
        case ERHIFormat::R8G8B8A8_UINT:
        case ERHIFormat::R8G8B8A8_SINT:
        case ERHIFormat::R8G8B8A8_SRGB:
        case ERHIFormat::B8G8R8A8_UNORM:
        case ERHIFormat::B8G8R8A8_SRGB:
            return GL_RGBA;

        // 16-bit
        case ERHIFormat::R16_UNORM:
        case ERHIFormat::R16_SNORM:
        case ERHIFormat::R16_UINT:
        case ERHIFormat::R16_SINT:
        case ERHIFormat::R16_FLOAT:
            return GL_RED;

        case ERHIFormat::R16G16_UNORM:
        case ERHIFormat::R16G16_SNORM:
        case ERHIFormat::R16G16_UINT:
        case ERHIFormat::R16G16_SINT:
        case ERHIFormat::R16G16_FLOAT:
            return GL_RG;

        case ERHIFormat::R16G16B16A16_UNORM:
        case ERHIFormat::R16G16B16A16_SNORM:
        case ERHIFormat::R16G16B16A16_UINT:
        case ERHIFormat::R16G16B16A16_SINT:
        case ERHIFormat::R16G16B16A16_FLOAT:
            return GL_RGBA;

        // 32-bit
        case ERHIFormat::R32_UINT:
        case ERHIFormat::R32_SINT:
        case ERHIFormat::R32_FLOAT:
            return GL_RED;

        case ERHIFormat::R32G32_UINT:
        case ERHIFormat::R32G32_SINT:
        case ERHIFormat::R32G32_FLOAT:
            return GL_RG;

        case ERHIFormat::R32G32B32_UINT:
        case ERHIFormat::R32G32B32_SINT:
        case ERHIFormat::R32G32B32_FLOAT:
            return GL_RGB;

        case ERHIFormat::R32G32B32A32_UINT:
        case ERHIFormat::R32G32B32A32_SINT:
        case ERHIFormat::R32G32B32A32_FLOAT:
            return GL_RGBA;

        // Depth / Stencil
        case ERHIFormat::D16_UNORM:
        case ERHIFormat::D32_FLOAT:
            return GL_DEPTH_COMPONENT;

        case ERHIFormat::S8_UINT:
            return GL_STENCIL_INDEX;

        case ERHIFormat::D24_UNORM_S8_UINT:
        case ERHIFormat::D32_FLOAT_S8X24_UINT:
            return GL_DEPTH_STENCIL;

        default:
            return GL_RGBA; // fallback
    }
}

GLenum OpenGLTexture::GetPixelType() const
{
    switch (m_format)
    {
        // 8-bit normalized / integer
        case ERHIFormat::R8_UNORM:
        case ERHIFormat::R8G8_UNORM:
        case ERHIFormat::R8G8B8_UNORM:
        case ERHIFormat::R8G8B8A8_UNORM:
        case ERHIFormat::B8G8R8A8_UNORM:
            return GL_UNSIGNED_BYTE;

        case ERHIFormat::R8_SNORM:
        case ERHIFormat::R8G8_SNORM:
        case ERHIFormat::R8G8B8_SNORM:
        case ERHIFormat::R8G8B8A8_SNORM:
            return GL_BYTE;

        case ERHIFormat::R8_UINT:
        case ERHIFormat::R8G8_UINT:
        case ERHIFormat::R8G8B8_UINT:
        case ERHIFormat::R8G8B8A8_UINT:
            return GL_UNSIGNED_BYTE;

        case ERHIFormat::R8_SINT:
        case ERHIFormat::R8G8_SINT:
        case ERHIFormat::R8G8B8_SINT:
        case ERHIFormat::R8G8B8A8_SINT:
            return GL_BYTE;

        // 16-bit
        case ERHIFormat::R16_FLOAT:
        case ERHIFormat::R16G16_FLOAT:
        case ERHIFormat::R16G16B16A16_FLOAT:
            return GL_HALF_FLOAT;

        case ERHIFormat::R16_UINT:
        case ERHIFormat::R16G16_UINT:
        case ERHIFormat::R16G16B16A16_UINT:
            return GL_UNSIGNED_SHORT;

        case ERHIFormat::R16_SINT:
        case ERHIFormat::R16G16_SINT:
        case ERHIFormat::R16G16B16A16_SINT:
            return GL_SHORT;

        // 32-bit
        case ERHIFormat::R32_FLOAT:
        case ERHIFormat::R32G32_FLOAT:
        case ERHIFormat::R32G32B32_FLOAT:
        case ERHIFormat::R32G32B32A32_FLOAT:
            return GL_FLOAT;

        case ERHIFormat::R32_UINT:
        case ERHIFormat::R32G32_UINT:
        case ERHIFormat::R32G32B32_UINT:
        case ERHIFormat::R32G32B32A32_UINT:
            return GL_UNSIGNED_INT;

        case ERHIFormat::R32_SINT:
        case ERHIFormat::R32G32_SINT:
        case ERHIFormat::R32G32B32_SINT:
        case ERHIFormat::R32G32B32A32_SINT:
            return GL_INT;

        // Depth / Stencil
        case ERHIFormat::D16_UNORM:
            return GL_UNSIGNED_SHORT;
        case ERHIFormat::D24_UNORM_S8_UINT:
            return GL_UNSIGNED_INT_24_8;
        case ERHIFormat::D32_FLOAT:
            return GL_FLOAT;
        case ERHIFormat::D32_FLOAT_S8X24_UINT:
            return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
        case ERHIFormat::S8_UINT:
            return GL_UNSIGNED_BYTE;

        default:
            return GL_UNSIGNED_BYTE;
    }
}

GLenum OpenGLTexture::RHIFormatToTextureFormat(ERHIFormat format)
{
    switch (format)
    {
        // 8-bit
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

        // 16-bit
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

        // 32-bit
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

        // Packed
        case ERHIFormat::B8G8R8A8_UNORM: return GL_RGBA8;
        case ERHIFormat::B8G8R8A8_SRGB: return GL_SRGB8_ALPHA8;

        // Depth/Stencil
        case ERHIFormat::D16_UNORM: return GL_DEPTH_COMPONENT16;
        case ERHIFormat::D24_UNORM_S8_UINT: return GL_DEPTH24_STENCIL8;
        case ERHIFormat::D32_FLOAT: return GL_DEPTH_COMPONENT32F;
        case ERHIFormat::D32_FLOAT_S8X24_UINT: return GL_DEPTH32F_STENCIL8;
        case ERHIFormat::S8_UINT: return GL_STENCIL_INDEX8;

        // sRGB
        case ERHIFormat::R8_SRGB: return GL_SRGB8;
        case ERHIFormat::R8G8_SRGB: return GL_SRGB8;
        case ERHIFormat::R8G8B8_SRGB: return GL_SRGB8;

        // Typeless and compressed formats are not directly supported in OpenGL, return a default or error
        // Typeless
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

} // namespace VE::Internal::RHI
