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
 *  Date: 2025-07-09
 *
 *  File: VRDO_Mesh.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>

#include "../Interface/VRD_Mesh.hpp"

#include <vector>

namespace Vantor::RenderDevice
{

    class VOpenGLMesh : public VMesh
    {
        public:
            VOpenGLMesh();
            VOpenGLMesh(std::vector<Vantor::Math::VVector3> positions, std::vector<unsigned int> indices);
            VOpenGLMesh(std::vector<Vantor::Math::VVector3> positions, std::vector<Vantor::Math::VVector2> uv, std::vector<unsigned int> indices);
            VOpenGLMesh(std::vector<Vantor::Math::VVector3> positions,
                        std::vector<Vantor::Math::VVector2> uv,
                        std::vector<Vantor::Math::VVector3> normals,
                        std::vector<unsigned int>           indices);
            VOpenGLMesh(std::vector<Vantor::Math::VVector3> positions,
                        std::vector<Vantor::Math::VVector2> uv,
                        std::vector<Vantor::Math::VVector3> normals,
                        std::vector<Vantor::Math::VVector3> tangents,
                        std::vector<Vantor::Math::VVector3> bitangents,
                        std::vector<unsigned int>           indices);

            ~VOpenGLMesh() override;

            void SetPositions(const std::vector<Vantor::Math::VVector3> positions) override;
            void SetUVs(const std::vector<Vantor::Math::VVector2> uv) override;
            void SetNormals(const std::vector<Vantor::Math::VVector3> normals) override;
            void SetTangents(const std::vector<Vantor::Math::VVector3> tangents, const std::vector<Vantor::Math::VVector3> bitangents) override;
            void SetIndices(const std::vector<unsigned int> &indices) override;
            void SetTopology(VE_RENDER_TOPOLOGY topology) override;

            void RenderRaw() override;

            void Finalize(bool interleaved = true) override;
            void FromSDF(const std::function<float(Vantor::Math::VVector3)> &sdf, float maxDistance, uint16_t gridResolution) override;

        private:
            unsigned int m_VAO = 0;
            unsigned int m_VBO;
            unsigned int m_EBO;

            VE_RENDER_TOPOLOGY Topology = TRIANGLES;

            std::vector<Vantor::Math::VVector3> Positions;
            std::vector<Vantor::Math::VVector2> UV;
            std::vector<Vantor::Math::VVector3> Normals;
            std::vector<Vantor::Math::VVector3> Tangents;
            std::vector<Vantor::Math::VVector3> Bitangents;
            std::vector<unsigned int>           Indices;

            void calculateNormals(bool smooth = true);
            void calculateTangents();
    };

} // namespace Vantor::RenderDevice
