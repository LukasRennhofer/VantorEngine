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
 *  File: vantorSkybox.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <glm/glm.hpp>
#include "Renderer/vantorScreenSpaceShader.h"
#include "vantorDrawableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace vantor::Graphics
{
    struct colorPreset
    {
            glm::vec3 cloudColorBottom, skyColorTop, skyColorBottom, lightColor, fogColor;
    };

    class Skybox : public vantor::Graphics::drawableObject
    {
        public:
            friend class VolumetricClouds;
            Skybox();
            ~Skybox();

            virtual void draw();
            virtual void setGui();
            virtual void update();

            colorPreset DefaultPreset();
            colorPreset SunsetPreset();
            colorPreset SunsetPreset1();

            void mixSkyColorPreset(float v, colorPreset p1, colorPreset p2);

            unsigned int getSkyTexture() { return skyboxFBO->tex; }

            glm::vec3 skyColorTop, skyColorBottom;

            vantor::Graphics::Renderer::Shader::ScreenSpaceShader *skyboxShader;
            vantor::Graphics::Renderer::Buffer::FrameBufferObject *skyboxFBO;

            colorPreset presetSunset, highSunPreset;

            bool lensFlareOn = true;
    };
} // NAMESPACE vantor::Graphics