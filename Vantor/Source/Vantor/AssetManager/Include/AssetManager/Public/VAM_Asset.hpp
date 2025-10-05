/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Core/Container/VCO_SafeString.hpp>
#include <memory>
#include <string>
#include <cstdint>

namespace VE::Asset {
    
    enum class EAssetType {
        Unknown = 0,
        Texture,
        Model,
        Audio,
        Text,
        // Material
    };

    enum class EAssetState {
        Unloaded = 0,
        Loading,
        Loaded,
        Failed
    };

    class VBaseAsset {
    public:
        VBaseAsset(const std::string& path, EAssetType type);
        virtual ~VBaseAsset() = default;

        // Pure virtual methods that derived classes must implement
        virtual bool Load() = 0;
        virtual void Unload() = 0;
        virtual bool IsValid() const = 0;

        // Getters
        const std::string& GetPath() const { return m_Path; }
        EAssetType GetType() const { return m_Type; }
        EAssetState GetState() const { return m_State; }
        uint32_t GetRefCount() const { return m_RefCount; }

        // Reference counting
        void AddRef() { m_RefCount++; }
        void RemoveRef() { 
            if (m_RefCount > 0) m_RefCount--; 
        }

    protected:
        void SetState(EAssetState state) { m_State = state; }

    private:
        std::string m_Path;
        EAssetType m_Type;
        EAssetState m_State;
        uint32_t m_RefCount;
    };

    // Smart pointer type for assets
    using AssetPtr = std::shared_ptr<VBaseAsset>;

    template<typename T>
    using TypedAssetPtr = std::shared_ptr<T>;

}