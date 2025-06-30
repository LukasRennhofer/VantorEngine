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
 *  Date: 2025-06-30
 *
 *  File: VIN_Imgui.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#ifdef VANTOR_INTEGRATION_IMGUI
#include "../../Core/BackLog/VCO_Backlog.hpp"
#include <Shared/imgui/imgui.h>
#ifdef VANTOR_WM_GLFW // GLFW Backend
#include <Shared/imgui/Backend/imgui_impl_glfw.h>
#include <Shared/GLFW/glfw3.h>
#endif
#ifdef VANTOR_API_OPENGL // OpenGL Backend
#include <Shared/imgui/Backend/imgui_impl_opengl3.h>
#include <Shared/imgui/Backend/imgui_impl_opengl3_loader.h>
#endif

namespace Vantor::Integration::Imgui
{
#ifdef VANTOR_WM_GLFW
    inline void InitContext(GLFWwindow *window)
    {
        Vantor::Backlog::Log("Integration::ImGui", "Initializing ImGui Context for GLFW", Vantor::Backlog::LogLevel::INFO);
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
#endif