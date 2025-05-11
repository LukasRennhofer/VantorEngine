/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-11
 *
 *  File: vantorFS.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "../Platform/Platforms/vantorPlatformEnvironment.hpp"

#if defined(__SWITCH__)
#include <switch.h>
#else
#include <filesystem>
#endif

namespace vantor::Helpers::FileSystem
{
    // Base functions
    inline std::vector<uint8_t> ReadBinary(const std::string &path)
    {
        std::vector<uint8_t> buffer;

#if defined(__SWITCH__)
        FILE *file = fopen(path.c_str(), "rb");
        if (!file) return buffer;

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        buffer.resize(size);
        fread(buffer.data(), 1, size, file);
        fclose(file);
#else
        std::ifstream file(path, std::ios::binary);
        if (!file) return buffer;

        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        buffer.resize(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);
#endif

        return buffer;
    }

    inline std::string ReadText(const std::string &path)
    {
#if defined(__SWITCH__)
        FILE *file = fopen(path.c_str(), "r");
        if (!file) return {};

        std::ostringstream ss;
        char               buf[512];
        while (size_t len = fread(buf, 1, sizeof(buf), file))
        {
            ss.write(buf, len);
        }

        fclose(file);
        return ss.str();
#else
        std::ifstream file(path);
        if (!file) return {};

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
#endif
    }

    inline std::vector<std::string> ListFiles(const std::string &directory, bool recursive = false)
    {
        std::vector<std::string> files;

#if defined(__SWITCH__)
        // ROMFS does not support directory iteration directly, so skip or
        // implement your own index if needed.
        return files;
#else
        if (!std::filesystem::exists(directory)) return files;

        if (recursive)
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(directory))
            {
                if (entry.is_regular_file())
                {
                    files.push_back(entry.path().string());
                }
            }
        }
        else
        {
            for (const auto &entry : std::filesystem::directory_iterator(directory))
            {
                if (entry.is_regular_file())
                {
                    files.push_back(entry.path().string());
                }
            }
        }

        return files;
#endif
    }

    inline void SplitPath(const std::string &fullPath, std::string &dir, std::string &fileName)
    {
        size_t found;
        found    = fullPath.find_last_of("/\\");
        dir      = fullPath.substr(0, found + 1);
        fileName = fullPath.substr(found + 1);
    }

    inline std::string GetFileNameFromPath(const std::string &fullPath)
    {
        if (fullPath.empty())
        {
            return fullPath;
        }

        std::string ret, empty;
        SplitPath(fullPath, empty, ret);
        return ret;
    }

    inline std::string GetDirectoryFromPath(const std::string &fullPath)
    {
        if (fullPath.empty())
        {
            return fullPath;
        }

        std::string ret, empty;
        SplitPath(fullPath, ret, empty);
        return ret;
    }

    inline std::string GetExtensionFromFileName(const std::string &filename)
    {
        size_t idx = filename.rfind('.');

        if (idx != std::string::npos)
        {
            std::string extension = filename.substr(idx + 1);
            return extension;
        }

        return "";
    }

    inline std::string ReplaceExtension(const std::string &filename, const std::string &extension)
    {
        size_t idx = filename.rfind('.');

        if (idx == std::string::npos)
        {
            return filename + "." + extension;
        }
        return filename.substr(0, idx + 1) + extension;
    }

    inline std::string ForceExtension(const std::string &filename, const std::string &extension)
    {
        std::string ext = "." + extension;
        if (ext.length() > filename.length()) return filename + ext;

        if (filename.substr(filename.length() - ext.length()).compare(ext))
        {
            return filename + ext;
        }
        return filename;
    }

    std::string RemoveExtension(const std::string &filename)
    {
        size_t idx = filename.rfind('.');

        if (idx == std::string::npos)
        {
            return filename;
        }
        return filename.substr(0, idx);
    }

    // Abstracted functions
    inline bool FileExists(const std::string &path) { return vantor::Platform::Environment::FileExists(path); }
} // namespace vantor::Helpers::FileSystem