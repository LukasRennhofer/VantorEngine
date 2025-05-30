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
 *  File: vantorPointLight.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <glm/glm.hpp>

namespace vantor::Graphics
{

    // ======== VANTOR Point Light Struct ========
    class PointLight
    {
        public:
            glm::vec3 Position   = glm::vec3(0.0f);
            glm::vec3 Color      = glm::vec3(1.0f);
            float     Intensity  = 1.0f;
            float     Radius     = 1.0f;
            bool      Visible    = true;
            bool      RenderMesh = false;
    };
} // namespace vantor::Graphics
