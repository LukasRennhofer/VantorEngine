#pragma once

#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/VOS_Object.hpp"

namespace Vantor::Renderer::Geometry
{
    class VCube : public Vantor::Object::VObject
    {
    public:
        ~VCube() = default;

        VCube()
        {
            // Each face has its own set of 4 vertices (no sharing) for proper normals and UVs
            vertecies = {
                // Back (-Z)
                {-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f},
                // Front (+Z)
                {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f},
                // Left (-X)
                {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, -0.5f},
                // Right (+X)
                {+0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, -0.5f},
                // Top (+Y)
                {-0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f},
                // Bottom (-Y)
                {-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f},
            };

            normals = {
                // Back
                {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
                // Front
                {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
                // Left
                {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
                // Right
                {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
                // Top
                {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
                // Bottom
                {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0},
            };

            uvs = {
                // Each face uses same UV pattern
                {0, 0}, {1, 0}, {1, 1}, {0, 1}, // back
                {0, 0}, {1, 0}, {1, 1}, {0, 1}, // front
                {0, 0}, {1, 0}, {1, 1}, {0, 1}, // left
                {0, 0}, {1, 0}, {1, 1}, {0, 1}, // right
                {0, 0}, {1, 0}, {1, 1}, {0, 1}, // top
                {0, 0}, {1, 0}, {1, 1}, {0, 1}, // bottom
            };

            indecies = {
                0, 1, 2, 2, 3, 0,       // back
                4, 5, 6, 6, 7, 4,       // front
                8, 9,10,10,11, 8,       // left
               12,13,14,14,15,12,       // right
               16,17,18,18,19,16,       // top
               20,21,22,22,23,20        // bottom
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
            mesh->SetNormals(normals);
            mesh->SetUVs(uvs);
            mesh->SetIndices(indecies);
            meshComponent->FinalizeMesh();
            Vantor::Backlog::Log("Renderer::Geometry::VCube", "VCube finished generating mesh", Vantor::Backlog::LogLevel::DEBUG);
        }

    private:
        std::vector<Vantor::Math::VVector3> vertecies;
        std::vector<Vantor::Math::VVector3> normals;
        std::vector<Vantor::Math::VVector2> uvs;
        std::vector<unsigned int>           indecies;
    };
}
