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
 *  File: vantorScreenSpaceShader.h
 *  Last Change: Automatically updated
 */

#pragma once
#include "vantorShader.h"
#include <glad/glad.h>
#include "../vantorDrawableObject.h"

namespace vantor::Graphics::Renderer::Shader
{
    class ScreenSpaceShader : drawableObject
    {
        public:
            ScreenSpaceShader(const char *fragmentPath);
            ~ScreenSpaceShader();

            Shader *const getShaderPtr() { return shad; }

            Shader &getShader() { return *shad; }

            virtual void draw();
            static void  drawQuad();

            static void disableTests()
            {
                // glDisable(GL_CLIP_DISTANCE0);
                glDisable(GL_DEPTH_TEST);
            }

            static void enableTests() { glEnable(GL_DEPTH_TEST); }

        private:
            Shader             *shad;
            static unsigned int quadVAO, quadVBO;
            static bool         initialized;

            void initializeQuad();
    };
} // NAMESPACE vantor::Graphics::Renderer::Shader