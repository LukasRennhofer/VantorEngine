/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_RenderPath.hpp>

#include <RHI/Interface/VRHI_Device.hpp>
#include <RHI/Interface/VRHI_RenderTarget.hpp>
#include <RHI/Interface/VRHI_RenderPass.hpp>

#include <vector>

namespace VE::Internal::RHI {

class OpenGLRenderPath : IRHIRenderPath {
public:
    OpenGLRenderPath() = default;
    ~OpenGLRenderPath() override = default;

    // Management
    void Initialize(std::shared_ptr<IRHIDevice> rDevice) override { 
        m_Device = rDevice; 

    }

    void Shutdown() override { m_RPasses.clear(); }

    // Execute all RenderPasses and Pipelines
    void Execute() override {
       // TODO: Find solution for Inputs and Output Management of Passes
    }

    // Get the Final Output RenderTarget of Rendering
    void SetOuputTarget(std::shared_ptr<IRHIRenderTarget> target) override { m_OutputTarget = target; }
    std::shared_ptr<IRHIRenderTarget> GetOutputTarget() override { return m_OutputTarget; }

    // Add a pass to the path, optionally with dependencies
    void AddRenderPass(std::shared_ptr<IRHIRenderPass> pass) override { m_RPasses.push_back(pass); }
    size_t GetPassCount() const override { m_RPasses.size(); }
    std::shared_ptr<IRHIRenderPass> GetPass(size_t index) const override { return m_RPasses.at(index); }

private:
    std::vector<std::shared_ptr<IRHIRenderPass>> m_RPasses;

    std::shared_ptr<IRHIDevice> m_Device;
    std::shared_ptr<IRHIRenderTarget> m_OutputTarget;
};

}// namespace VE::Internal::RHI
