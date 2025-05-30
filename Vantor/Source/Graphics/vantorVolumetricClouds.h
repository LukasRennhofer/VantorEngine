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
 *  File: vantorVolumetricClouds.h
 *  Last Change: Automatically updated
 */

#pragma once
#include "Renderer/vantorScreenSpaceShader.h"
#include "Renderer/vantorBuffers.h"
#include "vantorTexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <camera.h>
#include <algorithm>
#include "vantorDrawableObject.h"
#include "vantorCloudsModel.h"
#include "../Utils/sdl2Utils.h"

namespace vantor::Graphics
{
    class VolumetricClouds : public drawableObject
    {
        public:
            VolumetricClouds(int SW, int SH, CloudsModel *model);
            virtual void draw();
            ~VolumetricClouds();

            enum cloudsTextureNames
            {
                fragColor,
                bloom,
                alphaness,
                cloudDistance
            };

            void generateWeatherMap();

            unsigned int getCloudsTexture() { return (model->postProcess ? cloudsPostProcessingFBO->getColorAttachmentTex(0) : getCloudsRawTexture()); }

            unsigned int getCloudsTexture(int i) { return cloudsFBO->getColorAttachmentTex(i); }

            unsigned int getCloudsRawTexture() { return cloudsFBO->getColorAttachmentTex(0); }

        private:
            int SCR_WIDTH, SCR_HEIGHT;

            vantor::Graphics::Renderer::Buffer::TextureSet        *cloudsFBO;
            vantor::Graphics::Renderer::Buffer::FrameBufferObject *cloudsPostProcessingFBO;

            CloudsModel *model;
    };
} // NAMESPACE vantor::Graphics