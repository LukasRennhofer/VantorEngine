/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-11
 *
 * File: main.cpp
 * Last Change: Added Nintendo Switch (Homebrew) Files for Init
 */

#ifndef GLAD_H
#define GLAD_H

#include <stb_image.h>
#include <glad/glad.h>

#ifdef __SWITCH__
#include <switch.h>
#include <EGL/egl.h>	// EGL library for Switch
#include <EGL/eglext.h> // EGL extensions
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif
#endif // GLAD_H

// Engine Header
#include "CHIFEngine.h"

// Standard libraries
#include <iostream>
#include <vector>
#include <functional>

// ImGui setup (only for desktop)
#ifndef __SWITCH__
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_sdl2.h"
#include "external/imgui/imgui_impl_opengl3.h"
#else
// ImGui for Switch should use an EGL-compatible backend
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_opengl3.h"
#endif

extern "C" int main(int argc, char **argv)
{

	glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	Camera camera(startPosition);

	int success;
	chif::Window window(success, 1920, 1800);
	if (!success)
		return -1;

	window.camera = &camera;

	chif::gui::GUI gui(window);

	glm::vec3 fogColor(0.5, 0.6, 0.7);
	glm::vec3 lightColor(255, 255, 230);
	lightColor /= 255.0;

	chif::Buffer::FrameBufferObject SceneFBO(chif::Window::SCR_WIDTH, chif::Window::SCR_HEIGHT);
	glm::vec3 lightPosition, seed;
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)chif::Window::SCR_WIDTH / (float)chif::Window::SCR_HEIGHT, 5.f, 10000000.0f);
	glm::vec3 lightDir = glm::vec3(-.5, 0.5, 1.0);

	chif::sceneElements scene;
	scene.lightPos = lightPosition;
	scene.lightColor = lightColor;
	scene.fogColor = fogColor;
	scene.seed = seed;
	scene.projMatrix = proj;
	scene.cam = &camera;
	scene.sceneFBO = &SceneFBO;
	scene.lightDir = lightDir;

	chif::drawableObject::scene = &scene;

	int gridLength = 120;
	chif::terrain::Terrain terrain(gridLength);

	float waterHeight = 120.0;
	chif::Render3D::Water water(glm::vec2(0.0, 0.0), gridLength, waterHeight);
	terrain.waterPtr = &water;

	chif::Render3D::Skybox skybox;
	chif::weather::CloudsModel cloudsModel(&scene, &skybox);

	chif::weather::VolumetricClouds volumetricClouds(chif::Window::SCR_WIDTH, chif::Window::SCR_HEIGHT, &cloudsModel);
	chif::weather::VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel); // (expected) lower resolution framebuffers, so the rendering will be faster

	gui.subscribe(&terrain)
		.subscribe(&skybox)
		.subscribe(&cloudsModel)
		.subscribe(&water);

	// FONT

	// if (!chif::gui::font::initFont()) {
	// 	return 0;
	// }

	// if (!chif::gui::font::setFont("resources/calibri.ttf")) {
	// 	return 0;
	// }

	// chif::gui::font::destroyFont();

	chif::Shader::ScreenSpaceShader PostProcessing("shaders/post_processing.frag");
	chif::Shader::ScreenSpaceShader fboVisualizer("shaders/visualizeFbo.frag");

	// Model Loading
	stbi_set_flip_vertically_on_load(true); // Tell STB_IMAGE to flip it vertically on y axis (Glaube halt Vertikal)
	ModelShader MeshModelShader("shaders/model.vs", "shaders/model.fs");
	Model ourModel("resources/objects/backpack/backpack.obj");

	while (window.continueLoop())
	{
		scene.lightDir = glm::normalize(scene.lightDir);
		scene.lightPos = scene.lightDir * 1e6f + camera.Position;

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

		if (scene.wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glm::mat4 view = scene.cam->GetViewMatrix();
		scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)chif::Window::SCR_WIDTH / (float)chif::Window::SCR_HEIGHT, 5.f, 10000000.0f);

		water.bindReflectionFBO();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.cam->invertPitch();
		scene.cam->Position.y -= 2 * (scene.cam->Position.y - water.getHeight());

		terrain.up = 1.0;
		terrain.draw();

		chif::Buffer::FrameBufferObject const &reflFBO = water.getReflectionFBO();

		chif::Shader::ScreenSpaceShader::disableTests();

		reflectionVolumetricClouds.draw();
		water.bindReflectionFBO();

		chif::Shader::Shader &post = PostProcessing.getShader();
		post.use();
		post.setVec2("resolution", glm::vec2(1280, 720));
		post.setSampler2D("screenTexture", reflFBO.tex, 0);
		post.setSampler2D("depthTex", reflFBO.depthTex, 2);
		post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		PostProcessing.draw();

		chif::Shader::ScreenSpaceShader::enableTests();

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

		// TODO: chif::model instance
		// Model Rendering and Loading (Nur Test Version)
		MeshModelShader.use();
		MeshModelShader.setMat4("projection", scene.projMatrix);
		MeshModelShader.setMat4("view", view);

		// Render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 800.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f)); // Too Big
		MeshModelShader.setMat4("model", model);
		ourModel.Draw(MeshModelShader);

		chif::Shader::ScreenSpaceShader::disableTests();

		volumetricClouds.draw();
		skybox.draw();

		chif::Buffer::unbindCurrentFrameBuffer();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		post.use();
		post.setVec2("resolution", glm::vec2(chif::Window::SCR_WIDTH, chif::Window::SCR_HEIGHT));
		post.setVec3("cameraPosition", scene.cam->Position);
		post.setSampler2D("screenTexture", SceneFBO.tex, 0);
		post.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
		post.setSampler2D("depthTex", SceneFBO.depthTex, 2);
		post.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(chif::weather::VolumetricClouds::cloudDistance), 3);

		post.setBool("wireframe", scene.wireframe);

		post.setMat4("VP", scene.projMatrix * view);
		PostProcessing.draw();

		chif::Shader::Shader &fboVisualizerShader = fboVisualizer.getShader();
		fboVisualizerShader.use();
		fboVisualizerShader.setSampler2D("fboTex", volumetricClouds.getCloudsTexture(), 0);
		// fboVisualizer.draw(); //Debugging

		// FONT 2
		// chif::gui::font::RenderText2D("Hello, World!", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

		if (SHOW_IMGUI)
		{
			gui.draw();
		}

		window.swapBuffersAndPollEvents();
	}
}