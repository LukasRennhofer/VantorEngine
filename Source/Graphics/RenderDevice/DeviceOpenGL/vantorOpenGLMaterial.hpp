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
 *  File: vantorOpenGLMaterial.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "vantorOpenGLShadingTypes.hpp"
#include "vantorOpenGLShader.hpp"
#include "vantorOpenGLTexture.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>

namespace vantor::Graphics::RenderDevice::OpenGL
{
    enum MaterialType
    {
        MATERIAL_DEFAULT,
        MATERIAL_CUSTOM,
        MATERIAL_POST_PROCESS,
    };

    class Material
    {
        private:
            // shader state
            Shader                                    *m_Shader;
            std::map<std::string, UniformValue>        m_Uniforms;
            std::map<std::string, UniformValueSampler> m_SamplerUniforms; // NOTE(Joey): process samplers differently
        public:
            MaterialType Type  = MATERIAL_CUSTOM;
            glm::vec4    Color = glm::vec4(1.0f);

            // depth state
            bool   DepthTest    = true;
            bool   DepthWrite   = true;
            GLenum DepthCompare = GL_LESS;

            // face culling state
            bool   Cull             = true;
            GLenum CullFace         = GL_BACK;
            GLenum CullWindingOrder = GL_CCW;

            // blending state
            bool   Blend         = false;
            GLenum BlendSrc      = GL_ONE; // pre-multiplied alpha
            GLenum BlendDst      = GL_ONE_MINUS_SRC_ALPHA;
            GLenum BlendEquation = GL_FUNC_ADD;

            // shadow state
            bool ShadowCast    = true;
            bool ShadowReceive = true;

        private:
        public:
            Material();
            Material(Shader *shader);

            Shader *GetShader();
            void    SetShader(Shader *shader);

            Material Copy();

            void SetBool(std::string name, bool value);
            void SetInt(std::string name, int value);
            void SetFloat(std::string name, float value);
            void SetTexture(std::string name, Texture *value, unsigned int unit = 0);
            void SetTextureCube(std::string name, TextureCube *value, unsigned int unit = 0);
            void SetVector(std::string name, glm::vec2 value);
            void SetVector(std::string name, glm::vec3 value);
            void SetVector(std::string name, glm::vec4 value);
            void SetMatrix(std::string name, glm::mat2 value);
            void SetMatrix(std::string name, glm::mat3 value);
            void SetMatrix(std::string name, glm::mat4 value);

            std::map<std::string, UniformValue>        *GetUniforms();
            std::map<std::string, UniformValueSampler> *GetSamplerUniforms();
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL