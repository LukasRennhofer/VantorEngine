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
* File: vantorLine.hpp
* Last Change:
*/

#pragma once

#include "../../RenderDevice/vantorRenderDevice.hpp"

namespace vantor::Graphics::Geometry::Primitives {
    class LineStrip : public vantor::Graphics::RenderDevice::Mesh
    {
    public:
        LineStrip(float width, unsigned int segments)
        {
            float deltaX = 1.0f / segments;
            for (int i = 0; i <= segments; ++i)
            {
                Positions.push_back({ -0.5f + (float)i * deltaX,  0.5f * width, 0.0f });
                Positions.push_back({ -0.5f + (float)i * deltaX, -0.5f * width, 0.0f });

                UV.push_back({ (float)i * deltaX, 1.0f });
                UV.push_back({ (float)i * deltaX, 0.0f });
            }

            Topology = TRIANGLE_STRIP;

            Finalize();
        }
    };
}