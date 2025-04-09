/*
 *    				~ CHIFEngine LightDemo~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-27
 *
 * File: main.cpp
 * Last Change: Updated TestFramework
 */

 
 // Engine Header
 #include "../../src/CHIFEngine.h"
 
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
	 
	 // Camera
	 glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	 Camera camera(startPosition);
 
	 int success;
	 chif::Platform::Window window(success, 1920, 1800);
	 if (!success)
		 return -1;
 
	 window.camera = &camera;

	 // GUI 
	 chif::GUI::GUI gui(window);
 
	 glm::vec3 fogColor(0.5, 0.6, 0.7);
	 glm::vec3 lightColor(255, 255, 230);
	 lightColor /= 255.0;
 
	 chif::Graphics::Renderer::Buffer::FrameBufferObject SceneFBO(chif::Platform::Window::SCR_WIDTH, chif::Platform::Window::SCR_HEIGHT);
	 glm::vec3 lightPosition, seed;
	 glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)chif::Platform::Window::SCR_WIDTH / (float)chif::Platform::Window::SCR_HEIGHT, 5.f, 10000000.0f);
	 glm::vec3 lightDir = glm::vec3(-.5, 0.5, 1.0);
 
	 chif::Graphics::sceneElements scene;
	 scene.lightPos = lightPosition;
	 scene.lightColor = lightColor;
	 scene.fogColor = fogColor;
	 scene.seed = seed;
	 scene.projMatrix = proj;
	 scene.cam = &camera;
	 scene.sceneFBO = &SceneFBO;
	 scene.lightDir = lightDir;
 
	 chif::Graphics::drawableObject::scene = &scene;
 
	 chif::Graphics::Skybox skybox;
	 chif::Graphics::CloudsModel cloudsModel(&scene, &skybox);
 
	 chif::Graphics::VolumetricClouds volumetricClouds(chif::Platform::Window::SCR_WIDTH, chif::Platform::Window::SCR_HEIGHT, &cloudsModel);
	 chif::Graphics::VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel); // (expected) lower resolution framebuffers, so the rendering will be faster
 
	 gui.subscribe(&skybox)
		 .subscribe(&cloudsModel);
 
	 chif::Graphics::Renderer::Shader::ScreenSpaceShader PostProcessing("shaders\\post_processing.frag");
	 chif::Graphics::Renderer::Shader::ScreenSpaceShader fboVisualizer("shaders\\visualizeFbo.frag");
 
	 // Model Loading
	 stbi_set_flip_vertically_on_load(true);
	 ModelShader MeshModelShader("shaders\\model.vert", "shaders\\model.frag");
	 Model ourModel("resources/objects/backpack/backpack.obj");

	 // PBR Based lighting
	 chif::Graphics::Renderer::Shader::Shader LightShader("LightShader");
	 LightShader.attachShader("shaders\\chifLighting.vert");
	 LightShader.attachShader("shaders\\chifLighting.frag");
	 LightShader.linkPrograms();

     LightShader.use();
     LightShader.setInt("albedoMap", 0);
     LightShader.setInt("normalMap", 1);
     LightShader.setInt("metallicMap", 2);
     LightShader.setInt("roughnessMap", 3);
     LightShader.setInt("aoMap", 4);

	 glm::vec3 lightPositions[] = {
        glm::vec3(0.0f, 1000.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(150.0f, 150.0f, 150.0f),
    };

	LightShader.use();
    LightShader.setMat4("projection", proj);
 
	 while (window.continueLoop())
	 {
		 scene.lightDir = glm::normalize(scene.lightDir);
		 scene.lightPos = scene.lightDir * 1e6f + camera.Position;
 
		 float frametime = 1 / ImGui::GetIO().Framerate;
		 window.processInput(frametime);
 
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
		 scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)chif::Platform::Window::SCR_WIDTH / (float)chif::Platform::Window::SCR_HEIGHT, 5.f, 10000000.0f);
 
		 glEnable(GL_DEPTH_TEST);
		 glEnable(GL_CULL_FACE);
		 glCullFace(GL_BACK);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 scene.cam->invertPitch();
 
		 chif::Graphics::Renderer::Shader::ScreenSpaceShader::disableTests();
 
		 chif::Graphics::Renderer::Shader::Shader &post = PostProcessing.getShader();
		 post.use();
 
		 chif::Graphics::Renderer::Shader::ScreenSpaceShader::enableTests();
 
		 scene.cam->invertPitch();
 
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 glEnable(GL_DEPTH_TEST);
		 glEnable(GL_CULL_FACE);
 
 
		 scene.sceneFBO->bind();
 
		 MeshModelShader.use();
		 MeshModelShader.setMat4("projection", scene.projMatrix);
		 MeshModelShader.setMat4("view", view);
 
		 glm::mat4 model = glm::mat4(1.0f);
		 model = glm::translate(model, glm::vec3(0.0f, 800.0f, 0.0f));
		 model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f)); // Too Big
		 MeshModelShader.setMat4("model", model);
		 ourModel.Draw(MeshModelShader);

		 LightShader.setVec3("lightPositions[0]", lightPositions[0]);
         LightShader.setVec3("lightColors[0]", lightColors[0]);

		 LightShader.setMat4("view", view);
		 LightShader.setVec3("camPos", scene.cam->Position);

		 LightShader.setMat4("model", model);
         LightShader.setMat4("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
 
		 chif::Graphics::Renderer::Shader::ScreenSpaceShader::disableTests();
 
		 volumetricClouds.draw();
		 skybox.draw();
 
		 chif::Graphics::Renderer::Buffer::unbindCurrentFrameBuffer();
		 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 
		 post.use();
		 post.setVec2("resolution", glm::vec2(chif::Platform::Window::SCR_WIDTH, chif::Platform::Window::SCR_HEIGHT));
		 post.setVec3("cameraPosition", scene.cam->Position);
		 post.setSampler2D("screenTexture", SceneFBO.tex, 0);
		 post.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
		 post.setSampler2D("depthTex", SceneFBO.depthTex, 2);
		 post.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(chif::Graphics::VolumetricClouds::cloudDistance), 3);
 
		 post.setBool("wireframe", scene.wireframe);
 
		 post.setMat4("VP", scene.projMatrix * view);
		 PostProcessing.draw();

		 chif::Graphics::Renderer::Shader::Shader &fboVisualizerShader = fboVisualizer.getShader();
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