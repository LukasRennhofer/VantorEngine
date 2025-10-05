/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Math/Linear/VMA_Vector.hpp>

#include <InputDevice/Public/VID_Common.hpp>

namespace VE::Input
{

    class VInputDevice
    {
        public:
            virtual ~VInputDevice() = default;

            // Poll/update internal state
            virtual void Update() = 0;

            // Check if button is currently pressed
            virtual bool IsPressed(const VInputButton &button) const = 0;
            // virtual bool IsPressed(const VEInputKey &key) const = 0;

            // Check if button was just pressed this frame (edge trigger)
            virtual bool WasPressed(const VInputButton &button) const = 0;
            // virtual bool WasPressed(const VEInputKey &key) const = 0;

            // For axis inputs (gamepads, analog sticks etc)
            virtual float GetAxis(const VInputButton &axis) const = 0;

            virtual void SetMouseCursorState(bool visible) = 0;
            virtual bool GetMouseCursorState() const       = 0;

            virtual VE::Math::VVector2 GetMousePosition() const = 0;

            virtual VE::Math::VVector2 GetMouseDelta() const = 0;

            virtual VE::Math::VVector2 GetScrollOffset() const = 0;

            virtual std::string GetDeviceName() const = 0;

        protected:
            bool m_CursorState = true;
    };
} // namespace VE::Input