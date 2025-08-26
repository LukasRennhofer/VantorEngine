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

#include <InputDevice/Device/Interface/VID_Keyboard.hpp>
#include <Context/Interface/VCT_Window.hpp>
#include <unordered_map>

namespace VE::Internal::Input {

class GLFWKeyboardInputDevice : public IKeyboardInputDevice {
public:
    GLFWKeyboardInputDevice(VE::Internal::Context::VWindow* mContext);
    void Poll() override;
    void ProcessEvent(const VInputEvent&) override;
    bool IsButtonDown(int code) const override;
    bool WasButtonJustPressed(int code) const override;
    bool WasButtonJustReleased(int code) const override;
    float GetAxis(int) const override;
private:
    VE::Internal::Context::VWindow* m_Context;
    std::unordered_map<int, bool> m_current;
    std::unordered_map<int, bool> m_previous;
};

} // namespace VE::Internal::Input

#endif