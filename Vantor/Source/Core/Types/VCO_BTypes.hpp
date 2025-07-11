#pragma once

// These are the "Basic" coding style types of Vantor's Source Code

namespace Vantor::Core::Types {
    // An implementation of a Singleton class for Vantor
    template<typename T>
    class VSingleton {
    public:
        VSingleton(const VSingleton&) = delete;
        VSingleton& operator=(const VSingleton&) = delete;

        static T& Instance() {
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

}