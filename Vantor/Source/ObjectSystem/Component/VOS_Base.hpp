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
 *  File: VOS_Base.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// #include "../../Core/Global/VCO_ServiceRegistry.hpp"
#include "../../Math/Linear/VMA_Quaternation.hpp"
#include "../../RenderDevice/Interface/VRD_Mesh.hpp"
#include "../../RenderDevice/Interface/VRD_Mesh.hpp"
#include "../../Renderer/VRE_Material.hpp"
#include "VOS_Component.hpp"
#include "../../RenderDevice/Interface/VRD_Mesh.hpp"

// Math / Linear
#include "../../Math/Linear/VMA_Quaternation.hpp"

// Renderer
#include "../../Renderer/VRE_Material.hpp"

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

    // === Tranform Component ===
    class VTransformComponent : public VComponent
    {
        public:
            explicit VTransformComponent(VObject *owner)
                : VComponent(owner),
                  m_position(Vantor::Math::VVector3(1.0f, 1.0f, 1.0f)),
                  m_rotation(Vantor::Math::VQuaternion::Identity()),
                  m_scale(Vantor::Math::VVector3(1.0f, 1.0f, 1.0f))
            {
            }

            // === Setters ===
            void SetPosition(const Vantor::Math::VVector3 &position) { m_position = position; }
            void SetRotation(const Vantor::Math::VQuaternion &rotation) { m_rotation = rotation; }
            void SetScale(const Vantor::Math::VVector3 &scale) { m_scale = scale; }

            // === Getters ===
            const Vantor::Math::VVector3    &GetPosition() const { return m_position; }
            const Vantor::Math::VQuaternion &GetRotation() const { return m_rotation; }
            const Vantor::Math::VVector3    &GetScale() const { return m_scale; }

            // === Transform Matrix ===
            Vantor::Math::VMat4 GetTransform() const
            {
                // 1. Scale the object in local space
                // 2. Rotate the object around its local center  
                // 3. Then translate to world position
                
                Vantor::Math::VMat4 S = Vantor::Math::VMat4::Scale(m_scale);
                Vantor::Math::VMat4 R = m_rotation.ToMat4();
                
                // First apply scale then rotation in local space
                Vantor::Math::VMat4 RS = R * S;
                
                // Then manually set the translation component without affecting the rotation
                Vantor::Math::VMat4 result = RS;
                result.m[12] = m_position.x;  // Set translation X
                result.m[13] = m_position.y;  // Set translation Y
                result.m[14] = m_position.z;  // Set translation Z
                
                return result;
            }

        private:
            Vantor::Math::VVector3    m_position;
            Vantor::Math::VQuaternion m_rotation;
            Vantor::Math::VVector3    m_scale;
            
            // We set the center of the mesh to a cube center for now
            Vantor::Math::VVector3    m_pivot =  {0.0f, 0.0f, 0.0f};
    };

    // === Material Component ===
    class VMaterialComponent : public VComponent
    {
        public:
            explicit VMaterialComponent(VObject *owner) : VComponent(owner) {}

            void                         SetMaterial(Vantor::Renderer::VMaterial *material) { m_Material = material; }
            Vantor::Renderer::VMaterial *GetMaterial() { return m_Material; }

        private:
            Vantor::Renderer::VMaterial *m_Material = nullptr;
    };

    using VTagComponentPtr       = std::shared_ptr<VTagComponent>;
    using VMaterialComponentPtr  = std::shared_ptr<VMaterialComponent>;
    using VTransformComponentPtr = std::shared_ptr<VTransformComponent>;
    using VMeshComponentPtr      = std::shared_ptr<VMeshComponent>;
} // namespace Vantor::Object