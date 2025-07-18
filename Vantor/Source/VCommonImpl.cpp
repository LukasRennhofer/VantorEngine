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
 *  Date: 2025-07-16
 *
 *  File: VCommonImpl.cpp
 *  Last Change: Automatically updated
 */

// Here are all the Common Implementations defined
// to not interfere with compile errors

#define VGLSL_MAX_OUTPUT_SIZE (1024 * 1024 * 12) // Default is 12 Megabytes

#define VGLSL_IMPLEMENTATION
#include <Shared/VGLSL/vglsl.h>

#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <Shared/tinygltf/tiny_gltf.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <Shared/tinyobjloader/tiny_obj_loader.hpp>