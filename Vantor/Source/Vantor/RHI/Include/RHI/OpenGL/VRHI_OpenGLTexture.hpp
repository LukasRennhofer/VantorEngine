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

namespace Vantor::RHI
{

class OpenGLTexture : public IRHITexture
{
public:
    OpenGLTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data = nullptr);
    OpenGLTexture(uint32_t width, uint32_t height, ETextureFormat format, const void* data = nullptr);
    virtual ~OpenGLTexture();

    // IRHITexture implementation
    void Bind(uint32_t slot = 0) override;
    void Unbind() override;
    void UpdateData(const void* data, uint32_t width, uint32_t height) override;
    
    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    uint32_t GetHandle() const override { return m_texture; }
    ETextureFormat GetFormat() const override { return m_format; }

    void SetFilter(ETextureFilter minFilter, ETextureFilter magFilter) override;
    void SetWrap(ETextureWrap wrapS, ETextureWrap wrapT) override;

    // Static utility functions
    static GLenum TextureFormatToGL(ETextureFormat format);
    static GLenum RHIFormatToTextureFormat(ERHIFormat format);
    static GLenum TextureFilterToGL(ETextureFilter filter);
    static GLenum TextureWrapToGL(ETextureWrap wrap);

private:
    uint32_t m_texture;
    uint32_t m_width;
    uint32_t m_height;
    ETextureFormat m_format;
    uint32_t m_currentSlot;

    void CreateTexture(const void* data);
    GLenum GetPixelFormat() const;
    GLenum GetPixelType() const;
};

} // namespace Lab
