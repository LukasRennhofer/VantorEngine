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
 *  Date: 2025-05-11
 *
 *  File: vantorWater.h
 *  Last Change: Automatically updated
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
// #include <model.h>
#include "Renderer/vantorShader.h"
#include "Renderer/vantorBuffers.h"
#include "../Platform/vantorWindow.h"
#include "vantorDrawableObject.h"
#include "../Utils/sdl2Utils.h"

namespace vantor::Graphics
{
    class Water : public drawableObject
    {
        public:
            Water(glm::vec2 position, float scale, float height);
            virtual ~Water();
            virtual void draw();
            virtual void setGui();
            void         bindRefractionFBO();
            void         bindReflectionFBO();
            void         unbindFBO();

            void setPosition(glm::vec2 position, float scale, float height)
            {
                glm::mat4 identity;
                glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
                glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position.x, height, position.y));
                this->modelMatrix     = transMatrix * scaleMatrix;
            }

            void setHeight(float height)
            {
                float scale      = modelMatrix[0][0];
                float position_x = modelMatrix[3][0];
                float position_z = modelMatrix[3][2];

                glm::mat4 identity;
                glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
                glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position_x, height, position_z));
                this->modelMatrix     = transMatrix * scaleMatrix;
            }

            float getHeight() { return height; }

            glm::mat4 getModelMatrix() { return modelMatrix; }

            static const int FBOw = 1280;
            static const int FBOh = 720;

            vantor::Graphics::Renderer::Buffer::FrameBufferObject const &getReflectionFBO() { return *reflectionFBO; }

        private:
            void drawVertices();

            unsigned int                                           planeVAO, planeVBO, planeEBO;
            float                                                  scale, height;
            vantor::Graphics::Renderer::Buffer::FrameBufferObject *reflectionFBO;
            vantor::Graphics::Renderer::Buffer::FrameBufferObject *refractionFBO;

            unsigned int                                dudvMap, normalMap;
            glm::mat4                                   modelMatrix;
            vantor::Graphics::Renderer::Shader::Shader *shad;
            // Model * waterPlane;
    };
} // NAMESPACE vantor::Graphics