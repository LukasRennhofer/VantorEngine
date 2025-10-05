/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RenderPipeline/Public/VREP_RenderPass.hpp>
#include <RenderPipeline/Public/VREP_RenderPath.hpp>

#include <RHI/Interface/VRHI_RenderTarget.hpp>

namespace VE::Render {

    void VRenderPass::Initialize(VRenderPath* renderPath) {
        m_RenderPath = renderPath;
    }
    
}