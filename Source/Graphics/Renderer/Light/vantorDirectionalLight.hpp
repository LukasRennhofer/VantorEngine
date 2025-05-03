/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorDirectionalLight.hpp
 * Last Change: 
*/

#include <glm/glm.hpp>

namespace vantor::Graphics {
    class RenderTarget;

    // ========= VANTOR Directional Light Struct ========
    class DirectionalLight
    {
    public:
        glm::vec3 Direction = glm::vec3(0.0f);
        glm::vec3 Color     = glm::vec3(1.0f);
        float Intensity      = 1.0f;

        bool CastShadows = true;
        RenderTarget* ShadowMapRT;
        glm::mat4    LightSpaceViewProjection;
    };
} // namespace vantor::Graphics
