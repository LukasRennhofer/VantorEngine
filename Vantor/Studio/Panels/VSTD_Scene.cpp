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
 *  Date: 2025-07-28
 *
 *  File: VSTD_Scene.cpp
 *  Last Change: Automatically updated
 */

#include "VSTD_Scene.hpp"
#include <Shared/imgui/imgui.h>
#include <string>

namespace Vantor::Studio {

    VScenePanel::VScenePanel() {

    }

    void VScenePanel::OnImGuiRender() {
        // Get all entities from the registry
        auto allObjects = Vantor::Object::VORegistry::GetAllEntitiesList();

        for (auto& object : allObjects) {
            if (!object) continue;
            
            // Only draw root objects (objects without parents)
            if (auto parent = object->GetParent().lock()) {
                continue; // Skip objects that have parents - they'll be drawn as children
            }
            
            DrawObjectNode(object);
        }

        // Right-click context menu for creating new objects
        if (ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            if (ImGui::MenuItem("Create Empty Object")) {
                auto newObject = Vantor::Object::VORegistry::CreateEntity<Vantor::Object::VObject>();
                auto tagComponent = newObject->AddComponent<Vantor::Object::VTagComponent>();
                tagComponent->SetName("New Object");
                newObject->AddComponent<Vantor::Object::VTransformComponent>();
            }
            
            if (ImGui::BeginMenu("Create Primitive")) {
                if (ImGui::MenuItem("Cube")) {
                    // This would require access to geometry creation
                    // auto cube = Vantor::Object::VORegistry::CreateEntity<Vantor::Renderer::Geometry::VCube>();
                }
                if (ImGui::MenuItem("Sphere")) {
                    // auto sphere = Vantor::Object::VORegistry::CreateEntity<Vantor::Renderer::Geometry::VSphere>();
                }
                ImGui::EndMenu();
            }
            
            ImGui::EndPopup();
        }

        // Properties panel
        if (m_ShowProperties) {
            DrawObjectPropertiesPanel();
        }
    }

    void VScenePanel::DrawObjectNode(std::shared_ptr<Vantor::Object::VObject> object) {
        if (!object) return;

        // Get object name from tag component
        std::string name = "Unnamed Object";
        if (object->HasComponent<Vantor::Object::VTagComponent>()) {
            auto tagComponent = object->GetComponent<Vantor::Object::VTagComponent>();
            if (tagComponent) {
                name = tagComponent->GetName();
            }
        }

        // Add object ID to make names unique
        std::string displayName = name + "##" + std::to_string(object->GetID());

        // Set up tree node flags
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        // Check if this object is selected
        if (m_SelectedObject && m_SelectedObject->GetID() == object->GetID()) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        // Check if object has children
        const auto& children = object->GetChildren();
        if (children.empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        // Create tree node
        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)object->GetID(), flags, "%s", name.c_str());

        // Handle selection
        if (ImGui::IsItemClicked()) {
            m_SelectedObject = object;
        }

        // Right-click context menu for individual objects
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Object")) {
                if (m_SelectedObject && m_SelectedObject->GetID() == object->GetID()) {
                    m_SelectedObject = nullptr;
                }
                Vantor::Object::VORegistry::DestroyEntity(object->GetID());
            }
            
            if (ImGui::MenuItem("Duplicate Object")) {
                // TODO: Implement object duplication
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Add Transform Component") && !object->HasComponent<Vantor::Object::VTransformComponent>()) {
                object->AddComponent<Vantor::Object::VTransformComponent>();
            }
            
            if (ImGui::MenuItem("Add Tag Component") && !object->HasComponent<Vantor::Object::VTagComponent>()) {
                auto tagComponent = object->AddComponent<Vantor::Object::VTagComponent>();
                tagComponent->SetName("New Object");
            }
            
            if (ImGui::MenuItem("Add Mesh Component") && !object->HasComponent<Vantor::Object::VMeshComponent>()) {
                object->AddComponent<Vantor::Object::VMeshComponent>();
            }
            
            if (ImGui::MenuItem("Add Material Component") && !object->HasComponent<Vantor::Object::VMaterialComponent>()) {
                object->AddComponent<Vantor::Object::VMaterialComponent>();
            }
            
            ImGui::EndPopup();
        }

        // Draw children if node is open
        if (nodeOpen && !children.empty()) {
            for (const auto& child : children) {
                DrawObjectNode(child);
            }
        }

        // Pop tree node if it was opened
        if (nodeOpen && !children.empty()) {
            ImGui::TreePop();
        }
    }

    void VScenePanel::DrawObjectPropertiesPanel() {
        ImGui::Begin("Properties##ObjectProperties");

        if (m_SelectedObject) {
            // Object header
            std::string objectName = "Unnamed Object";
            if (m_SelectedObject->HasComponent<Vantor::Object::VTagComponent>()) {
                auto tagComponent = m_SelectedObject->GetComponent<Vantor::Object::VTagComponent>();
                if (tagComponent) {
                    objectName = tagComponent->GetName();
                }
            }

            ImGui::Text("Object: %s", objectName.c_str());
            ImGui::Text("ID: %llu", m_SelectedObject->GetID());
            ImGui::Separator();

            // Draw all components
            DrawComponents(m_SelectedObject);
        } else {
            ImGui::Text("No object selected");
            ImGui::Text("Select an object in the Scene Hierarchy to view its properties.");
        }

        ImGui::End();
    }

    void VScenePanel::DrawComponents(std::shared_ptr<Vantor::Object::VObject> object) {
        if (!object) return;

        // Draw Tag Component
        if (object->HasComponent<Vantor::Object::VTagComponent>()) {
            auto component = object->GetComponent<Vantor::Object::VTagComponent>();
            if (component) {
                DrawTagComponent(component.get());
            }
        }

        // Draw Transform Component
        if (object->HasComponent<Vantor::Object::VTransformComponent>()) {
            auto component = object->GetComponent<Vantor::Object::VTransformComponent>();
            if (component) {
                DrawTransformComponent(component.get());
            }
        }

        // Draw Mesh Component
        if (object->HasComponent<Vantor::Object::VMeshComponent>()) {
            auto component = object->GetComponent<Vantor::Object::VMeshComponent>();
            if (component) {
                DrawMeshComponent(component.get());
            }
        }

        // Draw Material Component
        if (object->HasComponent<Vantor::Object::VMaterialComponent>()) {
            auto component = object->GetComponent<Vantor::Object::VMaterialComponent>();
            if (component) {
                DrawMaterialComponent(component.get());
            }
        }
    }

    void VScenePanel::DrawTransformComponent(Vantor::Object::VTransformComponent* transform) {
        if (!transform) return;

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            // === Position ===
            auto position = transform->GetPosition();
            float pos[3] = { position.x, position.y, position.z };
            ImGui::Text("Position");

            ImGui::PushID("Position");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.1f, 0.1f, 1.0f)); // X - red background
            ImGui::PushItemWidth(70);
            ImGui::DragFloat("X", &pos[0], 0.1f); ImGui::SameLine();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.3f, 0.1f, 1.0f)); // Y - green background
            ImGui::DragFloat("Y", &pos[1], 0.1f); ImGui::SameLine();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.3f, 1.0f)); // Z - blue background
            ImGui::DragFloat("Z", &pos[2], 0.1f);
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
            ImGui::PopID();

            transform->SetPosition(Vantor::Math::VVector3(pos[0], pos[1], pos[2]));

            // === Rotation ===
            auto rotation = transform->GetRotation();
            float sinr_cosp = 2 * (rotation.w * rotation.x + rotation.y * rotation.z);
            float cosr_cosp = 1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y);
            float roll = std::atan2(sinr_cosp, cosr_cosp);

            float sinp = 2 * (rotation.w * rotation.y - rotation.z * rotation.x);
            float pitch = std::abs(sinp) >= 1 ? std::copysign(3.14159f / 2, sinp) : std::asin(sinp);

            float siny_cosp = 2 * (rotation.w * rotation.z + rotation.x * rotation.y);
            float cosy_cosp = 1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z);
            float yaw = std::atan2(siny_cosp, cosy_cosp);

            float rot[3] = {
                pitch * 180.0f / 3.14159f,
                yaw   * 180.0f / 3.14159f,
                roll  * 180.0f / 3.14159f
            };

            ImGui::Text("Rotation");
            ImGui::PushID("Rotation");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.1f, 0.1f, 1.0f)); // X - red
            ImGui::PushItemWidth(70);
            ImGui::DragFloat("X", &rot[0], 1.0f); ImGui::SameLine();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.3f, 0.1f, 1.0f)); // Y - green
            ImGui::DragFloat("Y", &rot[1], 1.0f); ImGui::SameLine();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.3f, 1.0f)); // Z - blue
            ImGui::DragFloat("Z", &rot[2], 1.0f);
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
            ImGui::PopID();

            float radX = rot[0] * 3.14159f / 180.0f;
            float radY = rot[1] * 3.14159f / 180.0f;
            float radZ = rot[2] * 3.14159f / 180.0f;
            auto newRotation = Vantor::Math::VQuaternion::FromEulerAngles({ radX, radY, radZ });
            transform->SetRotation(newRotation);

            // === Scale ===
            auto scale = transform->GetScale();
            float scl[3] = { scale.x, scale.y, scale.z };
            ImGui::Text("Scale");

            ImGui::PushID("Scale");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.1f, 0.1f, 1.0f)); // X - red
            ImGui::PushItemWidth(70);
            ImGui::DragFloat("X", &scl[0], 0.1f); ImGui::SameLine();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.3f, 0.1f, 1.0f)); // Y - green
            ImGui::DragFloat("Y", &scl[1], 0.1f); ImGui::SameLine();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.3f, 1.0f)); // Z - blue
            ImGui::DragFloat("Z", &scl[2], 0.1f);
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
            ImGui::PopID();

            transform->SetScale({ scl[0], scl[1], scl[2] });
        }
    }

    void VScenePanel::DrawMaterialComponent(Vantor::Object::VMaterialComponent* material) {
        if (!material) return;

        if (ImGui::CollapsingHeader("Material")) {
            ImGui::Text("Material: %s", material->GetMaterial() ? "Assigned" : "None");
            
            // TODO: Add material selection/editing UI
            if (ImGui::Button("Select Material...")) {
                // Open material browser/selector
            }
            
            if (material->GetMaterial()) {
                ImGui::SameLine();
                if (ImGui::Button("Clear Material")) {
                    material->SetMaterial(nullptr);
                }
            }
        }
    }

    void VScenePanel::DrawMeshComponent(Vantor::Object::VMeshComponent* mesh) {
        if (!mesh) return;

        if (ImGui::CollapsingHeader("Mesh")) {
            auto meshPtr = mesh->GetMesh();
            ImGui::Text("Mesh: %s", meshPtr ? "Assigned" : "None");
            
            // TODO: Add mesh selection/editing UI
            if (ImGui::Button("Select Mesh...")) {
                // Open mesh browser/selector
            }
            
            if (meshPtr) {
                ImGui::SameLine();
                if (ImGui::Button("Clear Mesh")) {
                    mesh->SetMesh(nullptr);
                }
                
                // TODO: Display mesh information (vertex count, etc.)
                ImGui::Text("Mesh Info:");
                ImGui::Text("  - Vertex Count: N/A");
                ImGui::Text("  - Triangle Count: N/A");
            }
        }
    }

    void VScenePanel::DrawTagComponent(Vantor::Object::VTagComponent* tag) {
        if (!tag) return;

        if (ImGui::CollapsingHeader("Tag", ImGuiTreeNodeFlags_DefaultOpen)) {
            std::string name = tag->GetName();
            char buffer[256];
            strncpy(buffer, name.c_str(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';
            
            if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                tag->SetName(std::string(buffer));
            }
        }
    }

} // namespace Vantor::Studio
