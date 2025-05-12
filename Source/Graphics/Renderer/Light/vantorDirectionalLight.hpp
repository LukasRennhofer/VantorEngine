/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorDirectionalLight.hpp
 *  Last Change: Automatically updated
 */

// !! WARNING: IT NEEDS TO BE CHANGED WHEN WORKING WITH VULKAN!!!

#pragma once

#include <glm/glm.hpp>
#include "../../RenderDevice/DeviceOpenGL/vantorOpenGLRenderTarget.hpp" // Doesnt work with vulkan

namespace vantor::Graphics
{
    class RenderTarget;
    // ========= VANTOR Directional Light Struct ========
    class DirectionalLight
    {
        public:
            glm::vec3 Direction = glm::vec3(0.0f);
            glm::vec3 Color     = glm::vec3(1.0f);
            float     Intensity = 1.0f;

            bool                                                  CastShadows = true;
            vantor::Graphics::RenderDevice::OpenGL::RenderTarget *ShadowMapRT; // CHANGE: Removed forward decleration to
                                                                               // RenderTarget
            glm::mat4 LightSpaceViewProjection;
    };
} // namespace vantor::Graphics
