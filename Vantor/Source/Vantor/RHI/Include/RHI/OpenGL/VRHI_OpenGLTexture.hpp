/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Texture.hpp>
#include <RHI/Interface/VRHI_Device.hpp>

#include <Shared/glad/glad.h>

namespace VE::Internal::RHI
{

class OpenGLTexture : public IRHITexture
{
public:
    OpenGLTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data = nullptr, ETextureType type = ETextureType::Texture2D, uint32_t depth = 1);
    
    virtual ~OpenGLTexture();

    // IRHITexture implementation
    void Bind(uint32_t slot = 0) override;
    void Unbind() override;
    void UpdateData(const void* data, uint32_t width, uint32_t height = 1, uint32_t depth = 1, uint32_t face = 0) override;

    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    uint32_t GetDepth() const override { return m_depth; }
    uint32_t GetHandle() const override { return m_texture; }
    ERHIFormat GetFormat() const override { return m_format; }
    ETextureType GetType() const override { return m_type; }

    void Resize(uint32_t newWidth, uint32_t newHeight, uint32_t newDepth = 1) override;

    void SetFilter(ETextureFilter minFilter, ETextureFilter magFilter) override;
    void SetWrap(ETextureWrap wrapS, ETextureWrap wrapT, ETextureWrap wrapR = ETextureWrap::Repeat) override;

    // Static utility functions
    static GLenum RHIFormatToTextureFormat(ERHIFormat format);
    static GLenum TextureFilterToGL(ETextureFilter filter);
    static GLenum TextureWrapToGL(ETextureWrap wrap);

private:
    // OpenGL handle
    uint32_t m_texture;
    // dimensions
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_depth;

    ERHIFormat m_format;
    ETextureType m_type;

    uint32_t m_currentSlot;

    void CreateTexture(const void* data, uint32_t face = 0);
    GLenum GetPixelFormat() const;
    GLenum GetPixelType() const;
};

} // namespace Lab
