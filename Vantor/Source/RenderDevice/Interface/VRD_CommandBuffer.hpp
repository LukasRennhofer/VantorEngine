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
 *  Date: 2025-07-11
 *
 *  File: VRD_CommandBuffer.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Math
#include "../../Math/Linear/VMA_Matrix.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"

#include "VRD_Mesh.hpp"
#include "VRD_Mesh.hpp"

// Materials
#include "../../Renderer/VRE_Material.hpp"

namespace Vantor::RenderDevice
{
    // TODO: Add Materials
    struct VRenderCommand
    {
            Vantor::Math::VMat4 Transform;
            // Vantor::Math::VMat4 PrevTransform;
            VMesh              *Mesh;
            Vantor::Renderer::VMaterial *Material;
    };

    class VCommandBuffer
    {
        public:
            virtual ~VCommandBuffer() noexcept = 0;

            // pushes render state relevant to a single render call to the command buffer.
            virtual void Push(VMesh *mesh, Vantor::Renderer::VMaterial *material, Vantor::Math::VMat4 transform) = 0;

            // clears the command buffer; usually done after issuing all the stored render commands.
            virtual void Clear() = 0;
            // sorts the command buffer; first by shader, then by texture bind.
            virtual void Sort() = 0;

            virtual std::vector<VRenderCommand> GetForwardRenderCommands(bool cull = false) = 0;

            // TODO
            // // returns the list of render commands. For minimizing state changes it is advised to first
            // // call Sort() before retrieving and issuing the render commands.
            // virtual std::vector<VRenderCommand> GetDeferredRenderCommands(bool cull = false) = 0;

            // // returns the list of render commands of both deferred and forward pushes that require
            // // alpha blending; which have to be rendered last.
            // virtual std::vector<VRenderCommand> GetAlphaRenderCommands(bool cull = false) = 0;

            // // TODO: returns the list of custom render commands per render target.
            // // virtual std::vector<VRenderCommand> GetCustomRenderCommands(RenderTarget *target, bool cull = false) = 0;

            // // returns the list of post-processing render commands.
            // virtual std::vector<VRenderCommand> GetPostProcessingRenderCommands() = 0;

            // // returns the list of all render commands with mesh shadow casting
            // virtual std::vector<VRenderCommand> GetShadowCastRenderCommands() = 0;
    };
} // namespace Vantor::RenderDevice