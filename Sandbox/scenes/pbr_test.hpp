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
* File: pbr_test.hpp
* Last Change:
*/

#pragma once

#include "../pch.hpp"

class ScenePbrTest 
{
private:
    vantor::Graphics::RenderDevice::Renderer *m_Renderer;
    vantor::Graphics::Camera   *m_Camera;

    vantor::Graphics::RenderDevice::Material m_PBRMaterial;
    vantor::Graphics::RenderDevice::Material m_PBRIrradianceMaterial;
    vantor::Graphics::RenderDevice::Material m_PBRPrefilterMaterial;
    vantor::Graphics::RenderDevice::Material m_PBRBRDFIntegrateMaterial;
    vantor::Graphics::RenderDevice::Shader   *m_PbrShader;
    vantor::Graphics::RenderDevice::Shader   *m_PbrIrradianceShader;
    vantor::Graphics::RenderDevice::Shader   *m_PbrPrefilterShader;
    vantor::Graphics::RenderDevice::Shader   *m_PbrBRDFintegrateShader;

public:
    // NOTE(Joey): pass over relevant data per scene: dependancy injection
    ScenePbrTest(vantor::Graphics::RenderDevice::Renderer *renderer, vantor::Graphics::Camera *camera);
    ~ScenePbrTest();

    void Init();

    void Update(float dt);
    void Render();
};

#endif