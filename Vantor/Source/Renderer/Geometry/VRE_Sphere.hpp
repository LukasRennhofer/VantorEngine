#pragma once

// Core/Backlog
#include "../../Core/BackLog/VCO_Backlog.hpp"
#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/VOS_Object.hpp"
// Math
#include "../../Math/VMA_Common.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::Renderer::Geometry {
    class VSphere : public Vantor::Object::VObject
    {
    public:
        ~VSphere() = default;

        VSphere(const unsigned int xSegments = 64, const unsigned int ySegments = 64)
        {
            // Generate vertex data
            for (unsigned int x = 0; x <= xSegments; ++x)
            {
                for (unsigned int y = 0; y <= ySegments; ++y)
                {
                    float xSegment = (float)x / (float)xSegments;
                    float ySegment = (float)y / (float)ySegments;
                    float xPos = std::cos(xSegment * 2.0f * Vantor::Math::PI) * std::sin(ySegment * Vantor::Math::PI);
                    float yPos = std::cos(ySegment * Vantor::Math::PI);
                    float zPos = std::sin(xSegment * 2.0f * Vantor::Math::PI) * std::sin(ySegment * Vantor::Math::PI);

                    Vantor::Math::VVector3 pos(xPos, yPos, zPos);
                    positions.push_back(pos);
                    uv.push_back(Vantor::Math::VVector2(xSegment, ySegment));
                    normals.push_back(pos.Normalized());  // Normalize normals explicitly
                }
            }

            // Generate indices (triangle list)
            for (unsigned int y = 0; y < ySegments; ++y)
            {
                for (unsigned int x = 0; x < xSegments; ++x)
                {
                    unsigned int i0 = y * (xSegments + 1) + x;
                    unsigned int i1 = (y + 1) * (xSegments + 1) + x;
                    unsigned int i2 = (y + 1) * (xSegments + 1) + (x + 1);
                    unsigned int i3 = y * (xSegments + 1) + (x + 1);

                    // First triangle
                    indices.push_back(i0);
                    indices.push_back(i1);
                    indices.push_back(i2);

                    // Second triangle
                    indices.push_back(i0);
                    indices.push_back(i2);
                    indices.push_back(i3);
                }
            }

            // Initialize tangent and bitangent arrays
            tangents.resize(positions.size(), Vantor::Math::VVector3(0.0f, 0.0f, 0.0f));
            bitangents.resize(positions.size(), Vantor::Math::VVector3(0.0f, 0.0f, 0.0f));
            
            // TODO : Remove tangent and bitangent calculation on CPU and move it to GPU

            // Compute tangents and bitangents
            for (size_t i = 0; i < indices.size(); i += 3)
            {
                unsigned int i0 = indices[i];
                unsigned int i1 = indices[i + 1];
                unsigned int i2 = indices[i + 2];

                const auto& p0 = positions[i0];
                const auto& p1 = positions[i1];
                const auto& p2 = positions[i2];

                const auto& uv0 = uv[i0];
                const auto& uv1 = uv[i1];
                const auto& uv2 = uv[i2];

                Vantor::Math::VVector3 edge1 = p1 - p0;
                Vantor::Math::VVector3 edge2 = p2 - p0;
                Vantor::Math::VVector2 deltaUV1 = uv1 - uv0;
                Vantor::Math::VVector2 deltaUV2 = uv2 - uv0;

                float f = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
                if (std::abs(f) < 1e-6f)
                    continue; // skip degenerate triangle

                float invF = 1.0f / f;

                Vantor::Math::VVector3 tangent = invF * (edge1 * deltaUV2.y - edge2 * deltaUV1.y);
                Vantor::Math::VVector3 bitangent = invF * (edge2 * deltaUV1.x - edge1 * deltaUV2.x);

                tangents[i0] += tangent;
                tangents[i1] += tangent;
                tangents[i2] += tangent;

                bitangents[i0] += bitangent;
                bitangents[i1] += bitangent;
                bitangents[i2] += bitangent;
            }

            // Orthonormalize tangent space using Gram-Schmidt and compute proper bitangent
            for (size_t i = 0; i < tangents.size(); ++i)
            {
                Vantor::Math::VVector3& n = normals[i];
                Vantor::Math::VVector3& t = tangents[i];
                Vantor::Math::VVector3& b = bitangents[i];

                // Gram-Schmidt orthonormalization
                t = (t - n * n.Dot(t)).Normalized();

                // Enforce right-handed TBN
                float handedness = (n.Cross(t).Dot(b)) < 0.0f ? -1.0f : 1.0f;
                b = n.Cross(t) * handedness;

                tangents[i] = t;
                bitangents[i] = b;
            }

        }

        void GenerateMesh()
        {
            if (!HasComponent<Vantor::Object::VMeshComponent>())
            {
                AddComponentVoid<Vantor::Object::VMeshComponent>();
            }

            auto meshComponent = GetComponent<Vantor::Object::VMeshComponent>();
            if (!meshComponent)
            {
                Vantor::Backlog::Log("Renderer::Geometry::VSphere", "MeshComponent pointer is null", Vantor::Backlog::LogLevel::ERR);
                return;
            }

            auto mesh = meshComponent->GetMesh();
            if (!mesh)
            {
                Vantor::Backlog::Log("Renderer::Geometry::VSphere", "Mesh pointer inside component is null", Vantor::Backlog::LogLevel::ERR);
                return;
            }

            mesh->SetPositions(positions);
            mesh->SetUVs(uv);
            mesh->SetNormals(normals);
            mesh->SetIndices(indices);
            mesh->SetTangents(tangents, bitangents);
            mesh->SetTopology(Vantor::RenderDevice::VE_RENDER_TOPOLOGY::TRIANGLES);
            meshComponent->FinalizeMesh();
        }

    private:
        std::vector<Vantor::Math::VVector3> positions;
        std::vector<Vantor::Math::VVector2> uv;
        std::vector<Vantor::Math::VVector3> normals;
        std::vector<Vantor::Math::VVector3> tangents;
        std::vector<Vantor::Math::VVector3> bitangents;
        std::vector<unsigned int> indices;
    };
}
