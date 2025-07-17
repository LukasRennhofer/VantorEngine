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
 *  File: VRDO_Texture.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>
#include <array>
#include <memory>
#include <unordered_map>

#include "../Interface/VRD_Texture.hpp"

namespace Vantor::RenderDevice
{
    // ----------------- OpenGL Texture Base -----------------

    // OpenGL Texture
    class VOpenGLTexture : public VTexture
    {
        public:
            VOpenGLTexture();
            virtual ~VOpenGLTexture();

            // Prevent copying
            VOpenGLTexture(const VOpenGLTexture &)            = delete;
            VOpenGLTexture &operator=(const VOpenGLTexture &) = delete;

            // Allow moving
            VOpenGLTexture(VOpenGLTexture &&other) noexcept;
            VOpenGLTexture &operator=(VOpenGLTexture &&other) noexcept;

            // VTexture interface implementation
            bool Create(const VTextureSpec &spec) override;
            void Destroy() override;
            bool IsValid() const override;

            bool SetData(const void *data, size_t dataSize, uint32_t level = 0, VTextureDataType dataType = VTextureDataType::UnsignedByte) override;
            bool SetSubData(const void      *data,
                            size_t           dataSize,
                            uint32_t         x,
                            uint32_t         y,
                            uint32_t         width,
                            uint32_t         height,
                            uint32_t         level    = 0,
                            VTextureDataType dataType = VTextureDataType::UnsignedByte) override;
            bool GenerateMipmaps() override;

            void            SetSampler(const VTextureSampler &sampler) override;
            VTextureSampler GetSampler() const override;

            void Bind(uint32_t unit = 0) const override;
            void Unbind(uint32_t unit = 0) const override;

            const VTextureSpec &GetSpec() const override { return m_spec; }
            uint32_t            GetWidth() const override { return m_spec.width; }
            uint32_t            GetHeight() const override { return m_spec.height; }
            uint32_t            GetDepth() const override { return m_spec.depth; }
            uint32_t            GetMipLevels() const override { return m_spec.mipLevels; }
            VTextureFormat      GetFormat() const override { return m_spec.format; }
            size_t              GetMemoryUsage() const override;

            // OpenGL-specific methods
            GLuint GetTextureID() const { return m_textureID; }
            GLenum GetTarget() const { return m_target; }

        protected:
            // OpenGL helpers
            virtual GLenum GetTextureTarget() const = 0;
            bool           CreateTextureObject();
            void           ApplySamplerParameters(const VTextureSampler &sampler);
            bool           ValidateDataSize(size_t dataSize, uint32_t width, uint32_t height, uint32_t level) const;

            GLuint                       m_textureID  = 0;
            GLenum                       m_target     = GL_TEXTURE_2D;
            mutable std::array<bool, 32> m_boundUnits = {}; // Track which units we're bound to

        private:
            void MoveFrom(VOpenGLTexture &&other) noexcept;
    };

    // ----------------- OpenGL 2D Texture -----------------

    /**
     * @brief OpenGL implementation of 2D textures
     */
    class VOpenGLTexture2D : public VOpenGLTexture
    {
        public:
            VOpenGLTexture2D()          = default;
            virtual ~VOpenGLTexture2D() = default;

            // VTexture2D static creation methods implementation
            static std::shared_ptr<VOpenGLTexture2D>
            CreateFromFile(const std::string &filePath, const VTextureSampler &sampler = {}, bool generateMipmaps = true);
            static std::shared_ptr<VOpenGLTexture2D> CreateFromMemory(const void            *data,
                                                                      uint32_t               width,
                                                                      uint32_t               height,
                                                                      VTextureFormat         format  = VTextureFormat::RGBA8,
                                                                      const VTextureSampler &sampler = {});
            static std::shared_ptr<VOpenGLTexture2D>
            CreateEmpty(uint32_t width, uint32_t height, VTextureFormat format = VTextureFormat::RGBA8, const VTextureSampler &sampler = {});

        protected:
            GLenum GetTextureTarget() const override { return GL_TEXTURE_2D; }

        private:
            // Helper for loading image files
            static bool LoadImageFile(const std::string &filePath, uint8_t *&data, int &width, int &height, int &channels);
    };

    // ----------------- OpenGL Cubemap Texture -----------------

    /**
     * @brief OpenGL implementation of cubemap textures
     */
    class VOpenGLTextureCube : public VOpenGLTexture
    {
        public:
            VOpenGLTextureCube()          = default;
            virtual ~VOpenGLTextureCube() = default;

            // VTextureCube interface implementation
            bool SetFaceData(uint32_t face, const void *data, size_t dataSize, uint32_t level = 0, VTextureDataType dataType = VTextureDataType::UnsignedByte);

            // VTextureCube static creation methods implementation
            static std::shared_ptr<VOpenGLTextureCube> CreateFromFiles(const std::string faceFiles[6], const VTextureSampler &sampler = {});

        protected:
            GLenum GetTextureTarget() const override { return GL_TEXTURE_CUBE_MAP; }

        private:
            static constexpr std::array<GLenum, 6> CUBE_FACES
                = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};
    };

    // ----------------- OpenGL Utility Functions -----------------

    GLenum   TextureFormatToGL(VTextureFormat format);
    GLenum   TextureFormatToGLFormat(VTextureFormat format);
    GLenum   TextureDataTypeToGL(VTextureDataType dataType);
    GLenum   TextureFilterToGL(VTextureFilter filter);
    GLenum   TextureWrapToGL(VTextureWrap wrap);
    uint32_t CalculateMipLevels(uint32_t width, uint32_t height);
    uint32_t GetMaxTextureSize();
    uint32_t GetMaxCubemapSize();
    uint32_t GetMaxTextureUnits();

} // namespace Vantor::RenderDevice