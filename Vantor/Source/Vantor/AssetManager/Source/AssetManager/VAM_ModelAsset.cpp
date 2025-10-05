/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <AssetManager/Public/VAM_ModelAsset.hpp>
#include <RHI/Interface/VRHI_Device.hpp>

#include <iostream>
#include <filesystem>
#include <algorithm>

namespace VE::Asset {

    VModelAsset::VModelAsset(const std::string& path)
        : VBaseAsset(path, EAssetType::Model), m_Model(nullptr) {
        
        if (!IsModelFile(path)) {
            std::cerr << "VModelAsset::VModelAsset() - Invalid model file extension: " << path << std::endl;
            SetState(EAssetState::Failed);
        }
    }

    VModelAsset::~VModelAsset() {
        Unload();
    }

    bool VModelAsset::Load() {
        if (GetState() == EAssetState::Loaded) {
            return true; // Already loaded
        }

        if (GetState() == EAssetState::Failed) {
            return false; // Previously failed to load
        }

        SetState(EAssetState::Loading);

        try {
            // Create the model
            m_Model = std::make_shared<VE::Graphics::VModel>();
            
            // Load the model from file
            if (!m_Model->LoadFromFile(GetPath())) {
                std::cerr << "VModelAsset::Load() - Failed to load model from file: " << GetPath() << std::endl;
                m_Model.reset();
                SetState(EAssetState::Failed);
                return false;
            }

            SetState(EAssetState::Loaded);
            std::cout << "VModelAsset::Load() - Successfully loaded model: " << GetPath() << std::endl;
            return true;

        } catch (const std::exception& e) {
            std::cerr << "VModelAsset::Load() - Exception loading model " << GetPath() 
                      << ": " << e.what() << std::endl;
            m_Model.reset();
            SetState(EAssetState::Failed);
            return false;
        }
    }

    void VModelAsset::Unload() {
        if (GetState() == EAssetState::Unloaded) {
            return; // Already unloaded
        }

        // Destroy GPU resources first
        DestroyGPUResources();

        // Release the model
        if (m_Model) {
            m_Model.reset();
            std::cout << "VModelAsset::Unload() - Unloaded model: " << GetPath() << std::endl;
        }

        SetState(EAssetState::Unloaded);
    }

    bool VModelAsset::IsValid() const {
        return GetState() == EAssetState::Loaded && m_Model && m_Model->IsLoaded();
    }

    bool VModelAsset::CreateGPUResources(VE::Internal::RHI::IRHIDevice* device) {
        if (!IsValid()) {
            std::cerr << "VModelAsset::CreateGPUResources() - Model is not valid" << std::endl;
            return false;
        }

        if (m_HasGPUResources) {
            return true; // Already created
        }

        if (!device) {
            std::cerr << "VModelAsset::CreateGPUResources() - Device is null" << std::endl;
            return false;
        }

        bool success = m_Model->CreateGPUResources(device);
        if (success) {
            m_HasGPUResources = true;
            std::cout << "VModelAsset::CreateGPUResources() - Created GPU resources for model: " 
                      << GetPath() << std::endl;
        } else {
            std::cerr << "VModelAsset::CreateGPUResources() - Failed to create GPU resources for model: " 
                      << GetPath() << std::endl;
        }

        return success;
    }

    void VModelAsset::DestroyGPUResources() {
        if (!m_HasGPUResources) {
            return; // No GPU resources to destroy
        }

        if (m_Model) {
            m_Model->DestroyGPUResources();
        }

        m_HasGPUResources = false;
        std::cout << "VModelAsset::DestroyGPUResources() - Destroyed GPU resources for model: " 
                  << GetPath() << std::endl;
    }

    uint32_t VModelAsset::GetMeshCount() const {
        if (!IsValid()) {
            return 0;
        }
        return m_Model->GetMeshCount();
    }

    const std::string& VModelAsset::GetModelPath() const {
        if (!IsValid()) {
            static const std::string empty;
            return empty;
        }
        return m_Model->GetPath();
    }

    bool VModelAsset::IsModelFile(const std::string& path) const {
        std::filesystem::path fsPath(path);
        std::string extension = fsPath.extension().string();
        
        // Convert to lowercase
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        // Common 3D model formats supported by Assimp
        const std::vector<std::string> supportedExtensions = {
            ".obj",     // Wavefront OBJ
            ".fbx",     // Autodesk FBX
            ".dae",     // Collada
            ".gltf",    // glTF 2.0
            ".glb",     // glTF Binary
            ".blend",   // Blender
            ".3ds",     // 3D Studio Max
            ".ase",     // ASCII Scene Exporter
            ".ifc",     // Industry Foundation Classes
            ".xgl",     // XML Geometry Language
            ".ply",     // Stanford PLY
            ".dxf",     // AutoCAD DXF
            ".lwo",     // Lightwave Object
            ".lws",     // Lightwave Scene
            ".lxo",     // Luxology Modo
            ".stl",     // Stereolithography
            ".x",       // DirectX X
            ".ac",      // AC3D
            ".ms3d",    // Milkshape 3D
            ".cob",     // Caligari TrueSpace
            ".scn",     // Scenestream
            ".mesh",    // Generic Mesh
            ".xml",     // Irrlicht Mesh
            ".irrmesh", // Irrlicht Mesh
            ".irr",     // Irrlicht Scene
            ".mdl",     // Quake Model
            ".md2",     // Quake II Model
            ".md3",     // Quake III Model
            ".pk3",     // Quake III Map
            ".mdc",     // Return to Castle Wolfenstein
            ".md5mesh", // Doom 3 Model
            ".md5anim", // Doom 3 Animation
            ".q3o",     // Quick3D Object
            ".q3s",     // Quick3D Scene
            ".raw",     // Raw Triangles
            ".smd",     // Valve Studiomodel Data
            ".vta",     // Valve Triangle Animation
            ".m",       // Lightwave Motion
            ".3d",      // 3D GameStudio
            ".b3d",     // Blitz3D
            ".csm",     // CharacterStudio Motion
            ".ter",     // Terragen Terrain
            ".hmp"      // 3D GameStudio Heightmap
        };

        return std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) 
               != supportedExtensions.end();
    }

}
