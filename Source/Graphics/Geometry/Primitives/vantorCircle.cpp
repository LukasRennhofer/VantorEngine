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
 *  File: vantorCircle.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include "vantorCircle.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

#include <cmath>

namespace vantor::Graphics::Geometry::Primitives
{
    Circle::Circle(unsigned int edgeSegments, unsigned int ringSegments)
    {
        for (unsigned int y = 0; y <= ringSegments; ++y)
        {
            for (unsigned int x = 0; x <= edgeSegments; ++x)
            {
                float xSegment  = static_cast<float>(x) / static_cast<float>(edgeSegments);
                float ringDepth = static_cast<float>(y) / static_cast<float>(ringSegments);
                float angle     = xSegment * glm::two_pi<float>();
                float xPos      = std::cos(angle);
                float yPos      = std::sin(angle);

                Positions.emplace_back(xPos * ringDepth, yPos * ringDepth, 0.0f);
            }
        }

        bool oddRow = false;
        for (int y = 0; y < ringSegments; ++y)
        {
            if (!oddRow)
            {
                for (int x = 0; x <= edgeSegments; ++x)
                {
                    Indices.push_back(y * (edgeSegments + 1) + x);
                    Indices.push_back((y + 1) * (edgeSegments + 1) + x);
                }
            }
            else
            {
                for (int x = edgeSegments; x >= 0; --x)
                {
                    Indices.push_back((y + 1) * (edgeSegments + 1) + x);
                    Indices.push_back(y * (edgeSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLE_STRIP;
        Finalize();
    }
} // namespace vantor::Graphics::Geometry::Primitives