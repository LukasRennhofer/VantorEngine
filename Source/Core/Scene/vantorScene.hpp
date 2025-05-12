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
 *  File: vantorScene.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <vector>
#include "../../Graphics/RenderDevice/vantorRenderDevice.hpp"
namespace vantor
{
    class SceneNode;

    class Scene
    {
        public:
            static SceneNode   *Root;
            static unsigned int CounterID;

        public:
            static void Clear();

            static SceneNode *MakeSceneNode();
            static SceneNode *MakeSceneNode(vantor::Graphics::RenderDevice::OpenGL::Mesh *mesh, vantor::Graphics::RenderDevice::OpenGL::Material *material);
            static SceneNode *MakeSceneNode(SceneNode *node);

            static void DeleteSceneNode(SceneNode *node);
    };
} // namespace vantor