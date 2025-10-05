/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <string>
#include <cstdint>
#include <atomic>

namespace VE::Internal::ObjectRuntime {

    // TODO: Refactor (this is just for now)
    inline uint32_t GenerateTypeID() {
        static std::atomic<uint32_t> counter{0};
        return ++counter;
    }
    
    class VObject; // Forward Decl.

    class VObject {
    public:
    
        struct Type {
            const char* name;     // static string literal from macro
            uint32_t    id;
            const Type* parent;   // nullptr for root

            // Walk parent chain to check inheritance
            bool IsA(const Type& other) const {
                const Type* cur = this;
                while (cur) {
                    if (cur->id == other.id) return true;
                    cur = cur->parent;
                }
                return false;
            }
        };

        virtual ~VObject() = default;

        // Per-instance type access
        virtual const Type& GetType() const {
            return StaticType(); // base fallback; overridden by macro for derived classes
        }

        // Utility: runtime safe cast (returns nullptr if not instance of T)
        template<typename T>
        T* CastTo() {
            return (GetType().IsA(T::StaticType())) ? static_cast<T*>(this) : nullptr;
        }

        template<typename T>
        const T* CastTo() const {
            return (GetType().IsA(T::StaticType())) ? static_cast<const T*>(this) : nullptr;
        }

        // For root class we define StaticType manually below
        static const Type& StaticType();

    protected:
        VObject() = default;
    };

    // -------------------------
    // Macros to declare/define RTTI for classes
    // Usage:
    //   class MyClass : public Base { VCLASS_DECLARE(MyClass, Base) ... };
    //   VCLASS_DEFINE(MyClass, Base)
    // -------------------------
    #define VCLASS_DECLARE(CLASS, BASE)               \
    public:                                           \
        static const ::VE::Internal::ObjectRuntime::VObject::Type& StaticType(); \
        virtual const ::VE::Internal::ObjectRuntime::VObject::Type& GetType() const override { return StaticType(); } \
    private:

    #define VCLASS_DEFINE(CLASS, BASE)                                \
    const ::VE::Internal::ObjectRuntime::VObject::Type& CLASS::StaticType() { \
        static const ::VE::Internal::ObjectRuntime::VObject::Type type = []() -> ::VE::Internal::ObjectRuntime::VObject::Type { \
            return ::VE::Internal::ObjectRuntime::VObject::Type{ #CLASS, ::VE::Internal::ObjectRuntime::GenerateTypeID(), &BASE::StaticType() }; \
        }();                                                           \
        return type;                                                   \
    }
}