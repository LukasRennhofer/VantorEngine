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
 *  File: vantorCloudsModel.h
 *  Last Change: Automatically updated
 */

#pragma once
#include <glm/glm.hpp>

#include "Renderer/vantorShader.h"
#include "Renderer/vantorScreenSpaceShader.h"
#include "../Core/Backlog/vantorBacklog.h"

#include "vantorSceneElements.h"

#include "../External/imgui/imgui.h"

#include "vantorSkybox.h"

#define INT_CEIL(n, d) (int) ceil((float) n / d)

namespace vantor::Graphics
{
    class CloudsModel : public drawableObject
    {
        public:
            friend class VolumetricClouds;

            CloudsModel(sceneElements *scene, vantor::Graphics::Skybox *sky);
            ~CloudsModel();

            virtual void draw() {};

            virtual void update();
            virtual void setGui();

        private:
            vantor::Graphics::Renderer::Shader::Shader            *volumetricCloudsShader, *weatherShader;
            vantor::Graphics::Renderer::Shader::ScreenSpaceShader *postProcessingShader;

            float coverage, cloudSpeed, crispiness, curliness, density, absorption;
            float earthRadius, sphereInnerRadius, sphereOuterRadius;
            float perlinFrequency;
            bool  enableGodRays;
            bool  enablePowder;
            bool  postProcess;

            glm::vec3 cloudColorTop, cloudColorBottom;

            glm::vec3    seed, oldSeed;
            unsigned int perlinTex, worley32, weatherTex;

            sceneElements            *scene;
            vantor::Graphics::Skybox *sky;

            void generateWeatherMap();
            void generateModelTextures();
            void initVariables();
            void initShaders();
    };
} // NAMESPACE vantor::Graphics