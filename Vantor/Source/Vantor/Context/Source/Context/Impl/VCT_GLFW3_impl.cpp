/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

// #include <Core/BackLog/VCO_Backlog.hpp>
#include <Context/Interface/VCT_Window.hpp>

#ifdef VANTOR_WM_GLFW
#include <Shared/GLFW/glfw3.h>

#ifdef VANTOR_INTEGRATION_IMGUI
#include <Integration/VIN_ImGui.hpp>
#endif

namespace VE::Internal::Context
{
    GLFWwindow                  *glfwWindow;
    VWindow::resizeCallbackFunc_t resizeCallbackFunc;

    VWindow::VWindow(int width, int height, const char *title)
    {
        // TODO: Make Vulkan Compatible too
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if (glfwWindow == NULL)
        {
            // TODO: VE::Internal::Core::Backlog::Log("Context", "Could not create GLFW Context for VWindow", VE::Internal::Core::Backlog::ELogLevel::ERR);
            close();
        }
        glfwMakeContextCurrent(glfwWindow);
        glfwGetWindowSize(glfwWindow, &width, &height);
        resizeCallbackFunc = NULL;

#ifdef VANTOR_INTEGRATION_IMGUI
        VE::Internal::Integration::Imgui::InitContext(glfwWindow);
#endif
    }

    void VWindow::close() { glfwTerminate(); }

    void glfwResizeCallback(GLFWwindow *window, int width, int height)
    {
        if (resizeCallbackFunc != NULL)
        {
            resizeCallbackFunc(width, height);
        }
    }

    void VWindow::setResizeCallback(resizeCallbackFunc_t callbackFunc)
    {
        resizeCallbackFunc = callbackFunc;
        glfwSetFramebufferSizeCallback(glfwWindow, glfwResizeCallback);
    }

    VWindow::loadProc VWindow::getLoadProc() const { return (loadProc) glfwGetProcAddress; }

    void VWindow::swapBuffers() { glfwSwapBuffers(glfwWindow); }

    void VWindow::pollEvents() { glfwPollEvents(); }

    bool VWindow::shouldWindowClose() { return glfwWindowShouldClose(glfwWindow); }

    void *VWindow::GetNativeHandle() { return glfwWindow; }
} // namespace VE::Internal::Context
#endif