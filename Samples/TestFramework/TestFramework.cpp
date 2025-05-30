/*
 *    				~ Vantor LightDemo~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-27
 *
 * File: main.cpp
 * Last Change: Updated TestFramework
 */

 
 // Engine Header
 #include "../../Source/Vantor.h"
 
 // Standard libraries
 #include <iostream>
 #include <vector>
 #include <functional>
 
 // ImGui setup (only for desktop)
 #ifndef __SWITCH__
 #include "../../Source/External/imgui/imgui.h"
 #include "../../Source/External/imgui/imgui_impl_sdl2.h"
 #include "../../Source/External/imgui/imgui_impl_opengl3.h"
 #else
 // ImGui for Switch should use an EGL-compatible backend
 #include "../../Source/External/imgui/imgui/imgui.h"
 #include "../../Source/External/imgui/imgui/imgui_impl_opengl3.h"
 #endif
 
 extern "C" int main(int argc, char **argv)
 {
	 vantor::Application app;
	 app.Initialize();
	 
	 // Camera
	 glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	 Camera camera(startPosition);
 
	 int success;
	 vantor::Platform::Window window(success, 1920, 1800);
	 if (!success)
		 return -1;
 
	 window.camera = &camera;

	 // GUI 
	 vantor::GUI::GUI gui(window);
 
	 glm::vec3 fogColor(0.5, 0.6, 0.7);
	 glm::vec3 lightColor(255, 255, 230);
	 lightColor /= 255.0;
 
	 vantor::Graphics::Renderer::Buffer::FrameBufferObject SceneFBO(vantor::Platform::Window::SCR_WIDTH, vantor::Platform::Window::SCR_HEIGHT);
	 glm::vec3 lightPosition, seed;
	 glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)vantor::Platform::Window::SCR_WIDTH / (float)vantor::Platform::Window::SCR_HEIGHT, 5.f, 10000000.0f);
	 glm::vec3 lightDir = glm::vec3(-.5, 0.5, 1.0);
 
	 vantor::Graphics::sceneElements scene;
	 scene.lightPos = lightPosition;
	 scene.lightColor = lightColor;
	 scene.fogColor = fogColor;
	 scene.seed = seed;
	 scene.projMatrix = proj;
	 scene.cam = &camera;
	 scene.sceneFBO = &SceneFBO;
	 scene.lightDir = lightDir;
 
	 vantor::Graphics::drawableObject::scene = &scene;
 
	 int gridLength = 120;
	 vantor::Graphics::Terrain terrain(gridLength);
 
	 float waterHeight = 120.0;
	 vantor::Graphics::Water water(glm::vec2(0.0, 0.0), gridLength, waterHeight);
	 terrain.waterPtr = &water;
 
	 vantor::Graphics::Skybox skybox;
	 vantor::Graphics::CloudsModel cloudsModel(&scene, &skybox);
 
	 vantor::Graphics::VolumetricClouds volumetricClouds(vantor::Platform::Window::SCR_WIDTH, vantor::Platform::Window::SCR_HEIGHT, &cloudsModel);
	 vantor::Graphics::VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel); // (expected) lower resolution framebuffers, so the rendering will be faster
 
	 gui.subscribe(&terrain)
		 .subscribe(&skybox)
		 .subscribe(&cloudsModel)
		 .subscribe(&water);
 
	 vantor::Graphics::Renderer::Shader::ScreenSpaceShader PostProcessing("shaders\\post_processing.frag");
	 vantor::Graphics::Renderer::Shader::ScreenSpaceShader fboVisualizer("shaders\\visualizeFbo.frag");
 
	 // Model Loading
	 stbi_set_flip_vertically_on_load(true);
	 ModelShader MeshModelShader("shaders\\model.vert", "shaders\\model.frag");
	 Model ourModel("resources/objects/backpack/backpack.obj");
 
	 while (window.continueLoop())
	 {
		app.Run();

		// if (!vantor::Platform::Input::controllers.empty()) {
		// 	if (vantor::Platform::Input::Pressed_Controller(0, vantor::Platform::Input::GAMEPAD_BUTTON_XBOX_B)) {
		// 		vantor::Backlog::Log("Main", "Xbox Controller 0: Button B is pressed", vantor::Backlog::LogLevel::DEBUG);
		// 	}
		// }
		if (vantor::Platform::Input::Pressed_Keyboard(vantor::Platform::Input::KEYBOARD_BUTTON_SPACE)) {
			vantor::Backlog::Log("Main", "Space bar is pressed", vantor::Backlog::LogLevel::DEBUG);
		}

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
		 scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)vantor::Platform::Window::SCR_WIDTH / (float)vantor::Platform::Window::SCR_HEIGHT, 5.f, 10000000.0f);
 
		 water.bindReflectionFBO();
		 glEnable(GL_DEPTH_TEST);
		 glEnable(GL_CULL_FACE);
		 glCullFace(GL_BACK);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 scene.cam->invertPitch();
		 scene.cam->Position.y -= 2 * (scene.cam->Position.y - water.getHeight());
 
		 terrain.up = 1.0;
		 terrain.draw();
 
		 vantor::Graphics::Renderer::Buffer::FrameBufferObject const &reflFBO = water.getReflectionFBO();
 
		 vantor::Graphics::Renderer::Shader::ScreenSpaceShader::disableTests();
 
		 reflectionVolumetricClouds.draw();
		 water.bindReflectionFBO();
 
		 vantor::Graphics::Renderer::Shader::Shader &post = PostProcessing.getShader();
		 post.use();
		 post.setVec2("resolution", glm::vec2(1280, 720));
		 post.setSampler2D("screenTexture", reflFBO.tex, 0);
		 post.setSampler2D("depthTex", reflFBO.depthTex, 2);
		 post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		 PostProcessing.draw();
 
		 vantor::Graphics::Renderer::Shader::ScreenSpaceShader::enableTests();
 
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
 
		 MeshModelShader.use();
		 MeshModelShader.setMat4("projection", scene.projMatrix);
		 MeshModelShader.setMat4("view", view);
 
		 glm::mat4 model = glm::mat4(1.0f);
		 model = glm::translate(model, glm::vec3(0.0f, 800.0f, 0.0f));
		 model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f)); // Too Big
		 MeshModelShader.setMat4("model", model);
		 ourModel.Draw(MeshModelShader);
 
		 vantor::Graphics::Renderer::Shader::ScreenSpaceShader::disableTests();
 
		 volumetricClouds.draw();
		 skybox.draw();
 
		 vantor::Graphics::Renderer::Buffer::unbindCurrentFrameBuffer();
		 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 
		 post.use();
		 post.setVec2("resolution", glm::vec2(vantor::Platform::Window::SCR_WIDTH, vantor::Platform::Window::SCR_HEIGHT));
		 post.setVec3("cameraPosition", scene.cam->Position);
		 post.setSampler2D("screenTexture", SceneFBO.tex, 0);
		 post.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
		 post.setSampler2D("depthTex", SceneFBO.depthTex, 2);
		 post.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(vantor::Graphics::VolumetricClouds::cloudDistance), 3);
 
		 post.setBool("wireframe", scene.wireframe);
 
		 post.setMat4("VP", scene.projMatrix * view);
		 PostProcessing.draw();

		 vantor::Graphics::Renderer::Shader::Shader &fboVisualizerShader = fboVisualizer.getShader();
		 fboVisualizerShader.use();
		 fboVisualizerShader.setSampler2D("fboTex", volumetricClouds.getCloudsTexture(), 0);

		 // GUI
		 if (SHOW_IMGUI)
		 {
			 gui.draw();
		 }
		
		window.swapBuffersAndPollEvents();
	 }
 }