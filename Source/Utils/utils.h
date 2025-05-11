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
 *  File: utils.h
 *  Last Change: Automatically updated
 */

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

void initializePlaneVAO(const int res, const int width, GLuint *planeVAO, GLuint *planeVBO, GLuint *planeEBO);

glm::vec3 genRandomVec3();