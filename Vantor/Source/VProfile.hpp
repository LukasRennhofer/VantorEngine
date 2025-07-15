/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-15
 *
 *  File: VProfile.hpp
 *  Last Change: Automatically updated
 */

// #include "Profile/VPRO_Base.hpp"

#pragma once

// TODO : Write VProfiler Class

#ifdef VANTOR_PROFILE

  // Initialize profiler system
    #define VPROFILER_INIT() \
        Vantor::Profile::VProfiler::Instance().Initialize()

    // Shutdown profiler system
    #define VPROFILER_SHUTDOWN() \
        Vantor::Profile::VProfiler::Instance().Shutdown()

    // Frame profiling macros
    #define VPROFILER_BEGIN_FRAME() \
        Vantor::Profile::VProfiler::Instance().BeginFrame(); \

    #define VPROFILER_END_FRAME() \
        Vantor::Profile::VProfiler::Instance().EndFrame(); \

    // Function/scope profiling macros
    #define VPROFILER_FUNCTION() \
        VPROFILER_SCOPE(__FUNCTION__)

    #define VPROFILER_SCOPE(name) \
        Vantor::Profile::VScopedProfiler _vprofiler_##__LINE__(name, VANTOR_PROFILE)

    #define VPROFILER_SCOPE_MANUAL(name, profiler_var) \
        Vantor::Profile::VScopedProfiler profiler_var(name, VANTOR_PROFILE)

    // GPU profiling macros
    #define VPROFILER_GPU_SCOPE(name) \
        Vantor::Profile::VGPUScopedProfiler _vgpu_profiler_##__LINE__(name, VANTOR_PROFILE)

    #define VPROFILER_GPU_RENDERPASS(pass_name) \
        VPROFILER_GPU_SCOPE("RenderPass: " pass_name)

    // Custom container profiling
    #define VPROFILER_CONTAINER_OPERATION(container_type, operation, size) \
        Vantor::Profile::VProfiler::Instance().RecordContainerOperation(container_type, operation, size); \

    // Render statistics recording
    #define VPROFILER_RECORD_DRAW_CALL(vertex_count, triangle_count) \
        Vantor::Profile::VProfiler::Instance().RecordDrawCall(vertex_count, triangle_count); \

    #define VPROFILER_RECORD_TEXTURE_BIND() \
        Vantor::Profile::VProfiler::Instance().RecordTextureBind(); \

    #define VPROFILER_RECORD_SHADER_SWITCH() \
        Vantor::Profile::VProfiler::Instance().RecordShaderSwitch(); \

    // Memory allocation tracking
    #define VPROFILER_RECORD_ALLOCATION(size, type) \
        Vantor::Profile::VProfiler::Instance().RecordAllocation(size, type); \

    #define VPROFILER_RECORD_DEALLOCATION(size, type) \
        Vantor::Profile::VProfiler::Instance().RecordDeallocation(size, type); \

    // Frame time recording
    #define VPROFILER_SET_FRAME_TIME(delta_time) \
        Vantor::Profile::VProfiler::Instance().SetFrameTime(delta_time); \

    // Conditional profiling for release builds with runtime toggle
    #define VPROFILER_RUNTIME_SCOPE(name) \
        Vantor::Profile::VScopedProfiler _vprofiler_runtime_##__LINE__(name, \
        Vantor::Profile::VProfiler::Instance().IsEnabled())

    // Thread-safe profiling for multi-threaded scenarios
    #define VPROFILER_THREAD_SCOPE(name) \
        Vantor::Profile::VThreadScopedProfiler _vthread_profiler_##__LINE__(name, VANTOR_PROFILE)

#else
    // Initialize profiler system
    #define VPROFILER_INIT()

    // Shutdown profiler system
    #define VPROFILER_SHUTDOWN()

    // Frame profiling macros
    #define VPROFILER_BEGIN_FRAME()

    #define VPROFILER_END_FRAME()

    #define VPROFILER_FUNCTION()

    #define VPROFILER_SCOPE(name)

    #define VPROFILER_SCOPE_MANUAL(name, profiler_var)

    // GPU profiling macros
    #define VPROFILER_GPU_SCOPE(name)

    #define VPROFILER_GPU_RENDERPASS(pass_name) 

    // Custom container profiling
    #define VPROFILER_CONTAINER_OPERATION(container_type, operation, size)

    // Render statistics recording
    #define VPROFILER_RECORD_DRAW_CALL(vertex_count, triangle_count) 

    #define VPROFILER_RECORD_TEXTURE_BIND()

    #define VPROFILER_RECORD_SHADER_SWITCH()
    // Memory allocation tracking
    #define VPROFILER_RECORD_ALLOCATION(size, type) 

    #define VPROFILER_RECORD_DEALLOCATION(size, type)

    // Frame time recording
    #define VPROFILER_SET_FRAME_TIME(delta_time)


    // Conditional profiling for release builds with runtime toggle
    #define VPROFILER_RUNTIME_SCOPE(name)

    // Thread-safe profiling for multi-threaded scenarios
    #define VPROFILER_THREAD_SCOPE(name)

#endif