#pragma once

#include "core/event/handler.hpp"
#include "core/type.hpp"
#include "core/log.hpp"
#include "core/assert.hpp"

#include <map>
#include <vector>
#include <typeinfo>
#include <functional>

namespace scsr
{

using ObjectIndex = u32;

struct Object {};

/// Generate unique index
template <typename Type>
class UniqueIndexer
{
public:
    template <typename T>
    static u32 GetIndex()
    {
        static u32 index = s_Counter++;
        return index;
    }
private:
    static u32 s_Counter; 
};

template <typename Type>
u32 UniqueIndexer<Type>::s_Counter = 0;

/// Object storage wrapper
struct ObjectInfo
{
    void* object = nullptr;

    using DestroyFn = std::function<void(void*)>;
    DestroyFn destroy = nullptr;

    ObjectInfo(DestroyFn fn) : 
        destroy(fn) 
    {
        RT_ASSERT(destroy, "Destroy function cannot be null");
    }
    ~ObjectInfo()
    {
        destroy(object);
    }

    void Reset()
    {
        if (object)
        {
            destroy(object);
            object = nullptr;
        }
    }
}; 

struct Storage;

using NormalSystemForm = std::function<void(Storage&, EventHandler&)>;
using Plugin = std::function<void(World&, Storage&)>;

template <typename Func>
static NormalSystemForm Convert(Func&& func)
{
    if constexpr (std::is_invocable_v<Func, Storage&, EventHandler&>)
    {
        return [f = std::forward<Func>(func)](Storage& storage, EventHandler& eventHandler) {
            f(storage, eventHandler);
        };
    }
    else if constexpr (std::is_invocable_v<Func, Storage&>)
    {
        return [f = std::forward<Func>(func)](Storage& storage, EventHandler& eventHandler) {
            f(storage);
        };
    }
    else
    {
        return [f = std::forward<Func>(func)](Storage&, EventHandler&) {
            f();
        };
    }
}

struct Storage
{
    template <typename T, typename... Args>
    void AddObject(Args&&... args)
    {
        ObjectIndex idx = UniqueIndexer<Object>::GetIndex<T>();
        // Has object and not null
        if (auto it = objects.find(idx); it != objects.end() && it->second.object)
        {
            LOG_WARN("Object <{}> already registered, reset", typeid(T).name());
            it->second.Reset();
            it->second.object = new T(std::forward<Args>(args)...);
            return;
        }
        else
        {
            auto newIt = objects.emplace(
                idx,
                ObjectInfo([](void* obj) { delete static_cast<T*>(obj); })
            );
            newIt.first->second.object = new T(std::forward<Args>(args)...);
            LOG_INFO("Object <{}> registered (immediate construct)", typeid(T).name());
        }
    }

    /// unregister object
    template <typename T>
    void RemoveObject()
    {
        ObjectIndex idx = UniqueIndexer<Object>::GetIndex<T>();
        // Has object and not null
        if (auto it = objects.find(idx); it != objects.end() && it->second.object)
        {
            it->second.Reset();
        }
    }

    template <typename T>
    bool HasObject() const
    {
        auto it = objects.find(UniqueIndexer<Object>::GetIndex<T>());
        return it != objects.end() && it->second.object;
    }

    template <typename T>
    T& GetObject()
    {
        auto it = objects.find(UniqueIndexer<Object>::GetIndex<T>());
        return *static_cast<T*>(it->second.object);
    }

    std::map<ObjectIndex, ObjectInfo> objects;
    std::vector<NormalSystemForm> systems;
    std::vector<Plugin> plugins;
};

struct Ticker
{
    usize tick = 0;
    usize delta = 0;
};

struct World
{
    World();

    template <typename T, typename... Args>
    World& RegisterObject(Args&&... args)
    {
        storage.AddObject<T>(std::forward<Args>(args)...);
        return *this;
    }

    template <typename Func>
    World& AddSystem(Func&& func)
    {
        storage.systems.push_back(Convert<Func>(std::forward<Func>(func)));
        return *this;
    }

    World& AddPlugin(Plugin func)
    {
        storage.plugins.push_back(func);
        return *this;
    }

    template <typename E>
    World& RegisterEvent(std::function<void(Event, Storage&)> func)
    {
        eventHandler.SetCallback(E::Type(), func);
        return *this;
    }

    bool ShouldExit();
    void Run();

    bool running;
    Storage storage;
    EventHandler eventHandler;
};

}