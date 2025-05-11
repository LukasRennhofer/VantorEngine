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
 *  File: vantorPlane.hpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#pragma once

#include "../../RenderDevice/DeviceOpenGL/vantorOpenGLMesh.hpp"

namespace vantor::Graphics::Geometry::Primitives
{
    class Plane : public vantor::Graphics::RenderDevice::OpenGL::Mesh
    {
        public:
            Plane(unsigned int xSegments, unsigned int ySegments);
    };
} // namespace vantor::Graphics::Geometry::Primitives