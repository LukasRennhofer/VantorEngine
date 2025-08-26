/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <InputDevice/Device/Backend/GLFW/VID_GLFWMouse.hpp>
#include <Shared/GLFW/glfw3.h>

namespace VE::Internal::Input {

GLFWMouseInputDevice::GLFWMouseInputDevice(VE::Internal::Context::VWindow* mContext)
    : m_Context(mContext), m_x(0), m_y(0) {}

void GLFWMouseInputDevice::Poll() {
    m_previous = m_current;
    double x, y;
    glfwGetCursorPos((GLFWwindow*)m_Context->GetNativeHandle(), &x, &y);
    m_x = x;
    m_y = y;
    for (int btn = GLFW_MOUSE_BUTTON_1; btn <= GLFW_MOUSE_BUTTON_LAST; ++btn) {
        m_current[btn] = glfwGetMouseButton((GLFWwindow*)m_Context->GetNativeHandle(), btn) == GLFW_PRESS;
    }
}

void GLFWMouseInputDevice::ProcessEvent(const VInputEvent&) {}

bool GLFWMouseInputDevice::IsButtonDown(int code) const {
    auto it = m_current.find(code);
    return it != m_current.end() && it->second;
}

bool GLFWMouseInputDevice::WasButtonJustPressed(int code) const {
    return IsButtonDown(code) && (!m_previous.count(code) || !m_previous.at(code));
}

bool GLFWMouseInputDevice::WasButtonJustReleased(int code) const {
    return !IsButtonDown(code) && m_previous.count(code) && m_previous.at(code);
}

float GLFWMouseInputDevice::GetAxis(int) const { return 0.0f; }
double GLFWMouseInputDevice::GetX() const { return m_x; }
double GLFWMouseInputDevice::GetY() const { return m_y; }

} // namespace VE::Internal::Input
