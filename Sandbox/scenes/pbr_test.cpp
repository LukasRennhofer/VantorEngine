/*
*    				~ Vantor (Sandbox) ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 25.04.2025
*
* File: pbr_test.cpp
* Last Change:
*/

#include "pbr_test.hpp"

ScenePbrTest::ScenePbrTest(vantor::Graphics::RenderDevice::Renderer *renderer, Cell::Camera *camera)
{
     m_Renderer = renderer;
     m_Camera = camera;

     m_PbrShader = vantor::Resources::LoadShader("pbr", "res/intern/shaders/pbr.vs", "res/intern/shaders/pbr.frag");
     m_PBRMaterial.SetShader(m_PbrShader);
}

ScenePbrTest::~ScenePbrTest()
{

}

void ScenePbrTest::Init()
{

}

void ScenePbrTest::Update(float dt)
{
    m_Camera->Update(dt);
}

void ScenePbrTest::Render()
{

}