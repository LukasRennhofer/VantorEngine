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
 *  Date: 2025-05-12
 *
 *  File: vantorLabel.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include "vantorFont.h"
#include "vantorFontAtlas.h"
#include "../../Helpers/vantorMath.hpp"

#include <string>
#include <memory> // for use of shared_ptr
#include <map>
#include <vector>

using namespace std;

namespace vantor::GUI
{
    class Label
    {
        public:
            enum FontFlags
            {
                LeftAligned      = 1 << 1,
                RightAligned     = 1 << 2,
                CenterAligned    = 1 << 3,
                WordWrap         = 1 << 4,
                Underlined       = 1 << 5,
                Bold             = 1 << 6,
                Italic           = 1 << 7,
                Indented         = 1 << 8,
                HorizontalLayout = 1 << 9
            };

            // Ctor takes a pointer to a font face
            Label(shared_ptr<Font> ftFace, int windowWidth, int windowHeight);
            Label(Font *ftFace, int windowWidth, int windowHeight);
            Label(shared_ptr<Font> ftFace, const char *text, float x, float y, int width, int height, int windowWidth, int windowHeight);
            Label(shared_ptr<Font> ftFace, const char *text, float x, float y, int windowWidth, int windowHeight);
            ~Label();

            void setWindowSize(int width, int height);

            // Degrees to rotate & axis on which to rotate (e.g. (90 0 1 0) to
            // rotate 90deg on the y axis)
            void rotate(float degrees, float x, float y, float z);
            // NOTE: This does not change the pixel size. Use setPixelSize() to
            // scale evenly
            void scale(float x, float y, float z);

            // Setters
            void setText(const char *text);
            void setText(const string &text);
            void setPosition(float x, float y);
            void setSize(int width, int height);
            void setFont(shared_ptr<Font> ftFace);
            void setColor(float r, float b, float g,
                          float a); // RGBA values are 0 - 1.0
            void setAlignment(FontFlags alignment);
            void setPixelSize(int size);
            void setIndentation(int pixels);
            void setFontFlags(int flags);
            void appendFontFlags(int flags);

            // Getters
            string    getText();
            float     getX();
            float     getY();
            int       getWidth();
            int       getHeight();
            char     *getFont();
            glm::vec4 getColor();
            FontFlags getAlignment();
            float     getRotation();
            int       getIndentation();
            int       getFontFlags();

            void drawString(const char *text, float x, float y);
            void drawString(const char *text, float x, float y, float width, float height);

            void render();

        private:
            struct Point
            {
                    GLfloat x; // x offset in window coordinates
                    GLfloat y; // y offset in window coordinates
                    GLfloat s; // glyph x offset in texture coordinates
                    GLfloat t; // glyph y offset in texture coordinates

                    Point() {}

                    Point(float x, float y, float s, float t) : x(x), y(y), s(s), t(t) {}
            };

            shared_ptr<Font> _ftFace;
            FT_Face          _face;
            FT_Error         _error;
            FT_GlyphSlot     _g;

            GLuint _programId;
            GLuint _vao;
            GLuint _vbo;

            GLint _uniformTextureHandle;
            GLint _uniformTextColorHandle;
            GLint _uniformMVPHandle;

            const char *_text;

            vector<Point> _coords;

            std::map<int, std::shared_ptr<FontAtlas>> _fontAtlas;

            int _flags;
            int _numVertices;

            // Window dimensions
            int _windowWidth;
            int _windowHeight;

            // Coordinates at which to start drawing
            int _x;
            int _y;

            // Label dimensions
            int _width;
            int _height;

            float _sx;
            float _sy;

            int _atlasWidth;
            int _atlasHeight;

            glm::mat4 _projection;
            glm::mat4 _view;
            glm::mat4 _model;
            glm::mat4 _mvp;

            char     *_font;
            glm::vec4 _textColor;
            FontFlags _alignment;
            int       _pixelSize;
            int       _indentationPix;

            bool _isInitialized;

            void           calculateAlignment(const char *text, float &x);
            vector<string> splitText(const char *text);
            int            calcWidth(const char *text);

            void recalculateVertices(const char *text, float x, float y, int width, int height);
            void recalculateVertices(const char *text, float x, float y);

            void recalculateMVP();
    };
} // Namespace vantor::GUI