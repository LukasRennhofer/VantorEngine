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
 *  File: vantorResource.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include "vantorResource.hpp"
#include "vantorResourceLoader.hpp"

#include "../Scene/vantorScene.hpp"
#include "../Scene/vantorSceneNode.hpp"

#include "../../Helpers/vantorString.hpp"
#include "../BackLog/vantorBacklog.h"

#include <stack>
#include <vector>

namespace vantor
{
    std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::Shader> Resources::m_Shaders
        = std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::Shader>();
    std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::Texture> Resources::m_Textures
        = std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::Texture>();
    std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::TextureCube> Resources::m_TexturesCube
        = std::map<unsigned int, vantor::Graphics::RenderDevice::OpenGL::TextureCube>();
    std::map<unsigned int, SceneNode *> Resources::m_Meshes = std::map<unsigned int, SceneNode *>();
    // --------------------------------------------------------------------------------------------
    void Resources::Init() { vantor::Graphics::RenderDevice::OpenGL::Texture placeholderTexture; }
    void Resources::Clean()
    {
        for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
        {
            delete it->second;
        }
    }

    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Shader *
    Resources::LoadShader(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines)
    {
        unsigned int id = SID(name);

        if (Resources::m_Shaders.find(id) != Resources::m_Shaders.end()) return &Resources::m_Shaders[id];

        vantor::Graphics::RenderDevice::OpenGL::Shader shader = ShaderLoader::Load(name, vsPath, fsPath, defines);
        Resources::m_Shaders[id]                              = shader;
        return &Resources::m_Shaders[id];
    }
    vantor::Graphics::RenderDevice::OpenGL::Shader *Resources::GetShader(std::string name)
    {
        unsigned int id = SID(name);

        if (Resources::m_Shaders.find(id) != Resources::m_Shaders.end())
        {
            return &Resources::m_Shaders[id];
        }
        else
        {
            vantor::Backlog::Log("ResourceLoader", "Requested shader: " + name + " not found!", vantor::Backlog::LogLevel::WARNING);
            return nullptr;
        }
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Texture *Resources::LoadTexture(std::string name, std::string path, GLenum target, GLenum format, bool srgb)
    {
        unsigned int id = SID(name);

        if (Resources::m_Textures.find(id) != Resources::m_Textures.end()) return &Resources::m_Textures[id];

        vantor::Backlog::Log("ResourceLoader", "Loading texture file at: " + path + ".", vantor::Backlog::LogLevel::INFO);

        vantor::Graphics::RenderDevice::OpenGL::Texture texture = TextureLoader::LoadTexture(path, target, format, srgb);

        vantor::Backlog::Log("ResourceLoader", "Succesfully loaded: " + path + ".", vantor::Backlog::LogLevel::INFO);

        if (texture.Width > 0)
        {
            Resources::m_Textures[id] = texture;
            return &Resources::m_Textures[id];
        }
        else
        {
            return nullptr;
        }
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Texture *Resources::LoadHDR(std::string name, std::string path)
    {
        unsigned int id = SID(name);

        if (Resources::m_Textures.find(id) != Resources::m_Textures.end()) return &Resources::m_Textures[id];

        vantor::Graphics::RenderDevice::OpenGL::Texture texture = TextureLoader::LoadHDRTexture(path);

        if (texture.Width > 0)
        {
            Resources::m_Textures[id] = texture;
            return &Resources::m_Textures[id];
        }
        else
        {
            return nullptr;
        }
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Texture *Resources::GetTexture(std::string name)
    {
        unsigned int id = SID(name);

        if (Resources::m_Textures.find(id) != Resources::m_Textures.end())
        {
            return &Resources::m_Textures[id];
        }
        else
        {
            vantor::Backlog::Log("ResourceLoader", "Requested texture: " + name + " not found!", vantor::Backlog::LogLevel::WARNING);
            return nullptr;
        }
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::TextureCube *Resources::LoadTextureCube(std::string name, std::string folder)
    {
        unsigned int id = SID(name);

        if (Resources::m_TexturesCube.find(id) != Resources::m_TexturesCube.end()) return &Resources::m_TexturesCube[id];

        vantor::Graphics::RenderDevice::OpenGL::TextureCube texture = TextureLoader::LoadTextureCube(folder);
        Resources::m_TexturesCube[id]                               = texture;
        return &Resources::m_TexturesCube[id];
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::TextureCube *Resources::GetTextureCube(std::string name)
    {
        unsigned int id = SID(name);

        if (Resources::m_TexturesCube.find(id) != Resources::m_TexturesCube.end())
        {
            return &Resources::m_TexturesCube[id];
        }
        else
        {
            vantor::Backlog::Log("ResourceLoader", "Requested texture cube: " + name + " not found!", vantor::Backlog::LogLevel::WARNING);
            return nullptr;
        }
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *Resources::LoadMesh(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer, std::string name, std::string path)
    {
        unsigned int id = SID(name);

        if (Resources::m_Meshes.find(id) != Resources::m_Meshes.end())
        {
            return Scene::MakeSceneNode(Resources::m_Meshes[id]);
        }

        SceneNode *node         = MeshLoader::LoadMesh(renderer, path);
        Resources::m_Meshes[id] = node;

        return Scene::MakeSceneNode(node);
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *Resources::GetMesh(std::string name)
    {
        unsigned int id = SID(name);

        if (Resources::m_Meshes.find(id) != Resources::m_Meshes.end())
        {
            return Scene::MakeSceneNode(Resources::m_Meshes[id]);
        }
        else
        {
            vantor::Backlog::Log("ResourceLoader", "Requested mesh: " + name + " not found!", vantor::Backlog::LogLevel::WARNING);
            return nullptr;
        }
    }
} // namespace vantor