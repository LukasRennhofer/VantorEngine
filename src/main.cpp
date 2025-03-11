
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
 * File: main.cpp
 * Last Change: 
*/

#ifndef GLAD_H
#define GLAD_H

#ifdef __SWITCH__
#include <switch.h>
#include <EGL/egl.h>	// EGL library for Switch
#include <EGL/eglext.h> // EGL extensions
#include <glad/glad.h>	// OpenGL ES (ensure glad is built with GLES support)
#else
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Only for desktop (Windows/Linux)
#endif

#endif // GLAD_H

// Engine headers
#include "Engine/Window.h"
#include "Engine/shader.h"
#include "Engine/ScreenSpaceShader.h"
#include "Engine/texture.h"
#include "Engine/version.h"
#include "Engine/constants.h"

// Drawable objects
#include "DrawableObjects/VolumetricClouds.h"
#include "DrawableObjects/Terrain.h"
#include "DrawableObjects/Skybox.h"
#include "DrawableObjects/Water.h"
#include "DrawableObjects/CloudsModel.h"

// Third-party libraries
#include <camera.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

// Error handling
#include "Engine/glError.h"

// Scene objects
#include "DrawableObjects/sceneElements.h"
#include "DrawableObjects/drawableObject.h"
#include "DrawableObjects/GUI.h"

// Standard libraries
#include <iostream>
#include <vector>
#include <functional>

// ImGui setup (only for desktop)
#ifndef __SWITCH__
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#else
// ImGui for Switch should use an EGL-compatible backend
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#endif

int main()
{

	glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	Camera camera(startPosition);

	int success;
	Window window(success, 1920, 1800);
	if (!success) return -1;

	window.camera = &camera;

	GUI gui(window);

	glm::vec3 fogColor(0.5,0.6,0.7);
	glm::vec3 lightColor(255, 255, 230);
	lightColor /= 255.0;

	FrameBufferObject SceneFBO(Window::SCR_WIDTH, Window::SCR_HEIGHT);
	glm::vec3 lightPosition, seed;
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
	glm::vec3 lightDir = glm::vec3(-.5, 0.5, 1.0);

	sceneElements scene;
	scene.lightPos = lightPosition;
	scene.lightColor = lightColor;
	scene.fogColor = fogColor;
	scene.seed = seed;
	scene.projMatrix = proj;
	scene.cam = &camera;
	scene.sceneFBO = &SceneFBO;
	scene.lightDir = lightDir;

	drawableObject::scene = &scene;

	int gridLength = 120;
	Terrain terrain(gridLength);

	float waterHeight = 120.0;
	Water water(glm::vec2(0.0, 0.0), gridLength, waterHeight);
	terrain.waterPtr = &water;

	Skybox skybox;
	CloudsModel cloudsModel(&scene, &skybox);
	
	VolumetricClouds volumetricClouds(Window::SCR_WIDTH, Window::SCR_HEIGHT, &cloudsModel);
	VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel); // (expected) lower resolution framebuffers, so the rendering will be faster
	
	gui.subscribe(&terrain)
		.subscribe(&skybox)
		.subscribe(&cloudsModel)
		.subscribe(&water);

	ScreenSpaceShader PostProcessing("shaders/post_processing.frag");
	ScreenSpaceShader fboVisualizer("shaders/visualizeFbo.frag");

	while (window.continueLoop())
	{
		scene.lightDir = glm::normalize(scene.lightDir);
		scene.lightPos = scene.lightDir*1e6f + camera.Position;

		float frametime = 1 / ImGui::GetIO().Framerate;
		window.processInput(frametime);

		terrain.updateTilesPositions();
		cloudsModel.update();
		gui.update();
		skybox.update();

		SceneFBO.bind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glm::mat4 view = scene.cam->GetViewMatrix();
		scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f,10000000.0f);

	
		water.bindReflectionFBO();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.cam->invertPitch();
		scene.cam->Position.y -= 2 * (scene.cam->Position.y - water.getHeight());
		
		terrain.up = 1.0;
		terrain.draw();
		FrameBufferObject const& reflFBO = water.getReflectionFBO();
		
		ScreenSpaceShader::disableTests();

		reflectionVolumetricClouds.draw();
		water.bindReflectionFBO();

		
		Shader& post = PostProcessing.getShader();
		post.use();
		post.setVec2("resolution", glm::vec2(1280, 720));
		post.setSampler2D("screenTexture", reflFBO.tex, 0);
		post.setSampler2D("depthTex", reflFBO.depthTex, 2);
		post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		PostProcessing.draw();

		ScreenSpaceShader::enableTests();
		
		scene.cam->invertPitch();
		scene.cam->Position.y += 2 * abs(scene.cam->Position.y - water.getHeight());

		water.bindRefractionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		terrain.up = -1.0;
		terrain.draw();

		scene.sceneFBO->bind();
		terrain.draw();
		water.draw();

		ScreenSpaceShader::disableTests();

		volumetricClouds.draw();
		skybox.draw();

		unbindCurrentFrameBuffer();
		//Shader& post = PostProcessing.getShader();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		post.use();
		post.setVec2("resolution", glm::vec2(Window::SCR_WIDTH, Window::SCR_HEIGHT));
		post.setVec3("cameraPosition", scene.cam->Position);
		post.setSampler2D("screenTexture", SceneFBO.tex, 0);
		post.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
		post.setSampler2D("depthTex", SceneFBO.depthTex, 2);
		post.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(VolumetricClouds::cloudDistance), 3);

		post.setBool("wireframe", scene.wireframe);

		post.setMat4("VP", scene.projMatrix * view);
		PostProcessing.draw();


		Shader& fboVisualizerShader = fboVisualizer.getShader();
		fboVisualizerShader.use();
		fboVisualizerShader.setSampler2D("fboTex", volumetricClouds.getCloudsTexture(), 0);
		// fboVisualizer.draw(); //Debugging

		if (SHOW_IMGUI) {
			gui.draw();
		}

		window.swapBuffersAndPollEvents();
	}
}