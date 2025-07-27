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

    // Texture info for different maps
    std::string diffuseTexturePath;
    std::string normalTexturePath;
    std::string metallicTexturePath;
    std::string roughnessTexturePath;
    std::string aoTexturePath;

    std::vector<unsigned char> diffuseTextureData;
    std::vector<unsigned char> normalTextureData;
    std::vector<unsigned char> metallicTextureData;
    std::vector<unsigned char> roughnessTextureData;
    std::vector<unsigned char> aoTextureData;

    int diffuseWidth = 0, diffuseHeight = 0, diffuseComponents = 0;
    int normalWidth = 0, normalHeight = 0, normalComponents = 0;
    int metallicWidth = 0, metallicHeight = 0, metallicComponents = 0;
    int roughnessWidth = 0, roughnessHeight = 0, roughnessComponents = 0;
    int aoWidth = 0, aoHeight = 0, aoComponents = 0;
};

// General texture loader function to reduce code duplication
std::shared_ptr<VTexture> LoadTexture(
    const std::vector<unsigned char>& embeddedData,
    int width, int height, int components,
    const std::string& path,
    VRDevice* renderer,
    const VTextureSampler& sampler = {}
) {
    if (!embeddedData.empty() && width > 0 && height > 0) {
        VTextureFormat format = VTextureFormat::RGBA8;
        if (components == 3) format = VTextureFormat::RGB8;
        else if (components == 4) format = VTextureFormat::RGBA8;
        else {
            std::cerr << "Unsupported texture format: " << components << " components.\n";
            return nullptr;
        }
        return renderer->CreateTexture2DInstance(embeddedData.data(), width, height, format, sampler);
    }

    if (!path.empty()) {
        int w, h, c;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, 0);
        if (!data) {
            std::cerr << "Failed to load texture from path: " << path << "\n";
            return nullptr;
        }
        VTextureFormat format = VTextureFormat::RGBA8;
        if (c == 3) format = VTextureFormat::RGB8;
        else if (c == 4) format = VTextureFormat::RGBA8;
        auto tex = renderer->CreateTexture2DInstance(data, w, h, format, sampler);
        stbi_image_free(data);
        return tex;
    }

    std::cerr << "No texture data found.\n";
    return nullptr;
}

std::shared_ptr<VTexture> LoadDiffuseTexture(const MeshData& mesh, VRDevice* renderer, const VTextureSampler& sampler = {}) {
    return LoadTexture(mesh.diffuseTextureData, mesh.diffuseWidth, mesh.diffuseHeight, mesh.diffuseComponents, mesh.diffuseTexturePath, renderer, sampler);
}

std::shared_ptr<VTexture> LoadNormalTexture(const MeshData& mesh, VRDevice* renderer, const VTextureSampler& sampler = {}) {
    return LoadTexture(mesh.normalTextureData, mesh.normalWidth, mesh.normalHeight, mesh.normalComponents, mesh.normalTexturePath, renderer, sampler);
}

std::shared_ptr<VTexture> LoadMetallicTexture(const MeshData& mesh, VRDevice* renderer, const VTextureSampler& sampler = {}) {
    return LoadTexture(mesh.metallicTextureData, mesh.metallicWidth, mesh.metallicHeight, mesh.metallicComponents, mesh.metallicTexturePath, renderer, sampler);
}

std::shared_ptr<VTexture> LoadRoughnessTexture(const MeshData& mesh, VRDevice* renderer, const VTextureSampler& sampler = {}) {
    return LoadTexture(mesh.roughnessTextureData, mesh.roughnessWidth, mesh.roughnessHeight, mesh.roughnessComponents, mesh.roughnessTexturePath, renderer, sampler);
}

std::shared_ptr<VTexture> LoadAOTexture(const MeshData& mesh, VRDevice* renderer, const VTextureSampler& sampler = {}) {
    return LoadTexture(mesh.aoTextureData, mesh.aoWidth, mesh.aoHeight, mesh.aoComponents, mesh.aoTexturePath, renderer, sampler);
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

    const tinygltf::Mesh& gltfMesh = model.meshes[0];
    const tinygltf::Primitive& primitive = gltfMesh.primitives[0];

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

    // Load vertex attributes
    getVVector3Attribute("POSITION", mesh.positions);
    getVVector3Attribute("NORMAL", mesh.normals);
    getVVector2Attribute("TEXCOORD_0", mesh.uvs);

    // Load textures
    if (primitive.material >= 0) {
        const tinygltf::Material& material = model.materials[primitive.material];

        // Albedo / Base Color (diffuse)
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            int texIdx = material.pbrMetallicRoughness.baseColorTexture.index;
            const tinygltf::Texture& tex = model.textures[texIdx];
            if (tex.source >= 0) {
                const tinygltf::Image& img = model.images[tex.source];
                if (!img.uri.empty()) {
                    mesh.diffuseTexturePath = img.uri;
                } else if (!img.image.empty()) {
                    mesh.diffuseTextureData = img.image;
                    mesh.diffuseWidth = img.width;
                    mesh.diffuseHeight = img.height;
                    mesh.diffuseComponents = img.component;
                }
            }
        }

        // Normal map
        if (material.normalTexture.index >= 0) {
            int texIdx = material.normalTexture.index;
            const tinygltf::Texture& tex = model.textures[texIdx];
            if (tex.source >= 0) {
                const tinygltf::Image& img = model.images[tex.source];
                if (!img.uri.empty()) {
                    mesh.normalTexturePath = img.uri;
                } else if (!img.image.empty()) {
                    mesh.normalTextureData = img.image;
                    mesh.normalWidth = img.width;
                    mesh.normalHeight = img.height;
                    mesh.normalComponents = img.component;
                }
            }
        }

        // Metallic and roughness can be combined in the same texture (metallicRoughnessTexture)
        if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
            int texIdx = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
            const tinygltf::Texture& tex = model.textures[texIdx];
            if (tex.source >= 0) {
                const tinygltf::Image& img = model.images[tex.source];
                if (!img.uri.empty()) {
                    mesh.metallicTexturePath = img.uri;   // same texture for metallic & roughness
                    mesh.roughnessTexturePath = img.uri;
                } else if (!img.image.empty()) {
                    mesh.metallicTextureData = img.image;
                    mesh.roughnessTextureData = img.image;
                    mesh.metallicWidth = img.width;
                    mesh.metallicHeight = img.height;
                    mesh.metallicComponents = img.component;
                    mesh.roughnessWidth = img.width;
                    mesh.roughnessHeight = img.height;
                    mesh.roughnessComponents = img.component;
                }
            }
        } else {
            // If separate metallic or roughness textures exist (non-standard, but possible)
            // (Note: glTF spec expects combined metallicRoughnessTexture, but some models may separate)
            // Handle here if needed...
        }

        // Ambient Occlusion map (occlusionTexture)
        if (material.occlusionTexture.index >= 0) {
            int texIdx = material.occlusionTexture.index;
            const tinygltf::Texture& tex = model.textures[texIdx];
            if (tex.source >= 0) {
                const tinygltf::Image& img = model.images[tex.source];
                if (!img.uri.empty()) {
                    mesh.aoTexturePath = img.uri;
                } else if (!img.image.empty()) {
                    mesh.aoTextureData = img.image;
                    mesh.aoWidth = img.width;
                    mesh.aoHeight = img.height;
                    mesh.aoComponents = img.component;
                }
            }
        }
    }

    return mesh;
}
