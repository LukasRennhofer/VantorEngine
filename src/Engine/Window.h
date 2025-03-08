
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
 * File: Window.h
 * Last Change: 
 */
 

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>
#include <iostream>

#include "version.h"

class Window
{
public:
	Window(int& success, unsigned int SCR_WIDTH = 1600, unsigned int SCR_HEIGHT = 900, std::string name = "CHIFEngine");
	~Window();
	GLFWwindow * w;
	GLFWwindow * getWindow() const { return w; }

	void processInput(float frameTime); //input handler

	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	void terminate() {
		glfwTerminate();
	}

	bool isWireframeActive() {
		return Window::wireframe;
	}

	bool continueLoop() {
		return !glfwWindowShouldClose(this->w);
	}

	void swapBuffersAndPollEvents() {
		glfwSwapBuffers(this->w);
		glfwPollEvents();
	}

	static Camera * camera;

private:
	int oldState, newState;
	int gladLoader();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static bool keyBools[10];

	static bool mouseCursorDisabled;

	static bool wireframe;

	static bool firstMouse;// = true;
	static float lastX;
	static float lastY;

	std::string name;
};