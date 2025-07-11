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
 *  Date: 2025-07-11
 *
 *  File: VOS_Base.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../../Core/Global/VCO_ServiceRegistry.hpp"
#include "../../RenderDevice/Interface/VRD_Mesh.hpp"
#include "VOS_Component.hpp"
#include "../../Core/Global/VCO_ServiceRegistry.hpp"

// RenderDevice
#include "../../RenderDevice/Interface/VRD_Mesh.hpp"

namespace Vantor::Object
{
    // ===  Mesh Component (Holding renderable data) ===
    class VMeshComponent : public VComponent
    {
        public:
            using MeshPtr = std::shared_ptr<Vantor::RenderDevice::VMesh>;

            explicit VMeshComponent(VObject *owner) : VComponent(owner) {}

            void SetMesh(const MeshPtr &mesh) { this->mesh = mesh; }

            MeshPtr GetMesh() const { return mesh; }

            void FinalizeMesh(bool interleaved = true)
            {
                if (mesh)
                {
                    mesh->Finalize(interleaved);
                }
            }

        private:
            MeshPtr mesh;
    };

    // === Tag Component (for Object Metadata) ===
    class VTagComponent : public VComponent
    {
        public:
            explicit VTagComponent(VObject *owner) : VComponent(owner) {}

            void        SetName(std::string name) { m_name = name; }
            std::string GetName() { return m_name; }

        private:
            std::string m_name;
    };

    using VMeshComponentPtr = std::shared_ptr<VMeshComponent>;
} // namespace Vantor::Object