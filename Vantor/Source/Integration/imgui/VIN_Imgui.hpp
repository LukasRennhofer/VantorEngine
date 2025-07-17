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
 *  Date: 2025-07-16
 *
 *  File: VIN_Imgui.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#ifdef VANTOR_INTEGRATION_IMGUI

#ifdef VANTOR_API_OPENGL
// Tell ImGui OpenGL backend to use glad loader
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#endif

#include <Shared/imgui/imgui.h>

#include "../../Core/BackLog/VCO_Backlog.hpp"
#ifdef VANTOR_API_OPENGL
#include <Shared/imgui/Backend/imgui_impl_opengl3.h>
#endif

#ifdef VANTOR_WM_GLFW
#define GLFW_INCLUDE_NONE // Prevent GLFW from including OpenGL headers
#include <Shared/GLFW/glfw3.h>
#include <Shared/imgui/Backend/imgui_impl_glfw.h>
#endif

// VORegistry
#include <cstring>

#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/Register/VOS_Registry.hpp"
#include "../../ObjectSystem/VOS_Object.hpp"

namespace Vantor::Integration::Imgui
{
#ifdef VANTOR_WM_GLFW
    inline void InitContext(GLFWwindow *window)
    {
        Vantor::Backlog::Log("Integration::ImGui", "Initializing ImGui Context for GLFW", Vantor::Backlog::LogLevel::INFO);

        // Get content scale of primary monitor for DPI scaling
        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        style.WindowRounding    = 4.0f;
        style.ChildRounding     = 4.0f;
        style.FrameRounding     = 4.0f;
        style.GrabRounding      = 4.0f;
        style.ScrollbarRounding = 4.0f;
        style.FrameBorderSize   = 1.0f;
        style.WindowBorderSize  = 1.0f;

        style.WindowPadding    = ImVec2(8, 8);
        style.FramePadding     = ImVec2(6, 4);
        style.ItemSpacing      = ImVec2(6, 4);
        style.ItemInnerSpacing = ImVec2(6, 4);

        // Dark Theme
        ImVec4 *colors                             = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]              = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]                  = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
        colors[ImGuiCol_ChildBg]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                   = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_Border]                    = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow]              = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                   = ImVec4(0.15f, 0.15f, 0.15f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]            = ImVec4(0.48f, 0.26f, 0.98f, 0.40f);
        colors[ImGuiCol_FrameBgActive]             = ImVec4(0.37f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBg]                   = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]             = ImVec4(0.21f, 0.16f, 0.48f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]          = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]                 = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]               = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]             = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]      = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]       = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]                 = ImVec4(0.45f, 0.26f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab]                = ImVec4(0.41f, 0.00f, 1.00f, 0.40f);
        colors[ImGuiCol_SliderGrabActive]          = ImVec4(0.48f, 0.26f, 0.98f, 0.52f);
        colors[ImGuiCol_Button]                    = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
        colors[ImGuiCol_ButtonHovered]             = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_ButtonActive]              = ImVec4(0.34f, 0.06f, 0.98f, 1.00f);
        colors[ImGuiCol_Header]                    = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_HeaderHovered]             = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
        colors[ImGuiCol_HeaderActive]              = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_Separator]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]          = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]           = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]                = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_ResizeGripHovered]         = ImVec4(1.00f, 1.00f, 1.00f, 0.13f);
        colors[ImGuiCol_ResizeGripActive]          = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        colors[ImGuiCol_TabHovered]                = ImVec4(0.40f, 0.26f, 0.98f, 0.50f);
        colors[ImGuiCol_Tab]                       = ImVec4(0.18f, 0.20f, 0.58f, 0.73f);
        colors[ImGuiCol_TabSelected]               = ImVec4(0.29f, 0.20f, 0.68f, 1.00f);
        colors[ImGuiCol_TabSelectedOverline]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_TabDimmed]                 = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabDimmedSelected]         = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
        colors[ImGuiCol_PlotLines]                 = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]          = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]             = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]      = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]             = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]         = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]          = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]             = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextLink]                  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]            = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_DragDropTarget]            = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavCursor]                 = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]     = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]         = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]          = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

#ifdef VANTOR_API_OPENGL
        // Initialize ImGui GLFW backend for OpenGL
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        ImGui_ImplOpenGL3_Init("#version 330 core");
#endif
    }
#endif

    inline void InitMain()
    {
        // TODO
    }

    inline void NewFrame()
    {
#ifdef VANTOR_API_OPENGL
        ImGui_ImplOpenGL3_NewFrame();
#endif
#ifdef VANTOR_WM_GLFW
        ImGui_ImplGlfw_NewFrame();
#endif
        ImGui::NewFrame();
    }

    inline void Render()
    {
#ifdef VANTOR_API_OPENGL
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
    }

    inline void BuildUpDocking()
    {
        static bool               dockspaceOpen             = true;
        static bool               opt_fullscreen_persistant = true;
        bool                      opt_fullscreen            = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags           = ImGuiDockNodeFlags_PassthruCentralNode;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Begin("Vantor Dockspace", &dockspaceOpen, window_flags);

        if (opt_fullscreen) ImGui::PopStyleVar(2);

        ImGuiIO    &io          = ImGui::GetIO();
        ImGuiStyle &style       = ImGui::GetStyle();
        float       minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x   = 370.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();

        style.WindowMinSize.x = minWinSizeX;
    }

    inline void DrawComponents(std::shared_ptr<Vantor::Object::VObject> object)
    {
        if (object->HasComponent<Vantor::Object::VTagComponent>())
        {
            if (ImGui::CollapsingHeader("Tag Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::BeginChild("TagComponentBox", ImVec2(0, 60), true, ImGuiWindowFlags_None); // box with border
                ImGui::Text("Tag");

                auto        tag = object->GetComponent<Vantor::Object::VTagComponent>()->GetName();
                std::string new_tag;

                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

                if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
                {
                    new_tag = std::string(buffer);
                    object->GetComponent<Vantor::Object::VTagComponent>()->SetName(new_tag);
                }

                ImGui::EndChild();
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
    }

    inline void ShowSceneHierachy()
    {
        ImGui::Begin("Scene Hierarchy");

        static std::shared_ptr<Vantor::Object::VObject> p_SelectedObject = nullptr; // Make it static so it holds the VObject everytime

        for (auto entity : Vantor::Object::VORegistry::GetAllEntitiesList())
        {
            std::string name = "NoTagName";

            if (entity->HasComponent<Vantor::Object::VTagComponent>())
            {
                name = entity->GetComponent<Vantor::Object::VTagComponent>()->GetName();
            }

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (p_SelectedObject && p_SelectedObject->GetID() == entity->GetID())
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

            bool opened = ImGui::TreeNodeEx((void *) (uint64_t) (uint32_t) entity->GetID(), flags, name.c_str());

            if (ImGui::IsItemClicked())
            {
                p_SelectedObject = entity;
            }

            if (opened)
            {
                ImGuiTreeNodeFlags flags  = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
                bool               opened = ImGui::TreeNodeEx((void *) 9817239, flags, name.c_str());
                if (opened) ImGui::TreePop();
                ImGui::TreePop();
            }
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (p_SelectedObject)
        {
            DrawComponents(p_SelectedObject);
        }

        ImGui::End();
    }

    inline void Destroy()
    {
#ifdef VANTOR_API_OPENGL
        ImGui_ImplOpenGL3_Shutdown();
#endif
#ifdef VANTOR_WM_GLFW
        ImGui_ImplGlfw_Shutdown();
#endif
        ImGui::DestroyContext();
    }
} // namespace Vantor::Integration::Imgui

#endif // VANTOR_INTEGRATION_IMGUI