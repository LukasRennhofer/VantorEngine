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
 * File: vantorScene.hpp
 * Last Change: Added Version settings and vantor::core::version child namespace
*/

#pragma once

#include <vector>

namespace vantor {
    class Mesh;
    class Material;
    class SceneNode;

    class Scene
    {
    public:
        static SceneNode* Root;
        static unsigned int CounterID;
    public:
        static void Clear();
 
        static SceneNode* MakeSceneNode();
        static SceneNode* MakeSceneNode(Mesh* mesh, Material* material);
        static SceneNode* MakeSceneNode(SceneNode* node);

        static void DeleteSceneNode(SceneNode* node);
    };
} // namespace vantor