#pragma once

#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/VOS_Object.hpp"

namespace Vantor::Renderer::Geometry
{
    class VPlane : public Vantor::Object::VObject
    {
    public:
        ~VPlane() = default;

        VPlane(float width = 1.0f, float height = 1.0f, float depth = 0.0f)
        {
            float hw = width * 0.5f;
            float hh = height * 0.5f;
            float hd = depth * 0.5f;

            if (depth <= 0.0001f)
            {
                // Flat 2D plane on the XY axis (Z offset = 0)
                positions = {
                    {-hw, -hh, 0.0f}, {+hw, -hh, 0.0f}, {+hw, +hh, 0.0f}, {-hw, +hh, 0.0f}
                };
                uvs = {
                    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
                };
                normals = {
                    {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}
                };
                indices = {0, 1, 2, 2, 3, 0};
            }
            else
            {
                // 3D box/board
                positions = {
                    // Front face (Z+)
                    {-hw, -hh, +hd}, {+hw, -hh, +hd}, {+hw, +hh, +hd}, {-hw, +hh, +hd},
                    // Back face (Z-)
                    {-hw, -hh, -hd}, {+hw, -hh, -hd}, {+hw, +hh, -hd}, {-hw, +hh, -hd},
                };
                normals = {
                    // Front normals
                    {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
                    // Back normals
                    {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
                };
                uvs = {
                    // Front UVs
                    {0, 0}, {1, 0}, {1, 1}, {0, 1},
                    // Back UVs (optional reuse of front UVs)
                    {0, 0}, {1, 0}, {1, 1}, {0, 1},
                };
                indices = {
                    // Front
                    0, 1, 2, 2, 3, 0,
                    // Back
                    6, 5, 4, 4, 7, 6,
                    // Left
                    4, 0, 3, 3, 7, 4,
                    // Right
                    1, 5, 6, 6, 2, 1,
                    // Top
                    3, 2, 6, 6, 7, 3,
                    // Bottom
                    4, 5, 1, 1, 0, 4
                };
            }
        }

        void GenerateMesh()
        {
            if (!HasComponent<Vantor::Object::VMeshComponent>())
            {
                Vantor::Backlog::Log("Renderer::Geometry::VPlane", "VPlane doesn't have a Mesh Component", Vantor::Backlog::LogLevel::ERR);
                return;
            }

            auto meshComponent = GetComponent<Vantor::Object::VMeshComponent>();
            if (!meshComponent)
            {
                Vantor::Backlog::Log("Renderer::Geometry::VPlane", "MeshComponent pointer is null", Vantor::Backlog::LogLevel::ERR);
                return;
            }

            auto mesh = meshComponent->GetMesh();
            if (!mesh)
            {
                Vantor::Backlog::Log("Renderer::Geometry::VPlane", "Mesh pointer inside component is null", Vantor::Backlog::LogLevel::ERR);
                return;
            }

            mesh->SetPositions(positions);
            mesh->SetIndices(indices);
            mesh->SetNormals(normals);
            mesh->SetUVs(uvs);
            meshComponent->FinalizeMesh();
            Vantor::Backlog::Log("Renderer::Geometry::VPlane", "VPlane finished generating mesh", Vantor::Backlog::LogLevel::DEBUG);
        }

    private:
        std::vector<Vantor::Math::VVector3> positions;
        std::vector<Vantor::Math::VVector3> normals;
        std::vector<Vantor::Math::VVector2> uvs;
        std::vector<unsigned int>           indices;
    };
}
