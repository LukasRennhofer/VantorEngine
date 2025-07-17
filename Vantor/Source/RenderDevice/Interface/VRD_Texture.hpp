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
 *  File: VRD_Texture.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::RenderDevice
{
    using namespace Vantor::Math;

    // ----------------- Texture Enums -----------------
    enum class VTextureFilter : uint8_t
    {
        Nearest = 0,   // GL_NEAREST - Pixelated look
        Linear,        // GL_LINEAR - Smooth interpolation
        NearestMipmap, // GL_NEAREST_MIPMAP_NEAREST - Mipmap with nearest filtering
        LinearMipmap,  // GL_LINEAR_MIPMAP_LINEAR - Mipmap with linear filtering
        MixedMipmap    // GL_LINEAR_MIPMAP_NEAREST - Mixed mipmap filtering
    };
    enum class VTextureWrap : uint8_t
    {
        Repeat = 0,     // GL_REPEAT - Tile the texture
        MirroredRepeat, // GL_MIRRORED_REPEAT - Mirror and tile
        ClampToEdge,    // GL_CLAMP_TO_EDGE - Clamp to edge pixels
        ClampToBorder   // GL_CLAMP_TO_BORDER - Clamp to border color
    };

    enum class VTextureFormat
    {
        // Color formats
        RGBA8,   // 8-bit RGBA
        RGBA16F, // 16-bit floating point RGBA
        RGBA32F, // 32-bit floating point RGBA
        RGB8,    // 8-bit RGB
        RGB16F,  // 16-bit floating point RGB
        RGB32F,  // 32-bit floating point RGB
        RG8,     // 8-bit RG
        RG16F,   // 16-bit floating point RG
        RG32F,   // 32-bit floating point RG
        R8,      // 8-bit single channel
        R16F,    // 16-bit floating point single channel
        R32F,    // 32-bit floating point single channel

        // HDR formats
        RGB10_A2,       // 10-bit RGB, 2-bit Alpha
        R11F_G11F_B10F, // Packed 11-11-10 floating point

        // Depth/Stencil formats
        Depth16,           // 16-bit depth
        Depth24,           // 24-bit depth
        Depth32F,          // 32-bit floating point depth
        Depth24_Stencil8,  // 24-bit depth, 8-bit stencil
        Depth32F_Stencil8, // 32-bit float depth, 8-bit stencil
        CompressedRGB,
        CompressedRGBA
    };

    enum class VTextureUsage : uint8_t
    {
        Static = 0,  // Data set once, used many times (textures loaded from files)
        Dynamic,     // Data changes occasionally (UI textures, particle textures)
        Streaming,   // Data changes frequently (video textures, dynamic content)
        RenderTarget // Used as render target (framebuffer attachments)
    };

    enum class VTextureDataType : uint8_t
    {
        UnsignedByte = 0, // GL_UNSIGNED_BYTE (0-255)
        Float,            // GL_FLOAT (-inf to +inf)
        HalfFloat,        // GL_HALF_FLOAT (16-bit float)
        UnsignedShort,    // GL_UNSIGNED_SHORT (0-65535)
        UnsignedInt       // GL_UNSIGNED_INT (0-4294967295)
    };

    // ----------------- Texture Specification -----------------
    struct VTextureSampler
    {
            VTextureFilter minFilter       = VTextureFilter::Linear;
            VTextureFilter magFilter       = VTextureFilter::Linear;
            VTextureWrap   wrapS           = VTextureWrap::Repeat;
            VTextureWrap   wrapT           = VTextureWrap::Repeat;
            VTextureWrap   wrapR           = VTextureWrap::Repeat;
            VVector4       borderColor     = {0.0f, 0.0f, 0.0f, 1.0f};
            float          lodBias         = 0.0f;
            float          minLod          = -1000.0f;
            float          maxLod          = 1000.0f;
            bool           generateMipmaps = true;
    };

    /**
     * @brief Complete texture specification
     */
    struct VTextureSpec
    {
            uint32_t        width     = 1;
            uint32_t        height    = 1;
            uint32_t        depth     = 1; // For 3D textures, layers for array textures
            uint32_t        mipLevels = 1; // Number of mipmap levels (0 = generate all)
            VTextureFormat  format    = VTextureFormat::RGBA8;
            VTextureUsage   usage     = VTextureUsage::Static;
            VTextureSampler sampler   = {};
            std::string     debugName = ""; // For debugging and profiling
    };

    // ----------------- Texture Interface -----------------
    class VTexture
    {
        public:
            virtual ~VTexture() = default;

            // ----------------- Resource Management -----------------
            virtual bool Create(const VTextureSpec &spec) = 0;
            virtual void Destroy()                        = 0;
            virtual bool IsValid() const                  = 0; // Check if texture is valid

            // ----------------- Data Upload -----------------
            // Upload pixel data to Texture (for procedural textures as an example)
            virtual bool SetData(const void *data, size_t dataSize, uint32_t level = 0, VTextureDataType dataType = VTextureDataType::UnsignedByte) = 0;
            // Upload pixel data to a region of the texture
            virtual bool SetSubData(const void      *data,
                                    size_t           dataSize,
                                    uint32_t         x,
                                    uint32_t         y,
                                    uint32_t         width,
                                    uint32_t         height,
                                    uint32_t         level    = 0,
                                    VTextureDataType dataType = VTextureDataType::UnsignedByte)
                = 0;

            // Generate Mipmaps of the Texture (For LOD) : NOTE(Lukas) : Should implement it with HDR density
            virtual bool GenerateMipmaps() = 0;

            // ----------------- Parameter Control -----------------
            // Update Sampler Settings
            virtual void            SetSampler(const VTextureSampler &sampler) = 0;
            virtual VTextureSampler GetSampler() const                         = 0;

            // ----------------- Binding -----------------
            virtual void Bind(uint32_t unit = 0) const   = 0;
            virtual void Unbind(uint32_t unit = 0) const = 0;

            // ----------------- Properties -----------------
            virtual const VTextureSpec &GetSpec() const      = 0;
            virtual uint32_t            GetWidth() const     = 0;
            virtual uint32_t            GetHeight() const    = 0;
            virtual uint32_t            GetDepth() const     = 0;
            virtual uint32_t            GetMipLevels() const = 0; // Get Number of Mipmap levels
            // Get internal Texture format
            virtual VTextureFormat GetFormat() const = 0;
            // Get Memory usage of Texture in bytes
            virtual size_t GetMemoryUsage() const = 0;

        protected:
            VTextureSpec m_spec = {};
    };

    // ----------------- Specialized Texture Interfaces -----------------

    // VTexture2D
    class VTexture2D : public VTexture
    {
        public:
            // Create Texture from file
            static std::shared_ptr<VTexture2D> CreateFromFile(const std::string &filePath, const VTextureSampler &sampler = {}, bool generateMipmaps = true);
            // Create with pixel data
            static std::shared_ptr<VTexture2D> CreateFromMemory(const void            *data,
                                                                uint32_t               width,
                                                                uint32_t               height,
                                                                VTextureFormat         format  = VTextureFormat::RGBA8,
                                                                const VTextureSampler &sampler = {});

            // Create empty texture (for RenderTargets)
            static std::shared_ptr<VTexture2D>
            CreateEmpty(uint32_t width, uint32_t height, VTextureFormat format = VTextureFormat::RGBA8, const VTextureSampler &sampler = {});
    };

    // VTextureCube
    class VTextureCube : public VTexture
    {
        public:
            // Upload data to a specific face of the cubemap
            virtual bool
            SetFaceData(uint32_t face, const void *data, size_t dataSize, uint32_t level = 0, VTextureDataType dataType = VTextureDataType::UnsignedByte)
                = 0;

            // Create from file
            static std::shared_ptr<VTextureCube> CreateFromFiles(const std::string faceFiles[6], const VTextureSampler &sampler = {});
    };

    // ----------------- Utility Functions -----------------

    uint32_t GetBytesPerPixel(VTextureFormat format);
    size_t   CalculateTextureMemoryUsage(const VTextureSpec &spec);
    bool     IsRenderableFormat(VTextureFormat format);
    bool     IsDepthFormat(VTextureFormat format);
    bool     IsCompressedFormat(VTextureFormat format);

} // namespace Vantor::RenderDevice