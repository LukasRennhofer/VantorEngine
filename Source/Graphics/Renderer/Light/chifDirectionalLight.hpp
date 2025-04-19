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
 * File: chifDirectionalLight.hpp
 * Last Change: 
*/

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace chif::Graphics {
    class RenderTarget;

    // ========= CHIF Directional Light Struct ========
    class DirectionalLight
    {
    public:
        math::vec3 Direction = math::vec3(0.0f);
        math::vec3 Color     = math::vec3(1.0f);
        float Intensity      = 1.0f;

        bool CastShadows = true;
        RenderTarget* ShadowMapRT;
        math::mat4    LightSpaceViewProjection;
    };
} // namespace chif::Graphics
