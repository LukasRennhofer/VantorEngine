/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <InputDevice/VID_Common.hpp>

// GLFW Backend
#ifdef VANTOR_WM_GLFW
#include <InputDevice/Device/Backend/GLFW/VID_GLFWGamepad.hpp>
#include <InputDevice/Device/Backend/GLFW/VID_GLFWKeyboard.hpp>
#include <InputDevice/Device/Backend/GLFW/VID_GLFWMouse.hpp>
#endif

#include <memory>

namespace VE::Internal::Input {

    std::shared_ptr<IKeyboardInputDevice> CreateKeyboard(EInputBackend backend) {
        std::shared_ptr<IKeyboardInputDevice> device;

        switch (backend) {
            case EInputBackend::Auto:
    #ifdef VANTOR_WM_GLFW
                device = std::make_shared<GLFWKeyboardInputDevice>();
    #else
                device = std::make_shared<IKeyboardInputDevice>();
    #endif
                break;

            case EInputBackend::GLFW:
    #ifdef VANTOR_WM_GLFW
                device = std::make_shared<GLFWKeyboardInputDevice>();
    #else
                device = std::make_shared<IKeyboardInputDevice>();
    #endif
                break;

            default:
                device = std::make_shared<IKeyboardInputDevice>();
                break;
        }
        return device;
    }

    std::shared_ptr<IMouseInputDevice> CreateMouse(EInputBackend backend) {
        std::shared_ptr<IMouseInputDevice> device;

        switch (backend) {
            case EInputBackend::Auto:
    #ifdef VANTOR_WM_GLFW
                device = std::make_shared<GLFWMouseInputDevice>();
    #else
                device = std::make_shared<IMouseInputDevice>();
    #endif
                break;

            case EInputBackend::GLFW:
    #ifdef VANTOR_WM_GLFW
                device = std::make_shared<GLFWMouseInputDevice>();
    #else
                device = std::make_shared<IMouseInputDevice>();
    #endif
                break;

            default:
                device = std::make_shared<IMouseInputDevice>();
                break;
        }

        return device;
    }

    std::shared_ptr<IGamepadInputDevice> CreateGamepad(EInputBackend backend) {
        std::shared_ptr<IGamepadInputDevice> device;

        switch (backend) {
            case EInputBackend::Auto:
    #ifdef VANTOR_WM_GLFW
                device = std::make_shared<GLFWGamepadInputDevice>();
    #else
                device = std::make_shared<IGamepadInputDevice>();
    #endif
                break;

            case EInputBackend::GLFW:
    #ifdef VANTOR_WM_GLFW
                device = std::make_shared<GLFWGamepadInputDevice>();
    #else
                device = std::make_shared<IGamepadInputDevice>();
    #endif
                break;

            default:
                device = std::make_shared<IGamepadInputDevice>();
                break;
        }
        return device;
    }

} // namespace VE::Internal::Input