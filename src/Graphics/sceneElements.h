
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
 * File: sceneElements.h
 * Last Change: 
 */
 

#ifndef SCENELEMENTS_H
#define SCENELEMENTS_H

#include <camera.h>
#include <glm/glm.hpp>
#include "Renderer/buffers.h"
#include <random>

namespace chif::Graphics {
	struct sceneElements {

		glm::vec3 lightPos, lightColor, lightDir, fogColor, seed;
		glm::mat4 projMatrix;
		Camera * cam;
		chif::Graphics::Renderer::Buffer::FrameBufferObject * sceneFBO;
		bool wireframe = false;
	};
} // NAMESPACE CHIF::GRAPHICS
#endif