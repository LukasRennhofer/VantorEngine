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
 * File: vantorResourceManager.cpp
 * Last Change: Added Version settings and vantor::core::version child namespace
*/

#include "vantorResourceManager.hpp"
#include <mutex>

namespace vantor::ResourceManager {

    struct ResourceEntry {
        std::shared_ptr<void> resource;
        ResourceType type;
    };

    static std::unordered_map<std::string, ResourceEntry> resourceMap;
    static std::unordered_map<ResourceType, std::unique_ptr<IResourceLoader>> loaders;
    static std::mutex resourceMutex;

    void Initialize() {
        // Register all default loaders here
        RegisterLoader(ResourceType::SHADER, std::make_unique<class ShaderLoader>());
        RegisterLoader(ResourceType::TEXTURE, std::make_unique<class TextureLoader>());
        // TODO: Add more: TextureLoader, AudioLoader, etc.
    }

    void Clear() {
        std::lock_guard lock(resourceMutex);
        resourceMap.clear();
    }

    void Reload(const std::string& path) {
        std::lock_guard lock(resourceMutex);
        auto it = resourceMap.find(path);
        if (it != resourceMap.end()) {
            auto type = it->second.type;
            auto& loader = loaders[type];
            if (loader) {
                it->second.resource = loader->LoadRaw(path);
            }
        }
    }

    void Unload(const std::string& path) {
        std::lock_guard lock(resourceMutex);
        resourceMap.erase(path);
    }

    void UnloadUnused() {
        std::lock_guard lock(resourceMutex);
        for (auto it = resourceMap.begin(); it != resourceMap.end(); ) {
            if (it->second.resource.use_count() == 1) {
                it = resourceMap.erase(it);
            } else {
                ++it;
            }
        }
    }

    void RegisterLoader(ResourceType type, std::unique_ptr<IResourceLoader>&& loader) {
        loaders[type] = std::move(loader);
    }

    // Generic load function for other types
    template<typename T>
    std::shared_ptr<T> Load(const std::string& path) {
        std::lock_guard lock(resourceMutex);
        auto it = resourceMap.find(path);
        if (it != resourceMap.end()) {
            return std::static_pointer_cast<T>(it->second.resource);
        }

        // Find the appropriate loader for the resource
        for (auto& [type, loader] : loaders) {
            auto ptr = loader->LoadRaw(path);
            if (ptr) {
                resourceMap[path] = { ptr, type };
                return std::static_pointer_cast<T>(ptr);
            }
        }

        return nullptr;
    }

    // Specialization for Shaders
    std::shared_ptr<vantor::Graphics::RenderDevice::Shader>
    LoadShader(const std::string& name,
               const std::string& vsPath,
               const std::string& fsPath,
               const std::vector<std::string>& defines) {
    
        std::lock_guard lock(resourceMutex);
    
        auto it = resourceMap.find(name);
        if (it != resourceMap.end()) {
            return std::static_pointer_cast<vantor::Graphics::RenderDevice::Shader>(it->second.resource);
        }
    
        for (auto& [type, loader] : loaders) {
            auto shaderLoader = dynamic_cast<ShaderLoader*>(loader.get());
            if (shaderLoader) {
                auto shader = shaderLoader->LoadRaw(name, vsPath, fsPath, defines);
                if (shader) {
                    resourceMap[name] = { shader, type };
                    return std::static_pointer_cast<vantor::Graphics::RenderDevice::Shader>(shader);
                }
            }
        }
    
        return nullptr;
    }
    

} // namespace vantor::ResourceManager
