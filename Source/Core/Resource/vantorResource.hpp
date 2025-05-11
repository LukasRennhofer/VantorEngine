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
 *  File: vantorResource.hpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#pragma once

#include "../../Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLShader.hpp"
#include "../../Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLTexture.hpp"
#include "../../Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLRenderer.hpp"
#include "../Scene/vantorSceneNode.hpp"

#include <map>
#include <string>

namespace vantor
{

    class Resources
    {
        private:
            // Store every Resource with a hashed string
            static std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::Shader>      m_Shaders;
            static std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::Texture>     m_Textures;
            static std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::TextureCube> m_TexturesCube;
            static std::map<unsigned int, SceneNode *>                                         m_Meshes;

        public:
        private:
            Resources();

        public:
            static void Init();
            static void Clean();

            // shader resources
            static vantor::Graphics::RenderDevice::OpenGL::Shader *
            LoadShader(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines = std::vector<std::string>());
            static vantor::Graphics::RenderDevice::OpenGL::Shader *GetShader(std::string name);
            // texture resources
            static vantor::Graphics::RenderDevice::OpenGL::Texture *
            LoadTexture(std::string name, std::string path, GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA, bool srgb = false);
            static vantor::Graphics::RenderDevice::OpenGL::Texture     *LoadHDR(std::string name, std::string path);
            static vantor::Graphics::RenderDevice::OpenGL::TextureCube *LoadTextureCube(std::string name, std::string folder);
            static vantor::Graphics::RenderDevice::OpenGL::Texture     *GetTexture(std::string name);
            static vantor::Graphics::RenderDevice::OpenGL::TextureCube *GetTextureCube(std::string name);
            // mesh/scene resources
            static SceneNode *LoadMesh(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer, std::string name, std::string path);
            static SceneNode *GetMesh(std::string name);
    };
} // namespace vantor