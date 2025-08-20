/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_RenderTarget.hpp>
#include <RHI/Interface/VRHI_Texture.hpp>

#include <Core/Container/VCO_Vector.hpp>

#include <Shared/glad/glad.h>
#include <unordered_map>

namespace Vantor::RHI
{

class OpenGLRenderTarget : public IRHIRenderTarget
{
public:
    OpenGLRenderTarget(uint32_t width, uint32_t height, uint32_t samples = 1);
    virtual ~OpenGLRenderTarget();

    // IRHIRenderTarget implementation
    void Bind() override;
    void Unbind() override;
    bool IsComplete() const override;
    
    void AttachTexture(EAttachmentType type, std::shared_ptr<IRHITexture> texture, uint32_t mipLevel = 0, uint32_t layer = 0) override;
    void DetachTexture(EAttachmentType type) override;
    std::shared_ptr<IRHITexture> GetAttachment(EAttachmentType type) const override;
    
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) override;
    void ClearDepth(float depth = 1.0f) override;
    void ClearStencil(int32_t stencil = 0) override;
    
    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    uint32_t GetHandle() const override { return m_framebuffer; }
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    
    uint32_t GetSampleCount() const override { return m_samples; }
    bool IsMultisampled() const override { return m_samples > 1; }

    // OpenGL specific
    static GLenum AttachmentTypeToGL(EAttachmentType type);
    static GLenum GetColorAttachment(uint32_t index);

private:
    void CheckFramebufferStatus() const;
    void UpdateDrawBuffers();

private:
    uint32_t m_framebuffer;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_samples;
    
    std::unordered_map<EAttachmentType, VRenderTargetAttachment> m_attachments;
    Vantor::Core::Container::TVector<GLenum> m_drawBuffers;
    uint32_t m_colorAttachmentCount;
    
    static uint32_t s_boundFramebuffer;
};

} // namespace Vantor::RHI
