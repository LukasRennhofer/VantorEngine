/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <vector>

#include <Math/Linear/VMA_Matrix.hpp>

#include <MaterialSystem/Public/VMAS_Material.hpp>

#include <Graphics/Public/Model/VGFX_Model.hpp>

namespace VE::Internal::RHI {
    class IRHIMesh;
    class IRHIShader;
}

namespace VE::Internal::RenderPipeline {

     struct VRenderCommand
    {
            VE::Math::VMat4 Transform;
            // Vantor::Math::VMat4 PrevTransform;
            VE::Graphics::VModel* Model;
            VE::VMaterial* Material;
    };

    class VCommandBuffer
    {
        public:
            VCommandBuffer();
            ~VCommandBuffer();

            // pushes render state relevant to a single render call to the command buffer.
            void Push(VE::Graphics::VModel* model, VE::VMaterial* material, const VE::Math::VMat4& transform);
            // clears the command buffer; usually done after issuing all the stored render commands.
            void Clear();
            // sorts the command buffer; first by shader, then by texture bind.
            void Sort();

            std::vector<VRenderCommand> GetForwardRenderCommands(bool cull = false);
            std::vector<VRenderCommand> GetDefferedRenderCommands(bool cull = false);

        private:
            std::vector<VRenderCommand> m_ForwardRenderCommands;
            std::vector<VRenderCommand> m_DeferredRenderCommands;
    };
}