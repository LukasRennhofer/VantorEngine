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
 * File: Window.cpp
 * Last Change: 
*/

#include "Window.h"

namespace chif {
    unsigned int Window::SCR_WIDTH = 1600;
    unsigned int Window::SCR_HEIGHT = 900;

    Camera* Window::camera = nullptr;
    SDL_GLContext chif::Window::glContext = nullptr;

    bool Window::keyBools[10] = { false };
    bool Window::wireframe = false;
    bool Window::firstMouse = true;
    float Window::lastX = SCR_WIDTH / 2.0f;
    float Window::lastY = SCR_HEIGHT / 2.0f;
    bool Window::mouseCursorDisabled = true;

    Window::Window(int& success, unsigned int scrW, unsigned int scrH, std::string name) : name(name) {
        std::cout << "[INIT::CORE] Using CHIFEngine Core " << chif::version::GetVersionString() << std::endl;
        Window::SCR_WIDTH = scrW;
        Window::SCR_HEIGHT = scrH;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
            std::cout << "[ERROR::WINDOW] SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
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
            std::cout << "[ERROR::WINDOW] Failed to create SDL window: " << SDL_GetError() << std::endl;
            success = 0;
            return;
        }

        glContext = SDL_GL_CreateContext(this->w);
        if (!glContext) {
            std::cout << "[ERROR::WINDOW] Failed to create OpenGL context: " << SDL_GetError() << std::endl;
            success = 0;
            return;
        }

        SDL_GL_SetSwapInterval(0); // Enable VSync / Disable VSync

        Window::camera = nullptr;
        oldState = newState = 0;

        success = gladLoader();
        if (success) {
            std::cout << "[INFO::WINDOW] SDL2 window and OpenGL context correctly initialized!" << std::endl;
        }
    }

    int Window::gladLoader() {
        SDL_ShowCursor(SDL_DISABLE);
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cout << "[ERROR::WINDOW] Failed to initialize GLAD" << std::endl;
            return 0;
        }
        return 1;
    }

    void Window::mouse_callback(SDL_Window* window, double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        if (!mouseCursorDisabled && Window::camera) {
            Window::camera->ProcessMouseMovement(xoffset, yoffset);
        }
    }

    void Window::scroll_callback(SDL_Window* window, double xoffset, double yoffset) {
        if (Window::camera)
            Window::camera->ProcessMouseScroll(yoffset);
    }

    void Window::framebuffer_size_callback(SDL_Window* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::processInput(float frameTime) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Handle quit event
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            // Handle keyboard input
            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
            if (camera) {
                if (currentKeyStates[SDL_SCANCODE_W]) {
                    camera->ProcessKeyboard(FORWARD, frameTime);
                }
                if (currentKeyStates[SDL_SCANCODE_S]) {
                    camera->ProcessKeyboard(BACKWARD, frameTime);
                }
                if (currentKeyStates[SDL_SCANCODE_A]) {
                    camera->ProcessKeyboard(LEFT, frameTime);
                }
                if (currentKeyStates[SDL_SCANCODE_D]) {
                    camera->ProcessKeyboard(RIGHT, frameTime);
                }
            }

            // Handle mouse motion
            if (event.type == SDL_MOUSEMOTION) {
                mouse_callback(this->w, event.motion.x, event.motion.y);
            }

            // Handle mouse button events
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    newState = (event.type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
                    if (newState == 0 && oldState == 1) {
                        mouseCursorDisabled = !mouseCursorDisabled;
                        SDL_ShowCursor(mouseCursorDisabled ? SDL_DISABLE : SDL_ENABLE);
                        if (mouseCursorDisabled) firstMouse = true;
                    }
                    oldState = newState;
                }
            }

            // Handle mouse wheel scrolling
            if (event.type == SDL_MOUSEWHEEL) {
                scroll_callback(this->w, event.wheel.x, event.wheel.y);
            }

            // Handle window resizing
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                framebuffer_size_callback(this->w, event.window.data1, event.window.data2);
            }
        }

        // Handle controller/joystick input
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                SDL_GameController* controller = SDL_GameControllerOpen(i);
                if (controller) {
                    Sint16 axisLeftY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
                    Sint16 axisLeftX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

                    if (camera) {
                        const int DEADZONE = 8000; // Deadzone to prevent accidental movement
                        if (axisLeftY < -DEADZONE) camera->ProcessKeyboard(FORWARD, frameTime);
                        if (axisLeftY > DEADZONE) camera->ProcessKeyboard(BACKWARD, frameTime);
                        if (axisLeftX < -DEADZONE) camera->ProcessKeyboard(LEFT, frameTime);
                        if (axisLeftX > DEADZONE) camera->ProcessKeyboard(RIGHT, frameTime);
                    }

                    SDL_GameControllerClose(controller);
                }
            }
        }
    }

    Window::~Window() {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(this->w);
        this->terminate();
    }
}