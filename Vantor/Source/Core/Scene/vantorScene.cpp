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
 *  File: vantorScene.cpp
 *  Last Change: Automatically updated
 */

#include "vantorScene.hpp"

#include "vantorSceneNode.hpp"
#include "../../Graphics/RenderDevice/vantorRenderDevice.hpp"

#include <stack>

namespace vantor
{
    SceneNode   *Scene::Root      = new SceneNode(0);
    unsigned int Scene::CounterID = 0;
    // --------------------------------------------------------------------------------------------
    void Scene::Clear()
    {
        Scene::DeleteSceneNode(Root);
        Scene::Root = new SceneNode(0);
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *Scene::MakeSceneNode()
    {
        SceneNode *node = new SceneNode(Scene::CounterID++);
        Root->AddChild(node);
        return node;
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *Scene::MakeSceneNode(vantor::Graphics::RenderDevice::Mesh *mesh, vantor::Graphics::RenderDevice::Material *material)
    {
        SceneNode *node = new SceneNode(Scene::CounterID++);

        node->Mesh     = mesh;
        node->Material = material;

        Root->AddChild(node);
        return node;
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *Scene::MakeSceneNode(SceneNode *node)
    {
        SceneNode *newNode = new SceneNode(Scene::CounterID++);

        newNode->Mesh     = node->Mesh;
        newNode->Material = node->Material;
        newNode->BoxMin   = node->BoxMin;
        newNode->BoxMax   = node->BoxMax;

        std::stack<SceneNode *> nodeStack;
        for (unsigned int i = 0; i < node->GetChildCount(); ++i)
            nodeStack.push(node->GetChildByIndex(i));
        while (!nodeStack.empty())
        {
            SceneNode *child = nodeStack.top();
            nodeStack.pop();

            SceneNode *newChild = new SceneNode(Scene::CounterID++);
            newChild->Mesh      = child->Mesh;
            newChild->Material  = child->Material;
            newChild->BoxMin    = child->BoxMin;
            newChild->BoxMax    = child->BoxMax;
            newNode->AddChild(newChild);

            for (unsigned int i = 0; i < child->GetChildCount(); ++i)
                nodeStack.push(child->GetChildByIndex(i));
        }

        Root->AddChild(newNode);
        return newNode;
    }
    // --------------------------------------------------------------------------------------------
    void Scene::DeleteSceneNode(SceneNode *node)
    {
        if (node->GetParent())
        {
            node->GetParent()->RemoveChild(node->GetID());
        }
        delete node;
    }
} // namespace vantor