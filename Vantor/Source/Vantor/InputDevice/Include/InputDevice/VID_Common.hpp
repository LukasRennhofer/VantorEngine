/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/


#pragma once

#include <cstdint>

namespace VE::Internal::Input {

// Input Backends
enum class EInputBackend {
    Auto, // Choose backend from avalible ones
    GLFW
};

// Keyboard keys
enum class EKeyboardKey : uint16_t {
    Unknown = 0,
    Space, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Escape, Enter, Tab, Backspace, Insert, Delete, Right, Left, Down, Up,
    PageUp, PageDown, Home, End, CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper,
    Menu
};

// Mouse buttons
enum class EMouseButton : uint8_t {
    Left = 0, Right, Middle, Button4, Button5, Button6, Button7, Button8
};

// Gamepad buttons
enum class GamepadButton : uint8_t {
    A = 0, B, X, Y, LeftBumper, RightBumper, Back, Start, Guide,
    LeftThumb, RightThumb, DPadUp, DPadRight, DPadDown, DPadLeft
};

// Gamepad axes
enum class EGamepadAxis : uint8_t {
    LeftX = 0, LeftY, RightX, RightY, LeftTrigger, RightTrigger
};

} // namespace VE::Internal::Input
