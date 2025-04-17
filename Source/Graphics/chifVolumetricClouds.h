
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
 * File: chifVolumetricClouds.h
 * Last Change: 
 */
 

#pragma once
#include "Renderer/chifScreenSpaceShader.h"
#include "Renderer/chifBuffers.h"
#include "chifTexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <camera.h>
#include <algorithm>
#include "chifDrawableObject.h"
#include "chifCloudsModel.h"
#include "../Utils/sdl2Utils.h"

namespace chif::Graphics {
	class VolumetricClouds : public drawableObject
	{
	public:
		VolumetricClouds(int SW, int SH, CloudsModel * model);
		virtual void draw();
		~VolumetricClouds();

		enum cloudsTextureNames {fragColor, bloom, alphaness, cloudDistance};

		void generateWeatherMap();

		unsigned int getCloudsTexture() { 
			return (model->postProcess ? cloudsPostProcessingFBO->getColorAttachmentTex(0) : getCloudsRawTexture());
		}

		unsigned int getCloudsTexture(int i) {
			return cloudsFBO->getColorAttachmentTex(i);
		}

		unsigned int getCloudsRawTexture(){
			return cloudsFBO->getColorAttachmentTex(0);
		}



	private:
		int SCR_WIDTH, SCR_HEIGHT;

		chif::Graphics::Renderer::Buffer::TextureSet * cloudsFBO;
		chif::Graphics::Renderer::Buffer::FrameBufferObject *cloudsPostProcessingFBO;

		CloudsModel * model;
	};
} // NAMESPACE chif::Graphics