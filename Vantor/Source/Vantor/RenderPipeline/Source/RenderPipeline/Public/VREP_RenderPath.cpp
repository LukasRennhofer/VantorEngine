/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RenderPipeline/Public/VREP_RenderPath.hpp>
#include <RenderPipeline/Pipeline/VREP_Internal.hpp>

#include <RenderPipeline/Pipeline/VREP_CommandBuffer.hpp>

#include <RHI/Interface/VRHI_Device.hpp>

namespace VE::Render {

    VRenderPath3D::VRenderPath3D() {}

    void VRenderPath3D::Initialize(VE::Internal::RHI::IRHIDevice *device)
    {

        m_CommandBuffer = std::make_unique<VE::Internal::RenderPipeline::VCommandBuffer>();

        m_Device = device;
        SetupDefaultRenderPasses();

        // Initialize all render passes
        for (auto &[type, pass] : m_RenderPasses)
        {
            pass->Initialize(this);
        }
    }

    void VRenderPath3D::Render()
    {
        m_CommandBuffer->Sort();

        // The Geometry Pass
        // auto *geometryPass = GetRenderPass(ERenderPassType::VD_Geometry);
        // if (geometryPass && geometryPass->IsEnabled())
        // {
        //     geometryPass->Execute();
        // }

        // The Forward Pass
        auto *forwardPass = GetRenderPass(ERenderPassType::VF_Forward);
        if (forwardPass && forwardPass->IsEnabled())
        {
            forwardPass->Execute();
        }

        m_CommandBuffer->Clear();
    }

    void VRenderPath3D::Shutdown()
    {
        for (auto &[type, pass] : m_RenderPasses)
        {
            pass->Cleanup();
        }
        m_RenderPasses.clear();
    }

     void VRenderPath3D::PushRender(VE::Graphics::VModel* model, 
                                    VMaterial* material, 
                                    const VE::Math::VMat4& transform) 
    {
        m_CommandBuffer->Push(model, material, transform);
    }

    void VRenderPath3D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        m_ViewportX      = x;
        m_ViewportY      = y;
        m_ViewportWidth  = width;
        m_ViewportHeight = height;
    }

    void VRenderPath3D::AddRenderPass(std::unique_ptr<VRenderPass> pass)
    {
        if (pass)
        {
            ERenderPassType type = pass->GetType();
            m_RenderPasses[type]  = std::move(pass);
        }
    }

    // void VRenderPath3D::RemoveRenderPass(VERenderPassType type)
    // {
    //     // TODO
    //     // auto it = m_RenderPasses.find(type);
    //     // if (it != m_RenderPasses.end())
    //     // {
    //     //     it->second->Cleanup();
    //     //     m_RenderPasses.erase(it);
    //     // }
    // }

    VRenderPass *VRenderPath3D::GetRenderPass(ERenderPassType type) const
    {
        auto it = m_RenderPasses.find(type);
        return (it != m_RenderPasses.end()) ? it->second.get() : nullptr;
    }

    void VRenderPath3D::SetupDefaultRenderPasses()
    {
        // Add Forward Pass
        auto forwardPass = std::make_unique<VE::Internal::RenderPipeline::VForwardRenderPass>();
        AddRenderPass(std::move(forwardPass));
    }
}