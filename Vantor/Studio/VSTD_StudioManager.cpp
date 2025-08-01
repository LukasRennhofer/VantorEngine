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
 *  File: VSTD_StudioManager.cpp
 *  Last Change: Automatically updated
 */

#include "VSTD_StudioManager.hpp"
#include "Panels/VSTD_Scene.hpp"
#include "Panels/VSTD_Viewport.hpp"
#include "Panels/VSTD_Backlog.hpp"

#include "../Source/Integration/imgui/VIN_Imgui.hpp"
#include "../Source/Core/VCO_Version.hpp"

#include <Shared/imgui/imgui.h>

namespace Vantor::Studio {

    VStudioManager::~VStudioManager() {
    }

    void VStudioManager::Initialize() {
        // Initialize all studio panels
        SetupStyle();
        SetupDefaultPanels();
    }

    void VStudioManager::Shutdown() {
        // Clear all panels - unique_ptr will handle cleanup
        // Panel manager will automatically clean up when destroyed
    }

    void VStudioManager::RenderInterface(Vantor::RenderDevice::VRenderTarget* target) {
        Vantor::Integration::Imgui::BuildUpDocking();
        // Setup main menu bar
        RenderMainMenuBar();
        
        m_PanelManager.GetPanel<VSViewportPanel>()->SetRenderTarget(target);

        // Render all panels through the panel manager
        m_PanelManager.OnImGuiRenderAll();
    }

    void VStudioManager::SetupDefaultPanels() {
        // Register the Scene Panel
        auto scenePanel = std::make_unique<VScenePanel>();
        m_PanelManager.RegisterPanel(std::move(scenePanel));

        // Register ViewportPanel
        auto viewPanel = std::make_unique<VSViewportPanel>();
        m_PanelManager.RegisterPanel(std::move(viewPanel));

         // Register BacklogPanel
        auto logPanel = std::make_unique<VSBacklog>();
        m_PanelManager.RegisterPanel(std::move(logPanel));

        // TODO: Add more panels as they are implemented
        // auto inspectorPanel = std::make_unique<VInspectorPanel>();
        // m_PanelManager.RegisterPanel(std::move(inspectorPanel));
        
        // auto consolePanel = std::make_unique<VConsolePanel>();
        // m_PanelManager.RegisterPanel(std::move(consolePanel));
        
        // auto assetBrowserPanel = std::make_unique<VAssetBrowserPanel>();
        // m_PanelManager.RegisterPanel(std::move(assetBrowserPanel));
    }

    void VStudioManager::RenderMainMenuBar() {
        if (ImGui::BeginMainMenuBar()) {

            // Window Menu
            if (ImGui::BeginMenu("Window")) {
                // Toggle panel visibility
                auto scenePanel = m_PanelManager.GetPanel<VScenePanel>();
                if (scenePanel) {
                    bool visible = scenePanel->isVisible();
                    if (ImGui::MenuItem("Scene Hierarchy", nullptr, visible)) {
                        // TODO: Implement panel visibility toggle
                        // This would require extending the panel interface
                    }
                }
                
                // TODO: Add more window options as panels are implemented
                // if (ImGui::MenuItem("Inspector", nullptr, false)) {}
                // if (ImGui::MenuItem("Console", nullptr, false)) {}
                // if (ImGui::MenuItem("Asset Browser", nullptr, false)) {}
                
                ImGui::Separator();
                
                if (ImGui::MenuItem("Reset Layout")) {
                    // TODO: Implement layout reset functionality
                }
                
                ImGui::EndMenu();
            }

            // Help Menu
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About Vantor Studio")) {
                    m_ShowAboutDialog = true;
                }
                
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Render dialogs
        RenderDialogs();
    }

    void VStudioManager::CreateEmptyGameObject() {
        auto newObject = Vantor::Object::VORegistry::CreateEntity<Vantor::Object::VObject>();
        auto tagComponent = newObject->AddComponent<Vantor::Object::VTagComponent>();
        tagComponent->SetName("Empty GameObject");
        newObject->AddComponent<Vantor::Object::VTransformComponent>();
    }

    void VStudioManager::CreatePrimitive(const std::string& primitiveType) {
        // TODO: This would need access to the geometry classes
        // For now, just create an empty object with the name
        auto newObject = Vantor::Object::VORegistry::CreateEntity<Vantor::Object::VObject>();
        auto tagComponent = newObject->AddComponent<Vantor::Object::VTagComponent>();
        tagComponent->SetName(primitiveType);
        newObject->AddComponent<Vantor::Object::VTransformComponent>();
        newObject->AddComponent<Vantor::Object::VMeshComponent>();
        
        // TODO: Set up the actual mesh based on primitive type
        // if (primitiveType == "Cube") {
        //     auto cube = Vantor::Object::VORegistry::CreateEntity<Vantor::Renderer::Geometry::VCube>();
        //     // Setup cube...
        // }
    }

    void VStudioManager::CreateLight(const std::string& lightType) {
        auto newObject = Vantor::Object::VORegistry::CreateEntity<Vantor::Object::VObject>();
        auto tagComponent = newObject->AddComponent<Vantor::Object::VTagComponent>();
        tagComponent->SetName(lightType);
        newObject->AddComponent<Vantor::Object::VTransformComponent>();
        
        // TODO: Add light component when it's implemented
        // newObject->AddComponent<Vantor::Object::VLightComponent>();
    }

    void VStudioManager::RenderDialogs() {
        // About Dialog
        if (m_ShowAboutDialog) {
            ImGui::OpenPopup("About Vantor Engine");
        }

        if (ImGui::BeginPopupModal("About Vantor Engine", &m_ShowAboutDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Vantor Engine");
            ImGui::Separator();
            ImGui::Text("Version: %s", Vantor::Core::Version::GetVersionString().c_str());
            ImGui::Text("Built using a lot of coffee");
            ImGui::Text("Copyright (c) 2025 Lukas Rennhofer");
            
            ImGui::Separator();
            
            if (ImGui::Button("Close")) {
                m_ShowAboutDialog = false;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
    }

    void VStudioManager::SetupStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        ImGuiIO& io = ImGui::GetIO();

        // Load custom font
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/VStudio/Fonts/JetBrainsMono-Regular.ttf", 18.0f);

        // Window & panel backgrounds
        colors[ImGuiCol_WindowBg]             = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_ChildBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_PopupBg]              = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_MenuBarBg]            = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);

        // Frames and widgets
        colors[ImGuiCol_FrameBg]              = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgActive]        = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

        // Title bars
        colors[ImGuiCol_TitleBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgActive]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.08f, 0.08f, 0.08f, 0.75f);

        // Scrollbars
        colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

        // Buttons
        colors[ImGuiCol_Button]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);

        // Headers (collapsing, tree, etc.)
        colors[ImGuiCol_Header]               = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered]        = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_HeaderActive]         = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

        // Tabs
        colors[ImGuiCol_Tab]                  = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabHovered]           = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TabActive]            = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocused]         = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

        // Separators
        colors[ImGuiCol_Separator]            = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_SeparatorActive]      = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

        // Resize grips
        colors[ImGuiCol_ResizeGrip]           = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
        colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.35f, 0.35f, 0.35f, 0.70f);
        colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.40f, 0.40f, 0.40f, 0.90f);

        // Tables
        colors[ImGuiCol_TableHeaderBg]        = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]    = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_TableBorderLight]     = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TableRowBgAlt]        = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);

        // Text
        colors[ImGuiCol_Text]                 = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.30f, 0.30f, 0.30f, 0.35f);

        // Misc
        colors[ImGuiCol_CheckMark]            = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_DragDropTarget]       = ImVec4(0.90f, 0.60f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]         = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_Border]               = ImVec4(0.15f, 0.15f, 0.15f, 0.50f);
        colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
        colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
        colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

        // Style metrics
        style.WindowPadding         = ImVec2(8.0f, 8.0f);
        style.FramePadding          = ImVec2(6.0f, 4.0f);
        style.ItemSpacing           = ImVec2(8.0f, 6.0f);
        style.ItemInnerSpacing      = ImVec2(6.0f, 4.0f);
        style.IndentSpacing         = 20.0f;
        style.ScrollbarSize         = 16.0f;
        style.GrabMinSize           = 12.0f;

        // Rounding
        style.WindowRounding        = 4.0f;
        style.FrameRounding         = 3.0f;
        style.PopupRounding         = 4.0f;
        style.ScrollbarRounding     = 8.0f;
        style.GrabRounding          = 3.0f;
        style.TabRounding           = 4.0f;

        // Borders
        style.WindowBorderSize      = 1.0f;
        style.FrameBorderSize       = 0.0f;
        style.PopupBorderSize       = 1.0f;
        style.ChildBorderSize       = 1.0f;

        // Alignment
        style.WindowTitleAlign      = ImVec2(0.0f, 0.5f);
        style.ColorButtonPosition   = ImGuiDir_Left;
        style.ButtonTextAlign       = ImVec2(0.5f, 0.5f);

        // Anti-aliasing and quality settings
        style.AntiAliasedLines      = true;                    // Anti-aliased lines
        style.AntiAliasedLinesUseTex = true;                   // Use texture for AA lines
        style.AntiAliasedFill       = true;                    // Anti-aliased fill
        style.CurveTessellationTol  = 1.25f;                   // Curve tessellation tolerance
        style.CircleTessellationMaxError = 0.30f;              // Circle tessellation error
    }

} // namespace Vantor::Studio
