/*
 *    				~ CHIFEngine Test Framework~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-27
 *
 * File: main.cpp.h
 * Last Change: Updated the projects docst
 */

#include "../../src/CHIFEngine.h"

extern "C" int main(int argc, char **argv)
 {
	 glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	 Camera camera(startPosition);
 
	 int success;
	 chif::Platform::Window window(success, 1920, 1800);
	 if (!success)
		 return -1;
 
	 window.camera = &camera;
 
	 chif::Graphics::Renderer::Buffer::FrameBufferObject SceneFBO(chif::Platform::Window::SCR_WIDTH, chif::Platform::Window::SCR_HEIGHT);
	 glm::vec3 lightPosition, seed;
	 glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)chif::Platform::Window::SCR_WIDTH / (float)chif::Platform::Window::SCR_HEIGHT, 5.f, 10000000.0f);
	 // Font Test Loading
	 shared_ptr<chif::GUI::Font> fontTest = shared_ptr<chif::GUI::Font>(new chif::GUI::Font("resources/calibri.ttf"));

	 unique_ptr<chif::GUI::Label> label = unique_ptr<chif::GUI::Label>(new chif::GUI::Label(
		fontTest,        
		"Hello world!",
		400,
		400,
		chif::Platform::Window::SCR_WIDTH,
		chif::Platform::Window::SCR_WIDTH
	  ));
 
	 while (window.continueLoop())
	 {
		 // TODO
 
		window.swapBuffersAndPollEvents();
	 }
 }
