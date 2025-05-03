
/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorSceneElements.h
 * Last Change: 
 */
 

#ifndef SCENELEMENTS_H
#define SCENELEMENTS_H

#include <camera.h>
#include <glm/glm.hpp>
#include "Renderer/vantorBuffers.h"
#include <random>

namespace vantor::Graphics {
	struct sceneElements {

		glm::vec3 lightPos, lightColor, lightDir, fogColor, seed;
		glm::mat4 projMatrix;
		Camera * cam;
		vantor::Graphics::Renderer::Buffer::FrameBufferObject * sceneFBO;
		bool wireframe = false;
	};
} // NAMESPACE VANTOR::GRAPHICS
#endif