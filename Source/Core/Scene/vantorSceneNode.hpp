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
 *  File: vantorSceneNode.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../../Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLMesh.hpp"
#include "../../Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLMaterial.hpp"

namespace vantor
{
    class Scene;

    class SceneNode
    {
        public:
            vantor::Graphics::RenderDevice::OpenGL::Mesh     *Mesh;
            vantor::Graphics::RenderDevice::OpenGL::Material *Material;

            glm::vec3 BoxMin = glm::vec3(-99999.0f);
            glm::vec3 BoxMax = glm::vec3(99999.0f);

        private:
            std::vector<SceneNode *> m_Children;
            SceneNode               *m_Parent;

            glm::mat4 m_Transform;
            glm::mat4 m_PrevTransform;
            glm::vec3 m_Position = glm::vec3(0.0f);
            glm::vec4 m_Rotation;
            glm::vec3 m_Scale = glm::vec3(1.0f);

            bool m_Dirty;

            unsigned int m_ID;

            static unsigned int CounterID;

        public:
            SceneNode(unsigned int id);
            ~SceneNode();

            void      SetPosition(glm::vec3 position);
            void      SetRotation(glm::vec4 rotation);
            void      SetScale(glm::vec3 scale);
            void      SetScale(float scale);
            glm::vec3 GetLocalPosition();
            glm::vec4 GetLocalRotation();
            glm::vec3 GetLocalScale();
            glm::vec3 GetWorldPosition();
            glm::vec3 GetWorldScale();

            unsigned int             GetID();
            void                     AddChild(SceneNode *node);
            void                     RemoveChild(unsigned int id);
            std::vector<SceneNode *> GetChildren();
            unsigned int             GetChildCount();
            SceneNode               *GetChild(unsigned int id);
            SceneNode               *GetChildByIndex(unsigned int index);
            SceneNode               *GetParent();

            glm::mat4 GetTransform();
            glm::mat4 GetPrevTransform();

            void UpdateTransform(bool updatePrevTransform = false);
    };
} // namespace vantor