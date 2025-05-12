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
 *  File: vantorSphere.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include "vantorSphere.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

#include <cmath>

namespace vantor::Graphics::Geometry::Primitives
{
    Sphere::Sphere(unsigned int xSegments, unsigned int ySegments)
    {
        for (unsigned int y = 0; y <= ySegments; ++y)
        {
            for (unsigned int x = 0; x <= xSegments; ++x)
            {
                float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
                float ySegment = static_cast<float>(x) / static_cast<float>(ySegments);
                float xPos     = std::cos(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());
                float yPos     = std::cos(ySegment * glm::pi<float>());
                float zPos     = std::sin(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());

                Positions.push_back(glm::vec3(xPos, yPos, zPos));
                UV.push_back(glm::vec2(xSegment, ySegment));
                Normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (int y = 0; y < ySegments; ++y)
        {
            for (int x = 0; x < xSegments; ++x)
            {
                Indices.push_back((y + 1) * (xSegments + 1) + x);
                Indices.push_back(y * (xSegments + 1) + x);
                Indices.push_back(y * (xSegments + 1) + x + 1);

                Indices.push_back((y + 1) * (xSegments + 1) + x);
                Indices.push_back(y * (xSegments + 1) + x + 1);
                Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
            }
        }

        Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLES;
        Finalize();
    }
} // namespace vantor::Graphics::Geometry::Primitives