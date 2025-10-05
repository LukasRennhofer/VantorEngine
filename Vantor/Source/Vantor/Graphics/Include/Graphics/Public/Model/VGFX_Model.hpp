/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Math/Linear/VMA_Vector.hpp>
#include <Core/Container/VCO_Vector.hpp>

namespace VE::Internal::RHI {
    class IRHIMesh;
    class IRHITexture;
    class IRHIDevice;
}

namespace VE::Graphics {
    
    struct VVertex {
        VE::Math::VVector3 Position;
        VE::Math::VVector3 Normal;
        VE::Math::VVector2 TexCoords;
        VE::Math::VVector3 Tangent;
        VE::Math::VVector3 Bitangent;
    };

    struct VMesh {
        VE::Internal::Core::Container::TVector<VVertex> vertices;
        VE::Internal::Core::Container::TVector<uint32_t> indices;
        std::shared_ptr<VE::Internal::RHI::IRHIMesh> rhiMesh;
        std::string name;
    };

    class VModel {
        public:
            VModel() = default;
            VModel(const std::string& path);
            ~VModel() = default;

            // Loading functions
            bool LoadFromFile(const std::string& path);
            bool LoadFromMemory(const void* data, size_t size, const std::string& format = "");
            
            // GPU resource creation
            bool CreateGPUResources(VE::Internal::RHI::IRHIDevice* device);
            void DestroyGPUResources();

            // Rendering
            void Draw() const;
            void Draw(uint32_t meshIndex) const;

            // Getters
            const VE::Internal::Core::Container::TVector<VMesh>& GetMeshes() const { return m_Meshes; }
            const VMesh& GetMesh(uint32_t index) const;
            uint32_t GetMeshCount() const { return static_cast<uint32_t>(m_Meshes.size()); }
            const std::string& GetPath() const { return m_FilePath; }
            bool IsLoaded() const { return m_IsLoaded; }

        private:
            VE::Internal::Core::Container::TVector<VMesh> m_Meshes;
            std::string m_FilePath;
            std::string m_Directory;
            bool m_IsLoaded = false;
            
            Assimp::Importer m_Importer;

            // Assimp processing
            void ProcessNode(aiNode* node, const aiScene* scene);
            VMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
            void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
            
            // Helper functions
            VE::Math::VVector3 AssimpVec3ToVE(const aiVector3D& vec);
            VE::Math::VVector2 AssimpVec2ToVE(const aiVector2D& vec);
    };
}