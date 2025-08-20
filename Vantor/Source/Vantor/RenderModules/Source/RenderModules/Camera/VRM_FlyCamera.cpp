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

namespace Vantor::RenderModules
{
    AFlyCamera::AFlyCamera(Vantor::Math::VVector3 position, Vantor::Math::VVector3 forward, Vantor::Math::VVector3 up) : ACamera(position, forward, up)
    {
        Yaw = -90.0f;

        Forward          = forward;
        m_WorldUp        = Up;
        m_TargetPosition = position;
    }

    void AFlyCamera::Update(float dt)
    {
        ACamera::Update(dt);

        Position = Vantor::Math::Lerp(Position, m_TargetPosition, Vantor::Math::Clamp01(dt * Damping));
        Yaw      = Vantor::Math::Lerp(Yaw, m_TargetYaw, Vantor::Math::Clamp01(dt * Damping * 2.0f));
        Pitch    = Vantor::Math::Lerp(Pitch, m_TargetPitch, Vantor::Math::Clamp01(dt * Damping * 2.0f));

        // calculate new cartesian basis vectors from yaw/pitch pair:
        Vantor::Math::VVector3 newForward;
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
    void AFlyCamera::InputKey(float dt, Vantor::Graphics::ECameraMovement direction)
    {
        float speed = MovementSpeed * dt;
        if (direction == Vantor::Graphics::ECameraMovement::CAMERA_FORWARD)
            m_TargetPosition = m_TargetPosition + Forward * speed;
        else if (direction == Vantor::Graphics::ECameraMovement::CAMERA_BACK)
            m_TargetPosition = m_TargetPosition - Forward * speed;
        else if (direction == Vantor::Graphics::ECameraMovement::CAMERA_LEFT)
            m_TargetPosition = m_TargetPosition - Right * speed;
        else if (direction == Vantor::Graphics::ECameraMovement::CAMERA_RIGHT)
            m_TargetPosition = m_TargetPosition + Right * speed;
        else if (direction == Vantor::Graphics::ECameraMovement::CAMERA_UP)
            m_TargetPosition = m_TargetPosition + m_WorldUp * speed;
        else if (direction == Vantor::Graphics::ECameraMovement::CAMERA_DOWN)
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
        MovementSpeed = Vantor::Math::Clamp(MovementSpeed + deltaY * 1.0f, 1.0f, 25.0f);
        Damping       = Vantor::Math::Clamp(Damping + deltaX * 0.5f, 1.0f, 25.0f);
    }
} // namespace Vantor::RenderModules