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
 *  Date: 2025-05-11
 *
 *  File: vantorSceneNode.cpp
 *  Last Change: Automatically updated
 */

#include "vantorSceneNode.hpp"
#include "vantorScene.hpp"
#include "../../Graphics/RenderDevice/vantorRenderDevice.hpp"

#include <assert.h>

namespace vantor
{
    // --------------------------------------------------------------------------------------------
    SceneNode::SceneNode(unsigned int id) : m_ID(id) {}
    // --------------------------------------------------------------------------------------------
    SceneNode::~SceneNode()
    {
        for (unsigned int i = 0; i < m_Children.size(); ++i)
        {
            delete m_Children[i];
        }
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetPosition(glm::vec3 position)
    {
        m_Position = position;
        m_Dirty    = true;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetRotation(glm::vec4 rotation)
    {
        m_Rotation = rotation;
        m_Dirty    = true;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetScale(glm::vec3 scale)
    {
        m_Scale = scale;
        m_Dirty = true;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetScale(float scale)
    {
        m_Scale = glm::vec3(scale);
        m_Dirty = true;
    }
    // --------------------------------------------------------------------------------------------
    glm::vec3 SceneNode::GetLocalPosition() { return m_Position; }
    // --------------------------------------------------------------------------------------------
    glm::vec4 SceneNode::GetLocalRotation() { return m_Rotation; }
    // --------------------------------------------------------------------------------------------
    glm::vec3 SceneNode::GetLocalScale() { return m_Scale; }
    // --------------------------------------------------------------------------------------------
    glm::vec3 SceneNode::GetWorldPosition()
    {
        glm::mat4 transform = GetTransform();
        glm::vec4 pos       = transform * glm::vec4(m_Position, 1.0f);
        glm::vec3 result    = glm::vec3(pos.x, pos.y, pos.z);
        return result;
    }
    // --------------------------------------------------------------------------------------------
    glm::vec3 SceneNode::GetWorldScale()
    {
        glm::mat4 transform = GetTransform();
        glm::vec3 scale     = glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
        if (scale.x < 0.0f) scale.x *= -1.0f;
        if (scale.y < 0.0f) scale.y *= -1.0f;
        if (scale.z < 0.0f) scale.z *= -1.0f;
        return scale;
    }
    // --------------------------------------------------------------------------------------------
    unsigned int SceneNode::GetID() { return m_ID; }
    // --------------------------------------------------------------------------------------------
    void SceneNode::AddChild(SceneNode *node)
    {
        if (node->m_Parent)
        {
            node->m_Parent->RemoveChild(node->m_ID);
        }
        node->m_Parent = this;
        m_Children.push_back(node);
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::RemoveChild(unsigned int id)
    {
        auto it = std::find(m_Children.begin(), m_Children.end(), GetChild(id));
        if (it != m_Children.end()) m_Children.erase(it);
    }
    // --------------------------------------------------------------------------------------------
    std::vector<SceneNode *> SceneNode::GetChildren() { return m_Children; }
    // --------------------------------------------------------------------------------------------
    unsigned int SceneNode::GetChildCount() { return m_Children.size(); }
    // --------------------------------------------------------------------------------------------
    SceneNode *SceneNode::GetChild(unsigned int id)
    {
        for (unsigned int i = 0; i < m_Children.size(); ++i)
        {
            if (m_Children[i]->GetID() == id) return m_Children[i];
        }
        return nullptr;
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *SceneNode::GetChildByIndex(unsigned int index)
    {
        assert(index < GetChildCount());
        return m_Children[index];
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *SceneNode::GetParent() { return m_Parent; }
    // --------------------------------------------------------------------------------------------
    glm::mat4 SceneNode::GetTransform()
    {
        if (m_Dirty)
        {
            UpdateTransform(false);
        }
        return m_Transform;
    }
    // --------------------------------------------------------------------------------------------
    glm::mat4 SceneNode::GetPrevTransform() { return m_PrevTransform; }
    // --------------------------------------------------------------------------------------------
    void SceneNode::UpdateTransform(bool updatePrevTransform)
    {
        if (updatePrevTransform)
        {
            m_PrevTransform = m_Transform;
        }
        if (m_Dirty)
        {
            m_Transform = glm::translate(m_Position);
            m_Transform = glm::scale(m_Transform, m_Scale);
            m_Transform = glm::rotate(m_Transform, m_Rotation.xyz, m_Rotation.w);
            if (m_Parent)
            {
                m_Transform = m_Parent->m_Transform * m_Transform;
            }
        }
        for (int i = 0; i < m_Children.size(); ++i)
        {
            if (m_Dirty)
            {
                m_Children[i]->m_Dirty = true;
            }
            m_Children[i]->UpdateTransform(updatePrevTransform);
        }
        m_Dirty = false;
    }
} // namespace vantor