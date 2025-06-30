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
 *  File: VRD_Mesh.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <vector>
#include <functional>
#include <cstdint>

// Math
#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::RenderDevice
{

    // Topologies from Cell Engine
    enum VE_RENDER_TOPOLOGY
    {
        POINTS,
        LINES,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
    };

    class VMesh
    {
        public:
            virtual ~VMesh() noexcept = default;

            // Mesh data setup
            virtual void SetPositions(const std::vector<Vantor::Math::VVector3> positions)                                                     = 0;
            virtual void SetUVs(const std::vector<Vantor::Math::VVector2> uv)                                                                  = 0;
            virtual void SetNormals(const std::vector<Vantor::Math::VVector3> normals)                                                         = 0;
            virtual void SetTangents(const std::vector<Vantor::Math::VVector3> tangents, const std::vector<Vantor::Math::VVector3> bitangents) = 0;

            virtual void SetIndices(const std::vector<unsigned int> &indices) = 0;
            virtual void SetTopology(VE_RENDER_TOPOLOGY topology)             = 0;

            // Buffer upload
            virtual void Finalize(bool interleaved = true) = 0;

            // Procedural generation
            virtual void FromSDF(const std::function<float(Vantor::Math::VVector3)> &sdf, float maxDistance, uint16_t gridResolution) = 0;
    };
} // namespace Vantor::RenderDevice