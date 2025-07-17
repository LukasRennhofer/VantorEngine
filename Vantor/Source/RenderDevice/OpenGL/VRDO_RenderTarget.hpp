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
 *  File: VRDO_RenderTarget.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../Interface/VRD_RenderTarget.hpp"
#include "VRDO_Texture.hpp"
#include "VRDO_Texture.hpp"

namespace Vantor::RenderDevice
{
    class VOpenGLRenderTarget : public VRenderTarget
    {
        public:
            VOpenGLRenderTarget();
            virtual ~VOpenGLRenderTarget();

            // Prevent copying
            VOpenGLRenderTarget(const VOpenGLRenderTarget &)            = delete;
            VOpenGLRenderTarget &operator=(const VOpenGLRenderTarget &) = delete;

            // Allow moving
            VOpenGLRenderTarget(VOpenGLRenderTarget &&other) noexcept;
            VOpenGLRenderTarget &operator=(VOpenGLRenderTarget &&other) noexcept;

            // VRenderTarget interface implementation
            bool Create(const VRenderTargetDescriptor &descriptor) override;
            void Destroy() override;

            void Bind() override;
            void Unbind() override;
            bool IsBound() const override;

            void Clear() override;
            void Clear(const VClearValues &clearValues) override;
            void ClearAttachment(VEAttachmentType attachment, const VClearValues &clearValues) override;

            void Resize(uint32_t width, uint32_t height) override;
            bool IsResizable() const override;

            uint32_t           GetWidth() const override;
            uint32_t           GetHeight() const override;
            float              GetAspectRatio() const override;
            const std::string &GetDebugName() const override;

            bool IsComplete() const override;
            bool IsValid() const override;

            std::shared_ptr<VTexture>              GetColorTexture(uint32_t index = 0) const override;
            std::shared_ptr<VTexture>              GetDepthTexture() const override;
            std::shared_ptr<VTexture>              GetStencilTexture() const override;
            std::vector<std::shared_ptr<VTexture>> GetAllColorTextures() const override;

            uint32_t GetSampleCount() const override;
            bool     IsMultisampled() const override;

            void BlitTo(VRenderTarget *destination,
                        uint32_t       srcX0,
                        uint32_t       srcY0,
                        uint32_t       srcX1,
                        uint32_t       srcY1,
                        uint32_t       dstX0,
                        uint32_t       dstY0,
                        uint32_t       dstX1,
                        uint32_t       dstY1,
                        bool           linearFilter = true) override;

            void CopyTo(VRenderTarget *destination) override;

            size_t GetMemoryUsage() const override;
            void   SetDebugName(const std::string &name) override;

            // OpenGL-specific methods
            GLuint GetFramebufferID() const { return m_FramebufferID; }

        protected:
            // OpenGL objects
            GLuint m_FramebufferID         = 0;
            GLuint m_PreviousFramebufferID = 0;

            // Texture attachments using the VTexture system
            std::vector<std::shared_ptr<VOpenGLTexture2D>> m_ColorTextures;
            std::shared_ptr<VOpenGLTexture2D>              m_DepthTexture;
            std::shared_ptr<VOpenGLTexture2D>              m_StencilTexture;

            // Helper methods
            void                              CreateAttachments();
            void                              DestroyAttachments();
            std::shared_ptr<VOpenGLTexture2D> CreateTextureAttachment(const VAttachmentDescriptor &desc);
            GLenum                            GetOpenGLAttachment(VEAttachmentType type) const;
            void                              CheckFramebufferStatus() const;

        private:
            void MoveFrom(VOpenGLRenderTarget &&other) noexcept;
    };

    // Factory functions for easy creation
    namespace VRenderTargetFactory
    {
        std::unique_ptr<VOpenGLRenderTarget>
        CreateColorTarget(uint32_t width, uint32_t height, VTextureFormat format = VTextureFormat::RGBA8, const std::string &debugName = "ColorTarget");

        std::unique_ptr<VOpenGLRenderTarget> CreateColorDepthTarget(uint32_t           width,
                                                                    uint32_t           height,
                                                                    VTextureFormat     colorFormat = VTextureFormat::RGBA8,
                                                                    VTextureFormat     depthFormat = VTextureFormat::Depth24,
                                                                    const std::string &debugName   = "ColorDepthTarget");

        std::unique_ptr<VOpenGLRenderTarget>
        CreateHDRTarget(uint32_t width, uint32_t height, VTextureFormat hdrFormat = VTextureFormat::RGBA16F, const std::string &debugName = "HDRTarget");

        std::unique_ptr<VOpenGLRenderTarget>
        CreateShadowMapTarget(uint32_t size = 1024, VTextureFormat depthFormat = VTextureFormat::Depth24, const std::string &debugName = "ShadowMap");
    } // namespace VRenderTargetFactory

} // namespace Vantor::RenderDevice