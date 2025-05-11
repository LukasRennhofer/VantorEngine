/*
*    				~ Vantor ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 25.04.2025
*
* File: vantorCube.cpp
* Last Change:
*/

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum and other OpenGL usage

#include "vantorCube.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

namespace vantor::Graphics::Geometry::Primitives {
    Cube::Cube() {
            Positions = std::vector<glm::vec3> {
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f, -0.5f),
                glm::vec3( 0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f,  0.5f, -0.5f),

                glm::vec3(-0.5f, -0.5f,  0.5f),
                glm::vec3( 0.5f, -0.5f,  0.5f),
                glm::vec3( 0.5f,  0.5f,  0.5f),
                glm::vec3( 0.5f,  0.5f,  0.5f),
                glm::vec3(-0.5f,  0.5f,  0.5f),
                glm::vec3(-0.5f, -0.5f,  0.5f),

                glm::vec3(-0.5f,  0.5f,  0.5f),
                glm::vec3(-0.5f,  0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f,  0.5f),
                glm::vec3(-0.5f,  0.5f,  0.5f),

                glm::vec3( 0.5f,  0.5f,  0.5f),
                glm::vec3( 0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f, -0.5f),
                glm::vec3( 0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f,  0.5f),
                glm::vec3( 0.5f, -0.5f,  0.5f),

                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f, -0.5f,  0.5f),
                glm::vec3( 0.5f, -0.5f,  0.5f),
                glm::vec3(-0.5f, -0.5f,  0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),

                glm::vec3(-0.5f,  0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f,  0.5f),
                glm::vec3( 0.5f,  0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f,  0.5f),
                glm::vec3(-0.5f,  0.5f, -0.5f),
                glm::vec3(-0.5f,  0.5f,  0.5f),
            };
            UV = std::vector<glm::vec2> {
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),

                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),

                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),

                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),

                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),

                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
            };
            Normals = std::vector<glm::vec3>{
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),

                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),

                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),

                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),

                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),

                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
            };

            Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLES;
            Finalize();
        }
} // namespace vantor::Graphics::Geometry::Primitives