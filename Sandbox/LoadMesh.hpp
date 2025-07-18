#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

#include <Shared/tinygltf/tiny_gltf.h>

#include <Shared/STB/stb_image.h>

#include "common.h"


struct MeshData {
    std::vector<VVector3> positions;
    std::vector<VVector3> normals;
    std::vector<VVector2> uvs;
    std::vector<unsigned int> indices;
    std::string diffuseTexturePath;

    std::vector<unsigned char> diffuseTextureData; // For embedded
    int textureWidth = 0;
    int textureHeight = 0;
    int textureComponents = 0; // Usually 3 (RGB) or 4 (RGBA)
};

std::shared_ptr<VTexture> LoadDiffuseTexture(
    const MeshData& mesh,
    VRDevice* renderer,
    const VTextureSampler& sampler = {}
) {

    // 1. Use embedded data if available
    if (!mesh.diffuseTextureData.empty() && mesh.textureWidth > 0 && mesh.textureHeight > 0) {
        VTextureFormat format = VTextureFormat::RGBA8;
        if (mesh.textureComponents == 3) format = VTextureFormat::RGB8;
        else if (mesh.textureComponents == 4) format = VTextureFormat::RGBA8;
        else {
            std::cerr << "Unsupported texture format: " << mesh.textureComponents << " components.\n";
            return nullptr;
        }

        return renderer->CreateTexture2DInstance(
            mesh.diffuseTextureData.data(),
            mesh.textureWidth,
            mesh.textureHeight,
            format,
            sampler
        );
    }

    // 2. Fall back to external path (load with stb_image or similar)
    if (!mesh.diffuseTexturePath.empty()) {
        int width, height, channels;
        unsigned char* data = stbi_load(mesh.diffuseTexturePath.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load texture from path: " << mesh.diffuseTexturePath << "\n";
            return nullptr;
        }

        VTextureFormat format = VTextureFormat::RGBA8;
        if (channels == 3) format = VTextureFormat::RGB8;
        else if (channels == 4) format = VTextureFormat::RGBA8;

        auto texture = renderer->CreateTexture2DInstance(data, width, height, format, sampler);
        stbi_image_free(data);
        return texture;
    }

    // 3. No texture available
    std::cerr << "No diffuse texture data found.\n";
    return nullptr;
}


inline MeshData LoadMeshFromGLTF(const std::string& path) {
    MeshData mesh;
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    if (!ret) {
        std::cerr << "Failed to load GLTF: " << err << "\n";
        return mesh;
    }

    if (!warn.empty()) {
        std::cout << "GLTF warning: " << warn << "\n";
    }

    if (model.meshes.empty()) {
        std::cerr << "No meshes found in GLTF file.\n";
        return mesh;
    }

    const tinygltf::Mesh& gltfMesh = model.meshes[0]; // Load only the first mesh
    const tinygltf::Primitive& primitive = gltfMesh.primitives[0]; // Only first primitive

    // Index buffer
    if (primitive.indices >= 0) {
        const auto& accessor = model.accessors[primitive.indices];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];

        const uint8_t* dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];

        for (size_t i = 0; i < accessor.count; ++i) {
            uint32_t index = 0;
            switch (accessor.componentType) {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    index = reinterpret_cast<const uint16_t*>(dataPtr)[i];
                    break;
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    index = reinterpret_cast<const uint32_t*>(dataPtr)[i];
                    break;
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    index = reinterpret_cast<const uint8_t*>(dataPtr)[i];
                    break;
                default:
                    std::cerr << "Unsupported index component type.\n";
                    return mesh;
            }
            mesh.indices.push_back((unsigned int)index);
        }
    }

    // Helper for VVector3 attributes
    auto getVVector3Attribute = [&](const std::string& name, std::vector<VVector3>& out) {
        if (primitive.attributes.count(name) == 0) return;

        const auto& accessor = model.accessors[primitive.attributes.at(name)];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];

        const float* data = reinterpret_cast<const float*>(
            &buffer.data[accessor.byteOffset + bufferView.byteOffset]);

        for (size_t i = 0; i < accessor.count; ++i) {
            VVector3 v = {
                data[i * 3 + 0],
                data[i * 3 + 1],
                data[i * 3 + 2]
            };
            out.push_back(v);
        }
    };

    // Helper for VVector2 attributes
    auto getVVector2Attribute = [&](const std::string& name, std::vector<VVector2>& out) {
        if (primitive.attributes.count(name) == 0) return;

        const auto& accessor = model.accessors[primitive.attributes.at(name)];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];

        const float* data = reinterpret_cast<const float*>(
            &buffer.data[accessor.byteOffset + bufferView.byteOffset]);

        for (size_t i = 0; i < accessor.count; ++i) {
            VVector2 v = {
                data[i * 2 + 0],
                data[i * 2 + 1]
            };
            out.push_back(v);
        }
    };

    // Load attributes
    getVVector3Attribute("POSITION", mesh.positions);
    getVVector3Attribute("NORMAL", mesh.normals);
    getVVector2Attribute("TEXCOORD_0", mesh.uvs);

    // Load diffuse texture (embedded or external)
    if (primitive.material >= 0) {
        const tinygltf::Material& material = model.materials[primitive.material];
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            int textureIndex = material.pbrMetallicRoughness.baseColorTexture.index;
            const tinygltf::Texture& texture = model.textures[textureIndex];

            if (texture.source >= 0) {
                const tinygltf::Image& image = model.images[texture.source];

                if (!image.uri.empty()) {
                    // External texture
                    mesh.diffuseTexturePath = image.uri;
                } else if (!image.image.empty()) {
                    // Embedded texture
                    mesh.diffuseTextureData = image.image;
                    mesh.textureWidth = image.width;
                    mesh.textureHeight = image.height;
                    mesh.textureComponents = image.component; // number of channels
                }
            }
        }
    }

    // std::cout << "Mesh has: " << model.materials.size() << " Textures" << std::endl;

    return mesh;
}
