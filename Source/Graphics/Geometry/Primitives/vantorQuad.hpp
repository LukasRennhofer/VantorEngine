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
 *  Date: 2025-05-11
 *
 *  File: vantorQuad.hpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#pragma once

#include "../../RenderDevice/DeviceOpenGL/vantorOpenGLMesh.hpp"

namespace vantor::Graphics::Geometry::Primitives
{
    class Quad : public vantor::Graphics::RenderDevice::OpenGL::Mesh
    {
        public:
            Quad(); // defaults to dimensions (1, 1)
            Quad(float width, float height);
    };
} // namespace vantor::Graphics::Geometry::Primitives