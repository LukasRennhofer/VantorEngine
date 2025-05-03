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
* File: vantorQuad.hpp
* Last Change:
*/

#pragma once

#include "../../RenderDevice/vantorRenderDevice.hpp"

namespace vantor::Graphics::Geometry::Primitives {
    class Quad : public vantor::Graphics::RenderDevice::Mesh
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