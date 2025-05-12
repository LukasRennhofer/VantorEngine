/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorCamera.cpp
 *  Last Change: Automatically updated
 */

#include "vantorCamera.hpp"

namespace vantor::Graphics
{
    // --------------------------------------------------------------------------------------------
    Camera::Camera() {}
    // --------------------------------------------------------------------------------------------
    Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : Position(position), Forward(forward), Up(up) { UpdateView(); }
    // --------------------------------------------------------------------------------------------
    void Camera::Update(float dt) { Frustum.Update(this); }
    // --------------------------------------------------------------------------------------------
    void Camera::SetPerspective(float fov, float aspect, float near, float far)
    {
        Perspective = true;
        Projection  = glm::perspective(fov, aspect, near, far);
        FOV         = fov;
        Aspect      = aspect;
        Near        = near;
        Far         = far;
    }
    // --------------------------------------------------------------------------------------------
    void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
    {
        Perspective = false;
        Projection  = glm::ortho(left, right, top, bottom, near, far);
        Near        = near;
        Far         = far;
    }
    // --------------------------------------------------------------------------------------------
    void Camera::UpdateView() { View = glm::lookAt(Position, Position + Forward, Up); }
    // --------------------------------------------------------------------------------------------
    float Camera::FrustumHeightAtDistance(float distance)
    {
        if (Perspective)
        {
            return 2.0f * distance * tanf(glm::radians(FOV * 0.5f));
        }
        else
        {
            return Frustum.Top().D;
        }
    }
    // --------------------------------------------------------------------------------------------
    float Camera::DistanceAtFrustumHeight(float frustumHeight)
    {
        if (Perspective)
        {
            return frustumHeight * 0.5f / tanf(glm::radians(FOV * 0.5f));
        }
        else
        {
            return Frustum.Near().D;
        }
    }

    // ------------------------------------------------------------------------
    void CameraFrustum::Update(Camera *camera)
    {
        float tan        = 2.0f * std::tan(camera->FOV * 0.5f);
        float nearHeight = tan * camera->Near;
        float nearWidth  = nearHeight * camera->Aspect;
        float farHeight  = tan * camera->Far;
        float farWidth   = farHeight * camera->Aspect;

        glm::vec3 nearCenter = camera->Position + camera->Forward * camera->Near;
        glm::vec3 farCenter  = camera->Position + camera->Forward * camera->Far;

        glm::vec3 v;
        // left plane
        v = (nearCenter - camera->Right * nearWidth * 0.5f) - camera->Position;
        Left().SetNormalD(glm::cross(glm::normalize(v), camera->Up), nearCenter - camera->Right * nearWidth * 0.5f);
        // right plane
        v = (nearCenter + camera->Right * nearWidth * 0.5f) - camera->Position;
        Right().SetNormalD(glm::cross(camera->Up, glm::normalize(v)), nearCenter + camera->Right * nearWidth * 0.5f);
        // top plane
        v = (nearCenter + camera->Up * nearHeight * 0.5f) - camera->Position;
        Top().SetNormalD(glm::cross(glm::normalize(v), camera->Right), nearCenter + camera->Up * nearHeight * 0.5f);
        // bottom plane
        v = (nearCenter - camera->Up * nearHeight * 0.5f) - camera->Position;
        Bottom().SetNormalD(glm::cross(camera->Right, glm::normalize(v)), nearCenter - camera->Up * nearHeight * 0.5f);
        // near plane
        Near().SetNormalD(camera->Forward, nearCenter);
        // far plane
        Far().SetNormalD(-camera->Forward, farCenter);
    }
    // ------------------------------------------------------------------------
    bool CameraFrustum::Intersect(glm::vec3 point)
    {
        FrustumPlane *planes = GetPlanes();
        for (int i = 0; i < 6; ++i)
        {
            if (planes[i].Distance(point) < 0)
            {
                return false;
            }
        }
        return true;
    }
    // ------------------------------------------------------------------------
    bool CameraFrustum::Intersect(glm::vec3 point, float radius)
    {
        FrustumPlane *planes = GetPlanes();
        for (int i = 0; i < 6; ++i)
        {
            if (planes[i].Distance(point) < -radius)
            {
                return false;
            }
        }
        return true;
    }
    // ------------------------------------------------------------------------
    bool CameraFrustum::Intersect(glm::vec3 boxMin, glm::vec3 boxMax)
    {
        FrustumPlane *planes = GetPlanes();
        for (int i = 0; i < 6; ++i)
        {
            glm::vec3 positive = boxMin;
            if (planes[i].Normal.x >= 0) positive.x = boxMax.x;
            if (planes[i].Normal.y >= 0) positive.y = boxMax.y;
            if (planes[i].Normal.z >= 0) positive.z = boxMax.z;

            if (planes[i].Distance(positive) < 0)
            {
                return false;
            }
        }
        return true;
    }

    // --------------------------------------------------------------------------------------------
    FlyCamera::FlyCamera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : Camera(position, forward, up)
    {
        Yaw = -90.0f;

        Forward          = forward;
        m_WorldUp        = Up;
        m_TargetPosition = position;
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::Update(float dt)
    {
        Camera::Update(dt);

        Position = glm::mix(Position, m_TargetPosition, glm::clamp(dt * Damping, 0.0f, 1.0f));
        Yaw      = glm::mix(Yaw, m_TargetYaw, glm::clamp(dt * Damping * 2.0f, 0.0f, 1.0f));
        Pitch    = glm::mix(Pitch, m_TargetPitch, glm::clamp(dt * Damping * 2.0f, 0.0f, 1.0f));

        // calculate new cartesian basis vectors from yaw/pitch pair:
        glm::vec3 newForward;
        newForward.x = cos(0.0174533 * Pitch) * cos(0.0174533 * Yaw);
        newForward.y = sin(0.0174533 * Pitch);
        newForward.z = cos(0.0174533 * Pitch) * sin(0.0174533 * Yaw);
        Forward      = glm::normalize(newForward);
        Right        = glm::normalize(glm::cross(Forward, m_WorldUp));
        Up           = glm::cross(Right, Forward);

        // calculate the new view matrix
        UpdateView();
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::InputKey(float dt, CAMERA_MOVEMENT direction)
    {
        float speed = MovementSpeed * dt;
        if (direction == CAMERA_FORWARD)
            m_TargetPosition = m_TargetPosition + Forward * speed;
        else if (direction == CAMERA_BACK)
            m_TargetPosition = m_TargetPosition - Forward * speed;
        else if (direction == CAMERA_LEFT)
            m_TargetPosition = m_TargetPosition - Right * speed;
        else if (direction == CAMERA_RIGHT)
            m_TargetPosition = m_TargetPosition + Right * speed;
        else if (direction == CAMERA_UP)
            m_TargetPosition = m_TargetPosition + m_WorldUp * speed;
        else if (direction == CAMERA_DOWN)
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

        // it's not allowed to move the pitch above or below 90 degrees asctime
        // the current world-up vector would break our LookAt calculation.
        if (m_TargetPitch > 89.0f) m_TargetPitch = 89.0f;
        if (m_TargetPitch < -89.0f) m_TargetPitch = -89.0f;
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::InputScroll(float deltaX, float deltaY)
    {
        MovementSpeed = glm::clamp(MovementSpeed + deltaY * 1.0f, 1.0f, 25.0f);
        Damping       = glm::clamp(Damping + deltaX * 0.5f, 1.0f, 25.0f);
    }

} // namespace vantor::Graphics
