
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
 * File: Skybox.h
 * Last Change: 
 */
 

#pragma once

#include <glm/glm.hpp>
#include "../Engine/ScreenSpaceShader.h"
#include "drawableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace chif::Render3D {
struct colorPreset {
	glm::vec3 cloudColorBottom, skyColorTop, skyColorBottom, lightColor, fogColor;
};

class Skybox : public chif::drawableObject {
public:
	friend class VolumetricClouds;
	Skybox();
	~Skybox();

	virtual void draw();
	virtual void setGui();
	virtual void update();

	colorPreset DefaultPreset();
	colorPreset SunsetPreset();
	colorPreset SunsetPreset1();

	void mixSkyColorPreset(float v, colorPreset p1, colorPreset p2);

	unsigned int getSkyTexture() {
		return skyboxFBO->tex;
	}
	
	glm::vec3 skyColorTop, skyColorBottom;

	chif::Shader::ScreenSpaceShader * skyboxShader;
	chif::Buffer::FrameBufferObject * skyboxFBO;

	colorPreset presetSunset, highSunPreset;

	bool lensFlareOn = true;
};
} // NAMESPACE CHIF::RENDER3D