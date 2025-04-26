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
* File: chifQuad.hpp
* Last Change:
*/

#pragma once

#include "../../RenderDevice/chifRenderDevice.hpp"

namespace chif::Graphics::Geometry::Primitives {
    class Quad : public chif::Graphics::RenderDevice::Mesh
    {
    public:

        Quad()
        {
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
            Topology = TRIANGLE_STRIP;

            Finalize();
        }
        // --------------------------------------------------------------------------------------------
        :Quad(float width, float height)
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
            Topology = TRIANGLE_STRIP;

            Finalize();
        }
    };
}