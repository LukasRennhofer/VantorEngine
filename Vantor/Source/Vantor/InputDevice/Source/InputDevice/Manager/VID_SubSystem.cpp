/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <InputDevice/Manager/VID_SubSystem.hpp>

namespace VE::Internal::InputDevice
{
    void VInputManager::AddDevice(std::shared_ptr<VE::Input::VInputDevice> device) { devices.push_back(device); }

    void VInputManager::MapAction(const std::string &actionName, const VE::Input::VInputButton &button) { actionMap[actionName].push_back(button); }

    void VInputManager::Update()
    {
        for (auto &device : devices)
            device->Update();
    }

    bool VInputManager::IsActionPressed(const std::string &actionName) const
    {
        auto it = actionMap.find(actionName);
        if (it == actionMap.end()) return false;

        for (const auto &input : it->second)
        {
            for (const auto &device : devices)
            {
                if (device->IsPressed(input)) return true;
            }
        }
        return false;
    }

    bool VInputManager::IsPressed(const int &key, const VE::Input::EInputDeviceType &deviceType) const
    {
        VE::Input::VInputButton it;
        it.id         = key;
        it.deviceType = deviceType;

        for (const auto &device : devices)
        {
            if (device->IsPressed(it)) return true;
        }

        return false;
    }

    bool VInputManager::WasActionPressed(const std::string &actionName) const
    {
        auto it = actionMap.find(actionName);
        if (it == actionMap.end()) return false;

        for (const auto &input : it->second)
        {
            for (const auto &device : devices)
            {
                if (device->WasPressed(input)) return true;
            }
        }
        return false;
    }

    bool VInputManager::WasPressed(const int &key, const VE::Input::EInputDeviceType &deviceType) const
    {
        VE::Input::VInputButton it;
        it.id         = key;
        it.deviceType = deviceType;

        for (const auto &device : devices)
        {
            if (device->WasPressed(it)) return true;
        }

        return false;
    }

    float VInputManager::GetActionAxis(const std::string &actionName) const
    {
        auto it = actionMap.find(actionName);
        if (it == actionMap.end()) return 0.f;

        for (const auto &input : it->second)
        {
            if (!input.isAxis) continue;

            for (const auto &device : devices)
            {
                float val = device->GetAxis(input);
                if (val != 0.f) return val;
            }
        }
        return 0.f;
    }
} // namespace VE::Input