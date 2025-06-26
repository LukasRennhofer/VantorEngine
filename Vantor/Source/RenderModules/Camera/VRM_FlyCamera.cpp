/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-06-26
 *
 *  File: VRM_FlyCamera.cpp
 *  Last Change: Automatically updated
 */

#include "VRM_FlyCamera.hpp"

// Math
#include "../../Math/VMA_Common.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"

// Fly Camera from Cell Engine @JoeyDeVries

namespace Vantor::RenderModules
{
    FlyCamera::FlyCamera(Vantor::Math::VVector3 position, Vantor::Math::VVector3 forward, Vantor::Math::VVector3 up) : Camera(position, forward, up)
    {
        Yaw = -90.0f;

        Forward          = forward;
        m_WorldUp        = Up;
        m_TargetPosition = position;
    }

    void FlyCamera::Update(float dt)
    {
        Camera::Update(dt);

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
    void FlyCamera::InputKey(float dt, Vantor::Renderer::VECameraMovement direction)
    {
        float speed = MovementSpeed * dt;
        if (direction == Vantor::Renderer::CAMERA_FORWARD)
            m_TargetPosition = m_TargetPosition + Forward * speed;
        else if (direction == Vantor::Renderer::CAMERA_BACK)
            m_TargetPosition = m_TargetPosition - Forward * speed;
        else if (direction == Vantor::Renderer::CAMERA_LEFT)
            m_TargetPosition = m_TargetPosition - Right * speed;
        else if (direction == Vantor::Renderer::CAMERA_RIGHT)
            m_TargetPosition = m_TargetPosition + Right * speed;
        else if (direction == Vantor::Renderer::CAMERA_UP)
            m_TargetPosition = m_TargetPosition + m_WorldUp * speed;
        else if (direction == Vantor::Renderer::CAMERA_DOWN)
            m_TargetPosition = m_TargetPosition - m_WorldUp * speed;
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::InputMouse(float deltaX, float deltaY)
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
    void FlyCamera::InputScroll(float deltaX, float deltaY)
    {
        MovementSpeed = Vantor::Math::Clamp(MovementSpeed + deltaY * 1.0f, 1.0f, 25.0f);
        Damping       = Vantor::Math::Clamp(Damping + deltaX * 0.5f, 1.0f, 25.0f);
    }
} // namespace Vantor::RenderModules