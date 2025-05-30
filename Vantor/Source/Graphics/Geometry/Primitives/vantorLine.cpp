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
 *  File: vantorLine.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include "vantorLine.hpp"

#include <vector>

namespace vantor::Graphics::Geometry::Primitives
{
    LineStrip::LineStrip(float width, unsigned int segments)
    {
        float deltaX = 1.0f / segments;
        for (int i = 0; i <= segments; ++i)
        {
            Positions.push_back({-0.5f + (float) i * deltaX, 0.5f * width, 0.0f});
            Positions.push_back({-0.5f + (float) i * deltaX, -0.5f * width, 0.0f});

            UV.push_back({(float) i * deltaX, 1.0f});
            UV.push_back({(float) i * deltaX, 0.0f});
        }

        Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLE_STRIP;

        Finalize();
    }
} // namespace vantor::Graphics::Geometry::Primitives