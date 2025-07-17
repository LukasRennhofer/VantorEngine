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
 *  Date: 2025-07-16
 *
 *  File: VRE_Cube.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Core/Backlog
#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/VOS_Object.hpp"
// Math
#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::Renderer::Geometry
{
    class VCube : public Vantor::Object::VObject
    {
        public:
            ~VCube() = default;

            VCube()
            {
                vertecies = {
                    {-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f},
                    {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f},
                };

                indecies = {
                    0, 1, 2, 2, 3, 0, // back
                    4, 5, 6, 6, 7, 4, // front
                    0, 4, 7, 7, 3, 0, // left
                    1, 5, 6, 6, 2, 1, // right
                    3, 2, 6, 6, 7, 3, // top
                    0, 1, 5, 5, 4, 0  // bottom
                };
            }

            void GenerateMesh()
            {
                if (!HasComponent<Vantor::Object::VMeshComponent>())
                {
                    Vantor::Backlog::Log("Renderer::Geometry::VCube", "VCube doesn't have a Mesh Component", Vantor::Backlog::LogLevel::ERR);
                    return;
                }

                auto meshComponent = GetComponent<Vantor::Object::VMeshComponent>();
                if (!meshComponent)
                {
                    Vantor::Backlog::Log("Renderer::Geometry::VCube", "MeshComponent pointer is null", Vantor::Backlog::LogLevel::ERR);
                    return;
                }

                auto mesh = meshComponent->GetMesh();
                if (!mesh)
                {
                    Vantor::Backlog::Log("Renderer::Geometry::VCube", "Mesh pointer inside component is null", Vantor::Backlog::LogLevel::ERR);
                    return;
                }

                mesh->SetPositions(vertecies);
                mesh->SetIndices(indecies);
                meshComponent->FinalizeMesh();
                Vantor::Backlog::Log("Renderer::Geometry::VCube", "VCube finished generating mesh", Vantor::Backlog::LogLevel::DEBUG);
            }

        private:
            std::vector<Vantor::Math::VVector3> vertecies;
            std::vector<unsigned int>           indecies;
    };
} // namespace Vantor::Renderer::Geometry