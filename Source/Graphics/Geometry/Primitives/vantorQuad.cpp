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
* File: vantorQuad.cpp
* Last Change:
*/

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum and other OpenGL usage

#include "vantorQuad.hpp"

namespace vantor::Graphics::Geometry::Primitives {
    Quad::Quad() {
            Positions =
            {
                { -1.0f,  1.0f, 0.0f, },
                { -1.0f, -1.0f, 0.0f, },
                {  1.0f,  1.0f, 0.0f, },
                {  1.0f, -1.0f, 0.0f, },
            };
            UV = {
                { 0.0f, 1.0f, },
                { 0.0f, 0.0f, },
                { 1.0f, 1.0f, },
                { 1.0f, 0.0f, },
            };
            Topology = vantor::Graphics::RenderDevice::OpenGL::TRIANGLE_STRIP;

            Finalize();
        }
        // --------------------------------------------------------------------------------------------
        Quad::Quad(float width, float height)
        {
            Positions =
            {
                { -width,  height, 0.0f, },
                { -width, -height, 0.0f, },
                {  width,  height, 0.0f, },
                {  width, -height, 0.0f, },
            };
            UV = {
                { 0.0f, 1.0f, },
                { 0.0f, 0.0f, },
                { 1.0f, 1.0f, },
                { 1.0f, 0.0f, },
            };
            Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLE_STRIP;

            Finalize();
        }
}