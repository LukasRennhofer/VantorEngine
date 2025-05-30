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
 *  File: vantorDrawableObject.h
 *  Last Change: Automatically updated
 */

#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "vantorSceneElements.h"

namespace vantor::Graphics
{
    class drawableObject
    {
        public:
            virtual void draw() = 0;
            virtual void setGui() {};

            virtual void update() {};

            static vantor::Graphics::sceneElements *scene;
    };
} // NAMESPACE vantor::Graphics

#endif