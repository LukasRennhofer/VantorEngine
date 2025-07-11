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
 *  Date: 2025-07-11
 *
 *  File: VRDO_RenderTarget.cpp
 *  Last Change: Automatically updated
 */

#include <algorithm>
#include <iostream>

#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "VRDO_RenderTarget.hpp"

namespace Vantor::RenderDevice
{

    // ----------------- VOpenGLRenderTarget Implementation -----------------
    VOpenGLRenderTarget::VOpenGLRenderTarget() : m_FramebufferID(0), m_PreviousFramebufferID(0) {}

    VOpenGLRenderTarget::~VOpenGLRenderTarget() { Destroy(); }

    VOpenGLRenderTarget::VOpenGLRenderTarget(VOpenGLRenderTarget &&other) noexcept { MoveFrom(std::move(other)); }

    VOpenGLRenderTarget &VOpenGLRenderTarget::operator=(VOpenGLRenderTarget &&other) noexcept
    {
        if (this != &other)
        {
            Destroy();
            MoveFrom(std::move(other));
        }
        return *this;
    }

    void VOpenGLRenderTarget::MoveFrom(VOpenGLRenderTarget &&other) noexcept
    {
        m_FramebufferID         = other.m_FramebufferID;
        m_PreviousFramebufferID = other.m_PreviousFramebufferID;
        m_Width                 = other.m_Width;
        m_Height                = other.m_Height;
        m_IsBound               = other.m_IsBound;
        m_IsResizable           = other.m_IsResizable;
        m_DebugName             = std::move(other.m_DebugName);
        m_Descriptor            = std::move(other.m_Descriptor);
        m_ColorTextures         = std::move(other.m_ColorTextures);
        m_DepthTexture          = std::move(other.m_DepthTexture);
        m_StencilTexture        = std::move(other.m_StencilTexture);

        other.m_FramebufferID         = 0;
        other.m_PreviousFramebufferID = 0;
        other.m_IsBound               = false;
    }

    bool VOpenGLRenderTarget::Create(const VRenderTargetDescriptor &descriptor)
    {
        // Validate descriptor
        if (descriptor.Width == 0 || descriptor.Height == 0)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Invalid dimensions", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        if (descriptor.Attachments.empty())
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "No attachments specified", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        // Clean up existing resources
        Destroy();

        // Store descriptor
        m_Descriptor  = descriptor;
        m_Width       = descriptor.Width;
        m_Height      = descriptor.Height;
        m_DebugName   = descriptor.DebugName;
        m_IsResizable = descriptor.Resizable;

        // Create framebuffer
        glGenFramebuffers(1, &m_FramebufferID);
        if (m_FramebufferID == 0)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Failed to create framebuffer", Vantor::Backlog::LogLevel::ERR);
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

        // Create attachments
        CreateAttachments();

        // Check framebuffer completeness
        CheckFramebufferStatus();

        // Set debug name if available
        if (!m_DebugName.empty())
        {
            glObjectLabel(GL_FRAMEBUFFER, m_FramebufferID, m_DebugName.length(), m_DebugName.c_str());
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Vantor::Backlog::Log("VOpenGLRenderTarget",
                             "Created render target: " + m_DebugName + " (" + std::to_string(m_Width) + "x" + std::to_string(m_Height) + ")",
                             Vantor::Backlog::LogLevel::INFO);

        return true;
    }

    void VOpenGLRenderTarget::Destroy()
    {
        if (m_IsBound)
        {
            Unbind();
        }

        DestroyAttachments();

        if (m_FramebufferID != 0)
        {
            glDeleteFramebuffers(1, &m_FramebufferID);
            m_FramebufferID = 0;
        }

        m_Width  = 0;
        m_Height = 0;
    }

    void VOpenGLRenderTarget::Bind()
    {
        if (m_FramebufferID == 0)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Cannot bind invalid framebuffer", Vantor::Backlog::LogLevel::ERR);
            return;
        }

        // Store previous framebuffer
        GLint currentFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
        m_PreviousFramebufferID = static_cast<GLuint>(currentFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
        glViewport(0, 0, m_Width, m_Height);

        m_IsBound = true;
    }

    void VOpenGLRenderTarget::Unbind()
    {
        if (m_IsBound)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_PreviousFramebufferID);
            m_IsBound = false;
        }
    }

    bool VOpenGLRenderTarget::IsBound() const { return m_IsBound; }

    void VOpenGLRenderTarget::Clear()
    {
        VClearValues defaultValues;
        Clear(defaultValues);
    }

    void VOpenGLRenderTarget::Clear(const VClearValues &clearValues)
    {
        if (!m_IsBound)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Cannot clear unbound framebuffer", Vantor::Backlog::LogLevel::WARNING);
            return;
        }

        GLbitfield clearMask = 0;

        // Clear color attachments
        if (!m_ColorTextures.empty())
        {
            glClearColor(clearValues.ColorValue.x, clearValues.ColorValue.y, clearValues.ColorValue.z, clearValues.ColorValue.w);
            clearMask |= GL_COLOR_BUFFER_BIT;
        }

        // Clear depth
        if (m_DepthTexture)
        {
            glClearDepth(clearValues.DepthValue);
            clearMask |= GL_DEPTH_BUFFER_BIT;
        }

        // Clear stencil
        if (m_StencilTexture)
        {
            glClearStencil(clearValues.StencilValue);
            clearMask |= GL_STENCIL_BUFFER_BIT;
        }

        if (clearMask != 0)
        {
            glClear(clearMask);
        }
    }

    void VOpenGLRenderTarget::ClearAttachment(VEAttachmentType attachment, const VClearValues &clearValues)
    {
        if (!m_IsBound)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Cannot clear unbound framebuffer", Vantor::Backlog::LogLevel::WARNING);
            return;
        }

        switch (attachment)
        {
            case VEAttachmentType::Color0:
            case VEAttachmentType::Color1:
            case VEAttachmentType::Color2:
            case VEAttachmentType::Color3:
            {
                uint32_t colorIndex = static_cast<uint32_t>(attachment) - static_cast<uint32_t>(VEAttachmentType::Color0);
                if (colorIndex < m_ColorTextures.size())
                {
                    float color[4] = {clearValues.ColorValue.x, clearValues.ColorValue.y, clearValues.ColorValue.z, clearValues.ColorValue.w};
                    glClearBufferfv(GL_COLOR, colorIndex, color);
                }
                break;
            }
            case VEAttachmentType::Depth:
                if (m_DepthTexture)
                {
                    glClearBufferfv(GL_DEPTH, 0, &clearValues.DepthValue);
                }
                break;
            case VEAttachmentType::Stencil:
                if (m_StencilTexture)
                {
                    GLint stencilValue = static_cast<GLint>(clearValues.StencilValue);
                    glClearBufferiv(GL_STENCIL, 0, &stencilValue);
                }
                break;
            case VEAttachmentType::DepthStencil:
                if (m_DepthTexture)
                {
                    glClearBufferfi(GL_DEPTH_STENCIL, 0, clearValues.DepthValue, clearValues.StencilValue);
                }
                break;
        }
    }

    void VOpenGLRenderTarget::Resize(uint32_t width, uint32_t height)
    {
        if (!m_IsResizable)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Render target is not resizable", Vantor::Backlog::LogLevel::WARNING);
            return;
        }

        if (width == m_Width && height == m_Height)
        {
            return; // No change needed
        }

        // Update descriptor and recreate
        m_Descriptor.Width  = width;
        m_Descriptor.Height = height;

        bool wasBound = m_IsBound;
        if (wasBound)
        {
            Unbind();
        }

        if (!Create(m_Descriptor))
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Failed to resize render target", Vantor::Backlog::LogLevel::ERR);
        }

        if (wasBound)
        {
            Bind();
        }
    }

    bool VOpenGLRenderTarget::IsResizable() const { return m_IsResizable; }

    uint32_t VOpenGLRenderTarget::GetWidth() const { return m_Width; }

    uint32_t VOpenGLRenderTarget::GetHeight() const { return m_Height; }

    float VOpenGLRenderTarget::GetAspectRatio() const { return m_Height > 0 ? static_cast<float>(m_Width) / static_cast<float>(m_Height) : 1.0f; }

    const std::string &VOpenGLRenderTarget::GetDebugName() const { return m_DebugName; }

    bool VOpenGLRenderTarget::IsComplete() const
    {
        if (m_FramebufferID == 0)
        {
            return false;
        }

        GLint currentFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);

        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    bool VOpenGLRenderTarget::IsValid() const { return m_FramebufferID != 0 && glIsFramebuffer(m_FramebufferID) == GL_TRUE; }

    std::shared_ptr<VTexture> VOpenGLRenderTarget::GetColorTexture(uint32_t index) const
    {
        if (index < m_ColorTextures.size())
        {
            return m_ColorTextures[index];
        }
        return nullptr;
    }

    std::shared_ptr<VTexture> VOpenGLRenderTarget::GetDepthTexture() const { return m_DepthTexture; }

    std::shared_ptr<VTexture> VOpenGLRenderTarget::GetStencilTexture() const { return m_StencilTexture; }

    std::vector<std::shared_ptr<VTexture>> VOpenGLRenderTarget::GetAllColorTextures() const
    {
        std::vector<std::shared_ptr<VTexture>> textures;
        textures.reserve(m_ColorTextures.size());

        for (const auto &texture : m_ColorTextures)
        {
            textures.push_back(texture);
        }

        return textures;
    }

    uint32_t VOpenGLRenderTarget::GetSampleCount() const
    {
        // For now, return 1 (no multisampling)
        // This can be extended later for MSAA support
        return 1;
    }

    bool VOpenGLRenderTarget::IsMultisampled() const { return GetSampleCount() > 1; }

    void VOpenGLRenderTarget::BlitTo(VRenderTarget *destination,
                                     uint32_t       srcX0,
                                     uint32_t       srcY0,
                                     uint32_t       srcX1,
                                     uint32_t       srcY1,
                                     uint32_t       dstX0,
                                     uint32_t       dstY0,
                                     uint32_t       dstX1,
                                     uint32_t       dstY1,
                                     bool           linearFilter)
    {
        auto *glDestination = dynamic_cast<VOpenGLRenderTarget *>(destination);
        if (!glDestination)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Destination is not an OpenGL render target", Vantor::Backlog::LogLevel::ERR);
            return;
        }

        GLint currentDrawFBO, currentReadFBO;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFBO);
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFBO);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glDestination->GetFramebufferID());

        GLbitfield mask = GL_COLOR_BUFFER_BIT;
        if (m_DepthTexture && glDestination->GetDepthTexture())
        {
            mask |= GL_DEPTH_BUFFER_BIT;
        }

        GLenum filter = linearFilter ? GL_LINEAR : GL_NEAREST;
        glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFBO);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFBO);
    }

    void VOpenGLRenderTarget::CopyTo(VRenderTarget *destination)
    {
        BlitTo(destination, 0, 0, m_Width, m_Height, 0, 0, destination->GetWidth(), destination->GetHeight(), true);
    }

    size_t VOpenGLRenderTarget::GetMemoryUsage() const
    {
        size_t totalMemory = 0;

        for (const auto &texture : m_ColorTextures)
        {
            if (texture)
            {
                totalMemory += texture->GetMemoryUsage();
            }
        }

        if (m_DepthTexture)
        {
            totalMemory += m_DepthTexture->GetMemoryUsage();
        }

        if (m_StencilTexture)
        {
            totalMemory += m_StencilTexture->GetMemoryUsage();
        }

        return totalMemory;
    }

    void VOpenGLRenderTarget::SetDebugName(const std::string &name)
    {
        m_DebugName = name;
        if (m_FramebufferID != 0)
        {
            glObjectLabel(GL_FRAMEBUFFER, m_FramebufferID, name.length(), name.c_str());
        }
    }

    void VOpenGLRenderTarget::CreateAttachments()
    {
        m_ColorTextures.clear();
        m_DepthTexture.reset();
        m_StencilTexture.reset();

        std::vector<GLenum> drawBuffers;

        for (const auto &attachmentDesc : m_Descriptor.Attachments)
        {
            auto texture = CreateTextureAttachment(attachmentDesc);
            if (!texture)
            {
                continue;
            }

            GLenum attachment = GetOpenGLAttachment(attachmentDesc.Type);
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->GetTextureID(), 0);

            switch (attachmentDesc.Type)
            {
                case VEAttachmentType::Color0:
                case VEAttachmentType::Color1:
                case VEAttachmentType::Color2:
                case VEAttachmentType::Color3:
                    m_ColorTextures.push_back(texture);
                    drawBuffers.push_back(attachment);
                    break;
                case VEAttachmentType::Depth:
                case VEAttachmentType::DepthStencil:
                    m_DepthTexture = texture;
                    break;
                case VEAttachmentType::Stencil:
                    m_StencilTexture = texture;
                    break;
            }
        }

        // Set draw buffers for color attachments
        if (!drawBuffers.empty())
        {
            glDrawBuffers(drawBuffers.size(), drawBuffers.data());
        }
        else
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
    }

    void VOpenGLRenderTarget::DestroyAttachments()
    {
        m_ColorTextures.clear();
        m_DepthTexture.reset();
        m_StencilTexture.reset();
    }

    std::shared_ptr<VOpenGLTexture2D> VOpenGLRenderTarget::CreateTextureAttachment(const VAttachmentDescriptor &desc)
    {
        VTextureSampler sampler = desc.Sampler;
        sampler.wrapS           = VTextureWrap::ClampToEdge;
        sampler.wrapT           = VTextureWrap::ClampToEdge;
        sampler.generateMipmaps = desc.GenerateMipmaps;

        auto texture = VOpenGLTexture2D::CreateEmpty(m_Width, m_Height, desc.Format, sampler);
        if (!texture)
        {
            Vantor::Backlog::Log("VOpenGLRenderTarget", "Failed to create texture attachment", Vantor::Backlog::LogLevel::ERR);
        }

        return texture;
    }

    GLenum VOpenGLRenderTarget::GetOpenGLAttachment(VEAttachmentType type) const
    {
        switch (type)
        {
            case VEAttachmentType::Color0:
                return GL_COLOR_ATTACHMENT0;
            case VEAttachmentType::Color1:
                return GL_COLOR_ATTACHMENT1;
            case VEAttachmentType::Color2:
                return GL_COLOR_ATTACHMENT2;
            case VEAttachmentType::Color3:
                return GL_COLOR_ATTACHMENT3;
            case VEAttachmentType::Depth:
                return GL_DEPTH_ATTACHMENT;
            case VEAttachmentType::Stencil:
                return GL_STENCIL_ATTACHMENT;
            case VEAttachmentType::DepthStencil:
                return GL_DEPTH_STENCIL_ATTACHMENT;
            default:
                return GL_COLOR_ATTACHMENT0;
        }
    }

    void VOpenGLRenderTarget::CheckFramebufferStatus() const
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::string errorMsg = "Framebuffer not complete: ";
            switch (status)
            {
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    errorMsg += "Incomplete attachment";
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    errorMsg += "Missing attachment";
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    errorMsg += "Unsupported framebuffer format";
                    break;
                default:
                    errorMsg += "Unknown error (" + std::to_string(status) + ")";
                    break;
            }
            Vantor::Backlog::Log("VOpenGLRenderTarget", errorMsg, Vantor::Backlog::LogLevel::ERR);
        }
    }

    // ----------------- Utility Functions Implementation -----------------

    namespace RenderTargetUtils
    {
        VRenderTargetDescriptor CreateColorTarget(uint32_t width, uint32_t height, VTextureFormat format, const std::string &debugName)
        {
            VRenderTargetDescriptor desc;
            desc.Width     = width;
            desc.Height    = height;
            desc.DebugName = debugName;

            VAttachmentDescriptor colorAttachment;
            colorAttachment.Type              = VEAttachmentType::Color0;
            colorAttachment.Format            = format;
            colorAttachment.Sampler.minFilter = VTextureFilter::Linear;
            colorAttachment.Sampler.magFilter = VTextureFilter::Linear;
            colorAttachment.Sampler.wrapS     = VTextureWrap::ClampToEdge;
            colorAttachment.Sampler.wrapT     = VTextureWrap::ClampToEdge;

            desc.Attachments.push_back(colorAttachment);
            return desc;
        }

        VRenderTargetDescriptor
        CreateColorDepthTarget(uint32_t width, uint32_t height, VTextureFormat colorFormat, VTextureFormat depthFormat, const std::string &debugName)
        {
            VRenderTargetDescriptor desc;
            desc.Width     = width;
            desc.Height    = height;
            desc.DebugName = debugName;

            VAttachmentDescriptor colorAttachment;
            colorAttachment.Type              = VEAttachmentType::Color0;
            colorAttachment.Format            = colorFormat;
            colorAttachment.Sampler.minFilter = VTextureFilter::Linear;
            colorAttachment.Sampler.magFilter = VTextureFilter::Linear;
            colorAttachment.Sampler.wrapS     = VTextureWrap::ClampToEdge;
            colorAttachment.Sampler.wrapT     = VTextureWrap::ClampToEdge;

            VAttachmentDescriptor depthAttachment;
            depthAttachment.Type              = VEAttachmentType::Depth;
            depthAttachment.Format            = depthFormat;
            depthAttachment.Sampler.minFilter = VTextureFilter::Linear;
            depthAttachment.Sampler.magFilter = VTextureFilter::Linear;
            depthAttachment.Sampler.wrapS     = VTextureWrap::ClampToEdge;
            depthAttachment.Sampler.wrapT     = VTextureWrap::ClampToEdge;

            desc.Attachments.push_back(colorAttachment);
            desc.Attachments.push_back(depthAttachment);
            return desc;
        }

        VRenderTargetDescriptor CreateHDRTarget(uint32_t width, uint32_t height, VTextureFormat hdrFormat, const std::string &debugName)
        {
            return CreateColorTarget(width, height, hdrFormat, debugName);
        }

        VRenderTargetDescriptor CreateMRTTarget(uint32_t                           width,
                                                uint32_t                           height,
                                                const std::vector<VTextureFormat> &colorFormats,
                                                VTextureFormat                     depthFormat,
                                                const std::string                 &debugName)
        {
            VRenderTargetDescriptor desc;
            desc.Width     = width;
            desc.Height    = height;
            desc.DebugName = debugName;

            // Add color attachments
            for (size_t i = 0; i < colorFormats.size() && i < 4; ++i)
            {
                VAttachmentDescriptor colorAttachment;
                colorAttachment.Type              = static_cast<VEAttachmentType>(static_cast<uint32_t>(VEAttachmentType::Color0) + i);
                colorAttachment.Format            = colorFormats[i];
                colorAttachment.Sampler.minFilter = VTextureFilter::Linear;
                colorAttachment.Sampler.magFilter = VTextureFilter::Linear;
                colorAttachment.Sampler.wrapS     = VTextureWrap::ClampToEdge;
                colorAttachment.Sampler.wrapT     = VTextureWrap::ClampToEdge;

                desc.Attachments.push_back(colorAttachment);
            }

            // Add depth attachment
            VAttachmentDescriptor depthAttachment;
            depthAttachment.Type              = VEAttachmentType::Depth;
            depthAttachment.Format            = depthFormat;
            depthAttachment.Sampler.minFilter = VTextureFilter::Linear;
            depthAttachment.Sampler.magFilter = VTextureFilter::Linear;
            depthAttachment.Sampler.wrapS     = VTextureWrap::ClampToEdge;
            depthAttachment.Sampler.wrapT     = VTextureWrap::ClampToEdge;

            desc.Attachments.push_back(depthAttachment);
            return desc;
        }
    } // namespace RenderTargetUtils

    // ----------------- Factory Functions Implementation -----------------

    namespace RenderTargetFactory
    {
        std::unique_ptr<VOpenGLRenderTarget> CreateColorTarget(uint32_t width, uint32_t height, VTextureFormat format, const std::string &debugName)
        {
            auto renderTarget = std::make_unique<VOpenGLRenderTarget>();
            auto desc         = RenderTargetUtils::CreateColorTarget(width, height, format, debugName);

            if (renderTarget->Create(desc))
            {
                return renderTarget;
            }

            return nullptr;
        }

        std::unique_ptr<VOpenGLRenderTarget>
        CreateColorDepthTarget(uint32_t width, uint32_t height, VTextureFormat colorFormat, VTextureFormat depthFormat, const std::string &debugName)
        {
            auto renderTarget = std::make_unique<VOpenGLRenderTarget>();
            auto desc         = RenderTargetUtils::CreateColorDepthTarget(width, height, colorFormat, depthFormat, debugName);

            if (renderTarget->Create(desc))
            {
                return renderTarget;
            }

            return nullptr;
        }

        std::unique_ptr<VOpenGLRenderTarget> CreateHDRTarget(uint32_t width, uint32_t height, VTextureFormat hdrFormat, const std::string &debugName)
        {
            return CreateColorTarget(width, height, hdrFormat, debugName);
        }

        std::unique_ptr<VOpenGLRenderTarget> CreateShadowMapTarget(uint32_t size, VTextureFormat depthFormat, const std::string &debugName)
        {
            auto renderTarget = std::make_unique<VOpenGLRenderTarget>();

            VRenderTargetDescriptor desc;
            desc.Width     = size;
            desc.Height    = size;
            desc.DebugName = debugName;

            VAttachmentDescriptor depthAttachment;
            depthAttachment.Type                = VEAttachmentType::Depth;
            depthAttachment.Format              = depthFormat;
            depthAttachment.Sampler.minFilter   = VTextureFilter::Linear;
            depthAttachment.Sampler.magFilter   = VTextureFilter::Linear;
            depthAttachment.Sampler.wrapS       = VTextureWrap::ClampToBorder;
            depthAttachment.Sampler.wrapT       = VTextureWrap::ClampToBorder;
            depthAttachment.Sampler.borderColor = {1.0f, 1.0f, 1.0f, 1.0f}; // White border for shadow comparison

            desc.Attachments.push_back(depthAttachment);

            if (renderTarget->Create(desc))
            {
                return renderTarget;
            }

            return nullptr;
        }
    } // namespace RenderTargetFactory

} // namespace Vantor::RenderDevice