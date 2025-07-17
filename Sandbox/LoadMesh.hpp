#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

#include "tiny_gltf.h"

#include "common.h"


struct MeshData {
    std::vector<VVector3> positions;
    std::vector<VVector3> normals;
    std::vector<VVector2> uvs;
    std::vector<unsigned int> indices;
};

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

    return mesh;
}
