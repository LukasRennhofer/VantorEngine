/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-04-02
 *
 * File: chifInlineDebugger.cpp
 * Last Change:
 */
 

 #include "chifInlineDebugger.h"
 
 // TODO: Adding Final version of debugger
 namespace chif::Debug {
 
     void EnableDebugging(bool state) {
         DebuggingEnabled = state;
     }
 
     void EnableEngineDebugging(bool state) {
         EngineDebuggingEnabled = state;
     }
 
     void Breakpoint(std::string file, int line, bool inEngine = false) {
         if (inEngine && EngineDebuggingEnabled) {
             chif::Backlog::Log(file, std::format("[Source::InEngine] Breakpoint hit at line {}", line), chif::Backlog::LogLevel::DEBUG);
         } else {
             if (DebuggingEnabled) {
                 chif::Backlog::Log(file, std::format("Breakpoint hit at line {}", line), chif::Backlog::LogLevel::DEBUG);
             }
         }
     }
 } // namespace chif::Debug
 