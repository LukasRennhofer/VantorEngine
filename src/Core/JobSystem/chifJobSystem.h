
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
 * File: chifJobSystem.h
 * Last Change:
 */

/*
    Modified Job System from Turánszki János and his documentation (https://wickedengine.net/2018/11/simple-job-system-using-standard-c/),
    which was really helpful !
*/

#pragma once

#include <functional>
#include <chrono>

struct JobDispatchArgs
{
	uint32_t jobIndex;
	uint32_t groupIndex;
};

// Basic Job System Functions
namespace chif::Core::JobSystem
{
	void Initialize();
	void Execute(const std::function<void()>& job);
	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);
	bool IsBusy();
	void Wait();
}