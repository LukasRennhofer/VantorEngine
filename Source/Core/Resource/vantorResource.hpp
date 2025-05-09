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
 * File: vantorResource.hpp
 * Last Change: Added Version settings and vantor::core::version child namespace
*/

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum and other OpenGL usage

#pragma once

#include "../../Graphics/RenderDevice/vantorRenderDevice.hpp"

#include <map>
#include <string>

namespace Cell
{
    class SceneNode;
    class Renderer;

    class Resources
    {
    private:
        // Store every Resource with a hashed string
        static std::map<unsigned int, vantor::Graphics::RenderDevice::Shader>      m_Shaders;
        static std::map<unsigned int, vantor::Graphics::RenderDevice::Texture>     m_Textures;
        static std::map<unsigned int, vantor::Graphics::RenderDevice::TextureCube> m_TexturesCube;
        static std::map<unsigned int, SceneNode*>  m_Meshes;
    public:

    private:
        Resources(); 
    public:
        static void Init();
        static void Clean();

        // shader resources
        static vantor::Graphics::RenderDevice::Shader*      LoadShader(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines = std::vector<std::string>());
        static vantor::Graphics::RenderDevice::Shader*      GetShader(std::string name);
        // texture resources
        static vantor::Graphics::RenderDevice::Texture*     LoadTexture(std::string name, std::string path, GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA, bool srgb = false);
        static vantor::Graphics::RenderDevice::Texture*     LoadHDR(std::string name, std::string path);
        static vantor::Graphics::RenderDevice::TextureCube* LoadTextureCube(std::string name, std::string folder);
        static vantor::Graphics::RenderDevice::Texture*     GetTexture(std::string name);
        static vantor::Graphics::RenderDevice::TextureCube* GetTextureCube(std::string name);
        // mesh/scene resources
        static SceneNode*  LoadMesh(Renderer* renderer, std::string name, std::string path);
        static SceneNode*  GetMesh(std::string name);
    };
}
#endif 