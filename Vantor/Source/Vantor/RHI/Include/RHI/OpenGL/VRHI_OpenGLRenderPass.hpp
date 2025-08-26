/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_RenderPass.hpp>

#include <RHI/Interface/VRHI_Device.hpp>
#include <RHI/Interface/VRHI_Texture.hpp>
#include <RHI/Interface/VRHI_RenderTarget.hpp>

#include <unordered_map>
#include <functional>

namespace VE::Internal::RHI {

class OpenGLRenderPass : IRHIRenderPass {
public:
    OpenGLRenderPass() = default;
    ~OpenGLRenderPass() override = default;

    // Execute the RenderPass 
    void Execute() {
        if (!m_Device)
            return;

        if (m_Output) {
            m_Output->Bind(); // TODO: Bind Buffer with Device (but it still bugs)
            m_Output->Clear(0, 0, 0, 1);
            m_Output->ClearDepth(1.0f);
        }

        // Bind inputs
        for (auto& [slot, tex] : m_Inputs)
            m_Device->BindTexture(tex, slot);

        // Execute user callback (drawing)
        if (m_ExecuteCallback)
            m_ExecuteCallback(m_Device);

        // Unbind the render target
        if (m_Output)
            m_Output->Unbind(); // TODO: Unbind Buffer with Device (but it still bugs)
    }

    // Execute User Callback
    void SetExecuteCallback(std::function<void(std::shared_ptr<IRHIDevice>)> callback) { m_ExecuteCallback = std::move(callback); }

    // Pass Input
    void AttachInput(std::shared_ptr<IRHITexture> inputTex, uint32_t bindingPoint) { m_Inputs[bindingPoint] = inputTex; }

    // Pass Output
    void AttachOutput(std::shared_ptr<IRHIRenderTarget> outputTarget) override { m_Output = outputTarget; }
    std::shared_ptr<IRHIRenderTarget> GetOuput() { return m_Output; }

    // For Binding Textures and RenderTargets
    void SetRenderDevice(std::shared_ptr<IRHIDevice> rDevice) override { m_Device = rDevice; }

    // Set Enabled/Disabled for RenderPath
    void SetEnabled(bool enabled) override { m_Enabled = enabled; }
    // Check if Pass is Enabled/Disabled for RenderPath
    bool IsEnabled() override { return m_Enabled; }

private:
    std::shared_ptr<IRHIDevice> m_Device;
    
    std::unordered_map<uint32_t, std::shared_ptr<IRHITexture>> m_Inputs;
    std::shared_ptr<IRHIRenderTarget> m_Output;

    std::function<void(std::shared_ptr<IRHIDevice>)> m_ExecuteCallback;

    bool m_Enabled = true;
};
} // namespace VE::Internal::RHI
