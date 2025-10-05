/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <AssetManager/Public/VAM_Asset.hpp>
#include <AssetManager/Public/VAM_TextureAsset.hpp>
#include <AssetManager/Public/VAM_ModelAsset.hpp>
#include <AssetManager/Public/VAM_TextAsset.hpp>

#include <RHI/Interface/VRHI_Device.hpp>

#include <unordered_map>
#include <string>
#include <memory>

namespace VE::Internal::AssetManager {

    class VAssetManager {
    public:
        VAssetManager();
        ~VAssetManager();

        // Initialize
        void Initialize();
        void Shutdown();

        // Generic asset loading
        template<typename T>
        std::shared_ptr<T> LoadAsset(const std::string& path);

        // Specific asset loading methods
        VE::Asset::TextureAssetPtr LoadTexture(const std::string& path);
        VE::Asset::ModelAssetPtr LoadModel(const std::string& path);
        VE::Asset::TextAssetPtr LoadText(const std::string& path);

        // Asset management
        void UnloadAsset(const std::string& path);
        void UnloadAllAssets();
        bool IsAssetLoaded(const std::string& path) const;

        // Asset caching
        void SetCacheSize(size_t maxAssets) { m_MaxCachedAssets = maxAssets; }
        void ClearCache();

        // Statistics
        size_t GetLoadedAssetCount() const { return m_LoadedAssets.size(); }
        size_t GetMemoryUsage() const; // TODO: Implement memory tracking

    private:
        std::unordered_map<std::string, VE::Asset::AssetPtr> m_LoadedAssets;
        size_t m_MaxCachedAssets;

        // Helper methods
        std::string NormalizePath(const std::string& path) const;
        void EvictOldestAsset();
        VE::Asset::EAssetType GetAssetTypeFromPath(const std::string& path) const;
    };
}
