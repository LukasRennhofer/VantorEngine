/*
 *                  ~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-21
 *
 * File: chifWindow.h (SDL2 port)
 * Last Change: 
*/

#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <format>

#include <camera.h>

#include "../Core/chifVersion.h"
#include "../Utils/constants.h"
#include "../Core/Backlog/chifBacklog.h"

namespace chif::Platform {
    class Window
    {
    public:
        Window(int& success, unsigned int SCR_WIDTH = 1600, unsigned int SCR_HEIGHT = 900, std::string name = "CHIFEngine");
        ~Window();

        // Replacing GLFWwindow* with SDL_Window*
        SDL_Window* w;
        SDL_Window* getWindow() const { return w; }

        void processInput(float frameTime); // input handler

        static unsigned int SCR_WIDTH;
        static unsigned int SCR_HEIGHT;

        void terminate() {
            SDL_Quit();
        }

        bool isWireframeActive() {
            return Window::wireframe;
        }

        bool continueLoop() {
            return !quit;
        }

        void swapBuffersAndPollEvents() {
            SDL_GL_SwapWindow(this->w);
            SDL_PumpEvents();
        }

        static Camera* camera;
		static SDL_GLContext glContext;

    private:
        int oldState, newState;
        int gladLoader();

        // SDL uses its event system instead of GLFW callbacks
        static void framebuffer_size_callback(SDL_Window* window, int width, int height);
        static void mouse_callback(SDL_Window* window, double xpos, double ypos);
        static void scroll_callback(SDL_Window* window, double xoffset, double yoffset);

        static bool keyBools[10];

        static bool mouseCursorDisabled;

        static bool wireframe;

        static bool firstMouse;
        static float lastX;
        static float lastY;

        std::string name;
        bool quit = false; // SDL-specific loop flag
    };
}