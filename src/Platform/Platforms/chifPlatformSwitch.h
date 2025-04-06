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
 * File: chifPlatformSwitch.h
 * Last Change: 
*/

// Only include this if its building for Nintendo Switch !

#include <switch.h>

// TODO
class chif::Platform::Switch {
    static int s_nxlinkSock = -1;
    
    void Initialize(); // Init of Nintendo Switch Headers etc.
    void ExitPlatform();
    // NxLink Utils
    static void initNxLink();
    static void deinitNxLink();
    extern "C" void userAppInit();
    extern "C" void userAppExit();
} // Namespace chif::Platform::Switch