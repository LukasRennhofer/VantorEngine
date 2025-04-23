/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifWindow.cpp
 * Last Change: 
*/

#include "chifWindow.h"

namespace chif::Platform {
    unsigned int Window::SCR_WIDTH = 1600;
    unsigned int Window::SCR_HEIGHT = 900;

    Camera* Window::camera = nullptr; // TODO: Remove Camera from Window Class
    SDL_GLContext chif::Platform::Window::glContext = nullptr;

    bool Window::keyBools[10] = { false };
    bool Window::wireframe = false;
    bool Window::firstMouse = true;
    float Window::lastX = SCR_WIDTH / 2.0f;
    float Window::lastY = SCR_HEIGHT / 2.0f;
    bool Window::mouseCursorDisabled = true;

    Window::Window(int& success, unsigned int scrW, unsigned int scrH, std::string name) : name(name) {
        Window::SCR_WIDTH = scrW;
        Window::SCR_HEIGHT = scrH;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
            chif::Backlog::Log("Window", "SDL could not initialize!", chif::Backlog::LogLevel::ERR);
            success = 0;
            return;
        }

        // Set SDL OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        this->w = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!this->w) {
            chif::Backlog::Log("Window", "Failed to create SDL window", chif::Backlog::LogLevel::ERR);
            success = 0;
            return;
        }

        glContext = SDL_GL_CreateContext(this->w);
        if (!glContext) {
            chif::Backlog::Log("Window", "Failed to create OpenGL context", chif::Backlog::LogLevel::ERR);
            success = 0;
            return;
        }

        SDL_GL_SetSwapInterval(0); // Enable VSync / Disable VSync

        Window::camera = nullptr;
        oldState = newState = 0;

        success = gladLoader();
        if (success) {
            chif::Backlog::Log("Window", "SDL2 window and OpenGL context correctly initialized!", chif::Backlog::LogLevel::INFO);
        }
    }

    int Window::gladLoader() {
        SDL_ShowCursor(SDL_DISABLE);
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            chif::Backlog::Log("Window", "Failed to initialize GLAD", chif::Backlog::LogLevel::ERR);
            return 0;
        }
        return 1;
    }

    void Window::scroll_callback(SDL_Window* window, double xoffset, double yoffset) {
        if (Window::camera)
            Window::camera->ProcessMouseScroll(yoffset);
    }

    void Window::framebuffer_size_callback(SDL_Window* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::mouse_callback(SDL_Window* window, double xrel, double yrel) {
        if (mouseCursorDisabled) {
            Window::camera->ProcessMouseMovement(xrel, -yrel);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_ShowCursor(SDL_DISABLE);
        }
    }
    
    void Window::processInput(float frameTime) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
    
            // Handle mouse motion while in camera mode (only if mouse cursor is disabled)
            if (event.type == SDL_MOUSEMOTION && mouseCursorDisabled) {
                mouse_callback(this->w, event.motion.xrel, event.motion.yrel);
            }
    
            // Handle mouse wheel scroll events
            if (event.type == SDL_MOUSEWHEEL) {
                scroll_callback(this->w, event.wheel.x, event.wheel.y);
            }
    
            // Handle window resize events
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                framebuffer_size_callback(this->w, event.window.data1, event.window.data2);
            }
        }
    
        // Get the current keyboard state
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
        // Switch between modes with the 'L' key
        if (currentKeyStates[SDL_SCANCODE_L]) {
            mouseCursorDisabled = !mouseCursorDisabled; // Toggle between modes
            // Adjust cursor and relative mouse mode accordingly
            SDL_ShowCursor(mouseCursorDisabled ? SDL_DISABLE : SDL_ENABLE);
            SDL_SetRelativeMouseMode(mouseCursorDisabled ? SDL_TRUE : SDL_FALSE);
            SDL_CaptureMouse(mouseCursorDisabled ? SDL_TRUE : SDL_FALSE); // Capture mouse when disabled
        }
    
        // If in camera mode (mouseCursorDisabled is true), process camera movement input
        if (camera && mouseCursorDisabled) {
            if (currentKeyStates[SDL_SCANCODE_W]) camera->ProcessKeyboard(FORWARD, frameTime);
            if (currentKeyStates[SDL_SCANCODE_S]) camera->ProcessKeyboard(BACKWARD, frameTime);
            if (currentKeyStates[SDL_SCANCODE_A]) camera->ProcessKeyboard(LEFT, frameTime);
            if (currentKeyStates[SDL_SCANCODE_D]) camera->ProcessKeyboard(RIGHT, frameTime);
        }
    
        // Escape to quit the application
        if (currentKeyStates[SDL_SCANCODE_ESCAPE]) quit = true;
    }
    


    Window::~Window() {
        // Saving Logs (Just for now)
        chif::Backlog::SaveLogs();
        // Terminate Application
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(this->w);
        this->terminate();
    }
}
