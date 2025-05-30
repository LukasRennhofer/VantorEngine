/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorJobSystem.h
 *  Last Change: Automatically updated
 */

/*
    Modified Job System from Turánszki János and his documentation
   (https://wickedengine.net/2018/11/simple-job-system-using-standard-c/), which
   was really helpful !
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
namespace vantor::Core::JobSystem
{
    void Initialize();
    void Execute(const std::function<void()> &job);
    void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)> &job);
    bool IsBusy();
    void Wait();
} // namespace vantor::Core::JobSystem