/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-11
 *
 *  File: VRES_Manager.cpp
 *  Last Change: Automatically updated
 */

#include <algorithm>
#include <iostream>

#include "../RenderDevice/VRD_Factory.hpp"
#include "VRES_Manager.hpp"
#include <iostream>

// #include <Shared/STB/stb_image.h> TODO: Load Texture2D from Memory

namespace Vantor::Resource
{
    // VTexture2DResource implementation
    void VTexture2DResource::Unload()
    {
        if (m_texture)
        {
            m_texture.reset();
        }
        m_memoryUsage = 0;
    }

    // VShaderProgramResource implementation
    void VShaderProgramResource::Unload()
    {
        if (m_shader)
        {
            m_shader.reset();
        }
        m_vertexPath.clear();
        m_fragmentPath.clear();
        m_memoryUsage = 0;
    }

    // VResourceManager implementation
    VResourceManager::~VResourceManager()
    {
        if (m_initialized)
        {
            Shutdown();
        }
    }

    bool VResourceManager::Initialize()
    {
        if (m_initialized)
        {
            return true;
        }

        // Add default resource paths
        std::string execDir = Platform::Environment::GetExecutableDirectory();

        m_initialized = true;

        Vantor::Backlog::Log("VResourceManager", "Resource Manager initialized successfully", Vantor::Backlog::LogLevel::INFO);

        return true;
    }

    void VResourceManager::Shutdown()
    {
        if (!m_initialized)
        {
            return;
        }

        UnloadAllResources();

        m_aliases.clear();

        m_initialized = false;

        Vantor::Backlog::Log("VResourceManager", "Resource Manager shut down", Vantor::Backlog::LogLevel::INFO);
    }

    std::shared_ptr<VTexture2DResource> VResourceManager::LoadTexture2D(const VResourceHandle                       &handle,
                                                                        const std::string                           &filePath,
                                                                        const Vantor::RenderDevice::VTextureSampler &sampler,
                                                                        bool                                         generateMipmaps)
    {
        if (!m_initialized)
        {
            Vantor::Backlog::Log("VResourceManager", "Resource Manager not initialized", Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(m_resourceMutex);

        // Check if already loaded
        auto it = m_resources.find(handle);
        if (it != m_resources.end())
        {
            auto texture = std::dynamic_pointer_cast<VTexture2DResource>(it->second);
            if (texture)
            {
                return texture;
            }
        }

        // Resolve file path
        if (!FileExists(filePath))
        {
            Vantor::Backlog::Log("VResourceManager", "Texture file not found: " + filePath, Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }

        try
        {
            // Create texture using render device factory
            auto renderTexture = Vantor::RenderDevice::CreateTexture2DInstance(filePath, sampler, generateMipmaps);
            if (!renderTexture)
            {
                Vantor::Backlog::Log("VResourceManager", "Failed to create texture object", Vantor::Backlog::LogLevel::ERR);
            }

            // Create resource wrapper
            auto resource = std::make_shared<VTexture2DResource>();
            resource->SetHandle(handle);
            resource->SetTexture(renderTexture);
            resource->SetMemoryUsage(renderTexture->GetMemoryUsage() + sizeof(VTexture2DResource));

            // Store resource
            m_resources[handle] = resource;

            Vantor::Backlog::Log("VResourceManager", "Texture loaded: " + handle + " (" + filePath + ")", Vantor::Backlog::LogLevel::INFO);

            return resource;
        }
        catch (const std::exception &e)
        {
            Vantor::Backlog::Log("VResourceManager", "Exception loading texture " + filePath + ": " + e.what(), Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }
    }

    std::shared_ptr<VShaderProgramResource>
    VResourceManager::LoadShaderProgram(const VResourceHandle &handle, const std::string &vertexPath, const std::string &fragmentPath)
    {
        if (!m_initialized)
        {
            Vantor::Backlog::Log("VResourceManager", "Resource Manager not initialized", Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(m_resourceMutex);

        // Check if already loaded
        auto it = m_resources.find(handle);
        if (it != m_resources.end())
        {
            auto shader = std::dynamic_pointer_cast<VShaderProgramResource>(it->second);
            if (shader)
            {
                return shader;
            }
        }

        if (!FileExists(vertexPath))
        {
            Vantor::Backlog::Log("VResourceManager", "Vertex shader file not found: " + vertexPath, Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }

        if (!FileExists(fragmentPath))
        {
            Vantor::Backlog::Log("VResourceManager", "Fragment shader file not found: " + fragmentPath, Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }

        try
        {
            // Read shader source files
            std::string vertexSource   = Platform::Environment::ReadText(vertexPath);
            std::string fragmentSource = Platform::Environment::ReadText(fragmentPath);

            if (vertexSource.empty())
            {
                Vantor::Backlog::Log("VResourceManager", "Failed to read vertex shader: " + vertexPath, Vantor::Backlog::LogLevel::ERR);
                return nullptr;
            }

            if (fragmentSource.empty())
            {
                Vantor::Backlog::Log("VResourceManager", "Failed to read fragment shader: " + fragmentPath, Vantor::Backlog::LogLevel::ERR);
                return nullptr;
            }

            // Create shader using render device factory
            auto renderShader = Vantor::RenderDevice::CreateShaderInstance(vertexSource.c_str(), fragmentSource.c_str());
            if (!renderShader)
            {
                Vantor::Backlog::Log("VResourceManager", "Failed to create shader object", Vantor::Backlog::LogLevel::ERR);
                return nullptr;
            }

            // Create resource wrapper
            auto resource = std::make_shared<VShaderProgramResource>();
            resource->SetHandle(handle);
            resource->SetShader(renderShader);
            resource->SetPaths(vertexPath, fragmentPath);
            resource->SetMemoryUsage(vertexSource.size() + fragmentSource.size() + sizeof(VShaderProgramResource));

            // Store resource
            m_resources[handle] = resource;

            Vantor::Backlog::Log("VResourceManager", "Shader program loaded: " + handle + " (" + vertexPath + " + " + fragmentPath + ")",
                                 Vantor::Backlog::LogLevel::INFO);

            return resource;
        }
        catch (const std::exception &e)
        {
            Vantor::Backlog::Log("VResourceManager", "Exception loading shader program: " + std::string(e.what()), Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }
    }

    std::shared_ptr<VResource> VResourceManager::GetResource(const VResourceHandle &handle)
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);

        auto it = m_resources.find(handle);
        if (it != m_resources.end())
        {
            return it->second;
        }

        return nullptr;
    }

    std::shared_ptr<VTexture2DResource> VResourceManager::GetTexture2D(const VResourceHandle &handle)
    {
        auto resource = GetResource(handle);
        return std::dynamic_pointer_cast<VTexture2DResource>(resource);
    }

    std::shared_ptr<VShaderProgramResource> VResourceManager::GetShaderProgram(const VResourceHandle &handle)
    {
        auto resource = GetResource(handle);
        return std::dynamic_pointer_cast<VShaderProgramResource>(resource);
    }

    bool VResourceManager::UnloadResource(const VResourceHandle &handle)
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);

        auto it = m_resources.find(handle);
        if (it == m_resources.end())
        {
            return false;
        }

        it->second->Unload();
        m_resources.erase(it);

        Vantor::Backlog::Log("VResourceManager", "Resource unloaded: " + handle, Vantor::Backlog::LogLevel::INFO);

        return true;
    }

    void VResourceManager::UnloadAllResources()
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);

        for (auto &pair : m_resources)
        {
            pair.second->Unload();
        }

        size_t count = m_resources.size();
        m_resources.clear();

        if (count > 0)
        {
            Vantor::Backlog::Log("VResourceManager", "Unloaded " + std::to_string(count) + " resources", Vantor::Backlog::LogLevel::INFO);
        }
    }

    bool VResourceManager::ReloadResource(const VResourceHandle &handle)
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);

        auto it = m_resources.find(handle);
        if (it == m_resources.end())
        {
            return false;
        }

        auto resource = it->second;

        // TODO: Reloading Here

        Vantor::Backlog::Log("VResourceManager", "Resource reload requested: " + handle, Vantor::Backlog::LogLevel::INFO);

        return true;
    }

    size_t VResourceManager::GetResourceCount() const
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);
        return m_resources.size();
    }

    size_t VResourceManager::GetTotalMemoryUsage() const
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);

        size_t totalMemory = 0;
        for (const auto &pair : m_resources)
        {
            totalMemory += pair.second->GetMemoryUsage();
        }

        return totalMemory;
    }

    std::vector<VResourceHandle> VResourceManager::GetResourceHandles() const
    {
        std::lock_guard<std::mutex> lock(m_resourceMutex);

        std::vector<VResourceHandle> handles;
        handles.reserve(m_resources.size());

        for (const auto &pair : m_resources)
        {
            handles.push_back(pair.first);
        }

        return handles;
    }

    std::string VResourceManager::GetFileExtension(const std::string &filePath) const
    {
        size_t dotPos = filePath.find_last_of('.');
        if (dotPos != std::string::npos && dotPos < filePath.length() - 1)
        {
            return filePath.substr(dotPos);
        }

        return "";
    }

    bool VResourceManager::FileExists(const std::string &path) const { return Platform::Environment::FileExists(path); }

} // namespace Vantor::Resource