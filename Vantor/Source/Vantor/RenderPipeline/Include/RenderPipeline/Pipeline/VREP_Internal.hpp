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

#include <RenderPipeline/Public/VREP_RenderPass.hpp>

namespace VE::Internal::RenderPipeline {
    class VGeometryRenderPass : public VE::Render::VRenderPass
    {
        public:
            VGeometryRenderPass();
            ~VGeometryRenderPass() = default;

            void Execute() override;
            void Cleanup() override;

            VE::Render::ERenderPassType GetType() const override { return VE::Render::ERenderPassType::VD_Geometry; };
    };

    class VForwardRenderPass : public VE::Render::VRenderPass
    {
        public:
            VForwardRenderPass();
            ~VForwardRenderPass() = default;

            void Execute() override;
            void Cleanup() override;

            VE::Render::ERenderPassType GetType() const override { return VE::Render::ERenderPassType::VF_Forward; };
    };
}