/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <memory>
#include <cstdint>

namespace VE::Internal::RHI
{

// Forward declarations
class IRHITexture;

enum class EAttachmentType
{
    Color,
    Depth,
    Stencil,
    DepthStencil
};

struct VRenderTargetAttachment
{
    EAttachmentType type;
    std::shared_ptr<IRHITexture> texture;
    uint32_t mipLevel = 0;
    uint32_t layer = 0; // For array textures or cubemaps
};

class IRHIRenderTarget
{
public:
    virtual ~IRHIRenderTarget() = default;

    // Render target management
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual bool IsComplete() const = 0;
    
    // Attachment management
    virtual void AttachTexture(EAttachmentType type, std::shared_ptr<IRHITexture> texture, uint32_t mipLevel = 0, uint32_t layer = 0) = 0;
    virtual void DetachTexture(EAttachmentType type) = 0;
    virtual std::shared_ptr<IRHITexture> GetAttachment(EAttachmentType type) const = 0;
    
    // Clear operations
    virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;
    virtual void ClearDepth(float depth = 1.0f) = 0;
    virtual void ClearStencil(int32_t stencil = 0) = 0;
    
    // Properties
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetHandle() const = 0;
    
    // Viewport management
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    
    // Multi-sample support
    virtual uint32_t GetSampleCount() const = 0;
    virtual bool IsMultisampled() const = 0;
};

} // namespace VE::Internal::RHI
