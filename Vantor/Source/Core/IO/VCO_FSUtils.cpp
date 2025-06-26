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
 *  Date: 2025-06-26
 *
 *  File: VCO_FSUtils.cpp
 *  Last Change: Automatically updated
 */

#include "VCO_FSUtils.hpp"

namespace Vantor::Core::IO
{
    // Split Path in dir and fileName and directly modify them
    void SplitPath(const std::string &fullPath, std::string &dir, std::string &fileName)
    {
        size_t found;
        found    = fullPath.find_last_of("/\\");
        dir      = fullPath.substr(0, found + 1);
        fileName = fullPath.substr(found + 1);
    }

    // Only get filename from fiven Path
    std::string GetFileNameFromPath(const std::string &fullPath)
    {
        if (fullPath.empty())
        {
            return fullPath;
        }

        std::string ret, empty;
        SplitPath(fullPath, empty, ret);
        return ret;
    }

    // Get directory from given Path
    std::string GetDirectoryFromPath(const std::string &fullPath)
    {
        if (fullPath.empty())
        {
            return fullPath;
        }

        std::string ret, empty;
        SplitPath(fullPath, ret, empty);
        return ret;
    }

    // Get extension from given filename
    std::string GetExtensionFromFileName(const std::string &filename)
    {
        size_t idx = filename.rfind('.');

        if (idx != std::string::npos)
        {
            std::string extension = filename.substr(idx + 1);
            return extension;
        }

        return "";
    }

    // Replace extension from given filename
    std::string ReplaceExtension(const std::string &filename, const std::string &extension)
    {
        size_t idx = filename.rfind('.');

        if (idx == std::string::npos)
        {
            return filename + "." + extension;
        }
        return filename.substr(0, idx + 1) + extension;
    }

    // Force Extension to filename
    std::string ForceExtension(const std::string &filename, const std::string &extension)
    {
        std::string ext = "." + extension;
        if (ext.length() > filename.length()) return filename + ext;

        if (filename.substr(filename.length() - ext.length()).compare(ext))
        {
            return filename + ext;
        }
        return filename;
    }

    // Remove extension from given filename
    std::string RemoveExtension(const std::string &filename)
    {
        size_t idx = filename.rfind('.');

        if (idx == std::string::npos)
        {
            return filename;
        }
        return filename.substr(0, idx);
    }
} // namespace Vantor::Core::IO