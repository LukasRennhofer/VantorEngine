/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Mesh.hpp>
#include <Math/Linear/VMA_Quaternation.hpp>
#include <MaterialSystem/Public/VMAS_Material.hpp>

#include <ActorRuntime/Public/VAR_Component.hpp>

namespace VE::Components
{
    // ===  Mesh Component (Holding renderable data) ===
    class CMeshComponent : public CComponent
    {
        public:
            using VMeshPtr = std::shared_ptr<VE::Internal::RHI::IRHIMesh>;

            explicit CMeshComponent(AActor *owner) : CComponent(owner) {}

            void SetMesh(const VMeshPtr &mesh) { this->mesh = mesh; }

            VMeshPtr GetMesh() const { return mesh; }

        private:
            VMeshPtr mesh;
    };

    // === Tag Component (for Object Metadata) ===
    class CTagComponent : public CComponent
    {
        public:
            explicit CTagComponent(AActor *owner) : CComponent(owner) {}

            void        SetName(std::string name) { m_name = name; }
            std::string GetName() { return m_name; }

        private:
            std::string m_name;
    };

    // === Tranform Component ===
    class CTransformComponent : public CComponent
    {
        public:
            explicit CTransformComponent(AActor *owner)
                : CComponent(owner),
                  m_position(VE::Math::VVector3(1.0f, 1.0f, 1.0f)),
                  m_rotation(VE::Math::VQuaternion::Identity()),
                  m_scale(VE::Math::VVector3(1.0f, 1.0f, 1.0f))
            {
            }

            // === Setters ===
            void SetPosition(const VE::Math::VVector3 &position) { m_position = position; }
            void SetRotation(const VE::Math::VQuaternion &rotation) { m_rotation = rotation; }
            void SetScale(const VE::Math::VVector3 &scale) { m_scale = scale; }

            // === Getters ===
            const VE::Math::VVector3    &GetPosition() const { return m_position; }
            const VE::Math::VQuaternion &GetRotation() const { return m_rotation; }
            const VE::Math::VVector3    &GetScale() const { return m_scale; }

            // === Transform Matrix ===
            VE::Math::VMat4 GetTransform() const
            {
                // 1. Scale the object in local space
                // 2. Rotate the object around its local center  
                // 3. Then translate to world position
                
                VE::Math::VMat4 S = VE::Math::VMat4::Scale(m_scale);
                VE::Math::VMat4 R = m_rotation.ToMat4();
                
                // First apply scale then rotation in local space
                VE::Math::VMat4 RS = R * S;
                
                // Then manually set the translation component without affecting the rotation
                VE::Math::VMat4 result = RS;
                result.m[12] = m_position.x;  // Set translation X
                result.m[13] = m_position.y;  // Set translation Y
                result.m[14] = m_position.z;  // Set translation Z
                
                return result;
            }

        private:
            VE::Math::VVector3    m_position;
            VE::Math::VQuaternion m_rotation;
            VE::Math::VVector3    m_scale;
            
            // We set the center of the mesh to a cube center for now
            VE::Math::VVector3    m_pivot =  {0.0f, 0.0f, 0.0f};
    };

    // === Material Component ===
    class CMaterialComponent : public CComponent
    {
        public:
            explicit CMaterialComponent(AActor *owner) : CComponent(owner) {}

            void                         SetMaterial(VE::VMaterial *material) { m_Material = material; }
            VE::VMaterial *GetMaterial() { return m_Material; }

        private:
            VE::VMaterial *m_Material = nullptr;
    };

    using CTagComponentPtr       = std::shared_ptr<CTagComponent>;
    using CMaterialComponentPtr  = std::shared_ptr<CMaterialComponent>;
    using CTransformComponentPtr = std::shared_ptr<CTransformComponent>;
    using CMeshComponentPtr      = std::shared_ptr<CMeshComponent>;
} // namespace VE::Internal::ActorRuntime