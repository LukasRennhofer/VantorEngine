
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
 * File: CloudsModel.h
 * Last Change: 
 */
 

#pragma once
#include <glm/glm.hpp>
#include "../Engine/Shader.h"
#include "sceneElements.h"
#include "../Engine/ScreenSpaceShader.h"
#include "../imgui/imgui.h"
#include "Skybox.h"

#define INT_CEIL(n,d) (int)ceil((float)n/d)

namespace chif::weather {
	class CloudsModel : public drawableObject
	{
		public:
			friend class VolumetricClouds;

			CloudsModel(sceneElements * scene, chif::Render3D::Skybox * sky);
			~CloudsModel();
			
			virtual void draw() {};

			virtual void update();
			virtual void setGui();

		private:
			chif::Shader::Shader * volumetricCloudsShader, *weatherShader;
			chif::Shader::ScreenSpaceShader * postProcessingShader;

			float coverage, cloudSpeed, crispiness, curliness, density, absorption;
			float earthRadius, sphereInnerRadius, sphereOuterRadius;
			float perlinFrequency;
			bool enableGodRays;
			bool enablePowder;
			bool postProcess;

			glm::vec3 cloudColorTop, cloudColorBottom;

			glm::vec3 seed, oldSeed;
			unsigned int perlinTex, worley32, weatherTex;

			sceneElements * scene;
			chif::Render3D::Skybox * sky;

			void generateWeatherMap();
			void generateModelTextures();
			void initVariables();
			void initShaders();
	};
} // NAMESPACE CHIF::WEATHER