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

namespace VE::Internal::RHI {

class IRHIRenderPass; // Forward Decl.
class IRHIRenderTarget; // Forward Decl.
class IRHIDevice; // Forward Decl.

// TODO: Implement with FrameGraph and CommandBuffer
class IRHIRenderPath {
public:
    virtual ~IRHIRenderPath() = default;

    // Management
    virtual void Initialize(std::shared_ptr<IRHIDevice>) = 0;
    virtual void Shutdown() = 0;

    virtual void SetDevice(IRHIDevice* rDevice) = 0;

    // Execute all RenderPasses and Pipelines
    virtual void Execute() = 0;

    // Get and set the Final Output RenderTarget of Rendering
    virtual void SetOuputTarget(std::shared_ptr<IRHIRenderTarget> target) = 0;
    virtual std::shared_ptr<IRHIRenderTarget> GetOutputTarget() = 0;

    // Add a pass to the path, optionally with dependencies
    virtual void AddRenderPass(std::shared_ptr<IRHIRenderPass> pass) = 0;
    virtual size_t GetPassCount() const = 0;
    virtual std::shared_ptr<IRHIRenderPass> GetPass(size_t index) const = 0;
};
} // namespace VE::Internal::RHI
