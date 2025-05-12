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
 *  File: vantorQuad.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include "vantorQuad.hpp"

namespace vantor::Graphics::Geometry::Primitives
{
    Quad::Quad()
    {
        Positions = {
            {
                -1.0f,
                1.0f,
                0.0f,
            },
            {
                -1.0f,
                -1.0f,
                0.0f,
            },
            {
                1.0f,
                1.0f,
                0.0f,
            },
            {
                1.0f,
                -1.0f,
                0.0f,
            },
        };
        UV = {
            {
                0.0f,
                1.0f,
            },
            {
                0.0f,
                0.0f,
            },
            {
                1.0f,
                1.0f,
            },
            {
                1.0f,
                0.0f,
            },
        };
        Topology = vantor::Graphics::RenderDevice::OpenGL::TRIANGLE_STRIP;

        Finalize();
    }
    // --------------------------------------------------------------------------------------------
    Quad::Quad(float width, float height)
    {
        Positions = {
            {
                -width,
                height,
                0.0f,
            },
            {
                -width,
                -height,
                0.0f,
            },
            {
                width,
                height,
                0.0f,
            },
            {
                width,
                -height,
                0.0f,
            },
        };
        UV = {
            {
                0.0f,
                1.0f,
            },
            {
                0.0f,
                0.0f,
            },
            {
                1.0f,
                1.0f,
            },
            {
                1.0f,
                0.0f,
            },
        };
        Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLE_STRIP;

        Finalize();
    }
} // namespace vantor::Graphics::Geometry::Primitives