/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

// TODO: Redo the Context and Window becomes VContext
// Add features like Fullscreen and Vsync

namespace VE::Internal::Context
{
    class VWindow
    {
        public:
            VWindow() = default;
            VWindow(int width, int height, const char *title);
            ~VWindow() { close(); }

            void close();
            typedef void (*resizeCallbackFunc_t)(int, int);
            void setResizeCallback(resizeCallbackFunc_t);
            typedef void *(*loadProc)(const char *);
            loadProc getLoadProc() const;
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
} // namespace VE::Internal::Context