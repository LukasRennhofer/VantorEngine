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
#include <functional>

namespace VE::Internal::RHI {

class IRHIDevice; // Forward Decl. for binding
class IRHIRenderTarget; // Forward Decl. for Pass Outputs
class IRHITexture; // Forward Decl. for Pass Inputs

// struct VRHIPipeline;

class IRHIRenderPass {
public:
    virtual ~IRHIRenderPass() = default;

    // Execute the RenderPass
    virtual void Execute() = 0; // Execute RenderPass with Input/Ouput

    // Execute User Callback
    virtual void SetExecuteCallback(std::function<void(std::shared_ptr<IRHIDevice>)> callback) = 0;

    // Pass Input
    virtual void AttachInput(std::shared_ptr<IRHITexture> inputTex, uint32_t bindingPoint) = 0;

    // Pass Output
    virtual void AttachOutput(std::shared_ptr<IRHIRenderTarget> outputTarget) = 0;
    virtual std::shared_ptr<IRHIRenderTarget> GetOuput() = 0;

    // For Binding Textures and RenderTargets
    virtual void SetRenderDevice(std::shared_ptr<IRHIDevice> rDevice) = 0;

    // Set Enabled/Disabled for RenderPath
    virtual void SetEnabled(bool enabled) = 0;
    // Check if Pass is Enabled/Disabled for RenderPath
    virtual bool IsEnabled() = 0;
};

} // namespace VE::Internal::RHI
