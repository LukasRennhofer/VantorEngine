
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
 * File: drawableObject.h
 * Last Change: 
 */
 

#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "sceneElements.h"

namespace chif {
	class drawableObject
	{
	public:
		virtual void draw() = 0;
		virtual void setGui() {};

		virtual void update() {};

		static sceneElements* scene;
	};
} // NAMESPACE CHIF

#endif