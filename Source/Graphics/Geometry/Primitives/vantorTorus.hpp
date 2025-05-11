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
* File: vantorTorus.hpp
* Last Change:
*/

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum and other OpenGL usage

#pragma once

#include "../../RenderDevice/DeviceOpenGL/vantorOpenGLMesh.hpp"

namespace vantor::Graphics::Geometry::Primitives {
    class Torus : public vantor::Graphics::RenderDevice::OpenGL::Mesh    {
    public:
        Torus(float r1, float r2, unsigned int lod1, unsigned int lod2);
    };
}