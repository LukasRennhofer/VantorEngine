/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifPointLight.hpp
 * Last Change: 
*/

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace chif::Graphics {

    // ======== CHIF Point Light Struct ========
    class PointLight
    {
    public:
        math::vec3 Position   = math::vec3(0.0f);
        math::vec3 Color      = math::vec3(1.0f);
        float      Intensity  = 1.0f;
        float      Radius     = 1.0f;
        bool       Visible    = true;
        bool       RenderMesh = false;
    };
} // namespace chif::Graphics
