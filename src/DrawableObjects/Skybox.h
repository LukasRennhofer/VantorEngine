
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

struct colorPreset {
	glm::vec3 cloudColorBottom, skyColorTop, skyColorBottom, lightColor, fogColor;
};

class Skybox : public drawableObject {
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

private:
	glm::vec3 skyColorTop, skyColorBottom;

	ScreenSpaceShader * skyboxShader;
	FrameBufferObject * skyboxFBO;

	colorPreset presetSunset, highSunPreset;

	bool lensFlareOn = true;
};