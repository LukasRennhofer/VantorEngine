/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifResourceManager.hpp
 * Last Change: Added Version settings and chif::core::version child namespace
*/

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <type_traits>

#include "../../Helpers/chifFS.hpp"
#include "../../Graphics/RenderDevice/chifRenderDevice.hpp"

namespace chif::ResourceManager {

    enum class ResourceType {
        TEXTURE,
        SHADER,
        MATERIAL,
        AUDIO,
        VIDEO,
        MODEL,
        UNIDENTIFIED = 0xFFF
    };

    void Initialize();
    void Clear();
    void Reload(const std::string& path);
    void Unload(const std::string& path);
    void UnloadUnused();

    // Generic resource loader
    template<typename T>
    std::shared_ptr<T> Load(const std::string& path);

    // Internal registry API (not for direct use unless needed)
    struct IResourceLoader {
        virtual ~IResourceLoader() = default;
        virtual std::shared_ptr<void> LoadRaw(const std::string& path) = 0;
    };

    // ============ LOADERS =============

    class ShaderLoader : public chif::ResourceManager::IResourceLoader {
        public:

            // Base version
            std::shared_ptr<void> LoadRaw(const std::string& path) override {
                // Default shader loading logic, if necessary
                return nullptr;
            }

            std::shared_ptr<void> LoadRaw(
                const std::string& name, 
                const std::string& vsPath, 
                const std::string& fsPath, 
                const std::vector<std::string>& defines) {
                
                using Shader = chif::Graphics::RenderDevice::Shader;
        
                // Load shader source code from files
                auto vsdata = chif::Helpers::FileSystem::ReadText(vsPath);
                auto fsdata = chif::Helpers::FileSystem::ReadText(fsPath);
                
                // If either shader file is empty, return null
                if (vsdata.empty() || fsdata.empty()) return nullptr;
        
                // Create shader object and return as std::shared_ptr<void>
                auto shader = std::make_shared<Shader>(name, vsdata, fsdata, defines);
                return std::static_pointer_cast<void>(shader);  // Cast to std::shared_ptr<void>
            }
    };

    class TextureLoader : public chif::ResourceManager::IResourceLoader {
        public:
            std::shared_ptr<void> LoadRaw(const std::string& path) override {
                using Texture = chif::Graphics::RenderDevice::Texture;
                auto data = chif::Helpers::FileSystem::ReadText(path);
                if (data.empty()) return nullptr;
        
                auto texture = std::make_shared<Texture>();
                return texture;
            }
    };

    void RegisterLoader(ResourceType type, std::unique_ptr<IResourceLoader>&& loader);

    // Specialization for Shaders
    std::shared_ptr<chif::Graphics::RenderDevice::Shader>
    LoadShader(const std::string& name,
               const std::string& vsPath,
               const std::string& fsPath,
               const std::vector<std::string>& defines = std::vector<std::string>());

} // namespace chif::ResourceManager