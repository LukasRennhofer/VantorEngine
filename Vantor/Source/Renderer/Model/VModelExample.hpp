/*
 * Example usage of VModel and VModelGLTF with VTransformComponent integration
 * 
 * This example shows how to:
 * 1. Load a GLTF model
 * 2. Create entities for each node
 * 3. Sync transforms between model nodes and VTransformComponents
 * 4. Play animations
 */

#include <Vantor/Vantor.hpp>
#include <Vantor/Renderer/Model/VRE_Model.hpp>
#include <Vantor/ObjectSystem/Component/VOS_Base.hpp>

// Example usage class
class VModelExample {
private:
    std::unique_ptr<Vantor::Renderer::VModelGLTF> m_Model;
    std::map<std::string, std::shared_ptr<Vantor::Object::VObject>> m_ModelEntities;
    std::map<std::string, Vantor::Object::VTransformComponent*> m_NodeToTransform;
    
public:
    bool LoadModel(const std::string& filePath) {
        m_Model = std::make_unique<Vantor::Renderer::VModelGLTF>();
        
        if (!m_Model->LoadFromFile(filePath)) {
            return false;
        }
        
        // Create entities for each node
        CreateEntitiesFromModel();
        
        return true;
    }
    
    void CreateEntitiesFromModel() {
        if (!m_Model) return;
        
        // Traverse all nodes and create entities
        m_Model->TraverseNodes([this](Vantor::Renderer::VModelNode* node) {
            if (node->name.empty()) return;
            
            // Create entity for this node
            auto entity = VORegistry::CreateEntity();
            
            // Add transform component
            entity->AddComponentVoid<Vantor::Object::VTransformComponent>();
            auto transform = entity->GetComponent<Vantor::Object::VTransformComponent>();
            
            // Apply node transform to component
            node->ApplyToTransformComponent(transform);
            
            // If node has mesh, add mesh and material components
            if (node->mesh) {
                entity->AddComponentVoid<Vantor::Object::VMeshComponent>();
                auto meshComp = entity->GetComponent<Vantor::Object::VMeshComponent>();
                meshComp->SetMesh(node->mesh);
                
                if (node->material) {
                    entity->AddComponentVoid<Vantor::Object::VMaterialComponent>();
                    auto matComp = entity->GetComponent<Vantor::Object::VMaterialComponent>();
                    matComp->SetMaterial(node->material);
                }
            }
            
            // Store mappings
            m_ModelEntities[node->name] = entity;
            m_NodeToTransform[node->name] = transform;
        });
    }
    
    void Update(float deltaTime) {
        if (!m_Model) return;
        
        // Update animations
        m_Model->UpdateAnimations(deltaTime);
        
        // Sync model transforms to components
        m_Model->ApplyToTransformComponents(m_NodeToTransform);
    }
    
    void PlayAnimation(const std::string& animationName, bool loop = true) {
        if (m_Model) {
            m_Model->PlayAnimation(animationName, loop);
        }
    }
    
    void ManuallyControlNode(const std::string& nodeName, 
                           const Vantor::Math::VVector3& position,
                           const Vantor::Math::VQuaternion& rotation) {
        auto it = m_NodeToTransform.find(nodeName);
        if (it != m_NodeToTransform.end()) {
            it->second->SetPosition(position);
            it->second->SetRotation(rotation);
            
            // Sync back to model
            std::map<std::string, Vantor::Object::VTransformComponent*> singleNode;
            singleNode[nodeName] = it->second;
            m_Model->SynchronizeWithTransformComponents(singleNode);
        }
    }
    
    Vantor::Object::VObject* GetEntityForNode(const std::string& nodeName) {
        auto it = m_ModelEntities.find(nodeName);
        return (it != m_ModelEntities.end()) ? it->second.get() : nullptr;
    }
    
    // Get all entities for rendering
    std::vector<Vantor::Object::VObject*> GetAllEntities() {
        std::vector<Vantor::Object::VObject*> entities;
        for (const auto& [name, entity] : m_ModelEntities) {
            entities.push_back(entity.get());
        }
        return entities;
    }
};

// Usage in main application:
/*
int main() {
    VApplication app;
    // ... initialize app ...
    
    VModelExample modelExample;
    
    // Load a GLTF model
    if (!modelExample.LoadModel("Models/character.gltf")) {
        std::cout << "Failed to load model!" << std::endl;
        return -1;
    }
    
    // Play an animation
    modelExample.PlayAnimation("Walk", true);
    
    // Game loop
    while (app.IsRunning()) {
        app.BeginFrame();
        
        // Update model and animations
        modelExample.Update(app.GetDeltaTime());
        
        // Manually control a specific node (e.g., head look-at)
        if (someCondition) {
            modelExample.ManuallyControlNode("Head", 
                headPosition, 
                lookAtRotation);
        }
        
        // Render all entities from the model
        auto renderpath = app.GetRenderDevice()->CreateRenderPath3D();
        for (auto* entity : modelExample.GetAllEntities()) {
            renderpath->PushRender(entity);
        }
        renderpath->Render();
        
        app.EndFrame();
    }
    
    return 0;
}
*/
