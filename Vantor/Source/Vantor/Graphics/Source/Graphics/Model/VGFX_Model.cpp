/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Graphics/Public/Model/VGFX_Model.hpp>
#include <RHI/Interface/VRHI_Device.hpp>
#include <RHI/Interface/VRHI_Mesh.hpp>

#include <iostream>
#include <filesystem>

namespace VE::Graphics {

    VModel::VModel(const std::string& path) {
        LoadFromFile(path);
    }

    bool VModel::LoadFromFile(const std::string& path) {
        m_FilePath = path;
        m_Directory = std::filesystem::path(path).parent_path().string();
        
        // Assimp loading flags
        unsigned int flags = aiProcess_Triangulate | 
                           // aiProcess_FlipUVs |  // causes texture sampling issues, idk why
                           aiProcess_CalcTangentSpace |
                           aiProcess_GenSmoothNormals |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_ImproveCacheLocality |
                           aiProcess_LimitBoneWeights |
                           aiProcess_RemoveRedundantMaterials |
                           aiProcess_SplitLargeMeshes |
                           aiProcess_Triangulate |
                           aiProcess_GenUVCoords |
                           aiProcess_SortByPType |
                           aiProcess_FindDegenerates |
                           aiProcess_FindInvalidData;

        const aiScene* scene = m_Importer.ReadFile(path, flags);
        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "VModel::LoadFromFile() - ERROR::ASSIMP::" << m_Importer.GetErrorString() << std::endl;
            return false;
        }

        // Clear any existing meshes
        m_Meshes.clear();

        // Process the scene
        ProcessNode(scene->mRootNode, scene);
        
        m_IsLoaded = true;
        std::cout << "VModel::LoadFromFile() - Successfully loaded model: " << path 
                  << " with " << m_Meshes.size() << " meshes" << std::endl;
        
        return true;
    }

    bool VModel::LoadFromMemory(const void* data, size_t size, const std::string& format) {
        // Assimp loading flags
        unsigned int flags = aiProcess_Triangulate | 
                           // aiProcess_FlipUVs |  // causes texture sampling issues, idk why
                           aiProcess_CalcTangentSpace |
                           aiProcess_GenSmoothNormals |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_ImproveCacheLocality |
                           aiProcess_LimitBoneWeights |
                           aiProcess_RemoveRedundantMaterials |
                           aiProcess_SplitLargeMeshes |
                           aiProcess_Triangulate |
                           aiProcess_GenUVCoords |
                           aiProcess_SortByPType |
                           aiProcess_FindDegenerates |
                           aiProcess_FindInvalidData;

        const aiScene* scene = m_Importer.ReadFileFromMemory(data, size, flags, format.c_str());
        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "VModel::LoadFromMemory() - ERROR::ASSIMP::" << m_Importer.GetErrorString() << std::endl;
            return false;
        }

        // Clear any existing meshes
        m_Meshes.clear();

        // Process the scene
        ProcessNode(scene->mRootNode, scene);
        
        m_IsLoaded = true;
        std::cout << "VModel::LoadFromMemory() - Successfully loaded model from memory with " 
                  << m_Meshes.size() << " meshes" << std::endl;
        
        return true;
    }

    bool VModel::CreateGPUResources(VE::Internal::RHI::IRHIDevice* device) {
        if (!device) {
            std::cerr << "VModel::CreateGPUResources() - Device is null" << std::endl;
            return false;
        }

        bool success = true;
        for (auto& mesh : m_Meshes) {
            if (mesh.vertices.empty() || mesh.indices.empty()) {
                continue;
            }

            // Create vertex layout
            VE::Internal::RHI::VVertexLayout layout;
            layout.stride = sizeof(VVertex);
            
            // Position attribute
            VE::Internal::RHI::VVertexAttribute posAttr;
            posAttr.location = 0;
            posAttr.format = VE::Internal::RHI::ERHIFormat::R32G32B32_FLOAT;
            posAttr.offset = offsetof(VVertex, Position);
            layout.attributes.push_back(posAttr);
            
            // Normal attribute  
            VE::Internal::RHI::VVertexAttribute normalAttr;
            normalAttr.location = 1;
            normalAttr.format = VE::Internal::RHI::ERHIFormat::R32G32B32_FLOAT;
            normalAttr.offset = offsetof(VVertex, Normal);
            layout.attributes.push_back(normalAttr);
            
            // TexCoords attribute
            VE::Internal::RHI::VVertexAttribute texAttr;
            texAttr.location = 2;
            texAttr.format = VE::Internal::RHI::ERHIFormat::R32G32_FLOAT;
            texAttr.offset = offsetof(VVertex, TexCoords);
            layout.attributes.push_back(texAttr);
            
            // Tangent attribute
            VE::Internal::RHI::VVertexAttribute tangentAttr;
            tangentAttr.location = 3;
            tangentAttr.format = VE::Internal::RHI::ERHIFormat::R32G32B32_FLOAT;
            tangentAttr.offset = offsetof(VVertex, Tangent);
            layout.attributes.push_back(tangentAttr);
            
            // Bitangent attribute
            VE::Internal::RHI::VVertexAttribute bitangentAttr;
            bitangentAttr.location = 4;
            bitangentAttr.format = VE::Internal::RHI::ERHIFormat::R32G32B32_FLOAT;
            bitangentAttr.offset = offsetof(VVertex, Bitangent);
            layout.attributes.push_back(bitangentAttr);

            // Create the mesh
            mesh.rhiMesh = device->CreateMesh(
                mesh.vertices.data(),
                static_cast<uint32_t>(mesh.vertices.size() * sizeof(VVertex)),
                mesh.indices.data(),
                static_cast<uint32_t>(mesh.indices.size()),
                layout
            );

            if (!mesh.rhiMesh) {
                std::cerr << "VModel::CreateGPUResources() - Failed to create GPU mesh: " << mesh.name << std::endl;
                success = false;
            }
        }

        return success;
    }

    void VModel::DestroyGPUResources() {
        for (auto& mesh : m_Meshes) {
            mesh.rhiMesh.reset();
        }
    }

    void VModel::Draw() const {
        for (const auto& mesh : m_Meshes) {
            if (mesh.rhiMesh) {
                mesh.rhiMesh->Bind();
                mesh.rhiMesh->Draw();
                mesh.rhiMesh->Unbind();
            }
        }
    }

    void VModel::Draw(uint32_t meshIndex) const {
        if (meshIndex >= m_Meshes.size()) {
            std::cerr << "VModel::Draw() - Invalid mesh index: " << meshIndex << std::endl;
            return;
        }

        const auto& mesh = m_Meshes[meshIndex];
        if (mesh.rhiMesh) {
            mesh.rhiMesh->Bind();
            mesh.rhiMesh->Draw();
            mesh.rhiMesh->Unbind();
        }
    }

    const VMesh& VModel::GetMesh(uint32_t index) const {
        if (index >= m_Meshes.size()) {
            throw std::out_of_range("VModel::GetMesh() - Index out of range");
        }
        return m_Meshes[index];
    }

    void VModel::ProcessNode(aiNode* node, const aiScene* scene) {
        // Process all the node's meshes
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }

        // Process all the node's children
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    VMesh VModel::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        VMesh vMesh;
        vMesh.name = mesh->mName.C_Str();

        // Process vertices
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            VVertex vertex;

            // Position
            vertex.Position = AssimpVec3ToVE(mesh->mVertices[i]);

            // Normal
            if (mesh->HasNormals()) {
                vertex.Normal = AssimpVec3ToVE(mesh->mNormals[i]);
            }

            // Texture coordinates
            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = VE::Math::VVector2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
            } else {
                vertex.TexCoords = VE::Math::VVector2(0.0f, 0.0f);
            }

            // Tangent
            if (mesh->HasTangentsAndBitangents()) {
                vertex.Tangent = AssimpVec3ToVE(mesh->mTangents[i]);
                vertex.Bitangent = AssimpVec3ToVE(mesh->mBitangents[i]);
            }

            vMesh.vertices.push_back(vertex);
        }

        // Process indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                vMesh.indices.push_back(face.mIndices[j]);
            }
        }

        // Process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            // For now
            LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
        }

        return vMesh;
    }

    void VModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            
            std::string texturePath = m_Directory + "/" + std::string(str.C_Str());
            
            // For now, just log the texture path
            std::cout << "VModel::LoadMaterialTextures() - Found texture: " << texturePath 
                      << " (type: " << typeName << ")" << std::endl;
        }
    }

    VE::Math::VVector3 VModel::AssimpVec3ToVE(const aiVector3D& vec) {
        return VE::Math::VVector3(vec.x, vec.y, vec.z);
    }

    VE::Math::VVector2 VModel::AssimpVec2ToVE(const aiVector2D& vec) {
        return VE::Math::VVector2(vec.x, vec.y);
    }

}
