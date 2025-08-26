/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Math/VMA_Common.hpp>
#include <Math/Linear/VMA_Vector.hpp>

#include <RenderModules/Camera/VRM_FlyCamera.hpp>

// Fly Camera from Cell Engine @JoeyDeVries

namespace VE::Internal::RenderModules
{
    AFlyCamera::AFlyCamera(VE::Internal::Math::VVector3 position, VE::Internal::Math::VVector3 forward, VE::Internal::Math::VVector3 up) : ACamera(position, forward, up)
    {
        Yaw = -90.0f;

        Forward          = forward;
        m_WorldUp        = Up;
        m_TargetPosition = position;
    }

    void AFlyCamera::Update(float dt)
    {
        ACamera::Update(dt);

        Position = VE::Internal::Math::Lerp(Position, m_TargetPosition, VE::Internal::Math::Clamp01(dt * Damping));
        Yaw      = VE::Internal::Math::Lerp(Yaw, m_TargetYaw, VE::Internal::Math::Clamp01(dt * Damping * 2.0f));
        Pitch    = VE::Internal::Math::Lerp(Pitch, m_TargetPitch, VE::Internal::Math::Clamp01(dt * Damping * 2.0f));

        // calculate new cartesian basis vectors from yaw/pitch pair:
        VE::Internal::Math::VVector3 newForward;
        newForward.x = cos(0.0174533 * Pitch) * cos(0.0174533 * Yaw);
        newForward.y = sin(0.0174533 * Pitch);
        newForward.z = cos(0.0174533 * Pitch) * sin(0.0174533 * Yaw);
        Forward      = newForward.Normalized();
        Right        = Forward.Cross(m_WorldUp).Normalized();
        Up           = Right.Cross(Forward);

        // calculate the new view matrix
        UpdateView();
    }
    // --------------------------------------------------------------------------------------------
    void AFlyCamera::InputKey(float dt, VE::Internal::Graphics::ECameraMovement direction)
    {
        float speed = MovementSpeed * dt;
        if (direction == VE::Internal::Graphics::ECameraMovement::CAMERA_FORWARD)
            m_TargetPosition = m_TargetPosition + Forward * speed;
        else if (direction == VE::Internal::Graphics::ECameraMovement::CAMERA_BACK)
            m_TargetPosition = m_TargetPosition - Forward * speed;
        else if (direction == VE::Internal::Graphics::ECameraMovement::CAMERA_LEFT)
            m_TargetPosition = m_TargetPosition - Right * speed;
        else if (direction == VE::Internal::Graphics::ECameraMovement::CAMERA_RIGHT)
            m_TargetPosition = m_TargetPosition + Right * speed;
        else if (direction == VE::Internal::Graphics::ECameraMovement::CAMERA_UP)
            m_TargetPosition = m_TargetPosition + m_WorldUp * speed;
        else if (direction == VE::Internal::Graphics::ECameraMovement::CAMERA_DOWN)
            m_TargetPosition = m_TargetPosition - m_WorldUp * speed;
    }
    // --------------------------------------------------------------------------------------------
    void AFlyCamera::InputMouse(float deltaX, float deltaY)
    {
        float xmovement = deltaX * MouseSensitivty;
        float ymovement = deltaY * MouseSensitivty;

        m_TargetYaw += xmovement;
        m_TargetPitch += ymovement;

        // prevents calculating the length of the null vector
        if (m_TargetYaw == 0.0f) m_TargetYaw = 0.01f;
        if (m_TargetPitch == 0.0f) m_TargetPitch = 0.01f;

        // it's not allowed to move the pitch above or below 90 degrees asctime the current
        // world-up vector would break our LookAt calculation.
        if (m_TargetPitch > 89.0f) m_TargetPitch = 89.0f;
        if (m_TargetPitch < -89.0f) m_TargetPitch = -89.0f;
    }
    // --------------------------------------------------------------------------------------------
    void AFlyCamera::InputScroll(float deltaX, float deltaY)
    {
        MovementSpeed = VE::Internal::Math::Clamp(MovementSpeed + deltaY * 1.0f, 1.0f, 25.0f);
        Damping       = VE::Internal::Math::Clamp(Damping + deltaX * 0.5f, 1.0f, 25.0f);
    }
} // namespace VE::Internal::RenderModules