/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Core/Container/VCO_Vector.hpp>
#include <ActorRuntime/VAR_Component.hpp>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace Vantor::ActorRuntime
{

    using VActorID = uint64_t;

    class AActor : public std::enable_shared_from_this<AActor>
    {
        public:
            AActor() : id(nextID++) {}
            virtual ~AActor() = default;

            VActorID GetID() const { return id; }

            // Component management

            template <typename T, typename... Args> std::shared_ptr<T> AddComponent(Args &&...args)
            {
                static_assert(std::is_base_of<CComponent, T>::value, "T must derive from CComponent");
                auto comp                              = std::make_shared<T>(this, std::forward<Args>(args)...);
                components[std::type_index(typeid(T))] = comp;
                return comp;
            }

            template <typename T, typename... Args> void AddComponentVoid(Args &&...args)
            {
                static_assert(std::is_base_of<CComponent, T>::value, "T must derive from CComponent");
                auto comp = AddComponent<T>(std::forward<Args>(args)...);
            }

            template <typename T> std::shared_ptr<T> GetComponent() const
            {
                auto it = components.find(std::type_index(typeid(T)));
                if (it != components.end())
                {
                    return std::static_pointer_cast<T>(it->second);
                }
                return nullptr;
            }

            template <typename T> bool HasComponent() const { return components.count(std::type_index(typeid(T))) > 0; }

            void RemoveComponent(std::type_index compType) { components.erase(compType); }

            // Scene graph
            void AddChild(std::shared_ptr<AActor> child)
            {
                if (child)
                {
                    child->parent = shared_from_this();
                    children.push_back(std::move(child));
                }
            }

            void RemoveChild(std::shared_ptr<AActor> child)
            {
                if (!child) return;
                auto it = std::find(children.begin(), children.end(), child);
                if (it != children.end())
                {
                    (*it)->parent.reset();
                    children.erase(it);
                }
            }

            std::weak_ptr<AActor> GetParent() const { return parent; }

            const Vantor::Core::Container::TVector<std::shared_ptr<AActor>> &GetChildren() const { return children; }

            VActorID id; // The Entity Actor ID

        private:
            std::unordered_map<std::type_index, VComponentPtr> components;

            Vantor::Core::Container::TVector<std::shared_ptr<AActor>> children;
            std::weak_ptr<AActor>                parent;

            static VActorID nextID;
    };

    // Initialize static member
    inline VActorID AActor::nextID = 1;

} // namespace Vantor::Actor