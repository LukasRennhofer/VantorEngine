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
 * File: chifPlatformSwitch.cpp
 * Last Change: 
*/

#include "chifPlatformSwitch.h"

#ifndef ENABLE_NXLINK
#define TRACE(fmt,...) ((void)0)
#else
#include <unistd.h>
#define TRACE(fmt,...) printf("%s: " fmt "\n", __PRETTY_FUNCTION__, ## __VA_ARGS__)

// TODO
class chif::Platform::Switch {
    // Init of Nintendo Switch Headers etc.
    void Initialize() {
        romfsInit();
        chdir("romfs:/");
    }

    void ExitPlatform() {
        romfsExit();
    }

    static void initNxLink()
    {
        if (R_FAILED(socketInitializeDefault()))
            return;

        s_nxlinkSock = nxlinkStdio();
        if (s_nxlinkSock >= 0)
            TRACE("printf output now goes to nxlink server");
        else
            socketExit();
    }

    static void deinitNxLink()
    {
        if (s_nxlinkSock >= 0)
        {
            close(s_nxlinkSock);
            socketExit();
            s_nxlinkSock = -1;
        }
    }

    extern "C" void userAppInit()
    {
        initNxLink();
    }

    extern "C" void userAppExit()
    {
        deinitNxLink();
    }
} // Namespace chif::Platform::Switch