/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorCamera.hpp
 * Last Change: 
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

namespace vantor::Graphics {

    enum CAMERA_MOVEMENT {
        CAMERA_FORWARD,
        CAMERA_BACK,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_UP,
        CAMERA_DOWN,
    };
    
    struct FrustumPlane
    {
        glm::vec3 Normal;
        float     D;
    
        void SetNormalD(glm::vec3 normal, glm::vec3 point)
        {
            Normal = glm::normalize(normal);
            D      = -glm::dot(Normal, point);
        }
    
        float Distance(glm::vec3 point) const
        {
            return glm::dot(Normal, point) + D;
        }
    };
    
    class Camera;
    
    class CameraFrustum
    {
    public:
        FrustumPlane Planes[6];
    
        enum PlaneIndex {
            LEFT   = 0,
            RIGHT  = 1,
            TOP    = 2,
            BOTTOM = 3,
            NEAR   = 4,
            FAR    = 5
        };
    
        CameraFrustum() = default;
    
        void Update(Camera* camera);
    
        bool Intersect(glm::vec3 point);
        bool Intersect(glm::vec3 point, float radius);
        bool Intersect(glm::vec3 boxMin, glm::vec3 boxMax);
    
        FrustumPlane* GetPlanes() { return Planes; }
        const FrustumPlane* GetPlanes() const { return Planes; }

        FrustumPlane& Left()   { return Planes[LEFT]; }
        FrustumPlane& Right()  { return Planes[RIGHT]; }
        FrustumPlane& Top()    { return Planes[TOP]; }
        FrustumPlane& Bottom() { return Planes[BOTTOM]; }
        FrustumPlane& Near()   { return Planes[NEAR]; }
        FrustumPlane& Far()    { return Planes[FAR]; }

        const FrustumPlane& Left()   const { return Planes[LEFT]; }
        const FrustumPlane& Right()  const { return Planes[RIGHT]; }
        const FrustumPlane& Top()    const { return Planes[TOP]; }
        const FrustumPlane& Bottom() const { return Planes[BOTTOM]; }
        const FrustumPlane& Near()   const { return Planes[NEAR]; }
        const FrustumPlane& Far()    const { return Planes[FAR]; }
    };
    
    class Camera
    {
    public:
        glm::mat4 Projection;
        glm::mat4 View;
    
        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 Forward  = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up       = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Right    = glm::vec3(1.0f, 0.0f, 0.0f);
    
        float FOV;
        float Aspect;
        float Near;
        float Far;
        bool  Perspective;
    
        CameraFrustum Frustum;
    
        Camera(); 
        Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up);
    
        void Update(float dt);
            
        void SetPerspective(float fov, float aspect, float near, float far);
        void SetOrthographic(float left, float right, float top, float bottom, float near, float far);
    
        void UpdateView();
    
        float FrustumHeightAtDistance(float distance);
        float DistanceAtFrustumHeight(float frustumHeight);
    };
} // namespace vantor::Graphics
