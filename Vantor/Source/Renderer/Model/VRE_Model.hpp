#pragma once

// RenderDevice
#include "../../RenderDevice/Interface/VRD_Mesh.hpp"
#include "../../RenderDevice/Interface/VRD_Texture.hpp"

#include "../../Core/Container/VCO_Vector.hpp" // Vector
#include "../../Core/VCO_Types.hpp"

// Math
#include "../../Math/Linear/VMA_Matrix.hpp"
#include "../../Math/Linear/VMA_Quaternation.hpp"

// Renderer / Material
#include "../VRE_MaterialLibrary.hpp"
#include "../VRE_Material.hpp"

// Object System
#include "../../ObjectSystem/Component/VOS_Base.hpp"

#include <Shared/tinygltf/tiny_gltf.h>

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace Vantor::Renderer {
    
    // Forward declarations
    struct VModelNode;
    struct VModelAnimation;
    struct VModelSkin;
    class VModel;

    // A Model Vertex, that is the standard layout of the Engine
    struct VModelVertex {
        Vantor::Math::VVector3 pos;
        Vantor::Math::VVector3 normal;
        Vantor::Math::VVector2 uv;
        Vantor::Math::VVector3 tangent;
        
        // Animation data
        Vantor::Math::VVector4 joints = {0.0f, 0.0f, 0.0f, 0.0f};  // Joint indices
        Vantor::Math::VVector4 weights = {0.0f, 0.0f, 0.0f, 0.0f}; // Joint weights
    };

    // Animation keyframe data
    struct VAnimationKeyframe {
        float time;
        Vantor::Math::VVector3 translation;
        Vantor::Math::VQuaternion rotation;
        Vantor::Math::VVector3 scale;
    };

    // Animation channel (affects one node)
    struct VAnimationChannel {
        std::string target; // "translation", "rotation", "scale"
        VModelNode* targetNode;
        std::vector<VAnimationKeyframe> keyframes;
        
        // Get interpolated transform at given time
        void GetTransformAtTime(float time, Vantor::Math::VVector3& translation, 
                               Vantor::Math::VQuaternion& rotation, Vantor::Math::VVector3& scale) const;
    };

    // Complete animation
    struct VModelAnimation {
        std::string name;
        float duration;
        std::vector<VAnimationChannel> channels;
        
        // Apply animation at given time to all affected nodes
        void ApplyAtTime(float time) const;
    };

    // Skin for skeletal animation
    struct VModelSkin {
        std::string name;
        std::vector<VModelNode*> joints;
        std::vector<Vantor::Math::VMat4> inverseBindMatrices;
        VModelNode* skeleton; // Root joint
        
        // Get joint matrices for current pose
        void GetJointMatrices(std::vector<Vantor::Math::VMat4>& jointMatrices) const;
    };

    struct VModelTexture {
        s32 imageIndex;
        std::shared_ptr<Vantor::RenderDevice::VTexture> rDeviceTexture;
        std::string path;
    };

    // A VModelNode represents an object in the glTF scene graph
    struct VModelNode {
        VModelNode* parent = nullptr;
        std::vector<std::unique_ptr<VModelNode>> children;
        std::shared_ptr<Vantor::RenderDevice::VMesh> mesh;
        VMaterial* material = nullptr;
        
        // Transform data (local to parent)
        Vantor::Math::VVector3 translation = {0.0f, 0.0f, 0.0f};
        Vantor::Math::VQuaternion rotation = Vantor::Math::VQuaternion::Identity();
        Vantor::Math::VVector3 scale = {1.0f, 1.0f, 1.0f};
        
        // Computed matrices
        Vantor::Math::VMat4 localMatrix = Vantor::Math::VMat4::Identity();
        Vantor::Math::VMat4 worldMatrix = Vantor::Math::VMat4::Identity();
        
        // Animation and skinning
        s32 skinIndex = -1;
        s32 jointIndex = -1; // If this node is a joint
        
        std::string name;
        
        // Update local matrix from TRS
        void UpdateLocalMatrix();
        
        // Update world matrix (call after parent's world matrix is updated)
        void UpdateWorldMatrix();
        
        // Recursively update all children
        void UpdateHierarchy();
        
        // Integration with VTransformComponent
        void ApplyToTransformComponent(Vantor::Object::VTransformComponent* transform) const;
        void UpdateFromTransformComponent(const Vantor::Object::VTransformComponent* transform);
    };

    class VModel {
    public:
        VModel();
        virtual ~VModel();
        
        // Core functionality
        virtual bool LoadFromFile(const std::string& filePath) = 0;
        virtual void UpdateAnimations(float deltaTime);
        
        // Getters
        const std::vector<std::unique_ptr<VModelNode>>& GetRootNodes() const { return m_RootNodes; }
        const std::vector<VModelTexture>& GetTextures() const { return m_Textures; }
        const std::vector<std::unique_ptr<VMaterial>>& GetMaterials() const { return m_Materials; }
        const std::vector<VModelAnimation>& GetAnimations() const { return m_Animations; }
        const std::vector<VModelSkin>& GetSkins() const { return m_Skins; }
        
        // Animation control
        void PlayAnimation(const std::string& name, bool loop = true);
        void PlayAnimation(size_t index, bool loop = true);
        void StopAnimation();
        void SetAnimationTime(float time);
        float GetAnimationTime() const { return m_CurrentAnimationTime; }
        bool IsAnimationPlaying() const { return m_IsAnimationPlaying; }
        
        // Node traversal
        VModelNode* FindNode(const std::string& name);
        void TraverseNodes(std::function<void(VModelNode*)> callback);
        
        // Transform updates
        void UpdateAllTransforms();
        
        // Component integration helpers
        void SynchronizeWithTransformComponents(const std::map<std::string, Vantor::Object::VTransformComponent*>& nodeToComponent);
        void ApplyToTransformComponents(const std::map<std::string, Vantor::Object::VTransformComponent*>& nodeToComponent);

    protected:
        // Model data
        std::vector<std::unique_ptr<VModelNode>> m_RootNodes;
        std::vector<VModelTexture> m_Textures;
        std::vector<std::unique_ptr<VMaterial>> m_Materials;
        std::vector<VModelAnimation> m_Animations;
        std::vector<VModelSkin> m_Skins;
        
        // Animation state
        float m_CurrentAnimationTime = 0.0f;
        s32 m_CurrentAnimationIndex = -1;
        bool m_IsAnimationPlaying = false;
        bool m_IsAnimationLooping = true;
        
        // Helper methods
        void UpdateNodeHierarchy(VModelNode* node);
        VModelNode* FindNodeRecursive(VModelNode* node, const std::string& name);
        void TraverseNodesRecursive(VModelNode* node, std::function<void(VModelNode*)> callback);
    };

    class VModelGLTF : public VModel {
    public:
        VModelGLTF();
        ~VModelGLTF() override;
        
        // VModel interface
        bool LoadFromFile(const std::string& filePath) override;
        
        // GLTF specific loading
        bool LoadFromGLTF(const tinygltf::Model& gltfModel);
        
    private:
        // Loading methods
        void LoadTextures(const tinygltf::Model& gltfModel);
        void LoadMaterials(const tinygltf::Model& gltfModel);
        void LoadMeshes(const tinygltf::Model& gltfModel);
        void LoadNodes(const tinygltf::Model& gltfModel);
        void LoadAnimations(const tinygltf::Model& gltfModel);
        void LoadSkins(const tinygltf::Model& gltfModel);
        
        // Helper methods
        std::unique_ptr<VModelNode> LoadNode(const tinygltf::Node& gltfNode, 
                                           const tinygltf::Model& gltfModel, 
                                           VModelNode* parent);
        
        std::shared_ptr<Vantor::RenderDevice::VMesh> LoadMesh(const tinygltf::Mesh& gltfMesh, 
                                                            const tinygltf::Model& gltfModel);
        
        void LoadVertexData(const tinygltf::Model& gltfModel, 
                           const tinygltf::Primitive& primitive,
                           std::vector<VModelVertex>& vertices,
                           std::vector<uint32_t>& indices);
        
        // Conversion helpers
        Vantor::Math::VVector3 GetVector3(const std::vector<double>& data, size_t offset = 0);
        Vantor::Math::VQuaternion GetQuaternion(const std::vector<double>& data, size_t offset = 0);
        Vantor::Math::VMat4 GetMatrix4(const std::vector<double>& data);
        
        // Data storage for loading
        std::vector<std::shared_ptr<Vantor::RenderDevice::VMesh>> m_LoadedMeshes;
        tinygltf::Model m_GltfModel; // Keep reference for loading
    };
}