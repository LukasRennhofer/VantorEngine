/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-16
 *
 *  File: VRDO_Texture.cpp
 *  Last Change: Automatically updated
 */

#include <Shared/glad/glad.h>
#include <Shared/glad/glad.h>
#include <algorithm>
#include <cstring>
#include <iostream>

#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "VRDO_Texture.hpp"
// Core/Backlog
#include "../../Core/BackLog/VCO_Backlog.hpp"

// STB for image loading
#define STB_IMAGE_IMPLEMENTATION
#include <Shared/STB/stb_image.h>

namespace Vantor::RenderDevice
{
    // ----------------- Utility Functions Implementation -----------------

    uint32_t GetBytesPerPixel(VTextureFormat format)
    {
        switch (format)
        {
            case VTextureFormat::R8:
                return 1;
            case VTextureFormat::RG8:
                return 2;
            case VTextureFormat::RGB8:
                return 3;
            case VTextureFormat::RGBA8:
                return 4;
            case VTextureFormat::RGB16F:
                return 6;
            case VTextureFormat::RGBA16F:
                return 8;
            case VTextureFormat::RGB32F:
                return 12;
            case VTextureFormat::RGBA32F:
                return 16;
            case VTextureFormat::Depth16:
                return 2;
            case VTextureFormat::Depth24:
                return 3;
            case VTextureFormat::Depth32F:
                return 4;
            case VTextureFormat::Depth24_Stencil8:
                return 4;
            case VTextureFormat::CompressedRGB:
                return 1; // Approximation for DXT1
            case VTextureFormat::CompressedRGBA:
                return 1; // Approximation for DXT5
            default:
                return 4; // Default to RGBA8
        }
    }

    size_t CalculateTextureMemoryUsage(const VTextureSpec &spec)
    {
        size_t   totalSize     = 0;
        uint32_t width         = spec.width;
        uint32_t height        = spec.height;
        uint32_t bytesPerPixel = GetBytesPerPixel(spec.format);

        uint32_t levels = spec.mipLevels;
        if (levels == 0)
        {
            levels = CalculateMipLevels(width, height);
        }

        for (uint32_t level = 0; level < levels; ++level)
        {
            size_t levelSize = width * height * spec.depth * bytesPerPixel;
            totalSize += levelSize;

            width  = std::max(1u, width / 2);
            height = std::max(1u, height / 2);
        }

        return totalSize;
    }

    bool IsRenderableFormat(VTextureFormat format)
    {
        switch (format)
        {
            case VTextureFormat::RGBA8:
            case VTextureFormat::RGB8:
            case VTextureFormat::RGBA16F:
            case VTextureFormat::RGB16F:
            case VTextureFormat::RGBA32F:
            case VTextureFormat::RGB32F:
            case VTextureFormat::Depth16:
            case VTextureFormat::Depth24:
            case VTextureFormat::Depth32F:
            case VTextureFormat::Depth24_Stencil8:
                return true;
            default:
                return false;
        }
    }

    bool IsDepthFormat(VTextureFormat format)
    {
        switch (format)
        {
            case VTextureFormat::Depth16:
            case VTextureFormat::Depth24:
            case VTextureFormat::Depth32F:
            case VTextureFormat::Depth24_Stencil8:
                return true;
            default:
                return false;
        }
    }

    bool IsCompressedFormat(VTextureFormat format)
    {
        switch (format)
        {
            case VTextureFormat::CompressedRGB:
            case VTextureFormat::CompressedRGBA:
                return true;
            default:
                return false;
        }
    }

    // ----------------- OpenGL Conversion Functions -----------------

    GLenum TextureFormatToGL(VTextureFormat format)
    {
        switch (format)
        {
            case VTextureFormat::R8:
                return GL_R8;
            case VTextureFormat::RG8:
                return GL_RG8;
            case VTextureFormat::RGB8:
                return GL_RGB8;
            case VTextureFormat::RGBA8:
                return GL_RGBA8;
            case VTextureFormat::RGB16F:
                return GL_RGB16F;
            case VTextureFormat::RGBA16F:
                return GL_RGBA16F;
            case VTextureFormat::RGB32F:
                return GL_RGB32F;
            case VTextureFormat::RGBA32F:
                return GL_RGBA32F;
            case VTextureFormat::Depth16:
                return GL_DEPTH_COMPONENT16;
            case VTextureFormat::Depth24:
                return GL_DEPTH_COMPONENT24;
            case VTextureFormat::Depth32F:
                return GL_DEPTH_COMPONENT32F;
            case VTextureFormat::Depth24_Stencil8:
                return GL_DEPTH24_STENCIL8;
            case VTextureFormat::CompressedRGB:
                return GL_COMPRESSED_RGB; // Use generic compressed format
            case VTextureFormat::CompressedRGBA:
                return GL_COMPRESSED_RGBA; // Use generic compressed format
            default:
                return GL_RGBA8;
        }
    }

    GLenum TextureFormatToGLFormat(VTextureFormat format)
    {
        switch (format)
        {
            case VTextureFormat::R8:
                return GL_RED;
            case VTextureFormat::RG8:
                return GL_RG;
            case VTextureFormat::RGB8:
                return GL_RGB;
            case VTextureFormat::RGBA8:
                return GL_RGBA;
            case VTextureFormat::RGB16F:
                return GL_RGB;
            case VTextureFormat::RGBA16F:
                return GL_RGBA;
            case VTextureFormat::RGB32F:
                return GL_RGB;
            case VTextureFormat::RGBA32F:
                return GL_RGBA;
            case VTextureFormat::Depth16:
                return GL_DEPTH_COMPONENT;
            case VTextureFormat::Depth24:
                return GL_DEPTH_COMPONENT;
            case VTextureFormat::Depth32F:
                return GL_DEPTH_COMPONENT;
            case VTextureFormat::Depth24_Stencil8:
                return GL_DEPTH_STENCIL;
            case VTextureFormat::CompressedRGB:
                return GL_RGB; // Use generic compressed format
            case VTextureFormat::CompressedRGBA:
                return GL_RGBA; // Use generic compressed format
            default:
                return GL_RGBA;
        }
    }

    GLenum TextureDataTypeToGL(VTextureDataType dataType)
    {
        switch (dataType)
        {
            case VTextureDataType::UnsignedByte:
                return GL_UNSIGNED_BYTE;
            case VTextureDataType::Float:
                return GL_FLOAT;
            case VTextureDataType::HalfFloat:
                return GL_HALF_FLOAT;
            case VTextureDataType::UnsignedShort:
                return GL_UNSIGNED_SHORT;
            case VTextureDataType::UnsignedInt:
                return GL_UNSIGNED_INT;
            default:
                return GL_UNSIGNED_BYTE;
        }
    }

    GLenum TextureFilterToGL(VTextureFilter filter)
    {
        switch (filter)
        {
            case VTextureFilter::Nearest:
                return GL_NEAREST;
            case VTextureFilter::Linear:
                return GL_LINEAR;
            case VTextureFilter::NearestMipmap:
                return GL_NEAREST_MIPMAP_NEAREST;
            case VTextureFilter::LinearMipmap:
                return GL_LINEAR_MIPMAP_LINEAR;
            case VTextureFilter::MixedMipmap:
                return GL_LINEAR_MIPMAP_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    GLenum TextureWrapToGL(VTextureWrap wrap)
    {
        switch (wrap)
        {
            case VTextureWrap::Repeat:
                return GL_REPEAT;
            case VTextureWrap::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            case VTextureWrap::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case VTextureWrap::ClampToBorder:
                return GL_CLAMP_TO_BORDER;
            default:
                return GL_REPEAT;
        }
    }

    uint32_t CalculateMipLevels(uint32_t width, uint32_t height)
    {
        uint32_t levels = 1;
        uint32_t size   = std::max(width, height);
        while (size > 1)
        {
            size /= 2;
            levels++;
        }
        return levels;
    }

    uint32_t GetMaxTextureSize()
    {
        static uint32_t maxSize = 0;
        if (maxSize == 0)
        {
            GLint size;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
            maxSize = static_cast<uint32_t>(size);
        }
        return maxSize;
    }

    uint32_t GetMaxCubemapSize()
    {
        static uint32_t maxSize = 0;
        if (maxSize == 0)
        {
            GLint size;
            glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &size);
            maxSize = static_cast<uint32_t>(size);
        }
        return maxSize;
    }

    uint32_t GetMaxTextureUnits()
    {
        static uint32_t maxUnits = 0;
        if (maxUnits == 0)
        {
            GLint units;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &units);
            maxUnits = static_cast<uint32_t>(units);
        }
        return maxUnits;
    }

    // ----------------- VOpenGLTexture Implementation -----------------

    VOpenGLTexture::VOpenGLTexture() : m_textureID(0), m_target(GL_TEXTURE_2D) { m_boundUnits.fill(false); }

    VOpenGLTexture::~VOpenGLTexture() { Destroy(); }

    VOpenGLTexture::VOpenGLTexture(VOpenGLTexture &&other) noexcept { MoveFrom(std::move(other)); }

    VOpenGLTexture &VOpenGLTexture::operator=(VOpenGLTexture &&other) noexcept
    {
        if (this != &other)
        {
            Destroy();
            MoveFrom(std::move(other));
        }
        return *this;
    }

    void VOpenGLTexture::MoveFrom(VOpenGLTexture &&other) noexcept
    {
        m_textureID  = other.m_textureID;
        m_target     = other.m_target;
        m_spec       = std::move(other.m_spec);
        m_boundUnits = other.m_boundUnits;

        other.m_textureID = 0;
        other.m_target    = GL_TEXTURE_2D;
        other.m_boundUnits.fill(false);
    }

    bool VOpenGLTexture::Create(const VTextureSpec &spec)
    {
        // Validate specification
        if (spec.width == 0 || spec.height == 0)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Invalid texture dimensions", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (spec.width > GetMaxTextureSize() || spec.height > GetMaxTextureSize())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", " Texture dimensions exceed maximum supported size", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        // Clean up existing texture
        Destroy();

        // Store specification
        m_spec   = spec;
        m_target = GetTextureTarget();

        // Calculate mip levels if not specified
        if (m_spec.mipLevels == 0)
        {
            m_spec.mipLevels = CalculateMipLevels(m_spec.width, m_spec.height);
        }

        // Create OpenGL texture object
        if (!CreateTextureObject())
        {
            return false;
        }

        // Allocate storage
        glBindTexture(m_target, m_textureID);

        GLenum internalFormat = TextureFormatToGL(m_spec.format);

        if (m_target == GL_TEXTURE_2D)
        {
            glTexStorage2D(GL_TEXTURE_2D, m_spec.mipLevels, internalFormat, m_spec.width, m_spec.height);
        }
        else if (m_target == GL_TEXTURE_CUBE_MAP)
        {
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_spec.mipLevels, internalFormat, m_spec.width, m_spec.height);
        }

        // Check for errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "OpenGL error during texture creation", Vantor::Backlog::LogLevel::ERR);
            Destroy();
            return false;
        }

        // Apply sampler parameters
        ApplySamplerParameters(m_spec.sampler);

        glBindTexture(m_target, 0);
        return true;
    }

    void VOpenGLTexture::Destroy()
    {
        if (m_textureID != 0)
        {
            // Unbind from all units
            for (uint32_t i = 0; i < m_boundUnits.size(); ++i)
            {
                if (m_boundUnits[i])
                {
                    Unbind(i);
                }
            }

            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
        }
        m_boundUnits.fill(false);
    }

    bool VOpenGLTexture::IsValid() const { return m_textureID != 0 && glIsTexture(m_textureID) == GL_TRUE; }

    bool VOpenGLTexture::CreateTextureObject()
    {
        glGenTextures(1, &m_textureID);
        if (m_textureID == 0)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Failed to generate texture object", Vantor::Backlog::LogLevel::ERR);
            std::cerr << "[VOpenGLTexture] Failed to generate texture object" << std::endl;
            return false;
        }
        return true;
    }

    bool VOpenGLTexture::SetData(const void *data, size_t dataSize, uint32_t level, VTextureDataType dataType)
    {
        if (!IsValid())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Cannot set data on invalid texture", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (level >= m_spec.mipLevels)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Mip level exceeds maximum", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        // Validate data size if data is provided
        if (data != nullptr && !ValidateDataSize(dataSize, GetWidth(), GetHeight(), level))
        {
            return false;
        }

        glBindTexture(m_target, m_textureID);

        GLenum format = TextureFormatToGLFormat(m_spec.format);
        GLenum type   = TextureDataTypeToGL(dataType);

        uint32_t levelWidth  = std::max(1u, m_spec.width >> level);
        uint32_t levelHeight = std::max(1u, m_spec.height >> level);

        if (m_target == GL_TEXTURE_2D)
        {
            glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, levelWidth, levelHeight, format, type, data);
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "OpenGL error during data upload", Vantor::Backlog::LogLevel::ERR);
            glBindTexture(m_target, 0);
            return false;
        }

        glBindTexture(m_target, 0);
        return true;
    }

    bool VOpenGLTexture::SetSubData(const void      *data,
                                    size_t           dataSize,
                                    uint32_t         x,
                                    uint32_t         y,
                                    uint32_t         width,
                                    uint32_t         height,
                                    uint32_t         level,
                                    VTextureDataType dataType)
    {
        if (!IsValid())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Cannot set sub data on invalid texture", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (level >= m_spec.mipLevels)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Mip level exceeds maximum", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        uint32_t levelWidth  = std::max(1u, m_spec.width >> level);
        uint32_t levelHeight = std::max(1u, m_spec.height >> level);

        if (x + width > levelWidth || y + height > levelHeight)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Sub-region exceeds texture bounds", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (data != nullptr && !ValidateDataSize(dataSize, width, height, 0))
        {
            return false;
        }

        glBindTexture(m_target, m_textureID);

        GLenum format = TextureFormatToGLFormat(m_spec.format);
        GLenum type   = TextureDataTypeToGL(dataType);

        if (m_target == GL_TEXTURE_2D)
        {
            glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, type, data);
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "OpenGL error during sub data upload", Vantor::Backlog::LogLevel::ERR);
            glBindTexture(m_target, 0);
            return false;
        }

        glBindTexture(m_target, 0);
        return true;
    }

    bool VOpenGLTexture::GenerateMipmaps()
    {
        if (!IsValid())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Cannot generate mipmaps on invalid texture", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (m_spec.mipLevels <= 1)
        {
            return true; // No mipmaps to generate
        }

        glBindTexture(m_target, m_textureID);
        glGenerateMipmap(m_target);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "OpenGL error during mipmap generation", Vantor::Backlog::LogLevel::ERR);
            glBindTexture(m_target, 0);
            return false;
        }

        glBindTexture(m_target, 0);
        return true;
    }

    void VOpenGLTexture::SetSampler(const VTextureSampler &sampler)
    {
        if (!IsValid())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Cannot set sampler on invalid texture", Vantor::Backlog::LogLevel::ERR);
            return;
        }

        m_spec.sampler = sampler;

        glBindTexture(m_target, m_textureID);
        ApplySamplerParameters(sampler);
        glBindTexture(m_target, 0);
    }

    VTextureSampler VOpenGLTexture::GetSampler() const { return m_spec.sampler; }

    void VOpenGLTexture::ApplySamplerParameters(const VTextureSampler &sampler)
    {
        glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, TextureFilterToGL(sampler.minFilter));
        glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, TextureFilterToGL(sampler.magFilter));
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, TextureWrapToGL(sampler.wrapS));
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, TextureWrapToGL(sampler.wrapT));

        if (m_target == GL_TEXTURE_CUBE_MAP)
        {
            glTexParameteri(m_target, GL_TEXTURE_WRAP_R, TextureWrapToGL(sampler.wrapR));
        }

        // Border color
        if (sampler.wrapS == VTextureWrap::ClampToBorder || sampler.wrapT == VTextureWrap::ClampToBorder || sampler.wrapR == VTextureWrap::ClampToBorder)
        {
            glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, &sampler.borderColor.x);
        }

        // LOD parameters
        glTexParameterf(m_target, GL_TEXTURE_LOD_BIAS, sampler.lodBias);
        glTexParameterf(m_target, GL_TEXTURE_MIN_LOD, sampler.minLod);
        glTexParameterf(m_target, GL_TEXTURE_MAX_LOD, sampler.maxLod);
    }

    void VOpenGLTexture::Bind(uint32_t unit) const
    {
        if (!IsValid())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Cannot bind invalid texture", Vantor::Backlog::LogLevel::ERR);
            return;
        }

        if (unit >= GetMaxTextureUnits())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Texture unit exceeds maximum supported units", Vantor::Backlog::LogLevel::ERR);
            return;
        }

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(m_target, m_textureID);
        m_boundUnits[unit] = true;
    }

    void VOpenGLTexture::Unbind(uint32_t unit) const
    {
        if (unit >= GetMaxTextureUnits())
        {
            return;
        }

        if (m_boundUnits[unit])
        {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(m_target, 0);
            m_boundUnits[unit] = false;
        }
    }

    size_t VOpenGLTexture::GetMemoryUsage() const { return CalculateTextureMemoryUsage(m_spec); }

    bool VOpenGLTexture::ValidateDataSize(size_t dataSize, uint32_t width, uint32_t height, uint32_t level) const
    {
        uint32_t bytesPerPixel = GetBytesPerPixel(m_spec.format);
        size_t   expectedSize  = width * height * bytesPerPixel;

        if (dataSize < expectedSize)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Data Size is smaller than expected", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        return true;
    }

    // ----------------- VOpenGLTexture2D Implementation -----------------

    std::shared_ptr<VOpenGLTexture2D> VOpenGLTexture2D::CreateFromFile(const std::string &filePath, const VTextureSampler &sampler, bool generateMipmaps)
    {
        // Load image file
        uint8_t *imageData = nullptr;
        int      width, height, channels;

        if (!LoadImageFile(filePath, imageData, width, height, channels))
        {
            return nullptr;
        }

        // Determine format based on channels
        VTextureFormat format;
        switch (channels)
        {
            case 1:
                format = VTextureFormat::R8;
                break;
            case 2:
                format = VTextureFormat::RG8;
                break;
            case 3:
                format = VTextureFormat::RGB8;
                break;
            case 4:
                format = VTextureFormat::RGBA8;
                break;
            default:
                Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Unsupported channel count", Vantor::Backlog::LogLevel::ERR);
                stbi_image_free(imageData);
                return nullptr;
        }

        // Create texture
        auto texture = std::make_shared<VOpenGLTexture2D>();

        VTextureSpec spec;
        spec.width                   = static_cast<uint32_t>(width);
        spec.height                  = static_cast<uint32_t>(height);
        spec.format                  = format;
        spec.sampler                 = sampler;
        spec.sampler.generateMipmaps = generateMipmaps;
        spec.debugName               = filePath;

        if (generateMipmaps)
        {
            spec.mipLevels = CalculateMipLevels(spec.width, spec.height);
        }

        if (!texture->Create(spec))
        {
            stbi_image_free(imageData);
            return nullptr;
        }

        // Upload data
        size_t dataSize = width * height * channels;
        if (!texture->SetData(imageData, dataSize, 0, VTextureDataType::UnsignedByte))
        {
            stbi_image_free(imageData);
            return nullptr;
        }

        // Generate mipmaps if requested
        if (generateMipmaps && !texture->GenerateMipmaps())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Failed to generate mipmaps", Vantor::Backlog::LogLevel::ERR);
        }

        stbi_image_free(imageData);
        return texture;
    }

    std::shared_ptr<VOpenGLTexture2D>
    VOpenGLTexture2D::CreateFromMemory(const void *data, uint32_t width, uint32_t height, VTextureFormat format, const VTextureSampler &sampler)
    {
        auto texture = std::make_shared<VOpenGLTexture2D>();

        VTextureSpec spec;
        spec.width     = width;
        spec.height    = height;
        spec.format    = format;
        spec.sampler   = sampler;
        spec.debugName = "Memory Texture";

        if (sampler.generateMipmaps)
        {
            spec.mipLevels = CalculateMipLevels(width, height);
        }

        if (!texture->Create(spec))
        {
            return nullptr;
        }

        if (data != nullptr)
        {
            size_t dataSize = width * height * GetBytesPerPixel(format);
            if (!texture->SetData(data, dataSize, 0, VTextureDataType::UnsignedByte))
            {
                return nullptr;
            }

            if (sampler.generateMipmaps && !texture->GenerateMipmaps())
            {
                Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Failed to generate mipmaps for memory texture", Vantor::Backlog::LogLevel::ERR);
            }
        }

        return texture;
    }

    std::shared_ptr<VOpenGLTexture2D> VOpenGLTexture2D::CreateEmpty(uint32_t width, uint32_t height, VTextureFormat format, const VTextureSampler &sampler)
    {
        return CreateFromMemory(nullptr, width, height, format, sampler);
    }

    bool VOpenGLTexture2D::LoadImageFile(const std::string &filePath, uint8_t *&data, int &width, int &height, int &channels)
    {
        // Configure STB to flip images vertically (OpenGL convention)
        stbi_set_flip_vertically_on_load(true);

        data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

        if (data == nullptr)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Failed to load image", Vantor::Backlog::LogLevel::ERR);
            return false;
        }
        return true;
    }

    // ----------------- VOpenGLTextureCube Implementation -----------------

    bool VOpenGLTextureCube::SetFaceData(uint32_t face, const void *data, size_t dataSize, uint32_t level, VTextureDataType dataType)
    {
        if (!IsValid())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Cannot set face data on invalid cubemap", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (face >= 6)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Invalid face index (must be 0-5)", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (level >= m_spec.mipLevels)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Mip level exceeds maximum", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        // Validate data size if data is provided
        if (data != nullptr && !ValidateDataSize(dataSize, GetWidth(), GetHeight(), level))
        {
            return false;
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

        GLenum format = TextureFormatToGLFormat(m_spec.format);
        GLenum type   = TextureDataTypeToGL(dataType);
        GLenum target = CUBE_FACES[face];

        uint32_t levelSize = std::max(1u, m_spec.width >> level);

        glTexSubImage2D(target, level, 0, 0, levelSize, levelSize, format, type, data);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "OpenGL error during face data upload", Vantor::Backlog::LogLevel::ERR);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            return false;
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        return true;
    }

    std::shared_ptr<VOpenGLTextureCube> VOpenGLTextureCube::CreateFromFiles(const std::string faceFiles[6], const VTextureSampler &sampler)
    {
        std::array<uint8_t *, 6> faceData = {};
        int                      width = 0, height = 0, channels = 0;
        bool                     success = true;

        for (int i = 0; i < 6; ++i)
        {
            int faceWidth, faceHeight, faceChannels;
            faceData[i] = stbi_load(faceFiles[i].c_str(), &faceWidth, &faceHeight, &faceChannels, 0);

            if (faceData[i] == nullptr)
            {
                Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Failed to load face", Vantor::Backlog::LogLevel::ERR);
                success = false;
                break;
            }

            if (i == 0)
            {
                width    = faceWidth;
                height   = faceHeight;
                channels = faceChannels;
            }
            else if (faceWidth != width || faceHeight != height || faceChannels != channels)
            {
                success = false;
                break;
            }

            if (width != height)
            {
                success = false;
                break;
            }
        }

        // Clean up on failure
        if (!success)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (faceData[i] != nullptr)
                {
                    stbi_image_free(faceData[i]);
                }
            }
            return nullptr;
        }

        // Determine format
        VTextureFormat format;
        switch (channels)
        {
            case 3:
                format = VTextureFormat::RGB8;
                break;
            case 4:
                format = VTextureFormat::RGBA8;
                break;
            default:
                Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Unsupported channel count for cubemap", Vantor::Backlog::LogLevel::ERR);
                for (int i = 0; i < 6; ++i)
                {
                    stbi_image_free(faceData[i]);
                }
                return nullptr;
        }

        // Create cubemap
        auto cubemap = std::make_shared<VOpenGLTextureCube>();

        VTextureSpec spec;
        spec.width     = static_cast<uint32_t>(width);
        spec.height    = static_cast<uint32_t>(height);
        spec.format    = format;
        spec.sampler   = sampler;
        spec.debugName = "Cubemap";

        if (sampler.generateMipmaps)
        {
            spec.mipLevels = CalculateMipLevels(spec.width, spec.height);
        }

        if (!cubemap->Create(spec))
        {
            for (int i = 0; i < 6; ++i)
            {
                stbi_image_free(faceData[i]);
            }
            return nullptr;
        }

        // Upload all faces
        size_t faceDataSize = width * height * channels;
        for (int i = 0; i < 6; ++i)
        {
            if (!cubemap->SetFaceData(i, faceData[i], faceDataSize, 0, VTextureDataType::UnsignedByte))
            {
                for (int j = 0; j < 6; ++j)
                {
                    stbi_image_free(faceData[j]);
                }
                return nullptr;
            }
        }

        // Generate mipmaps if requested
        if (sampler.generateMipmaps && !cubemap->GenerateMipmaps())
        {
            Vantor::Backlog::Log("VRDeviceOpenGL::Texture", "Failed to generate mipmaps for cubemap", Vantor::Backlog::LogLevel::ERR);
        }

        // Clean up
        for (int i = 0; i < 6; ++i)
        {
            stbi_image_free(faceData[i]);
        }

        return cubemap;
    }

} // namespace Vantor::RenderDevice