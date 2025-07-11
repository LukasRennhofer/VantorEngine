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
 *  Date: 2025-07-11
 *
 *  File: VCO_BTypes.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// These are the "Basic" coding style types of Vantor's Source Code

namespace Vantor::Core::Types
{
    // An implementation of a Singleton class for Vantor
    template <typename T> class VSingleton
    {
        public:
            VSingleton(const VSingleton &)            = delete;
            VSingleton &operator=(const VSingleton &) = delete;

            static T &Instance()
            {
                static T instance;
                return instance;
            }

            virtual ~VSingleton() = default;

        protected:
            VSingleton() = default;
    };

    // Can be used like this:
    // class AnotherSingletonClass : public VSingleton<AnotherSingletonClass> {
    //     friend class VSingleton<AnotherSingletonClass>; // Allow access to constructor
    // public:
    //     void LoadTexture(const std::string& path);
    // private:
    //     AnotherSingletonClass() = default;
    // };

} // namespace Vantor::Core::Types