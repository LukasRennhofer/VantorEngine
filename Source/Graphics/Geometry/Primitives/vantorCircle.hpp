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
* File: vantorCircle.hpp
* Last Change:
*/

#pragma once

#include "../../RenderDevice/vantorRenderDevice.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

#include <cmath>

namespace vantor::Graphics::Geometry::Primitives {
    class Circle : public vantor::Graphics::RenderDevice::Mesh
    {
    public:
        Circle(unsigned int edgeSegments, unsigned int ringSegments) {
            for (unsigned int y = 0; y <= ringSegments; ++y)
            {
                for (unsigned int x = 0; x <= edgeSegments; ++x)
                {
                    float xSegment = static_cast<float>(x) / static_cast<float>(edgeSegments);
                    float ringDepth = static_cast<float>(y) / static_cast<float>(ringSegments);
                    float angle = xSegment * glm::two_pi<float>();
                    float xPos = std::cos(angle);
                    float yPos = std::sin(angle);

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
                        Indices.push_back(y       * (edgeSegments + 1) + x);
                        Indices.push_back((y + 1) * (edgeSegments + 1) + x);
                    }
                }
                else
                {
                    for (int x = edgeSegments; x >= 0; --x)
                    {
                        Indices.push_back((y + 1) * (edgeSegments + 1) + x);
                        Indices.push_back(y       * (edgeSegments + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }

            Topology = TRIANGLE_STRIP;
            Finalize();
        }
    };
} // namespace vantor::Graphics::Geometry::Primitives