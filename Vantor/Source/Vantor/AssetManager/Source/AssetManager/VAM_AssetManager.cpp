/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™ 
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <AssetManager/Manager/VAM_AssetManager.hpp>

#include <AssetManager/Public/VAM_TextureAsset.hpp>
#include <AssetManager/Public/VAM_ModelAsset.hpp>

#include <iostream>
#include <filesystem>
#include <algorithm>

namespace VE::Internal::AssetManager {

    // Global instance
    static VAssetManager* g_AssetManager = nullptr;

    VAssetManager::VAssetManager() : m_MaxCachedAssets(100) // Default cache size
    {
    }

    VAssetManager::~VAssetManager()
    {
        Shutdown();
    }

    void VAssetManager::Initialize()
    {
        std::cout << "VAssetManager::Initialize() - Asset manager initialized" << std::endl;
    }

    void VAssetManager::Shutdown()
    {
        UnloadAllAssets();
        std::cout << "VAssetManager::Shutdown() - Asset manager shut down" << std::endl;
    }

    template<typename T>
    std::shared_ptr<T> VAssetManager::LoadAsset(const std::string& path)
    {
        static_assert(std::is_base_of_v<VE::Asset::VBaseAsset, T>, "T must derive from VBaseAsset");
        
        std::string normalizedPath = NormalizePath(path);
        
        // Check if already loaded
        auto it = m_LoadedAssets.find(normalizedPath);
        if (it != m_LoadedAssets.end())
        {
            auto typedAsset = std::dynamic_pointer_cast<T>(it->second);
            if (typedAsset)
            {
                typedAsset->AddRef();
                return typedAsset;
            }
        }

        // Create new asset
        auto asset = std::make_shared<T>(normalizedPath);
        
        // Load the asset
        if (!asset->Load())
        {
            std::cerr << "VAssetManager::LoadAsset() - Failed to load asset: " << normalizedPath << std::endl;
            return nullptr;
        }

        // Store in cache
        m_LoadedAssets[normalizedPath] = asset;
        asset->AddRef();

        // Handle cache eviction if needed
        if (m_LoadedAssets.size() > m_MaxCachedAssets)
        {
            EvictOldestAsset();
        }

        return asset;
    }


    VE::Asset::TextureAssetPtr VAssetManager::LoadTexture(const std::string& path)
    {
        auto textureAsset = LoadAsset<VE::Asset::VTextureAsset>(path);
        
        return textureAsset;
    }

    VE::Asset::ModelAssetPtr VAssetManager::LoadModel(const std::string& path)
    {
        auto modelAsset = LoadAsset<VE::Asset::VModelAsset>(path);
        
        return modelAsset;
    }

    VE::Asset::TextAssetPtr VAssetManager::LoadText(const std::string& path)
    {
        auto textAsset = LoadAsset<VE::Asset::VTextAsset>(path);

        return textAsset;
    }

    void VAssetManager::UnloadAsset(const std::string& path)
    {
        std::string normalizedPath = NormalizePath(path);
        
        auto it = m_LoadedAssets.find(normalizedPath);
        if (it != m_LoadedAssets.end())
        {
            it->second->RemoveRef();
            
            // Remove from cache if no references
            if (it->second->GetRefCount() == 0)
            {
                it->second->Unload();
                m_LoadedAssets.erase(it);
                std::cout << "VAssetManager::UnloadAsset() - Unloaded asset: " << normalizedPath << std::endl;
            }
        }
    }

    void VAssetManager::UnloadAllAssets()
    {
        for (auto& [path, asset] : m_LoadedAssets)
        {
            asset->Unload();
        }
        m_LoadedAssets.clear();
        std::cout << "VAssetManager::UnloadAllAssets() - All assets unloaded" << std::endl;
    }

    bool VAssetManager::IsAssetLoaded(const std::string& path) const
    {
        std::string normalizedPath = NormalizePath(path);
        auto it = m_LoadedAssets.find(normalizedPath);
        return it != m_LoadedAssets.end() && it->second->GetState() == VE::Asset::EAssetState::Loaded;
    }

    void VAssetManager::ClearCache()
    {
        // Remove assets with zero references
        for (auto it = m_LoadedAssets.begin(); it != m_LoadedAssets.end();)
        {
            if (it->second->GetRefCount() == 0)
            {
                it->second->Unload();
                it = m_LoadedAssets.erase(it);
            }
            else
            {
                ++it;
            }
        }
        std::cout << "VAssetManager::ClearCache() - Cache cleared" << std::endl;
    }

    size_t VAssetManager::GetMemoryUsage() const
    {
        // TODO: Implement proper memory tracking
        return m_LoadedAssets.size() * sizeof(VE::Asset::AssetPtr);
    }

    std::string VAssetManager::NormalizePath(const std::string& path) const
    {
        // Convert to absolute path and normalize separators
        std::filesystem::path fsPath(path);
        std::string normalized = std::filesystem::absolute(fsPath).generic_string();
        
        // Note: Removed lowercase conversion to preserve case-sensitive filesystem paths on Linux
        
        return normalized;
    }

    void VAssetManager::EvictOldestAsset()
    {
        // Simple eviction: remove first asset with zero references
        for (auto it = m_LoadedAssets.begin(); it != m_LoadedAssets.end(); ++it)
        {
            if (it->second->GetRefCount() == 0)
            {
                std::cout << "VAssetManager::EvictOldestAsset() - Evicting: " << it->first << std::endl;
                it->second->Unload();
                m_LoadedAssets.erase(it);
                return;
            }
        }
        
        // If no unreferenced assets, remove the first one anyway
        if (!m_LoadedAssets.empty())
        {
            auto it = m_LoadedAssets.begin();
            std::cout << "VAssetManager::EvictOldestAsset() - Force evicting: " << it->first << std::endl;
            it->second->Unload();
            m_LoadedAssets.erase(it);
        }
    }

    VE::Asset::EAssetType VAssetManager::GetAssetTypeFromPath(const std::string& path) const
    {
        std::filesystem::path fsPath(path);
        std::string extension = fsPath.extension().string();
        
        // Convert to lowercase
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        // Image formats
        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || 
            extension == ".bmp" || extension == ".tga" || extension == ".hdr" || 
            extension == ".exr")
        {
            return VE::Asset::EAssetType::Texture;
        }
        
        // Model formats (common ones supported by Assimp)
        if (extension == ".obj" || extension == ".fbx" || extension == ".dae" || 
            extension == ".gltf" || extension == ".glb" || extension == ".blend" || 
            extension == ".3ds" || extension == ".ply" || extension == ".stl" ||
            extension == ".x" || extension == ".md2" || extension == ".md3" ||
            extension == ".md5mesh" || extension == ".ase" || extension == ".lwo")
        {
            return VE::Asset::EAssetType::Model;
        }
        
        // Add more asset types as needed
        // TODO: Add model formats (.obj, .gltf, .fbx), audio formats, etc.
        
        return VE::Asset::EAssetType::Unknown;
    }

    // Global accessor
    VAssetManager* GetAssetManager()
    {
        if (!g_AssetManager)
        {
            g_AssetManager = new VAssetManager();
        }
        return g_AssetManager;
    }

}
