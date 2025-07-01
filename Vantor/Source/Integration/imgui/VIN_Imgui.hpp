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
 *  Date: 2025-07-01
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
#include <GLFW/glfw3.h>
#include <Shared/imgui/Backend/imgui_impl_glfw.h>
#endif

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

        ImGui::StyleColorsDark();

        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

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
        // If you want a generic ImGui context initializer not tied to GLFW, add here.
        // Currently unused, can be implemented if supporting other windowing systems.
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
