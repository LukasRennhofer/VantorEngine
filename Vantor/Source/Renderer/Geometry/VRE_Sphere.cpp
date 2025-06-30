/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-06-30
 *
 *  File: VRE_Sphere.cpp
 *  Last Change: Automatically updated
 */

#include "VRE_Sphere.hpp"

// Math
#include "../../Math/VMA_Common.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::Renderer
{
    VSphere::VSphere(Vantor::RenderDevice::VRDevice *device, unsigned int xSegments, unsigned int ySegments)
    {
        std::vector<Vantor::Math::VVector3> currentPositions;
        std::vector<Vantor::Math::VVector2> currentUV;
        std::vector<Vantor::Math::VVector3> currentNormals;
        std::vector<unsigned int>           currentIndicies;

        for (unsigned int y = 0; y <= ySegments; ++y)
        {
            for (unsigned int x = 0; x <= xSegments; ++x)
            {
                float xSegment = (float) x / (float) xSegments;
                float ySegment = (float) y / (float) ySegments;
                float xPos     = std::cos(xSegment * Vantor::Math::TAU) * std::sin(ySegment * Vantor::Math::PI);
                float yPos     = std::cos(ySegment * Vantor::Math::PI);
                float zPos     = std::sin(xSegment * Vantor::Math::TAU) * std::sin(ySegment * Vantor::Math::PI);

                currentPositions.push_back(Vantor::Math::VVector3(xPos, yPos, zPos));
                currentUV.push_back(Vantor::Math::VVector2(xSegment, ySegment));
                currentNormals.push_back(Vantor::Math::VVector3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (int y = 0; y < ySegments; ++y)
        {
            for (int x = 0; x < xSegments; ++x)
            {
                currentIndicies.push_back((y + 1) * (xSegments + 1) + x);
                currentIndicies.push_back(y * (xSegments + 1) + x);
                currentIndicies.push_back(y * (xSegments + 1) + x + 1);

                currentIndicies.push_back((y + 1) * (xSegments + 1) + x);
                currentIndicies.push_back(y * (xSegments + 1) + x + 1);
                currentIndicies.push_back((y + 1) * (xSegments + 1) + x + 1);
            }
        }

        // Generate sphere vertex data
        Vantor::RenderDevice::VMeshCreateInfo info;
        info.Positions = currentPositions;
        info.UVs       = currentUV;
        info.Normals   = currentNormals;
        info.Indices   = currentIndicies;

        objectMesh = device->CreateMesh(info);
    }
} // namespace Vantor::Renderer