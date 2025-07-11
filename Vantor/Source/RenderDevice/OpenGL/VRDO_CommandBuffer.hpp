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
 *  Date: 2025-07-09
 *
 *  File: VRDO_CommandBuffer.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../Interface/VRD_CommandBuffer.hpp"

#include <map>

namespace Vantor::RenderDevice
{
    class VOpenGLCommandBuffer : public VCommandBuffer
    {
        public:
            VOpenGLCommandBuffer();
            ~VOpenGLCommandBuffer() override;

            // pushes render state relevant to a single render call to the command buffer.
            void Push(VMesh *mesh, Vantor::Math::VMat4 transform, Vantor::Math::VMat4 prevTransform) override;
            // clears the command buffer; usually done after issuing all the stored render commands.
            void Clear() override;
            // sorts the command buffer; first by shader, then by texture bind.
            void Sort() override;

            std::vector<VRenderCommand> GetForwardRenderCommands(bool cull = false) override;

            // // returns the list of render commands. For minimizing state changes it is advised to first
            // // call Sort() before retrieving and issuing the render commands.
            // std::vector<VRenderCommand> GetDeferredRenderCommands(bool cull = false) override;

            // // returns the list of render commands of both deferred and forward pushes that require
            // // alpha blending; which have to be rendered last.
            // std::vector<VRenderCommand> GetAlphaRenderCommands(bool cull = false) override;

            // // TODO: returns the list of custom render commands per render target.
            // // std::vector<VRenderCommand> GetCustomRenderCommands(RenderTarget *target, bool cull = false) override;

            // // returns the list of post-processing render commands.
            // std::vector<VRenderCommand> GetPostProcessingRenderCommands() override;

            // // returns the list of all render commands with mesh shadow casting
            // std::vector<VRenderCommand> GetShadowCastRenderCommands() override;

        private:
            std::vector<VRenderCommand> m_ForwardRenderCommands;
            // std::vector<VRenderCommand> m_DeferredRenderCommands;
            // std::vector<VRenderCommand> m_AlphaRenderCommands;
            // std::vector<VRenderCommand> m_PostProcessingRenderCommands;
            // std::map<RenderTarget*, std::vector<RenderCommand>> m_CustomRenderCommands; // TODO
    };
} // namespace Vantor::RenderDevice