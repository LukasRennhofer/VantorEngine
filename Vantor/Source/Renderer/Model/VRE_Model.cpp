/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-17
 *
 *  File: VRE_Model.cpp
 *  Last Change: Automatically updated
 */

#include "VRE_Model.hpp"
#include "../../Resource/VRES_Manager.hpp"
#include "../../RenderDevice/Interface/VRD_RenderDevice.hpp"
#include <iostream>
#include <algorithm>

namespace Vantor::Renderer {

// =============================================================================
// VAnimationChannel Implementation
// =============================================================================

void VAnimationChannel::GetTransformAtTime(float time, Vantor::Math::VVector3& translation, 
                                          Vantor::Math::VQuaternion& rotation, 
                                          Vantor::Math::VVector3& scale) const {
    if (keyframes.empty()) {
        return;
    }
    
    if (keyframes.size() == 1 || time <= keyframes[0].time) {
        translation = keyframes[0].translation;
        rotation = keyframes[0].rotation;
        scale = keyframes[0].scale;
        return;
    }
    
    if (time >= keyframes.back().time) {
        const auto& lastFrame = keyframes.back();
        translation = lastFrame.translation;
        rotation = lastFrame.rotation;
        scale = lastFrame.scale;
        return;
    }
    
    // Find keyframes to interpolate between
    size_t nextIndex = 0;
    for (size_t i = 0; i < keyframes.size() - 1; ++i) {
        if (time >= keyframes[i].time && time <= keyframes[i + 1].time) {
            nextIndex = i + 1;
            break;
        }
    }
    
    const auto& currentFrame = keyframes[nextIndex - 1];
    const auto& nextFrame = keyframes[nextIndex];
    
    float timeDiff = nextFrame.time - currentFrame.time;
    float t = (time - currentFrame.time) / timeDiff;
    
    // Interpolate translation and scale linearly
    translation = currentFrame.translation + (nextFrame.translation - currentFrame.translation) * t;
    scale = currentFrame.scale + (nextFrame.scale - currentFrame.scale) * t;
    
    // SLERP rotation
    rotation = currentFrame.rotation.Slerp(nextFrame.rotation, t);
}

// =============================================================================
// VModelAnimation Implementation
// =============================================================================

void VModelAnimation::ApplyAtTime(float time) const {
    for (const auto& channel : channels) {
        if (!channel.targetNode) continue;
        
        Vantor::Math::VVector3 translation;
        Vantor::Math::VQuaternion rotation(0.0f, 0.0f, 0.0f, 0.0f);
        Vantor::Math::VVector3 scale;
        
        channel.GetTransformAtTime(time, translation, rotation, scale);
        
        if (channel.target == "translation") {
            channel.targetNode->translation = translation;
        } else if (channel.target == "rotation") {
            channel.targetNode->rotation = rotation;
        } else if (channel.target == "scale") {
            channel.targetNode->scale = scale;
        }
        
        channel.targetNode->UpdateLocalMatrix();
    }
}

// =============================================================================
// VModelSkin Implementation
// =============================================================================

void VModelSkin::GetJointMatrices(std::vector<Vantor::Math::VMat4>& jointMatrices) const {
    jointMatrices.resize(joints.size());
    
    for (size_t i = 0; i < joints.size(); ++i) {
        if (joints[i]) {
            // Joint matrix = globalTransform * inverseBindMatrix
            jointMatrices[i] = joints[i]->worldMatrix * inverseBindMatrices[i];
        } else {
            jointMatrices[i] = Vantor::Math::VMat4::Identity();
        }
    }
}

// =============================================================================
// VModelNode Implementation
// =============================================================================

void VModelNode::UpdateLocalMatrix() {
    // Create TRS matrix
    Vantor::Math::VMat4 T = Vantor::Math::VMat4::Translate(translation);
    Vantor::Math::VMat4 R = rotation.ToMat4();
    Vantor::Math::VMat4 S = Vantor::Math::VMat4::Scale(scale);
    
    localMatrix = T * R * S;
}

void VModelNode::UpdateWorldMatrix() {
    if (parent) {
        worldMatrix = parent->worldMatrix * localMatrix;
    } else {
        worldMatrix = localMatrix;
    }
}

void VModelNode::UpdateHierarchy() {
    UpdateWorldMatrix();
    
    for (const auto& child : children) {
        child->UpdateHierarchy();
    }
}

void VModelNode::ApplyToTransformComponent(Vantor::Object::VTransformComponent* transform) const {
    if (!transform) return;
    
    transform->SetPosition(translation);
    transform->SetRotation(rotation);
    transform->SetScale(scale);
}

void VModelNode::UpdateFromTransformComponent(const Vantor::Object::VTransformComponent* transform) {
    if (!transform) return;
    
    translation = transform->GetPosition();
    rotation = transform->GetRotation();
    scale = transform->GetScale();
    
    UpdateLocalMatrix();
}

// =============================================================================
// VModel Implementation
// =============================================================================

VModel::VModel() {
}

VModel::~VModel() {
}

void VModel::UpdateAnimations(float deltaTime) {
    if (!m_IsAnimationPlaying || m_CurrentAnimationIndex < 0 || 
        m_CurrentAnimationIndex >= static_cast<s32>(m_Animations.size())) {
        return;
    }
    
    m_CurrentAnimationTime += deltaTime;
    
    const auto& animation = m_Animations[m_CurrentAnimationIndex];
    
    if (m_CurrentAnimationTime >= animation.duration) {
        if (m_IsAnimationLooping) {
            m_CurrentAnimationTime = fmod(m_CurrentAnimationTime, animation.duration);
        } else {
            m_CurrentAnimationTime = animation.duration;
            m_IsAnimationPlaying = false;
        }
    }
    
    animation.ApplyAtTime(m_CurrentAnimationTime);
    UpdateAllTransforms();
}

void VModel::PlayAnimation(const std::string& name, bool loop) {
    for (size_t i = 0; i < m_Animations.size(); ++i) {
        if (m_Animations[i].name == name) {
            PlayAnimation(i, loop);
            return;
        }
    }
    std::cout << "Animation '" << name << "' not found!" << std::endl;
}

void VModel::PlayAnimation(size_t index, bool loop) {
    if (index >= m_Animations.size()) {
        std::cout << "Animation index " << index << " out of range!" << std::endl;
        return;
    }
    
    m_CurrentAnimationIndex = static_cast<s32>(index);
    m_CurrentAnimationTime = 0.0f;
    m_IsAnimationPlaying = true;
    m_IsAnimationLooping = loop;
}

void VModel::StopAnimation() {
    m_IsAnimationPlaying = false;
}

void VModel::SetAnimationTime(float time) {
    m_CurrentAnimationTime = time;
    
    if (m_CurrentAnimationIndex >= 0 && m_CurrentAnimationIndex < static_cast<s32>(m_Animations.size())) {
        const auto& animation = m_Animations[m_CurrentAnimationIndex];
        animation.ApplyAtTime(m_CurrentAnimationTime);
        UpdateAllTransforms();
    }
}

VModelNode* VModel::FindNode(const std::string& name) {
    for (const auto& rootNode : m_RootNodes) {
        VModelNode* found = FindNodeRecursive(rootNode.get(), name);
        if (found) return found;
    }
    return nullptr;
}

void VModel::TraverseNodes(std::function<void(VModelNode*)> callback) {
    for (const auto& rootNode : m_RootNodes) {
        TraverseNodesRecursive(rootNode.get(), callback);
    }
}

void VModel::UpdateAllTransforms() {
    for (const auto& rootNode : m_RootNodes) {
        rootNode->UpdateHierarchy();
    }
}

void VModel::SynchronizeWithTransformComponents(const std::map<std::string, Vantor::Object::VTransformComponent*>& nodeToComponent) {
    for (const auto& [nodeName, component] : nodeToComponent) {
        VModelNode* node = FindNode(nodeName);
        if (node && component) {
            node->UpdateFromTransformComponent(component);
        }
    }
    UpdateAllTransforms();
}

void VModel::ApplyToTransformComponents(const std::map<std::string, Vantor::Object::VTransformComponent*>& nodeToComponent) {
    for (const auto& [nodeName, component] : nodeToComponent) {
        VModelNode* node = FindNode(nodeName);
        if (node && component) {
            node->ApplyToTransformComponent(component);
        }
    }
}

VModelNode* VModel::FindNodeRecursive(VModelNode* node, const std::string& name) {
    if (!node) return nullptr;
    
    if (node->name == name) {
        return node;
    }
    
    for (const auto& child : node->children) {
        VModelNode* found = FindNodeRecursive(child.get(), name);
        if (found) return found;
    }
    
    return nullptr;
}

void VModel::TraverseNodesRecursive(VModelNode* node, std::function<void(VModelNode*)> callback) {
    if (!node) return;
    
    callback(node);
    
    for (const auto& child : node->children) {
        TraverseNodesRecursive(child.get(), callback);
    }
}

// =============================================================================
// VModelGLTF Implementation
// =============================================================================

VModelGLTF::VModelGLTF() {
}

VModelGLTF::~VModelGLTF() {
}

bool VModelGLTF::LoadFromFile(const std::string& filePath) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    bool ret = false;
    if (filePath.find(".glb") != std::string::npos) {
        ret = loader.LoadBinaryFromFile(&m_GltfModel, &err, &warn, filePath);
    } else {
        ret = loader.LoadASCIIFromFile(&m_GltfModel, &err, &warn, filePath);
    }
    
    if (!warn.empty()) {
        std::cout << "GLTF Warning: " << warn << std::endl;
    }
    
    if (!err.empty()) {
        std::cout << "GLTF Error: " << err << std::endl;
        return false;
    }
    
    if (!ret) {
        std::cout << "Failed to load GLTF file: " << filePath << std::endl;
        return false;
    }
    
    return LoadFromGLTF(m_GltfModel);
}

bool VModelGLTF::LoadFromGLTF(const tinygltf::Model& gltfModel) {
    try {
        LoadTextures(gltfModel);
        LoadMaterials(gltfModel);
        LoadMeshes(gltfModel);
        LoadSkins(gltfModel);
        LoadNodes(gltfModel);
        LoadAnimations(gltfModel);
        
        UpdateAllTransforms();
        
        std::cout << "Successfully loaded GLTF model with:" << std::endl;
        std::cout << "  - " << m_RootNodes.size() << " root nodes" << std::endl;
        std::cout << "  - " << m_Textures.size() << " textures" << std::endl;
        std::cout << "  - " << m_Materials.size() << " materials" << std::endl;
        std::cout << "  - " << m_LoadedMeshes.size() << " meshes" << std::endl;
        std::cout << "  - " << m_Animations.size() << " animations" << std::endl;
        std::cout << "  - " << m_Skins.size() << " skins" << std::endl;
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Exception loading GLTF: " << e.what() << std::endl;
        return false;
    }
}

void VModelGLTF::LoadTextures(const tinygltf::Model& gltfModel) {
    m_Textures.reserve(gltfModel.textures.size());
    
    for (size_t i = 0; i < gltfModel.textures.size(); ++i) {
        const auto& gltfTexture = gltfModel.textures[i];
        
        if (gltfTexture.source < 0 || gltfTexture.source >= static_cast<int>(gltfModel.images.size())) {
            continue;
        }
        
        const auto& image = gltfModel.images[gltfTexture.source];
        
        VModelTexture texture;
        texture.imageIndex = gltfTexture.source;
        texture.path = image.uri;
        
        // Create texture from image data
        if (!image.image.empty()) {
            // TODO: Create texture from image.image data
            // For now, we'll leave rDeviceTexture as nullptr
            texture.rDeviceTexture = nullptr;
        }
        
        m_Textures.push_back(texture);
    }
}

void VModelGLTF::LoadMaterials(const tinygltf::Model& gltfModel) {
    m_Materials.reserve(gltfModel.materials.size());
    
    for (const auto& gltfMaterial : gltfModel.materials) {
        auto material = std::make_unique<VMaterial>();
        
        // Set material name
        if (!gltfMaterial.name.empty()) {
            // TODO: Set material name if VMaterial supports it
        }
        
        // Load PBR metallic roughness
        if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            int texIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
            if (texIndex < static_cast<int>(m_Textures.size()) && m_Textures[texIndex].rDeviceTexture) {
                // TODO: Set base color texture
                // material->SetTexture("baseColor", m_Textures[texIndex].rDeviceTexture.get(), 0);
            }
        }
        
        // Set base color factor
        const auto& baseColor = gltfMaterial.pbrMetallicRoughness.baseColorFactor;
        if (baseColor.size() >= 4) {
            // TODO: Set base color if VMaterial supports it
            // material->SetVector("baseColorFactor", 
            //     Vantor::Math::VVector4(baseColor[0], baseColor[1], baseColor[2], baseColor[3]));
        }
        
        m_Materials.push_back(std::move(material));
    }
}

void VModelGLTF::LoadMeshes(const tinygltf::Model& gltfModel) {
    m_LoadedMeshes.reserve(gltfModel.meshes.size());
    
    for (const auto& gltfMesh : gltfModel.meshes) {
        auto mesh = LoadMesh(gltfMesh, gltfModel);
        m_LoadedMeshes.push_back(mesh);
    }
}

void VModelGLTF::LoadNodes(const tinygltf::Model& gltfModel) {
    // First pass: create all nodes
    std::vector<std::unique_ptr<VModelNode>> allNodes;
    allNodes.reserve(gltfModel.nodes.size());
    
    for (size_t i = 0; i < gltfModel.nodes.size(); ++i) {
        allNodes.push_back(LoadNode(gltfModel.nodes[i], gltfModel, nullptr));
    }
    
    // Second pass: setup hierarchy
    for (size_t i = 0; i < gltfModel.nodes.size(); ++i) {
        const auto& gltfNode = gltfModel.nodes[i];
        
        for (int childIndex : gltfNode.children) {
            if (childIndex >= 0 && childIndex < static_cast<int>(allNodes.size())) {
                allNodes[childIndex]->parent = allNodes[i].get();
                allNodes[i]->children.push_back(std::move(allNodes[childIndex]));
            }
        }
    }
    
    // Find root nodes (nodes without parents)
    for (auto& node : allNodes) {
        if (node && !node->parent) {
            m_RootNodes.push_back(std::move(node));
        }
    }
}

void VModelGLTF::LoadAnimations(const tinygltf::Model& gltfModel) {
    m_Animations.reserve(gltfModel.animations.size());
    
    for (const auto& gltfAnimation : gltfModel.animations) {
        VModelAnimation animation;
        animation.name = gltfAnimation.name;
        animation.duration = 0.0f;
        
        for (const auto& gltfChannel : gltfAnimation.channels) {
            VAnimationChannel channel;
            
            // Find target node
            if (gltfChannel.target_node >= 0) {
                // TODO: Map node index to actual node pointer
                // This requires keeping track of node indices during loading
            }
            
            // Get sampler
            if (gltfChannel.sampler >= 0 && gltfChannel.sampler < static_cast<int>(gltfAnimation.samplers.size())) {
                const auto& sampler = gltfAnimation.samplers[gltfChannel.sampler];
                
                // Load keyframes from input/output accessors
                // TODO: Implement accessor data reading
                
                channel.target = gltfChannel.target_path;
            }
            
            animation.channels.push_back(channel);
        }
        
        m_Animations.push_back(animation);
    }
}

void VModelGLTF::LoadSkins(const tinygltf::Model& gltfModel) {
    m_Skins.reserve(gltfModel.skins.size());
    
    for (const auto& gltfSkin : gltfModel.skins) {
        VModelSkin skin;
        skin.name = gltfSkin.name;
        
        // Load joints
        skin.joints.reserve(gltfSkin.joints.size());
        for (int jointIndex : gltfSkin.joints) {
            // TODO: Map joint indices to actual node pointers
            skin.joints.push_back(nullptr);
        }
        
        // Load inverse bind matrices
        if (gltfSkin.inverseBindMatrices >= 0) {
            // TODO: Load inverse bind matrices from accessor
            skin.inverseBindMatrices.resize(gltfSkin.joints.size(), Vantor::Math::VMat4::Identity());
        }
        
        m_Skins.push_back(skin);
    }
}

std::unique_ptr<VModelNode> VModelGLTF::LoadNode(const tinygltf::Node& gltfNode, 
                                               const tinygltf::Model& gltfModel, 
                                               VModelNode* parent) {
    auto node = std::make_unique<VModelNode>();
    node->parent = parent;
    node->name = gltfNode.name;
    
    // Load transformation
    if (gltfNode.matrix.size() == 16) {
        // Matrix transformation
        Vantor::Math::VMat4 matrix = GetMatrix4(gltfNode.matrix);
        // TODO: Decompose matrix into TRS
        node->localMatrix = matrix;
    } else {
        // TRS transformation
        if (gltfNode.translation.size() == 3) {
            node->translation = GetVector3(gltfNode.translation);
        }
        if (gltfNode.rotation.size() == 4) {
            node->rotation = GetQuaternion(gltfNode.rotation);
        }
        if (gltfNode.scale.size() == 3) {
            node->scale = GetVector3(gltfNode.scale);
        }
        node->UpdateLocalMatrix();
    }
    
    // Load mesh
    if (gltfNode.mesh >= 0 && gltfNode.mesh < static_cast<int>(m_LoadedMeshes.size())) {
        node->mesh = m_LoadedMeshes[gltfNode.mesh];
    }
    
    // Load skin
    if (gltfNode.skin >= 0) {
        node->skinIndex = gltfNode.skin;
    }
    
    return node;
}

std::shared_ptr<Vantor::RenderDevice::VMesh> VModelGLTF::LoadMesh(const tinygltf::Mesh& gltfMesh, 
                                                                const tinygltf::Model& gltfModel) {
    if (gltfMesh.primitives.empty()) {
        return nullptr;
    }
    
    // For now, just load the first primitive
    const auto& primitive = gltfMesh.primitives[0];
    
    std::vector<VModelVertex> vertices;
    std::vector<uint32_t> indices;
    
    LoadVertexData(gltfModel, primitive, vertices, indices);
    
    // TODO : Create mesh using render device
    
    
    Vantor::RenderDevice::VMeshCreateInfo createInfo;
    createInfo.SetFinalized = false;
    
    // TODO: Create Mesh using Factory
    auto mesh = renderDevice->CreateMesh(createInfo);
    
    // Convert to engine format
    std::vector<Vantor::Math::VVector3> positions;
    std::vector<Vantor::Math::VVector3> normals;
    std::vector<Vantor::Math::VVector2> uvs;
    
    positions.reserve(vertices.size());
    normals.reserve(vertices.size());
    uvs.reserve(vertices.size());
    
    for (const auto& vertex : vertices) {
        positions.push_back(vertex.pos);
        normals.push_back(vertex.normal);
        uvs.push_back(vertex.uv);
    }
    
    mesh->SetPositions(positions);
    mesh->SetNormals(normals);
    mesh->SetUVs(uvs);
    
    if (!indices.empty()) {
        mesh->SetIndices(indices);
    }
    
    mesh->Finalize();
    
    return mesh;
}

void VModelGLTF::LoadVertexData(const tinygltf::Model& gltfModel, 
                              const tinygltf::Primitive& primitive,
                              std::vector<VModelVertex>& vertices,
                              std::vector<uint32_t>& indices) {
    // TODO: Implement full vertex data loading from GLTF accessors
    // This is a complex implementation that would need to:
    // 1. Read accessor data for positions, normals, UVs, etc.
    // 2. Handle different component types and data layouts
    // 3. Convert to engine vertex format
    
    // For now, create a simple placeholder
    vertices.clear();
    indices.clear();
    
    std::cout << "LoadVertexData: Full implementation needed for accessor data reading" << std::endl;
}

Vantor::Math::VVector3 VModelGLTF::GetVector3(const std::vector<double>& data, size_t offset) {
    if (data.size() < offset + 3) {
        return Vantor::Math::VVector3(0.0f, 0.0f, 0.0f);
    }
    return Vantor::Math::VVector3(
        static_cast<float>(data[offset]),
        static_cast<float>(data[offset + 1]),
        static_cast<float>(data[offset + 2])
    );
}

Vantor::Math::VQuaternion VModelGLTF::GetQuaternion(const std::vector<double>& data, size_t offset) {
    if (data.size() < offset + 4) {
        return Vantor::Math::VQuaternion::Identity();
    }
    return Vantor::Math::VQuaternion(
        static_cast<float>(data[offset]),      // x
        static_cast<float>(data[offset + 1]),  // y
        static_cast<float>(data[offset + 2]),  // z
        static_cast<float>(data[offset + 3])   // w
    );
}

Vantor::Math::VMat4 VModelGLTF::GetMatrix4(const std::vector<double>& data) {
    Vantor::Math::VMat4 matrix = Vantor::Math::VMat4::Identity();
    
    if (data.size() >= 16) {
        for (int i = 0; i < 16; ++i) {
            matrix.m[i] = static_cast<float>(data[i]);
        }
    }
    
    return matrix;
}

} // namespace Vantor::Renderer
