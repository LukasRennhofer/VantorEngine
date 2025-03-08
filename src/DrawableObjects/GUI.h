
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
 * File: GUI.h
 * Last Change: 
 */
 

#pragma once
#include "../Engine/Window.h"
#include "drawableObject.h"

#include <list>

class GUI : public drawableObject
{
public:
	GUI(Window& w);
	~GUI();

	virtual void draw();
	virtual void update();

	GUI& subscribe(drawableObject* subscriber);

private:
	std::list<drawableObject*> subscribers;

};