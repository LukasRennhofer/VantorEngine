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
 *  File: VCT_GLFW3_impl.cpp
 *  Last Change: Automatically updated
 */

#include <iostream>

#include "../Interface/VCT_Window.hpp"
#include "../../Core/Backlog/VCO_Backlog.hpp"

#ifdef VANTOR_WM_GLFW
#include <Shared/GLFW/glfw3.h>

#ifdef VANTOR_INTEGRATION_IMGUI
#include "../../Integration/imgui/VIN_Imgui.hpp"
#endif

namespace Vantor::Context
{
    GLFWwindow                  *glfwWindow;
    Window::resizeCallbackFunc_t resizeCallbackFunc;

    Window::Window(int width, int height, const char *title)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if (glfwWindow == NULL)
        {
            Vantor::Backlog::Log("Context::Window", "Could not create GLFW Context Window", Vantor::Backlog::LogLevel::ERR);
            close();
        }
        glfwMakeContextCurrent(glfwWindow);
        glfwGetWindowSize(glfwWindow, &width, &height);
        resizeCallbackFunc = NULL;

#ifdef VANTOR_INTEGRATION_IMGUI
        Vantor::Integration::Imgui::InitContext(glfwWindow);
#endif
    }

    void Window::close() { glfwTerminate(); }

    void glfwResizeCallback(GLFWwindow *window, int width, int height)
    {
        if (resizeCallbackFunc != NULL)
        {
            resizeCallbackFunc(width, height);
        }
    }

    void Window::setResizeCallback(resizeCallbackFunc_t callbackFunc)
    {
        resizeCallbackFunc = callbackFunc;
        glfwSetFramebufferSizeCallback(glfwWindow, glfwResizeCallback);
    }

    Window::loadProc Window::getLoadProc() { return (loadProc) glfwGetProcAddress; }

    void Window::swapBuffers() { glfwSwapBuffers(glfwWindow); }

    void Window::pollEvents() { glfwPollEvents(); }

    bool Window::shouldWindowClose() { return glfwWindowShouldClose(glfwWindow); }

    void *Window::GetNativeHandle() { return glfwWindow; }
} // namespace Vantor::Context
#endif