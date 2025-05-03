
/*
 *    				~ Vantor ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-11
 *
 * File: vantorFont.cpp
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
 */

#include "vantorFont.h"

#include <fstream>
#include <exception>

#include "../../Core/Backlog/vantorBacklog.h"

namespace vantor::GUI {
    Font::Font(const char* fontFile) {
        // Initialize FreeType
        _error = FT_Init_FreeType(&_ft);
        if(_error) {
            vantor::Backlog::Log("Font", "Failed to initialize FreeType", vantor::Backlog::LogLevel::ERR);
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
            vantor::Backlog::Log("Font", "Failed to open font: unknown font format", vantor::Backlog::LogLevel::ERR);
        }
        else if(_error) {
            vantor::Backlog::Log("Font", "Failed to open font", vantor::Backlog::LogLevel::ERR);
        }
    }

    FT_Face Font::getFaceHandle() {
        return _face;
    }
} // NAMESPACE VANTOR::GUI