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
 *  Date: 2025-07-09
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

#include "../../Core/BackLog/VCO_Backlog.hpp"
#include <Shared/imgui/imgui.h>

#ifdef VANTOR_API_OPENGL
#include <Shared/imgui/Backend/imgui_impl_opengl3.h>
#endif

#ifdef VANTOR_WM_GLFW
#define GLFW_INCLUDE_NONE // Prevent GLFW from including OpenGL headers
#include <Shared/GLFW/glfw3.h>
#include <Shared/imgui/Backend/imgui_impl_glfw.h>
#endif

// VORegistry
#include "../../ObjectSystem/Register/VOS_Registry.hpp"
#include "../../ObjectSystem/Component/VOS_Base.hpp"
#include "../../ObjectSystem/VOS_Object.hpp"

#include <cstring>

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

        style.WindowRounding = 6.0f;
        style.FrameRounding  = 4.0f;
        style.GrabRounding   = 4.0f;

        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize  = 1.0f;
        style.PopupBorderSize  = 1.0f;

        ImVec4 accent = ImVec4(0.15f, 0.55f, 0.75f, 1.0f);   // Cool blue
        ImVec4 bg     = ImVec4(0.10f, 0.105f, 0.11f, 1.00f); // Soft dark

        ImVec4 *colors                    = style.Colors;
        colors[ImGuiCol_WindowBg]         = bg;
        colors[ImGuiCol_Border]           = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
        colors[ImGuiCol_TitleBgActive]    = accent;
        colors[ImGuiCol_ButtonHovered]    = accent;
        colors[ImGuiCol_ButtonActive]     = ImVec4(0.2f, 0.6f, 0.9f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]   = accent;
        colors[ImGuiCol_FrameBgActive]    = ImVec4(0.25f, 0.6f, 0.85f, 1.0f);
        colors[ImGuiCol_CheckMark]        = accent;
        colors[ImGuiCol_SliderGrabActive] = accent;

#ifdef VANTOR_API_OPENGL
        // Initialize ImGui GLFW backend for OpenGL
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        // Use GLSL version 330 core, you can adjust if you use a different GLSL version
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
