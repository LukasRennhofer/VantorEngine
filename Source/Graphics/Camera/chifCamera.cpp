/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifCamera.cpp
 * Last Change: 
*/

#include "chifCamera.hpp"

namespace chif::Graphics {
    // --------------------------------------------------------------------------------------------
    Camera::Camera()
    {
    }
    // --------------------------------------------------------------------------------------------
    Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
        : Position(position), Forward(forward), Up(up)
    {
        UpdateView();
    }
    // --------------------------------------------------------------------------------------------
    void Camera::Update(float dt)
    {
        Frustum.Update(this);
    }
    // --------------------------------------------------------------------------------------------
    void Camera::SetPerspective(float fov, float aspect, float near, float far)
    {
        Perspective = true;
        Projection = glm::perspective(fov, aspect, near, far);
        FOV = fov;
        Aspect = aspect;
        Near = near;
        Far = far;
    }
    // --------------------------------------------------------------------------------------------
    void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
    {
        Perspective = false;
        Projection = glm::ortho(left, right, top, bottom, near, far);
        Near = near;
        Far = far;
    }
    // --------------------------------------------------------------------------------------------
    void Camera::UpdateView()
    {
        View = glm::lookAt(Position, Position + Forward, Up);
    }
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
    void CameraFrustum::Update(Camera* camera)
    {
        float tan = 2.0f * std::tan(camera->FOV * 0.5f);
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
        v = (nearCenter + camera->Right * nearWidth  * 0.5f) - camera->Position;
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
        FrustumPlane* planes = GetPlanes();
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
        FrustumPlane* planes = GetPlanes();
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
        FrustumPlane* planes = GetPlanes();
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

} // namespace chif::Graphics
