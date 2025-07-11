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
 *  Date: 2025-07-10
 *
 *  File: VRES_Manager.hpp
 *  Last Change: Created new simplified Resource Manager
 */

#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

// Platform utilities
#include "../Platform/VPL_Environment.hpp"

// Core
#include "../Core/BackLog/VCO_Backlog.hpp"
#include "../Core/Types/VCO_BTypes.hpp" // VSingleton

// Interfaces
#include "../RenderDevice/Interface/VRD_Texture.hpp"
#include "../RenderDevice/Interface/VRD_Shader.hpp"

// Forward declarations for render device
namespace Vantor::RenderDevice
{
}

namespace Vantor::Resource
{
    // Resource handle type
    using VResourceHandle = std::string;
    
    // Resource type categories
    enum class VEResourceType
    {
        Unknown,
        Texture2D,
        ShaderProgram,
        Model,
        Audio
    };
    
    // Base resource class
    class VResource
    {
        public:
            VResource() = default;
            virtual ~VResource() = default;
            
            virtual VEResourceType GetType() const = 0;
            virtual size_t GetMemoryUsage() const = 0;
            virtual void Unload() = 0;
            
            const VResourceHandle& GetHandle() const { return m_handle; }
            void SetHandle(const VResourceHandle& handle) { m_handle = handle; }
            
        protected:
            VResourceHandle m_handle;
    };
    
    // Texture2D resource
    class VTexture2DResource : public VResource
    {
        public:
            VTexture2DResource() = default;
            ~VTexture2DResource() override = default;
            
            VEResourceType GetType() const override { return VEResourceType::Texture2D; }
            size_t GetMemoryUsage() const override { return m_memoryUsage; }
            void Unload() override;
            
            // Texture-specific data
            std::shared_ptr<Vantor::RenderDevice::VTexture> GetTexture() const { return m_texture; }
            void SetTexture(std::shared_ptr<Vantor::RenderDevice::VTexture> texture) { m_texture = texture; }
            
            int GetWidth() const { return m_width; }
            int GetHeight() const { return m_height; }
            
            void SetMemoryUsage(size_t usage) { m_memoryUsage = usage; }
            
        private:
            std::shared_ptr<Vantor::RenderDevice::VTexture> m_texture;
            int m_width = 0;
            int m_height = 0;
            size_t m_memoryUsage = 0;
    };
    
    // Shader program resource
    class VShaderProgramResource : public VResource
    {
        public:
            VShaderProgramResource() = default;
            ~VShaderProgramResource() override = default;
            
            VEResourceType GetType() const override { return VEResourceType::ShaderProgram; }
            size_t GetMemoryUsage() const override { return m_memoryUsage; }
            void Unload() override;
            
            // Shader-specific data
            std::shared_ptr<Vantor::RenderDevice::VShader> GetShader() const { return m_shader; }
            void SetShader(std::shared_ptr<Vantor::RenderDevice::VShader> shader) { m_shader = shader; }
            
            const std::string& GetVertexPath() const { return m_vertexPath; }
            const std::string& GetFragmentPath() const { return m_fragmentPath; }
            void SetPaths(const std::string& vertexPath, const std::string& fragmentPath) 
            { 
                m_vertexPath = vertexPath; 
                m_fragmentPath = fragmentPath; 
            }
            
            void SetMemoryUsage(size_t usage) { m_memoryUsage = usage; }
            
        private:
            std::shared_ptr<Vantor::RenderDevice::VShader> m_shader;
            std::string m_vertexPath;
            std::string m_fragmentPath;
            size_t m_memoryUsage = 0;
    };
    
    // Main Resource Manager (Singleton)
    class VResourceManager : public Vantor::Core::Types::VSingleton<VResourceManager>
    {
        friend class Vantor::Core::Types::VSingleton<VResourceManager>;
        
        public:
            ~VResourceManager() override;
            
            // Initialization
            bool Initialize();
            void Shutdown();
            bool IsInitialized() const { return m_initialized; }
            
            // Resource loading functions
            std::shared_ptr<VTexture2DResource> LoadTexture2D(const VResourceHandle& handle, 
                                                              const std::string& filePath, 
                                                                const Vantor::RenderDevice::VTextureSampler& sampler = {},
                                                                bool generateMipmaps = true);
            
            std::shared_ptr<VShaderProgramResource> LoadShaderProgram(const VResourceHandle& handle,
                                                                     const std::string& vertexPath,
                                                                     const std::string& fragmentPath);
            
            // Preload functions to avoid returning pointers
            void PreloadTexture2D(const VResourceHandle& handle, 
                                const std::string& filePath, 
                                const Vantor::RenderDevice::VTextureSampler& sampler = {},
                                bool generateMipmaps = true)
            {
                auto texture = LoadTexture2D(handle, filePath, sampler, generateMipmaps);
            }

            void PreloadShaderProgram(const VResourceHandle& handle,
                                    const std::string& vertexPath,
                                    const std::string& fragmentPath)
            {
                auto shader = LoadShaderProgram(handle, vertexPath, fragmentPath);
            }

            
            // Resource access
            std::shared_ptr<VResource> GetResource(const VResourceHandle& handle);
            std::shared_ptr<VTexture2DResource> GetTexture2D(const VResourceHandle& handle);
            std::shared_ptr<VShaderProgramResource> GetShaderProgram(const VResourceHandle& handle);
            
            // Resource management
            bool UnloadResource(const VResourceHandle& handle);
            void UnloadAllResources();
            bool ReloadResource(const VResourceHandle& handle);
            
            // Statistics
            size_t GetResourceCount() const;
            size_t GetTotalMemoryUsage() const;
            std::vector<VResourceHandle> GetResourceHandles() const;
            
        private:
            VResourceManager() = default;
            
            // Internal helpers
            std::string GetFileExtension(const std::string& filePath) const;
            bool FileExists(const std::string& path) const;
            
            // Thread safety
            mutable std::mutex m_resourceMutex;
            
            // Resource storage
            std::unordered_map<VResourceHandle, std::shared_ptr<VResource>> m_resources;
            
            // Path management
            std::unordered_map<std::string, std::string> m_aliases;
            
            bool m_initialized = false;
    };
    
} // namespace Vantor::Resource
