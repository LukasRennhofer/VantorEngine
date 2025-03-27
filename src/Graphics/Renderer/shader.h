
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
 * File: shader.h
 * Last Change: Updated the projects docs
 */
 

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "BaseShader.h"

namespace chif::Graphics::Renderer::Shader {
	class Shader
	{
	public:
		Shader(std::string name);
		Shader(std::string name, const char * computeShader);
		Shader * attachShader(chif::Graphics::Renderer::Shader::BaseShader s);

		unsigned int ID;
		
		void linkPrograms();

		virtual ~Shader();
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec2(const std::string &name, glm::vec2 vector) const;
		void setVec3(const std::string &name, glm::vec3 vector) const;
		void setVec4(const std::string &name, glm::vec4 vector) const;
		void setMat4(const std::string &name, glm::mat4 matrix) const;
		void setSampler2D(const std::string &name, unsigned int texture, int id) const;
		void setSampler3D(const std::string &name, unsigned int texture, int id) const;

	protected:

		bool linked, isCompute;
		std::list<unsigned int> shaders;
		std::string name;
	};
} // NAMESPACE chif::Graphics::Renderer::Shader