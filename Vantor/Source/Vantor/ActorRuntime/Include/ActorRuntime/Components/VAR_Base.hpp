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
#include <Graphics/VGFX_Material.hpp>

#include <ActorRuntime/VAR_Component.hpp>

namespace Vantor::ActorRuntime::Components
{
    // ===  Mesh Component (Holding renderable data) ===
    class CMeshComponent : public CComponent
    {
        public:
            using VMeshPtr = std::shared_ptr<Vantor::RHI::IRHIMesh>;

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
    class CMaterialComponent : public CComponent
    {
        public:
            explicit CMaterialComponent(AActor *owner) : CComponent(owner) {}

            void                         SetMaterial(Vantor::Graphics::VMaterial *material) { m_Material = material; }
            Vantor::Graphics::VMaterial *GetMaterial() { return m_Material; }

        private:
            Vantor::Graphics::VMaterial *m_Material = nullptr;
    };

    using CTagComponentPtr       = std::shared_ptr<CTagComponent>;
    using CMaterialComponentPtr  = std::shared_ptr<CMaterialComponent>;
    using CTransformComponentPtr = std::shared_ptr<CTransformComponent>;
    using CMeshComponentPtr      = std::shared_ptr<CMeshComponent>;
} // namespace Vantor::ActorRuntime