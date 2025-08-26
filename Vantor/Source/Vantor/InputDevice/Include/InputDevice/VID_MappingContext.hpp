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

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace VE::Internal::Input {

struct VInputBinding {
    enum class Type { Keyboard, Mouse, GamepadButton, GamepadAxis };
    Type type;
    int code; // Key/button/axis code
    float scale = 1.0f; // For axis bindings
};

struct VActionState {
    bool pressed = false;
    bool justPressed = false;
    bool justReleased = false;
    float value = 0.0f; // For axis
};

class VInputMappingContext {
public:
    void BindAction(const std::string& action, const VInputBinding& binding) {
        m_actionBindings[action].push_back(binding);
    }
    void BindAxis(const std::string& axis, const VInputBinding& binding) {
        m_axisBindings[axis].push_back(binding);
    }
    const std::vector<VInputBinding>& GetBindings(const std::string& action) const {
        static std::vector<VInputBinding> empty;
        auto it = m_actionBindings.find(action);
        return it != m_actionBindings.end() ? it->second : empty;
    }
    VActionState GetActionState(const std::string& action) const {
        auto it = m_actionStates.find(action);
        return it != m_actionStates.end() ? it->second : VActionState{};
    }
    float GetAxisValue(const std::string& axis) const {
        auto it = m_axisStates.find(axis);
        return it != m_axisStates.end() ? it->second : 0.0f;
    }
    void Update(const std::unordered_map<std::string, VActionState>& deviceStates) {
        for (auto& [action, bindings] : m_actionBindings) {
            VActionState state{};
            for (const auto& binding : bindings) {
                auto it = deviceStates.find(action);
                if (it != deviceStates.end() && it->second.pressed) {
                    state.pressed = true;
                    state.value = 1.0f;
                    break;
                }
            }
            state.justPressed = state.pressed && !m_actionStates[action].pressed;
            state.justReleased = !state.pressed && m_actionStates[action].pressed;
            m_actionStates[action] = state;
        }
        for (auto& [axis, bindings] : m_axisBindings) {
            float value = 0.0f;
            for (const auto& binding : bindings) {
                auto it = deviceStates.find(axis);
                if (it != deviceStates.end()) {
                    value += it->second.value * binding.scale;
                }
            }
            m_axisStates[axis] = value;
        }
    }
private:
    std::unordered_map<std::string, std::vector<VInputBinding>> m_actionBindings;
    std::unordered_map<std::string, std::vector<VInputBinding>> m_axisBindings;
    std::unordered_map<std::string, VActionState> m_actionStates;
    std::unordered_map<std::string, float> m_axisStates;
};

} // namespace VE::Internal::Input
