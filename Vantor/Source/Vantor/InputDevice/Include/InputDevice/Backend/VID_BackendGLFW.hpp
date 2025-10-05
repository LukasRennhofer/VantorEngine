/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <InputDevice/Public/VID_IDevice.hpp>

#include <Context/Interface/VCT_Window.hpp>

#ifdef VANTOR_WM_GLFW

#include <Shared/GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <array>

namespace VE::Input {

class VGLFWInputDevice : public VInputDevice
{
public:
    explicit VGLFWInputDevice(VE::Internal::Context::VWindow* wnd)
        : window(wnd), nativeWindow(static_cast<GLFWwindow*>(wnd->GetNativeHandle()))
    {
        SetupMappings();
        SetupCallbacks();
    }

    void Update() override
    {
        previousKeys = currentKeys;

        // --- Mouse ---
        prevMouseX = mouseX; prevMouseY = mouseY;
        glfwGetCursorPos(nativeWindow, &mouseX, &mouseY);

        scrollDeltaX = scrollX; scrollDeltaY = scrollY;
        scrollX = 0.0; scrollY = 0.0;

        // --- Keyboard ---
        for (auto [key, glfwKey] : keyMapping)
            currentKeys[static_cast<int>(key)] = glfwGetKey(nativeWindow, glfwKey) == GLFW_PRESS;

        // --- Mouse Buttons ---
        for (auto [key, glfwButton] : mouseButtonMapping)
            currentKeys[static_cast<int>(key)] = glfwGetMouseButton(nativeWindow, glfwButton) == GLFW_PRESS;

        // TODO: Gamepad support (first controller)
    }

    bool IsPressed(const VInputButton& button) const override
    {
        if (button.isAxis) return false;
        auto it = currentKeys.find(button.id);
        return it != currentKeys.end() && it->second;
    }

    bool WasPressed(const VInputButton& button) const override
    {
        if (button.isAxis) return false;
        auto itCurr = currentKeys.find(button.id);
        auto itPrev = previousKeys.find(button.id);
        return itCurr != currentKeys.end() && itPrev != previousKeys.end() && itCurr->second && !itPrev->second;
    }

    float GetAxis(const VInputButton& axis) const override
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
            glfwSetInputMode(nativeWindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
    }

    bool GetMouseCursorState() const override { return m_CursorState; }

    VE::Math::VVector2 GetMousePosition() const override { return {float(mouseX), float(mouseY)}; }
    VE::Math::VVector2 GetMouseDelta() const override { return {float(mouseX - prevMouseX), float(mouseY - prevMouseY)}; }
    VE::Math::VVector2 GetScrollOffset() const { return {float(scrollX), float(scrollY)}; }
    std::string GetDeviceName() const override { return "GLFWInputDevice"; }

private:
    VE::Internal::Context::VWindow* window;
    GLFWwindow* nativeWindow;

    std::unordered_map<int, bool>  currentKeys;
    std::unordered_map<int, bool>  previousKeys;
    std::unordered_map<int, float> currentAxes;

    std::unordered_map<EInputKey, int> keyMapping;
    std::unordered_map<EInputKey, int> mouseButtonMapping;
    std::unordered_map<EInputKey, int> gamepadButtonMapping;
    std::unordered_map<EInputKey, int> gamepadAxisMapping;

    double mouseX = 0.0, mouseY = 0.0;
    double prevMouseX = 0.0, prevMouseY = 0.0;
    double scrollX = 0.0, scrollY = 0.0;
    double scrollDeltaX = 0.0, scrollDeltaY = 0.0;

    void SetupMappings()
    {
        using K = EInputKey;

        static constexpr std::pair<K, int> keyboard[] = {
            {K::KEY_A, GLFW_KEY_A}, {K::KEY_B, GLFW_KEY_B}, {K::KEY_C, GLFW_KEY_C}, {K::KEY_D, GLFW_KEY_D},
            {K::KEY_E, GLFW_KEY_E}, {K::KEY_F, GLFW_KEY_F}, {K::KEY_G, GLFW_KEY_G}, {K::KEY_H, GLFW_KEY_H},
            {K::KEY_I, GLFW_KEY_I}, {K::KEY_J, GLFW_KEY_J}, {K::KEY_K, GLFW_KEY_K}, {K::KEY_L, GLFW_KEY_L},
            {K::KEY_M, GLFW_KEY_M}, {K::KEY_N, GLFW_KEY_N}, {K::KEY_O, GLFW_KEY_O}, {K::KEY_P, GLFW_KEY_P},
            {K::KEY_Q, GLFW_KEY_Q}, {K::KEY_R, GLFW_KEY_R}, {K::KEY_S, GLFW_KEY_S}, {K::KEY_T, GLFW_KEY_T},
            {K::KEY_U, GLFW_KEY_U}, {K::KEY_V, GLFW_KEY_V}, {K::KEY_W, GLFW_KEY_W}, {K::KEY_X, GLFW_KEY_X},
            {K::KEY_Y, GLFW_KEY_Y}, {K::KEY_Z, GLFW_KEY_Z},

            {K::KEY_0, GLFW_KEY_0}, {K::KEY_1, GLFW_KEY_1}, {K::KEY_2, GLFW_KEY_2}, {K::KEY_3, GLFW_KEY_3},
            {K::KEY_4, GLFW_KEY_4}, {K::KEY_5, GLFW_KEY_5}, {K::KEY_6, GLFW_KEY_6}, {K::KEY_7, GLFW_KEY_7},
            {K::KEY_8, GLFW_KEY_8}, {K::KEY_9, GLFW_KEY_9},

            {K::KEY_ESCAPE, GLFW_KEY_ESCAPE}, {K::KEY_TAB, GLFW_KEY_TAB}, {K::KEY_CAPS_LOCK, GLFW_KEY_CAPS_LOCK},
            {K::KEY_LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT}, {K::KEY_LEFT_CTRL, GLFW_KEY_LEFT_CONTROL},
            {K::KEY_LEFT_ALT, GLFW_KEY_LEFT_ALT}, {K::KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_SHIFT},
            {K::KEY_RIGHT_CTRL, GLFW_KEY_RIGHT_CONTROL}, {K::KEY_RIGHT_ALT, GLFW_KEY_RIGHT_ALT},
            {K::KEY_SPACE, GLFW_KEY_SPACE}, {K::KEY_ENTER, GLFW_KEY_ENTER}, {K::KEY_BACKSPACE, GLFW_KEY_BACKSPACE},
            {K::KEY_LEFT, GLFW_KEY_LEFT}, {K::KEY_RIGHT, GLFW_KEY_RIGHT}, {K::KEY_UP, GLFW_KEY_UP}, {K::KEY_DOWN, GLFW_KEY_DOWN},

            {K::KEY_F1, GLFW_KEY_F1}, {K::KEY_F2, GLFW_KEY_F2}, {K::KEY_F3, GLFW_KEY_F3}, {K::KEY_F4, GLFW_KEY_F4},
            {K::KEY_F5, GLFW_KEY_F5}, {K::KEY_F6, GLFW_KEY_F6}, {K::KEY_F7, GLFW_KEY_F7}, {K::KEY_F8, GLFW_KEY_F8},
            {K::KEY_F9, GLFW_KEY_F9}, {K::KEY_F10, GLFW_KEY_F10}, {K::KEY_F11, GLFW_KEY_F11}, {K::KEY_F12, GLFW_KEY_F12},

            {K::KEY_INSERT, GLFW_KEY_INSERT}, {K::KEY_DELETE, GLFW_KEY_DELETE}, {K::KEY_HOME, GLFW_KEY_HOME},
            {K::KEY_END, GLFW_KEY_END}, {K::KEY_PAGE_UP, GLFW_KEY_PAGE_UP}, {K::KEY_PAGE_DOWN, GLFW_KEY_PAGE_DOWN}
        };

        for (auto [k, v] : keyboard) keyMapping[k] = v;

        static constexpr std::pair<K, int> mouseButtons[] = {
            {K::MOUSE_LEFT, GLFW_MOUSE_BUTTON_LEFT}, {K::MOUSE_RIGHT, GLFW_MOUSE_BUTTON_RIGHT},
            {K::MOUSE_MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE}, {K::MOUSE_BUTTON_4, GLFW_MOUSE_BUTTON_4},
            {K::MOUSE_BUTTON_5, GLFW_MOUSE_BUTTON_5}
        };

        for (auto [k, v] : mouseButtons) mouseButtonMapping[k] = v;

        // TODO: Gamepad mappings
    }

    void SetupCallbacks()
    {
        glfwSetWindowUserPointer(nativeWindow, this);

        glfwSetScrollCallback(nativeWindow, [](GLFWwindow* wnd, double xoffset, double yoffset)
        {
            auto* input = static_cast<VGLFWInputDevice*>(glfwGetWindowUserPointer(wnd));
            if (input)
            {
                input->scrollX += xoffset;
                input->scrollY += yoffset;
            }
        });
    }
};

} // namespace VE::Input

#endif
