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
 *  File: vantorTorus.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include "vantorTorus.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

#include <vector>
#include <cmath>

namespace vantor::Graphics::Geometry::Primitives
{
    Torus::Torus(float r1, float r2, unsigned int numSteps1, unsigned int numSteps2)
    {
        Positions.resize((numSteps1 + 1) * (numSteps2 + 1));
        Normals.resize((numSteps1 + 1) * (numSteps2 + 1));
        UV.resize((numSteps1 + 1) * (numSteps2 + 1));

        std::vector<glm::vec3> p(numSteps1 + 1);
        float                  a    = 0.0f;
        float                  step = 2.0f * glm::pi<float>() / numSteps1;
        for (int i = 0; i <= numSteps1; ++i)
        {
            float x = std::cos(a) * r1;
            float y = std::sin(a) * r1;
            p[i].x  = x;
            p[i].y  = y;
            p[i].z  = 0.0f;
            a += step;
        }

        for (int i = 0; i <= numSteps1; ++i)
        {
            glm::vec3 u = glm::normalize(glm::vec3(0.0f) - p[i]) * r2;
            glm::vec3 v = glm::vec3(0.0f, 0.0f, 1.0f) * r2;

            float a    = 0.0f;
            float step = 2.0f * glm::pi<float>() / numSteps2;
            for (int j = 0; j <= numSteps2; ++j)
            {
                float c = std::cos(a);
                float s = std::sin(a);

                Positions[i * (numSteps2 + 1) + j] = p[i] + c * u + s * v;
                UV[i * (numSteps2 + 1) + j].x      = ((float) i) / ((float) numSteps1) * glm::two_pi<float>();
                UV[i * (numSteps2 + 1) + j].y      = ((float) j) / ((float) numSteps2);
                Normals[i * (numSteps2 + 1) + j]   = glm::normalize(c * u + s * v);
                a += step;
            }
        }

        Indices.resize(numSteps1 * numSteps2 * 6);

        int index = 0;
        for (int i = 0; i < numSteps1; ++i)
        {
            int i1 = i;
            int i2 = (i1 + 1);

            for (int j = 0; j < numSteps2; ++j)
            {
                int j1 = j;
                int j2 = (j1 + 1);

                Indices[index++] = i1 * (numSteps2 + 1) + j1;
                Indices[index++] = i1 * (numSteps2 + 1) + j2;
                Indices[index++] = i2 * (numSteps2 + 1) + j1;

                Indices[index++] = i2 * (numSteps2 + 1) + j2;
                Indices[index++] = i2 * (numSteps2 + 1) + j1;
                Indices[index++] = i1 * (numSteps2 + 1) + j2;
            }
        }

        Topology = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY::TRIANGLES;
        Finalize();
    }
} // namespace vantor::Graphics::Geometry::Primitives