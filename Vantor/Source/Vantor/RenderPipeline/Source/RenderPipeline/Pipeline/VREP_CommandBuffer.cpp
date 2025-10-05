/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RenderPipeline/Pipeline/VREP_CommandBuffer.hpp>

#include <RHI/Interface/VRHI_Mesh.hpp>
#include <RHI/Interface/VRHI_Shader.hpp>

namespace VE::Internal::RenderPipeline {

    VCommandBuffer::VCommandBuffer() {}

    VCommandBuffer::~VCommandBuffer() noexcept { Clear(); }

    void VCommandBuffer::Push(VE::Graphics::VModel* model, VE::VMaterial* material, const VE::Math::VMat4& transform)
    {
        VRenderCommand command = {};
        command.Model          = model;
        command.Transform      = transform;
        command.Material       = material;

        // Just for now
        m_ForwardRenderCommands.push_back(command);
    }

    void VCommandBuffer::Clear()
    {
        m_ForwardRenderCommands.clear();
        m_DeferredRenderCommands.clear();
    }

    // TODO
    bool renderSortForward(const VRenderCommand &a, const VRenderCommand &b)
    {
        // TODO : Becuase we have no Materials yet
        return true;
    }

    // custom per-element sort compare function used by the VCommandBuffer::Sort() function.
    // bool renderSortDeferred(const VRenderCommand &a, const VRenderCommand &b)
    // {
    //     return a.Material->GetShader()->GetShaderID() < b.Material->GetShader()->GetShaderID();
    // }
    // sort render state
    // TODO
    bool renderSortCustom(const VRenderCommand &a, const VRenderCommand &b)
    {
        // return std::make_tuple(a.Material->Blend, a.Material->GetShader()->ID) <
        //        std::make_tuple(b.Material->Blend, b.Material->GetShader()->ID);
        return true;
    }

    // TODO
    bool renderSortShader(const VRenderCommand &a, const VRenderCommand &b)
    {
        // return a.Material->GetShader()->ID < b.Material->GetShader()->ID;
        return true;
    }

    void VCommandBuffer::Sort()
    {
        // TODO
        // std::sort(m_DeferredRenderCommands.begin(), m_DeferredRenderCommands.end(), renderSortDeferred);
        // for (auto rtIt = m_CustomRenderCommands.begin(); rtIt != m_CustomRenderCommands.end(); rtIt++)
        // {
        //     std::sort(rtIt->second.begin(), rtIt->second.end(), renderSortCustom);
        // }
    }

    std::vector<VRenderCommand> VCommandBuffer::GetForwardRenderCommands(bool cull)
    {
        // TODO: Work with Camera Frustum
        return m_ForwardRenderCommands;
    }

    std::vector<VRenderCommand> VCommandBuffer::GetDefferedRenderCommands(bool cull) { return m_DeferredRenderCommands; }
}