
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
 * File: camera.h
 * Last Change: 
 */
 

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2000.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;
const float MAX_FOV = 100.0f;


class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -5.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		updateCameraVectors();
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;

		//projectCameraPosition();
	}


	float Random2D(glm::vec2 st)
	{
		return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
	}

	float InterpolatedNoise(int ind, float x, float y) {
		float integer_X = floor(x);
		float fractional_X = glm::fract(x);
		float integer_Y = floor(y);
		float fractional_Y = glm::fract(y);
		glm::vec2 randomInput = glm::vec2(integer_X, integer_Y);
		float a = Random2D(randomInput);
		float b = Random2D(randomInput + glm::vec2(1.0, 0.0));
		float c = Random2D(randomInput + glm::vec2(0.0, 1.0));
		float d = Random2D(randomInput + glm::vec2(1.0, 1.0));
		//float v1 = Random2D(randomInput);
		//float v2 = Random2D(randomInput + vec2(1.0, 0.0));
		//float v3 = Random2D(randomInput + vec2(0.0, 1.0));
		//float v4 = Random2D(randomInput + vec2(1.0, 1.0));

		glm::vec2 w = glm::vec2(fractional_X, fractional_Y);
		w = w * w*w*(10.0f + w * (-15.0f + 6.0f*w));

		//fractional_X = smoothstep(0.0, 1.0, fractional_X);
		//fractional_Y = smoothstep(0.0, 1.0, fractional_Y);
		//return a + fractional_X*(b-a) + fractional_Y*c + fractional_X*fractional_Y*(d-c) - a*fractional_Y - fractional_X*fractional_Y*(b-a);
		float k0 = a,
			k1 = b - a,
			k2 = c - a,
			k3 = d - c - b + a;

		return k0 + k1 * w.x + k2 * w.y + k3 * w.x*w.y;
	}

	float perlin(glm::vec2 st, int octaves, float freq, float gDispFactor) {

		//st *= freq;
		glm::mat2 m;// = glm::mat2(0.8, -0.6, 0.6, 0.8);
		m[0][0] = 0.8;
		m[0][1] = -0.6;
		m[1][0] = 0.6;
		m[1][1] = 0.8;
		//int numOctaves = 10;
		float persistence = 0.5;
		float total = 0.0,
			frequency = 0.005*freq,
			amplitude = gDispFactor;
		for (int i = 0; i < octaves; ++i) {
			frequency *= 2.0;
			amplitude *= persistence;

			//st = frequency*m*st;

			glm::vec2 v = frequency * st;

			total += InterpolatedNoise(0, v.x, v.y) * amplitude;
		}
		return total*total*total;
	}

	void projectCameraPosition() {
		glm::vec2 st = glm::vec2(Position.x,Position.z);
		float y = perlin(st, 13, 0.01, 20.);
		Position.y = y;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= MAX_FOV)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= MAX_FOV)
			Zoom = MAX_FOV;
	}

	void invertPitch() {
		this->Pitch = -Pitch;
		updateCameraVectors();
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif