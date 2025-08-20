/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

namespace Vantor::Core::Types
{
    // An implementation of a Singleton class for Vantor
    template <typename T> class TSingleton
    {
        public:
            TSingleton(const TSingleton &)            = delete;
            TSingleton &operator=(const TSingleton &) = delete;

            static T &Instance()
            {
                static T instance;
                return instance;
            }

            virtual ~TSingleton() = default;

        protected:
            TSingleton() = default;
    };

    // Can be used like this:
    // class AnotherSingletonClass : public TSingleton<AnotherSingletonClass> {
    //     friend class TSingleton<AnotherSingletonClass>; // Allow access to constructor
    // public:
    //     void LoadTexture(const std::string& path);
    // private:
    //     AnotherSingletonClass() = default;
    // };

} // namespace Vantor::Core::Types