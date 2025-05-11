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
 * File: vantorBackground.hpp
 * Last Change: 
*/

// !! WARNING: IT NEEDS TO BE CHANGED WHEN WORKING WITH VULKAN!!!

#pragma once

#include "../../../Core/Scene/vantorSceneNode.hpp"
#include "../../RenderDevice/vantorRenderDevice.hpp"

namespace vantor::Graphics
{
    class Background : public vantor::SceneNode
    {
    private:
        vantor::Graphics::RenderDevice::OpenGL::TextureCube *m_CubeMap;
        vantor::Graphics::RenderDevice::OpenGL::Shader      *m_Shader;

    public:
        Background();
        ~Background();

        void SetCubemap(vantor::Graphics::RenderDevice::OpenGL::TextureCube* cubemap);
    };
}