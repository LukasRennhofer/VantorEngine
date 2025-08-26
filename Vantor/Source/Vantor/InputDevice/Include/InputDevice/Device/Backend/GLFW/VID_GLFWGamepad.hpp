/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#ifdef VANTOR_WM_GLFW

#include <InputDevice/Device/Interface/VID_Gamepad.hpp>
#include <unordered_map>

namespace VE::Internal::Input {

class GLFWGamepadInputDevice : public IGamepadInputDevice {
public:
    GLFWGamepadInputDevice(int gamepadIndex = 0);
    void Poll() override;
    void ProcessEvent(const VInputEvent&) override;
    bool IsButtonDown(int code) const override;
    bool WasButtonJustPressed(int code) const override;
    bool WasButtonJustReleased(int code) const override;
    float GetAxis(int code) const override;
private:
    int m_gamepadIndex;
    std::unordered_map<int, bool> m_current;
    std::unordered_map<int, bool> m_previous;
    std::unordered_map<int, float> m_axes;
};

} // namespace VE::Internal::Input

#endif