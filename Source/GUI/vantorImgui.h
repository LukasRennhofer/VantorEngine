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
 *  File: vantorImgui.h
 *  Last Change: Automatically updated
 */

#pragma once

#include "../Platform/vantorWindow.h"

#include "../External/imgui/imgui.h"
#ifdef VANTOR_WM_SDL2
#include "../External/imgui/imgui_impl_sdl2.h"
#endif
#ifdef VANTOR_WM_GLFW
#include "../External/imgui/imgui_impl_glfw.h"
#endif
#ifdef VANTOR_API_OPENGL
#include "../External/imgui/imgui_impl_opengl3.h"
#endif
#include "../Utils/utils.h"

#include <list>
#include <vector>
#include <format>

namespace vantor::ImGui
{
    // TODO
} // namespace vantor::ImGui