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
 *  File: VRDO_CommandBuffer.cpp
 *  Last Change: Automatically updated
 */

#include <algorithm>

#include "VRDO_CommandBuffer.hpp"

namespace Vantor::RenderDevice
{
    VCommandBuffer::~VCommandBuffer() noexcept
    {
        // empty body, needed by linker
    }

    VOpenGLCommandBuffer::VOpenGLCommandBuffer() {}

    VOpenGLCommandBuffer::~VOpenGLCommandBuffer() noexcept { Clear(); }

    void VOpenGLCommandBuffer::Push(VMesh *mesh, Vantor::Renderer::VMaterial *material, Vantor::Math::VMat4 transform)
    {
        VRenderCommand command = {};
        command.Mesh           = mesh;
        command.Transform      = transform;
        command.Material       = material;

        // Just for now we will put the default command into the deffered  pass
        m_DeferredRenderCommands.push_back(command);

        // if material requires alpha support, add it to alpha render commands for later rendering.
        // TODO
        // if (material->Blend)
        // {
        //     material->Type = MATERIAL_CUSTOM;
        //     m_AlphaRenderCommands.push_back(command);
        // }
        // else
        // {
        //     // check the type of the material and process differently where necessary
        //     if (material->Type == MATERIAL_DEFAULT)
        //     {
        //         m_DeferredRenderCommands.push_back(command);
        //     }
        //     else if (material->Type == MATERIAL_CUSTOM)
        //     {
        //         // check if this render target has been pushed before, if so add to vector,
        //         // otherwise create new vector with this render target.
        //         if (m_CustomRenderCommands.find(target) != m_CustomRenderCommands.end())
        //             m_CustomRenderCommands[target].push_back(command);
        //         else
        //         {
        //             m_CustomRenderCommands[target] = std::vector<RenderCommand>();
        //             m_CustomRenderCommands[target].push_back(command);
        //         }
        //     }
        //     else if (material->Type == MATERIAL_POST_PROCESS)
        //     {
        //         m_PostProcessingRenderCommands.push_back(command);
        //     }
        // }
    }

    void VOpenGLCommandBuffer::Clear()
    {
        m_ForwardRenderCommands.clear();
        m_DeferredRenderCommands.clear();
        // TODO
        // // m_CustomRenderCommands.clear();
        // m_PostProcessingRenderCommands.clear();
        // m_AlphaRenderCommands.clear();
    }

    // TODO
    bool renderSortForward(const VRenderCommand &a, const VRenderCommand &b)
    {
        // TODO : Becuase we have no Materials yet
        return true;
    }

    // custom per-element sort compare function used by the VOpenGLCommandBuffer::Sort() function.
    bool renderSortDeferred(const VRenderCommand &a, const VRenderCommand &b)
    {
        return a.Material->GetShader()->GetShaderID() < b.Material->GetShader()->GetShaderID();
    }
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

    void VOpenGLCommandBuffer::Sort()
    {
        // TODO
        std::sort(m_DeferredRenderCommands.begin(), m_DeferredRenderCommands.end(), renderSortDeferred);
        // for (auto rtIt = m_CustomRenderCommands.begin(); rtIt != m_CustomRenderCommands.end(); rtIt++)
        // {
        //     std::sort(rtIt->second.begin(), rtIt->second.end(), renderSortCustom);
        // }
    }

    std::vector<VRenderCommand> VOpenGLCommandBuffer::GetForwardRenderCommands(bool cull)
    {
        // TODO: Work with Camera Frustum
        return m_ForwardRenderCommands;
    }

    std::vector<VRenderCommand> VOpenGLCommandBuffer::GetDefferedRenderCommands(bool cull) { return m_DeferredRenderCommands; }

    // std::vector<VRenderCommand> VOpenGLCommandBuffer::GetDeferredRenderCommands(bool cull)
    // {
    //     // TODO : Work with Camera Frustum
    //     // if (cull)
    //     // {
    //     //     std::vector<VRenderCommand> commands;
    //     //     for (auto it = m_DeferredRenderCommands.begin(); it != m_DeferredRenderCommands.end(); ++it)
    //     //     {
    //     //         VRenderCommand command = *it;
    //     //         if (m_Renderer->GetCamera()->Frustum.Intersect(command.BoxMin, command.BoxMax)) {
    //     //             commands.push_back(command);
    //     //         }
    //     //     }
    //     //     return commands;
    //     // }
    //     // else
    //     // {
    //     //     return m_DeferredRenderCommands;
    //     // }
    //     return m_DeferredRenderCommands;
    // }
    // // TODO
    // // std::vector<VRenderCommand> VOpenGLCommandBuffer::GetCustomRenderCommands(RenderTarget *target, bool cull)
    // // {
    // //     // TODO
    // //     // only cull when on main/null render target
    // //     if (target == nullptr && cull)
    // //     {
    // //         std::vector<RenderCommand> commands;
    // //         for (auto it = m_CustomRenderCommands[target].begin(); it != m_CustomRenderCommands[target].end(); ++it)
    // //         {
    // //             RenderCommand command = *it;
    // //             if (m_Renderer->GetCamera()->Frustum.Intersect(command.BoxMin, command.BoxMax)) {
    // //                 commands.push_back(command);
    // //             }
    // //         }
    // //         return commands;
    // //     }
    // //     else
    // //     {
    // //         return m_CustomRenderCommands[target];
    // //     }
    // // }

    // std::vector<VRenderCommand> VOpenGLCommandBuffer::GetAlphaRenderCommands(bool cull)
    // {
    //     // TODO
    //     // if (cull)
    //     // {
    //     //     std::vector<RenderCommand> commands;
    //     //     for (auto it = m_AlphaRenderCommands.begin(); it != m_AlphaRenderCommands.end(); ++it)
    //     //     {
    //     //         RenderCommand command = *it;
    //     //         if (m_Renderer->GetCamera()->Frustum.Intersect(command.BoxMin, command.BoxMax)) {
    //     //             commands.push_back(command);
    //     //         }
    //     //     }
    //     //     return commands;
    //     // }
    //     // else
    //     // {
    //     //     return m_AlphaRenderCommands;
    //     // }
    //     return m_AlphaRenderCommands;
    // }

    // std::vector<VRenderCommand> VOpenGLCommandBuffer::GetPostProcessingRenderCommands() { return m_PostProcessingRenderCommands; }

    // std::vector<VRenderCommand> VOpenGLCommandBuffer::GetShadowCastRenderCommands()
    // {
    //     // TODO
    //     // std::vector<VRenderCommand> commands;
    //     // for (auto it = m_DeferredRenderCommands.begin(); it != m_DeferredRenderCommands.end(); ++it)
    //     // {
    //     //     if (it->Material->ShadowCast)
    //     //     {
    //     //         commands.push_back(*it);
    //     //     }
    //     // }
    //     // for (auto it = m_CustomRenderCommands[nullptr].begin(); it != m_CustomRenderCommands[nullptr].end(); ++it)
    //     // {
    //     //     if (it->Material->ShadowCast)
    //     //     {
    //     //         commands.push_back(*it);
    //     //     }
    //     // }
    //     // return commands;
    //     // This is just so the compiler behaves right, delete it after its alright
    //     std::vector<VRenderCommand> commands;
    //     return commands;
    // }
} // namespace Vantor::RenderDevice