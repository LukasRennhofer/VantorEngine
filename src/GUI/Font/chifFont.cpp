
/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-11
 *
 * File: chifFont.cpp
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
 */

#include "chifFont.h"

#include <fstream>
#include <exception>

#include "../../Core/Backlog/chifBacklog.h"

namespace chif::GUI {
    Font::Font(const char* fontFile) {
        // Initialize FreeType
        _error = FT_Init_FreeType(&_ft);
        if(_error) {
            chif::Backlog::Log("Font", "Failed to initialize FreeType", chif::Backlog::LogLevel::ERR);
        }
        setFontFile(fontFile);
    }

    Font::~Font() {}

    void Font::setFontFile(const char* fontFile) {
        _fontFile = (char*)fontFile;

        // Create a new font
        _error = FT_New_Face(_ft,       // FreeType instance handle
                            _fontFile, // Font family to use
                            0,         // index of font (in case there are more than one in the file)
                            &_face);   // font face handle

        if(_error == FT_Err_Unknown_File_Format) {
            chif::Backlog::Log("Font", "Failed to open font: unknown font format", chif::Backlog::LogLevel::ERR);
        }
        else if(_error) {
            chif::Backlog::Log("Font", "Failed to open font", chif::Backlog::LogLevel::ERR);
        }
    }

    FT_Face Font::getFaceHandle() {
        return _face;
    }
} // NAMESPACE CHIF::GUI