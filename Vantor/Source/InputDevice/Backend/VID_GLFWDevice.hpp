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
 *  Date: 2025-07-16
 *
 *  File: VID_GLFWDevice.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../VID_IDevice.hpp"

#ifdef VANTOR_WM_GLFW
#include <Shared/GLFW/glfw3.h>
#include <optional>
#include <optional>
#include <string>
#include <unordered_map>

namespace Vantor::Input
{
    class VGLFWInputDevice : public VIInputDevice
    {
        public:
            VGLFWInputDevice(Vantor::Context::Window *wnd) : window(wnd)
            {
                SetupMappings();
                glfwSetScrollCallback(nativeWindow,
                                      [](GLFWwindow *window, double xoffset, double yoffset)
                                      {
                                          auto *input = static_cast<VGLFWInputDevice *>(glfwGetWindowUserPointer(window));
                                          if (input)
                                          {
                                              input->scrollX += xoffset;
                                              input->scrollY += yoffset;
                                          }
                                      });

                glfwSetWindowUserPointer(nativeWindow, this);
            }

            void Update() override
            {
                previousKeys = currentKeys;

                // Update Mouse
                prevMouseX = mouseX;
                prevMouseY = mouseY;
                glfwGetCursorPos(nativeWindow, &mouseX, &mouseY);

                scrollDeltaX = scrollX;
                scrollDeltaY = scrollY;
                scrollX      = 0.0;
                scrollY      = 0.0;

                // --- Keyboard ---
                for (const auto &[key, glfwKey] : keyMapping)
                {
                    currentKeys[static_cast<int>(key)] = glfwGetKey(nativeWindow, glfwKey) == GLFW_PRESS;
                }

                // --- Mouse Buttons ---
                for (const auto &[key, glfwButton] : mouseButtonMapping)
                {
                    currentKeys[static_cast<int>(key)] = glfwGetMouseButton(nativeWindow, glfwButton) == GLFW_PRESS;
                }

                // --- Gamepad Buttons (only first controller supported) ---
                //     if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) // TODO
                //     {
                //         GLFWgamepadstate state;
                //         if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                //         {
                //             for (const auto &[key, glfwButton] : gamepadButtonMapping)
                //             {
                //                 currentKeys[static_cast<int>(key)] = state.buttons[glfwButton] == GLFW_PRESS;
                //             }

                //             for (const auto &[key, glfwAxis] : gamepadAxisMapping)
                //             {
                //                 currentAxes[static_cast<int>(key)] = state.axes[glfwAxis];
                //             }
                //         }
                //     }
            }

            bool IsPressed(const VInputButton &button) const override
            {
                if (button.isAxis) return false;
                auto it = currentKeys.find(button.id);
                return it != currentKeys.end() && it->second;
            }

            bool WasPressed(const VInputButton &button) const override
            {
                if (button.isAxis) return false;
                return currentKeys.at(button.id) && !previousKeys.at(button.id);
            }

            float GetAxis(const VInputButton &axis) const override
            {
                if (!axis.isAxis) return 0.f;
                auto it = currentAxes.find(axis.axisId);
                return it != currentAxes.end() ? it->second : 0.f;
            }

            void SetMouseCursorState(bool visible) override
            {
                if (m_CursorState != visible)
                {
                    m_CursorState = visible;

                    if (visible)
                    {
                        glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                    else
                    {
                        glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    }
                }
            }

            bool GetMouseCursorState() const override { return m_CursorState; }

            Vantor::Math::VVector2 GetMousePosition() const override { return Vantor::Math::VVector2((float) mouseX, (float) mouseY); }

            Vantor::Math::VVector2 GetMouseDelta() const override
            {
                return Vantor::Math::VVector2((float) mouseX - (float) prevMouseX, (float) mouseY - (float) prevMouseY);
            }

            Vantor::Math::VVector2 GetScrollOffset() const { return Vantor::Math::VVector2((float) scrollX, (float) scrollY); }

            std::string GetDeviceName() const override { return "GLFWInputDevice"; }

        private:
            Vantor::Context::Window *window;

            GLFWwindow *nativeWindow = static_cast<GLFWwindow *>(window->GetNativeHandle());

            std::unordered_map<int, bool>  currentKeys;
            std::unordered_map<int, bool>  previousKeys;
            std::unordered_map<int, float> currentAxes;

            std::unordered_map<VEInputKey, int> keyMapping;
            std::unordered_map<VEInputKey, int> mouseButtonMapping;
            std::unordered_map<VEInputKey, int> gamepadButtonMapping;
            std::unordered_map<VEInputKey, int> gamepadAxisMapping;

            double mouseX = 0.0, mouseY = 0.0;
            double prevMouseX = 0.0, prevMouseY = 0.0;
            double scrollX = 0.0, scrollY = 0.0;
            double scrollDeltaX = 0.0, scrollDeltaY = 0.0;

            void SetupMappings()
            {
                using K = VEInputKey;

                // --- Keyboard ---
                keyMapping[K::KEY_A] = GLFW_KEY_A;
                keyMapping[K::KEY_B] = GLFW_KEY_B;
                keyMapping[K::KEY_C] = GLFW_KEY_C;
                keyMapping[K::KEY_D] = GLFW_KEY_D;
                keyMapping[K::KEY_E] = GLFW_KEY_E;
                keyMapping[K::KEY_F] = GLFW_KEY_F;
                keyMapping[K::KEY_G] = GLFW_KEY_G;
                keyMapping[K::KEY_H] = GLFW_KEY_H;
                keyMapping[K::KEY_I] = GLFW_KEY_I;
                keyMapping[K::KEY_J] = GLFW_KEY_J;
                keyMapping[K::KEY_K] = GLFW_KEY_K;
                keyMapping[K::KEY_L] = GLFW_KEY_L;
                keyMapping[K::KEY_M] = GLFW_KEY_M;
                keyMapping[K::KEY_N] = GLFW_KEY_N;
                keyMapping[K::KEY_O] = GLFW_KEY_O;
                keyMapping[K::KEY_P] = GLFW_KEY_P;
                keyMapping[K::KEY_Q] = GLFW_KEY_Q;
                keyMapping[K::KEY_R] = GLFW_KEY_R;
                keyMapping[K::KEY_S] = GLFW_KEY_S;
                keyMapping[K::KEY_T] = GLFW_KEY_T;
                keyMapping[K::KEY_U] = GLFW_KEY_U;
                keyMapping[K::KEY_V] = GLFW_KEY_V;
                keyMapping[K::KEY_W] = GLFW_KEY_W;
                keyMapping[K::KEY_X] = GLFW_KEY_X;
                keyMapping[K::KEY_Y] = GLFW_KEY_Y;
                keyMapping[K::KEY_Z] = GLFW_KEY_Z;

                keyMapping[K::KEY_0] = GLFW_KEY_0;
                keyMapping[K::KEY_1] = GLFW_KEY_1;
                keyMapping[K::KEY_2] = GLFW_KEY_2;
                keyMapping[K::KEY_3] = GLFW_KEY_3;
                keyMapping[K::KEY_4] = GLFW_KEY_4;
                keyMapping[K::KEY_5] = GLFW_KEY_5;
                keyMapping[K::KEY_6] = GLFW_KEY_6;
                keyMapping[K::KEY_7] = GLFW_KEY_7;
                keyMapping[K::KEY_8] = GLFW_KEY_8;
                keyMapping[K::KEY_9] = GLFW_KEY_9;

                keyMapping[K::KEY_ESCAPE]      = GLFW_KEY_ESCAPE;
                keyMapping[K::KEY_TAB]         = GLFW_KEY_TAB;
                keyMapping[K::KEY_CAPS_LOCK]   = GLFW_KEY_CAPS_LOCK;
                keyMapping[K::KEY_LEFT_SHIFT]  = GLFW_KEY_LEFT_SHIFT;
                keyMapping[K::KEY_LEFT_CTRL]   = GLFW_KEY_LEFT_CONTROL;
                keyMapping[K::KEY_LEFT_ALT]    = GLFW_KEY_LEFT_ALT;
                keyMapping[K::KEY_RIGHT_SHIFT] = GLFW_KEY_RIGHT_SHIFT;
                keyMapping[K::KEY_RIGHT_CTRL]  = GLFW_KEY_RIGHT_CONTROL;
                keyMapping[K::KEY_RIGHT_ALT]   = GLFW_KEY_RIGHT_ALT;
                keyMapping[K::KEY_SPACE]       = GLFW_KEY_SPACE;
                keyMapping[K::KEY_ENTER]       = GLFW_KEY_ENTER;
                keyMapping[K::KEY_BACKSPACE]   = GLFW_KEY_BACKSPACE;

                keyMapping[K::KEY_LEFT]  = GLFW_KEY_LEFT;
                keyMapping[K::KEY_RIGHT] = GLFW_KEY_RIGHT;
                keyMapping[K::KEY_UP]    = GLFW_KEY_UP;
                keyMapping[K::KEY_DOWN]  = GLFW_KEY_DOWN;

                keyMapping[K::KEY_F1]  = GLFW_KEY_F1;
                keyMapping[K::KEY_F2]  = GLFW_KEY_F2;
                keyMapping[K::KEY_F3]  = GLFW_KEY_F3;
                keyMapping[K::KEY_F4]  = GLFW_KEY_F4;
                keyMapping[K::KEY_F5]  = GLFW_KEY_F5;
                keyMapping[K::KEY_F6]  = GLFW_KEY_F6;
                keyMapping[K::KEY_F7]  = GLFW_KEY_F7;
                keyMapping[K::KEY_F8]  = GLFW_KEY_F8;
                keyMapping[K::KEY_F9]  = GLFW_KEY_F9;
                keyMapping[K::KEY_F10] = GLFW_KEY_F10;
                keyMapping[K::KEY_F11] = GLFW_KEY_F11;
                keyMapping[K::KEY_F12] = GLFW_KEY_F12;

                keyMapping[K::KEY_INSERT]    = GLFW_KEY_INSERT;
                keyMapping[K::KEY_DELETE]    = GLFW_KEY_DELETE;
                keyMapping[K::KEY_HOME]      = GLFW_KEY_HOME;
                keyMapping[K::KEY_END]       = GLFW_KEY_END;
                keyMapping[K::KEY_PAGE_UP]   = GLFW_KEY_PAGE_UP;
                keyMapping[K::KEY_PAGE_DOWN] = GLFW_KEY_PAGE_DOWN;

                // --- Mouse ---
                mouseButtonMapping[K::MOUSE_LEFT]     = GLFW_MOUSE_BUTTON_LEFT;
                mouseButtonMapping[K::MOUSE_RIGHT]    = GLFW_MOUSE_BUTTON_RIGHT;
                mouseButtonMapping[K::MOUSE_MIDDLE]   = GLFW_MOUSE_BUTTON_MIDDLE;
                mouseButtonMapping[K::MOUSE_BUTTON_4] = GLFW_MOUSE_BUTTON_4;
                mouseButtonMapping[K::MOUSE_BUTTON_5] = GLFW_MOUSE_BUTTON_5;

                // --- Gamepad Buttons (GLFW uses Xbox-style layout) --- TODO
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_A] = GLFW_GAMEPAD_BUTTON_A;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_B] = GLFW_GAMEPAD_BUTTON_B;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_X] = GLFW_GAMEPAD_BUTTON_X;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_Y] = GLFW_GAMEPAD_BUTTON_Y;

                // gamepadButtonMapping[K::GAMEPAD_BUTTON_BACK]  = GLFW_GAMEPAD_BUTTON_BACK;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_START] = GLFW_GAMEPAD_BUTTON_START;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_GUIDE] = GLFW_GAMEPAD_BUTTON_GUIDE;

                // gamepadButtonMapping[K::GAMEPAD_BUTTON_LEFT_BUMPER]  = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_RIGHT_BUMPER] = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_LEFT_STICK]   = GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
                // gamepadButtonMapping[K::GAMEPAD_BUTTON_RIGHT_STICK]  = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;

                // gamepadButtonMapping[K::GAMEPAD_DPAD_UP]    = GLFW_GAMEPAD_BUTTON_DPAD_UP;
                // gamepadButtonMapping[K::GAMEPAD_DPAD_DOWN]  = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
                // gamepadButtonMapping[K::GAMEPAD_DPAD_LEFT]  = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
                // gamepadButtonMapping[K::GAMEPAD_DPAD_RIGHT] = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;

                // // --- Gamepad Axes ---
                // gamepadAxisMapping[K::GAMEPAD_AXIS_LEFT_X]  = GLFW_GAMEPAD_AXIS_LEFT_X;
                // gamepadAxisMapping[K::GAMEPAD_AXIS_LEFT_Y]  = GLFW_GAMEPAD_AXIS_LEFT_Y;
                // gamepadAxisMapping[K::GAMEPAD_AXIS_RIGHT_X] = GLFW_GAMEPAD_AXIS_RIGHT_X;
                // gamepadAxisMapping[K::GAMEPAD_AXIS_RIGHT_Y] = GLFW_GAMEPAD_AXIS_RIGHT_Y;
                // gamepadAxisMapping[K::GAMEPAD_AXIS_LEFT_TRIGGER]  = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
                // gamepadAxisMapping[K::GAMEPAD_AXIS_RIGHT_TRIGGER] = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
            }
    };
} // namespace Vantor::Input
#endif