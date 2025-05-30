/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorWindow.cpp
 *  Last Change: Automatically updated
 */

#include "vantorWindow.h"

// TODO: FullScreen for SDL2
// !! WARNING: MUST BE CHANGED WHEN VULKAN USED!!

namespace vantor::Platform
{
    unsigned int Window::SCR_WIDTH  = 1600;
    unsigned int Window::SCR_HEIGHT = 900;

#ifdef VANTOR_WM_SDL2
    SDL_GLContext vantor::Platform::Window::glContext = nullptr;
#endif

    bool  Window::keyBools[10]        = {false};
    bool  Window::wireframe           = false;
    bool  Window::firstMouse          = true;
    float Window::lastX               = SCR_WIDTH / 2.0f;
    float Window::lastY               = SCR_HEIGHT / 2.0f;
    bool  Window::mouseCursorDisabled = true;

    Window::Window(int &success, bool FullScreen, unsigned int scrW, unsigned int scrH, std::string name) : name(name)
    {
        Window::SCR_WIDTH  = scrW;
        Window::SCR_HEIGHT = scrH;

/*

    - SDL2 INIT -

*/
#ifdef VANTOR_WM_SDL2
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
        {
            vantor::Backlog::Log("Window", "SDL could not initialize!", vantor::Backlog::LogLevel::ERR);
            success = 0;
            return;
        }

        // Set SDL OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        this->window
            = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!this->window)
        {
            vantor::Backlog::Log("Window", "Failed to create SDL window", vantor::Backlog::LogLevel::ERR);
            success = 0;
            return;
        }

        glContext = SDL_GL_CreateContext(this->window);
        if (!glContext)
        {
            vantor::Backlog::Log("Window", "Failed to create OpenGL context", vantor::Backlog::LogLevel::ERR);
            success = 0;
            return;
        }

        SDL_GL_SetSwapInterval(0); // Enable VSync / Disable VSync

        success = gladLoader();
        if (success)
        {
            vantor::Backlog::Log("Window", "SDL2 window and OpenGL context correctly initialized!", vantor::Backlog::LogLevel::INFO);
        }

        SDL_ShowCursor(SDL_DISABLE); // MOVE AWAY!!
#endif
#ifdef VANTOR_WM_GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        glfwWindowHint(GLFW_RESIZABLE, true);

        GLFWmonitor       *monitor = glfwGetPrimaryMonitor();   // Get the main monitor
        const GLFWvidmode *mode    = glfwGetVideoMode(monitor); // Get resolution

        if (FullScreen)
        {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // No window borders

            GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, name.c_str(), monitor, nullptr);
        }
        else
        {
            GLFWwindow *window = glfwCreateWindow(scrW, scrH, name.c_str(), nullptr, nullptr);
        }

        if (window == nullptr)
        {
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED); // TODO: Make this choosable

#endif
    }

    int Window::gladLoader()
    {
#ifdef VANTOR_WM_SDL2
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
        {
            vantor::Backlog::Log("Window", "Failed to initialize GLAD with SDL2", vantor::Backlog::LogLevel::ERR);
            return 0;
        }
        return 1;
#endif
#ifdef VANTOR_WM_GLFW
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            vantor::Backlog::Log("Window", "Failed to initialize GLAD with GLFW", vantor::Backlog::LogLevel::ERR);
            return 0;
        }
        return 1;
#endif
    }

    Window::~Window()
    {
        // Saving Logs (Just for now)
        vantor::Backlog::SaveLogs();
// Terminate Application
#ifdef VANTOR_WM_SDL2
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(this->w);
#endif
        this->terminate();
    }
} // namespace vantor::Platform
