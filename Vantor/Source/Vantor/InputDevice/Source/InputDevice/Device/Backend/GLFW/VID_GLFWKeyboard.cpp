/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <InputDevice/Device/Backend/GLFW/VID_GLFWKeyboard.hpp>

#include <Shared/GLFW/glfw3.h>

namespace VE::Internal::Input {

GLFWKeyboardInputDevice::GLFWKeyboardInputDevice(VE::Internal::Context::VWindow* mContext)
    : m_Context(mContext) {}

void GLFWKeyboardInputDevice::Poll() {
    m_previous = m_current;
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        m_current[key] = glfwGetKey((GLFWwindow*)m_Context->GetNativeHandle(), key) == GLFW_PRESS;
    }
}

void GLFWKeyboardInputDevice::ProcessEvent(const VInputEvent&) {}

bool GLFWKeyboardInputDevice::IsButtonDown(int code) const {
    auto it = m_current.find(code);
    return it != m_current.end() && it->second;
}

bool GLFWKeyboardInputDevice::WasButtonJustPressed(int code) const {
    return IsButtonDown(code) && (!m_previous.count(code) || !m_previous.at(code));
}

bool GLFWKeyboardInputDevice::WasButtonJustReleased(int code) const {
    return !IsButtonDown(code) && m_previous.count(code) && m_previous.at(code);
}

float GLFWKeyboardInputDevice::GetAxis(int) const { return 0.0f; }

} // namespace VE::Internal::Input
