/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <InputDevice/Device/Backend/GLFW/VID_GLFWGamepad.hpp>
#include <Shared/GLFW/glfw3.h>

namespace VE::Internal::Input {

GLFWGamepadInputDevice::GLFWGamepadInputDevice(int gamepadIndex)
    : m_gamepadIndex(gamepadIndex) {}

void GLFWGamepadInputDevice::Poll() {
    m_previous = m_current;
    m_axes.clear();
    if (!glfwJoystickPresent(m_gamepadIndex)) return;
    int count;
    const unsigned char* buttons = glfwGetJoystickButtons(m_gamepadIndex, &count);
    for (int i = 0; i < count; ++i) {
        m_current[i] = buttons[i] == GLFW_PRESS;
    }
    const float* axes = glfwGetJoystickAxes(m_gamepadIndex, &count);
    for (int i = 0; i < count; ++i) {
        m_axes[i] = axes[i];
    }
}

void GLFWGamepadInputDevice::ProcessEvent(const VInputEvent&) {}

bool GLFWGamepadInputDevice::IsButtonDown(int code) const {
    auto it = m_current.find(code);
    return it != m_current.end() && it->second;
}

bool GLFWGamepadInputDevice::WasButtonJustPressed(int code) const {
    return IsButtonDown(code) && (!m_previous.count(code) || !m_previous.at(code));
}

bool GLFWGamepadInputDevice::WasButtonJustReleased(int code) const {
    return !IsButtonDown(code) && m_previous.count(code) && m_previous.at(code);
}

float GLFWGamepadInputDevice::GetAxis(int code) const {
    auto it = m_axes.find(code);
    return it != m_axes.end() ? it->second : 0.0f;
}

} // namespace VE::Internal::Input
