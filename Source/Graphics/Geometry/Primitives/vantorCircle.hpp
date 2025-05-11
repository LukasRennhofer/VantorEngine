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

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum and other OpenGL usage

#pragma once

#include "../../RenderDevice/DeviceOpenGL/vantorOpenGLMesh.hpp"

namespace vantor::Graphics::Geometry::Primitives
{
    class Circle : public vantor::Graphics::RenderDevice::OpenGL::Mesh
    {
    public:
        Circle(unsigned int edgeSegments, unsigned int ringSegments);
    };
}