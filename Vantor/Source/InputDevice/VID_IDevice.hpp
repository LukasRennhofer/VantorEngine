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
 *  Date: 2025-07-01
 *
 *  File: VID_IDevice.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Window
#include "../Context/Interface/VCT_Window.hpp"

// Math
#include "../Math/Linear/VMA_Vector.hpp"

namespace Vantor::Input
{
    enum class VEInputKey
    {
        // --- Keyboard ---
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,

        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,

        KEY_ESCAPE,
        KEY_TAB,
        KEY_CAPS_LOCK,
        KEY_LEFT_SHIFT,
        KEY_LEFT_CTRL,
        KEY_LEFT_ALT,
        KEY_RIGHT_SHIFT,
        KEY_RIGHT_CTRL,
        KEY_RIGHT_ALT,
        KEY_SPACE,
        KEY_ENTER,
        KEY_BACKSPACE,

        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,

        KEY_INSERT,
        KEY_DELETE,
        KEY_HOME,
        KEY_END,
        KEY_PAGE_UP,
        KEY_PAGE_DOWN,

        // --- Mouse ---
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_MIDDLE,
        MOUSE_BUTTON_4,
        MOUSE_BUTTON_5,
        MOUSE_SCROLL_X,
        MOUSE_SCROLL_Y,
        MOUSE_POSITION_X,
        MOUSE_POSITION_Y,

        // --- Gamepad Buttons (Generic Layout) ---
        GAMEPAD_BUTTON_A, // Bottom face button (Xbox: A, DS: Cross)
        GAMEPAD_BUTTON_B, // Right face button (Xbox: B, DS: Circle)
        GAMEPAD_BUTTON_X, // Left face button (Xbox: X, DS: Square)
        GAMEPAD_BUTTON_Y, // Top face button (Xbox: Y, DS: Triangle)

        GAMEPAD_BUTTON_BACK,
        GAMEPAD_BUTTON_START,
        GAMEPAD_BUTTON_GUIDE, // Xbox logo / PS button

        GAMEPAD_BUTTON_LEFT_BUMPER,
        GAMEPAD_BUTTON_RIGHT_BUMPER,
        GAMEPAD_BUTTON_LEFT_STICK,
        GAMEPAD_BUTTON_RIGHT_STICK,

        GAMEPAD_DPAD_UP,
        GAMEPAD_DPAD_DOWN,
        GAMEPAD_DPAD_LEFT,
        GAMEPAD_DPAD_RIGHT,

        // --- Gamepad Axes ---
        GAMEPAD_AXIS_LEFT_X,
        GAMEPAD_AXIS_LEFT_Y,
        GAMEPAD_AXIS_RIGHT_X,
        GAMEPAD_AXIS_RIGHT_Y,
        GAMEPAD_AXIS_LEFT_TRIGGER, // Typically 0.0–1.0
        GAMEPAD_AXIS_RIGHT_TRIGGER
    };

    enum class VEInputDeviceType
    {
        Keyboard,
        Mouse,
        Gamepad,
    };

    struct VInputButton
    {
            VEInputDeviceType deviceType;
            int               id;          // keycode, mouse button, gamepad button index
            int               axisId = -1; // axis index (if applicable)
            bool              isAxis = false;
    };

    class VIInputDevice
    {
        public:
            virtual ~VIInputDevice() = default;

            // Poll/update internal state
            virtual void Update() = 0;

            // Check if button is currently pressed
            virtual bool IsPressed(const VInputButton &button) const = 0;

            // Check if button was just pressed this frame (edge trigger)
            virtual bool WasPressed(const VInputButton &button) const = 0;

            // For axis inputs (gamepads, analog sticks etc)
            virtual float GetAxis(const VInputButton &axis) const = 0;

            virtual Vantor::Math::VVector2 GetMousePosition() const = 0;

            virtual Vantor::Math::VVector2 GetMouseDelta() const = 0;

            virtual Vantor::Math::VVector2 GetScrollOffset() const = 0;

            virtual std::string GetDeviceName() const = 0;
    };
} // namespace Vantor::Input