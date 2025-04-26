/*
*    				~ CHIFEngine ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the MIT License. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 25.04.2025
*
* File: chifPlane.hpp
* Last Change:
*/

#pragma once

#include "../../RenderDevice/chifRenderDevice.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

namespace chif::Graphics::Geometry::Primitives {
	class Plane : public chif::Graphics::RenderDevice::Mesh
    {
    public:
        Plane(unsigned int xSegments, unsigned int ySegments) {
            bool oddRow = false;

            float dX = 1.0f / xSegments;
            float dY = 1.0f / ySegments;

            for (int y = 0; y <= ySegments; ++y)
            {
                for (int x = 0; x <= xSegments; ++x)
                {
                    Positions.push_back(glm::vec3(dX * x * 2.0f - 1.0f, dY * y * 2.0f - 1.0f, 0.0f));
                    UV.push_back(glm::vec2(dX * x, 1.0f - y * dY));
                    Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }
            }

            for (int y = 0; y < ySegments; ++y)
            {
                if (!oddRow)
                {
                    for (int x = 0; x <= xSegments; ++x)
                    {
                        Indices.push_back(y       * (xSegments + 1) + x);
                        Indices.push_back((y + 1) * (xSegments + 1) + x);
                    }
                }
                else
                {
                    for (int x = xSegments; x >= 0; --x)
                    {
                        Indices.push_back((y + 1) * (xSegments + 1) + x);
                        Indices.push_back(y       * (xSegments + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }

            Topology = TRIANGLE_STRIP;
            Finalize();
        }
    };
}