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
 *  Date: 2025-06-30
 *
 *  File: VCT_Window.hpp
 *  Last Change: Automatically updated
 */

#pragma once

namespace Vantor::Context
{
    class Window
    {
        public:
            Window() = default;
            Window(int width, int height, const char *title);

            void close();
            typedef void (*resizeCallbackFunc_t)(int, int);
            void setResizeCallback(resizeCallbackFunc_t);
            typedef void *(*loadProc)(const char *);
            loadProc getLoadProc();
            void     swapBuffers();
            void     pollEvents();
            bool     shouldWindowClose();

            inline int getMaxWidth() { return m_maxWidth; };
            inline int getMaxHeight() { return m_maxHeight; };
            inline int getWidth() { return m_width; };
            inline int getHeight() { return m_height; };

            void *GetNativeHandle();

        private:
            int m_maxWidth;
            int m_maxHeight;
            int m_width;
            int m_height;
    };
} // namespace Vantor::Context