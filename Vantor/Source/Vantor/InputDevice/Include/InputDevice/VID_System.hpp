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
#include <InputDevice/VID_MappingContext.hpp>

// Devices
#include <InputDevice/Device/Interface/VID_Gamepad.hpp>
#include <InputDevice/Device/Interface/VID_Keyboard.hpp>
#include <InputDevice/Device/Interface/VID_Mouse.hpp>

#include <Core/Types/VCO_Singleton.hpp>

#include <memory>
#include <algorithm>
#include <vector>
#include <string>

namespace VE::Internal::Input {


class VInputSubsystem : public VE::Internal::Core::Types::TSingleton<VInputSubsystem> {
    friend class VE::Internal::Core::Types::TSingleton<VInputSubsystem>;
public:
    void Initialize() {}
    void Poll() {
        m_deviceActionStates.clear();
        m_deviceAxisStates.clear();
        for (auto& kb : m_keyboards) {
            kb->Poll();
            for (int code = 0; code < 512; ++code) {
                VInputEvent e;
                e.type = kb->IsButtonDown(code) ? EInputEventType::ButtonDown : EInputEventType::ButtonUp;
                e.code = code;
                e.value = kb->GetAxis(code);
                kb->ProcessEvent(e);
            }
        }
        for (auto& mouse : m_mice) {
            mouse->Poll();
            for (int code = 0; code < 16; ++code) {
                VInputEvent e;
                e.type = mouse->IsButtonDown(code) ? EInputEventType::ButtonDown : EInputEventType::ButtonUp;
                e.code = code;
                e.value = mouse->GetAxis(code);
                mouse->ProcessEvent(e);
            }
        }
        for (auto& gp : m_gamepads) {
            gp->Poll();
            for (int code = 0; code < 32; ++code) {
                VInputEvent e;
                e.type = gp->IsButtonDown(code) ? EInputEventType::ButtonDown : EInputEventType::ButtonUp;
                e.code = code;
                e.value = gp->GetAxis(code);
                gp->ProcessEvent(e);
            }
        }
        UpdateMappingContext();
    }
    void SetMappingContext(std::shared_ptr<VInputMappingContext> context) { m_context = context; }
    bool IsActionPressed(const std::string& action) const { return m_context ? m_context->GetActionState(action).pressed : false; }
    bool WasActionJustReleased(const std::string& action) const { return m_context ? m_context->GetActionState(action).justReleased : false; }
    float GetAxis(const std::string& axis) const { return m_context ? m_context->GetAxisValue(axis) : 0.0f; }
    void ConnectKeyboard(std::shared_ptr<IKeyboardInputDevice> keyboard) { m_keyboards.push_back(keyboard); }
    void DisconnectKeyboard(IKeyboardInputDevice* keyboard) {
        m_keyboards.erase(std::remove_if(m_keyboards.begin(), m_keyboards.end(),
            [keyboard](const std::shared_ptr<IKeyboardInputDevice>& k) { return k.get() == keyboard; }), m_keyboards.end());
    }
    const std::vector<std::shared_ptr<IKeyboardInputDevice>>& GetKeyboards() const { return m_keyboards; }
    IKeyboardInputDevice* GetKeyboard() { return m_keyboards.empty() ? nullptr : m_keyboards.front().get(); }
    void ConnectMouse(std::shared_ptr<IMouseInputDevice> mouse) { m_mice.push_back(mouse); }
    void DisconnectMouse(IMouseInputDevice* mouse) {
        m_mice.erase(std::remove_if(m_mice.begin(), m_mice.end(),
            [mouse](const std::shared_ptr<IMouseInputDevice>& m) { return m.get() == mouse; }), m_mice.end());
    }
    const std::vector<std::shared_ptr<IMouseInputDevice>>& GetMice() const { return m_mice; }
    IMouseInputDevice* GetMouse() { return m_mice.empty() ? nullptr : m_mice.front().get(); }
    void ConnectGamepad(std::shared_ptr<IGamepadInputDevice> gamepad) { m_gamepads.push_back(gamepad); }
    void DisconnectGamepad(IGamepadInputDevice* gamepad) {
        m_gamepads.erase(std::remove_if(m_gamepads.begin(), m_gamepads.end(),
            [gamepad](const std::shared_ptr<IGamepadInputDevice>& g) { return g.get() == gamepad; }), m_gamepads.end());
    }
    const std::vector<std::shared_ptr<IGamepadInputDevice>>& GetGamepads() const { return m_gamepads; }
    IGamepadInputDevice* GetGamepad() { return m_gamepads.empty() ? nullptr : m_gamepads.front().get(); }

private:
    VInputSubsystem() = default;

    std::vector<std::shared_ptr<IKeyboardInputDevice>> m_keyboards;
    std::vector<std::shared_ptr<IMouseInputDevice>> m_mice;
    std::vector<std::shared_ptr<IGamepadInputDevice>> m_gamepads;
    std::shared_ptr<VInputMappingContext> m_context;
    std::unordered_map<std::string, VActionState> m_deviceActionStates;
    std::unordered_map<std::string, float> m_deviceAxisStates;

    void UpdateMappingContext() {
        if (m_context) m_context->Update(m_deviceActionStates);
    }
};

} // namespace VE::Internal::Input