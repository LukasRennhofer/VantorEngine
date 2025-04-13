/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: sdlUtils.h
 * Last Change: Updated the projects docs
*/

/*
    File for ports of GLFW based functions to SDL2 based Utils

    These functions are cross-platform based and can be used on any supported system

    -> See the README for more
*/

#include "sdl2Utils.h"

double sdlUtilGetTime() {
    static Uint64 start = SDL_GetPerformanceCounter();
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();
    return static_cast<double>(now - start) / static_cast<double>(freq);
}