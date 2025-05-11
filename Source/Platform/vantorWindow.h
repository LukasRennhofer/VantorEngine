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
 *  Date: 2025-05-11
 *
 *  File: vantorWindow.h
 *  Last Change: Automatically updated
 */

#pragma once

// --- System & Graphics Includes ---
#include <glad/glad.h>
#include <iostream>
#include <format>

// --- Platform Includes ---
#ifdef VANTOR_WM_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif

#ifdef VANTOR_WM_GLFW
#ifdef __cplusplus
extern "C"
{
#endif
#include <GLFW/glfw3.h>
#ifdef __cplusplus
}
#endif
#endif

// --- Internal Includes ---
#include "../Core/vantorVersion.h"
#include "../Utils/constants.h"
#include "../Core/Backlog/vantorBacklog.h"

namespace vantor::Platform
{
    class Window
    {
        public:
            Window(int         &success,
                   bool         FullScreen = true,
                   unsigned int SCR_WIDTH  = 1600,
                   unsigned int SCR_HEIGHT = 900,
                   std::string  name       = "Vantor " + vantor::Core::version::GetVersionString());
            ~Window();

#ifdef VANTOR_WM_SDL2
            SDL_Window *window;
            SDL_Window *getWindow() const { return window; }
#endif

#ifdef VANTOR_WM_GLFW
            GLFWwindow *window;
            GLFWwindow *getWindow() const { return window; }
#endif

            void processInput(float frameTime);

            static unsigned int SCR_WIDTH;
            static unsigned int SCR_HEIGHT;

            void terminate()
            {
#ifdef VANTOR_WM_SDL2
                SDL_Quit();
#endif
#ifdef VANTOR_WM_GLFW
                glfwTerminate();
#endif
            }

            bool continueLoop() { return !quit; }

            void swapBuffersAndPollEvents()
            {
#ifdef VANTOR_WM_SDL2
                SDL_GL_SwapWindow(this->window);
                SDL_PumpEvents();
#endif
#ifdef VANTOR_WM_GLFW
                glfwSwapBuffers(this->window);
                glfwPollEvents();
#endif
            }

#ifdef VANTOR_WM_SDL2
            static SDL_GLContext glContext;
#endif

        private:
            int oldState, newState;
            int gladLoader();

#ifdef VANTOR_WM_SDL2
            static void framebuffer_size_callback(SDL_Window *window, int width, int height);
            static void mouse_callback(SDL_Window *window, double xpos, double ypos);
            static void scroll_callback(SDL_Window *window, double xoffset, double yoffset);
#endif

#ifdef VANTOR_WM_GLFW
            static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
            static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
            static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
#endif

            static bool  keyBools[10];
            static bool  mouseCursorDisabled;
            static bool  wireframe;
            static bool  firstMouse;
            static float lastX;
            static float lastY;

            std::string name;
            bool        quit = false;
    };
} // namespace vantor::Platform
