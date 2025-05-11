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
 * File: vantorBackground.cpp
 * Last Change: 
*/

// !! WARNING: IT NEEDS TO BE CHANGED WHEN WORKING WITH VULKAN!!!

#include "vantorBackground.hpp"

#include "../../../Core/Scene/vantorScene.hpp"
#include "../../../Core/Resource/vantorResource.hpp"

#include "../../Geometry/Primitives/vantorCube.hpp"

namespace vantor::Graphics
{
    // --------------------------------------------------------------------------------------------
    Background::Background() : vantor::SceneNode(vantor::Scene::CounterID++)
    {
        vantor::Scene::Root->AddChild(this);

        m_Shader  = vantor::Resources::LoadShader("background", "res/intern/shaders/background.vs", "res/intern/shaders/background.fs");
        Material  = new vantor::Graphics::RenderDevice::OpenGL::Material(m_Shader);
        Mesh      = new vantor::Graphics::Geometry::Primitives::Cube;
        BoxMin    = glm::vec3(-99999.0);
        BoxMax    = glm::vec3( 99999.0);

        // default material configuration
        Material->SetFloat("Exposure", 1.0f);
        Material->DepthCompare = GL_LEQUAL;
        Material->Cull = false;
        Material->ShadowCast = false;
        Material->ShadowReceive = false;
    }
    // --------------------------------------------------------------------------------------------
    Background::~Background()
    {

    }
    // --------------------------------------------------------------------------------------------
    void Background::SetCubemap(vantor::Graphics::RenderDevice::OpenGL::TextureCube* cubemap)
    {
        m_CubeMap = cubemap;
        Material->SetTextureCube("background", m_CubeMap, 0);
    }
};