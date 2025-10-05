/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <AssetManager/Public/VAM_TextAsset.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

#include <vector>

namespace VE::Asset {

    VTextAsset::VTextAsset(const std::string& path)
        : VBaseAsset(path, EAssetType::Text)
    {
    }

    VTextAsset::~VTextAsset()
    {
        Unload();
    }

    bool VTextAsset::Load()
    {
        SetState(EAssetState::Loading);

        // Check if file exists
        if (!std::filesystem::exists(GetPath()))
        {
            std::cerr << "VTextAsset::Load() - File does not exist: " << GetPath() << std::endl;
            SetState(EAssetState::Failed);
            return false;
        }

        // Check if it's a text file
        if (!IsTextFile(GetPath()))
        {
            std::cerr << "VTextAsset::Load() - File is not a recognized text format: " << GetPath() << std::endl;
            SetState(EAssetState::Failed);
            return false;
        }

        try
        {
            // Open file
            std::ifstream file(GetPath(), std::ios::in);
            if (!file.is_open())
            {
                std::cerr << "VTextAsset::Load() - Failed to open file: " << GetPath() << std::endl;
                SetState(EAssetState::Failed);
                return false;
            }

            // Read entire file content
            std::stringstream buffer;
            buffer << file.rdbuf();
            m_Text = buffer.str();

            file.close();

            std::cout << "VTextAsset::Load() - Successfully loaded text file: " << GetPath() 
                      << " (" << m_Text.length() << " characters)" << std::endl;

            SetState(EAssetState::Loaded);
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "VTextAsset::Load() - Exception while loading file: " << GetPath() 
                      << " - " << e.what() << std::endl;
            SetState(EAssetState::Failed);
            return false;
        }
    }

    void VTextAsset::Unload()
    {
        if (GetState() == EAssetState::Loaded)
        {
            m_Text.clear();
            SetState(EAssetState::Unloaded);
            std::cout << "VTextAsset::Unload() - Text asset unloaded: " << GetPath() << std::endl;
        }
    }

    bool VTextAsset::IsValid() const
    {
        return GetState() == EAssetState::Loaded && !m_Text.empty();
    }

    bool VTextAsset::IsTextFile(const std::string& path) const
    {
        std::filesystem::path fsPath(path);
        std::string extension = fsPath.extension().string();
        
        // Convert to lowercase for comparison
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        // Common text file extensions
        static const std::vector<std::string> textExtensions = {
            ".txt", ".text",
            ".md", ".markdown",
            ".json", ".xml", ".yaml", ".yml",
            ".ini", ".cfg", ".config",
            ".log",
            ".py",
            ".glsl", ".vglsl", ".vglsl", ".geom", ".comp",
            ".hlsl", ".fx",
            ".lua",
            ".sh", ".bat", ".cmd"
        };
        
        // Check if extension matches any known text format
        for (const auto& textExt : textExtensions)
        {
            if (extension == textExt)
            {
                return true;
            }
        }
        
        // Special case: files without extensions might be text files
        if (extension.empty())
        {
            // Check some common filenames without extensions
            std::string filename = fsPath.filename().string();
            std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
            
            static const std::vector<std::string> textFilenames = {
                "readme", "license", "changelog", "makefile", "dockerfile", "cmakelists"
            };
            
            for (const auto& textFilename : textFilenames)
            {
                if (filename == textFilename)
                {
                    return true;
                }
            }
        }
        
        return false;
    }

}
