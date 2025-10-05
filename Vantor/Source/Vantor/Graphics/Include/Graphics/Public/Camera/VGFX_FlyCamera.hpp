/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

// Renderer
#include <Math/Linear/VMA_Vector.hpp>
#include <Graphics/Public/Camera/VGFX_Camera.hpp>

// Fly Camera from Cell Engine @JoeyDeVries

namespace VE::Internal::RenderModules
{
    class AFlyCamera : public VE::Graphics::ACamera
    {
        public:
            float Yaw;
            float Pitch;

            float MovementSpeed   = 10.0f;
            float MouseSensitivty = 0.1f;
            float Damping         = 5.0f;

        private:
            VE::Math::VVector3 m_TargetPosition;
            VE::Math::VVector3 m_WorldUp;
            float                  m_TargetYaw;
            float                  m_TargetPitch;

        public:
            AFlyCamera(VE::Math::VVector3 position,
                      VE::Math::VVector3 forward = VE::Math::VVector3(0.0f, 0.0f, -1.0f),
                      VE::Math::VVector3 up      = VE::Math::VVector3(0.0f, 1.0f, 0.0f));

            virtual void Update(float dt);

            virtual void InputKey(float dt, VE::Graphics::ECameraMovement direction);
            virtual void InputMouse(float deltaX, float deltaY);
            virtual void InputScroll(float deltaX, float deltaY);
    };
} // namespace VE::Internal::RenderModules