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
 *  File: VRD_RenderTarget.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../../Math/Linear/VMA_Vector.hpp"
#include "VRD_Texture.hpp"
namespace Vantor::RenderDevice
{
    // RenderTarget Attachments
    enum class VEAttachmentType
    {
        Color0,      // First color attachment
        Color1,      // Second color attachment (for MRT)
        Color2,      // Third color attachment
        Color3,      // Fourth color attachment
        Depth,       // Depth attachment
        Stencil,     // Stencil attachment
        DepthStencil // Combined depth-stencil attachment
    };

    // Attachment descriptor for creating render targets
    struct VAttachmentDescriptor
    {
            VEAttachmentType Type            = VEAttachmentType::Color0;
            VTextureFormat   Format          = VTextureFormat::RGBA8;
            VTextureSampler  Sampler         = {};
            bool             GenerateMipmaps = false;
            uint32_t         Samples         = 1; // For multisampling
    };

    // Render target descriptor
    struct VRenderTargetDescriptor
    {
            uint32_t                           Width  = 1920;
            uint32_t                           Height = 1080;
            std::vector<VAttachmentDescriptor> Attachments;
            std::string                        DebugName = "RenderTarget";
            bool                               Resizable = true;
    };

    // Render target clear values
    struct VClearValues
    {
            Vantor::Math::VVector4 ColorValue   = {0.0f, 0.0f, 0.0f, 1.0f};
            float                  DepthValue   = 1.0f;
            uint32_t               StencilValue = 0;
    };

    // Base render target interface
    class VRenderTarget
    {
        public:
            virtual ~VRenderTarget() = default;

            // Creation and destruction
            virtual bool Create(const VRenderTargetDescriptor &descriptor) = 0;
            virtual void Destroy()                                         = 0;

            // Binding and unbinding
            virtual void Bind()          = 0;
            virtual void Unbind()        = 0;
            virtual bool IsBound() const = 0;

            // Clearing
            virtual void Clear()                                                                       = 0;
            virtual void Clear(const VClearValues &clearValues)                                        = 0;
            virtual void ClearAttachment(VEAttachmentType attachment, const VClearValues &clearValues) = 0;

            // Resize support
            virtual void Resize(uint32_t width, uint32_t height) = 0;
            virtual bool IsResizable() const                     = 0;

            // Properties
            virtual uint32_t           GetWidth() const       = 0;
            virtual uint32_t           GetHeight() const      = 0;
            virtual float              GetAspectRatio() const = 0;
            virtual const std::string &GetDebugName() const   = 0;

            // Validation
            virtual bool IsComplete() const = 0;
            virtual bool IsValid() const    = 0;

            // Texture access
            virtual std::shared_ptr<VTexture>              GetColorTexture(uint32_t index = 0) const = 0;
            virtual std::shared_ptr<VTexture>              GetDepthTexture() const                   = 0;
            virtual std::shared_ptr<VTexture>              GetStencilTexture() const                 = 0;
            virtual std::vector<std::shared_ptr<VTexture>> GetAllColorTextures() const               = 0;

            // Multisampling
            virtual uint32_t GetSampleCount() const = 0;
            virtual bool     IsMultisampled() const = 0;

            // Blitting and copying
            virtual void BlitTo(VRenderTarget *destination,
                                uint32_t       srcX0,
                                uint32_t       srcY0,
                                uint32_t       srcX1,
                                uint32_t       srcY1,
                                uint32_t       dstX0,
                                uint32_t       dstY0,
                                uint32_t       dstX1,
                                uint32_t       dstY1,
                                bool           linearFilter = true)
                = 0;

            virtual void CopyTo(VRenderTarget *destination) = 0;

            // Debug and profiling
            virtual size_t GetMemoryUsage() const                = 0;
            virtual void   SetDebugName(const std::string &name) = 0;

        protected:
            uint32_t                m_Width       = 1920;
            uint32_t                m_Height      = 1080;
            bool                    m_IsBound     = false;
            bool                    m_IsResizable = true;
            std::string             m_DebugName   = "RenderTarget";
            VRenderTargetDescriptor m_Descriptor;
    };

    // Utility functions for creating common render target configurations
    namespace VRenderTargetUtils
    {
        VRenderTargetDescriptor
        CreateColorTarget(uint32_t width, uint32_t height, VTextureFormat format = VTextureFormat::RGBA8, const std::string &debugName = "ColorTarget");

        VRenderTargetDescriptor CreateColorDepthTarget(uint32_t           width,
                                                       uint32_t           height,
                                                       VTextureFormat     colorFormat = VTextureFormat::RGBA8,
                                                       VTextureFormat     depthFormat = VTextureFormat::Depth24,
                                                       const std::string &debugName   = "ColorDepthTarget");

        VRenderTargetDescriptor
        CreateHDRTarget(uint32_t width, uint32_t height, VTextureFormat hdrFormat = VTextureFormat::RGBA16F, const std::string &debugName = "HDRTarget");

        VRenderTargetDescriptor CreateMRTTarget(uint32_t                           width,
                                                uint32_t                           height,
                                                const std::vector<VTextureFormat> &colorFormats,
                                                VTextureFormat                     depthFormat = VTextureFormat::Depth24,
                                                const std::string                 &debugName   = "MRTTarget");
    } // namespace VRenderTargetUtils
} // namespace Vantor::RenderDevice