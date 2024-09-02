#pragma once

#include "core/event/handler.hpp"
#include "core/type.hpp"
#include "core/log.hpp"
#include "core/assert.hpp"

#include <map>
#include <typeinfo>
#include <functional>
#include <vector>

namespace scsr
{

template <typename T>
concept Object = requires(T t, Event e)
{
    { t.OnEvent(e) } -> std::same_as<void>;
};

using ObjectIndex = u32;

/// Generate unique index for each object type
class ObjectIndexer
{
public:
    template <typename T>
    static ObjectIndex GetIndex()
    {
        static ObjectIndex index = s_Counter++;
        return index;
    }
private:
    static ObjectIndex s_Counter; 
};

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
    Storage() {
        LOG_INFO("Storage created");
    }
    /// @brief Register object by static type
    /// @param object Object to register
    template <typename T>
    void Add(T&& object)
    {
        ObjectIndex idx = ObjectIndexer::GetIndex<T>();
        // Has object and not null
        if (auto it = objects.find(idx); it != objects.end() && it->second.object)
        {
            LOG_WARN("Object <{}> already registered, reset", typeid(T).name());
            it->second.Reset();
            it->second.object = new T(std::forward<T>(object));
            return;
        }
        else
        {
            auto newIt = objects.emplace(
                idx,
                ObjectInfo([](void* obj) { delete static_cast<T*>(obj); })
            );
            newIt.first->second.object = new T(std::forward<T>(object));
            LOG_INFO("Object <{}> registered (immediate construct)", typeid(T).name());
        }
    }

    /// unregister object
    template <typename T>
    void Remove()
    {
        ObjectIndex idx = ObjectIndexer::GetIndex<T>();
        // Has object and not null
        if (auto it = objects.find(idx); it != objects.end() && it->second.object)
        {
            it->second.Reset();
        }
    }

    template <typename T>
    bool Has() const
    {
        auto it = objects.find(ObjectIndexer::GetIndex<T>());
        return it != objects.end() && it->second.object;
    }

    template <typename T>
    T& Get()
    {
        auto it = objects.find(ObjectIndexer::GetIndex<T>());
        return *static_cast<T*>(it->second.object);
    }

    std::map<ObjectIndex, ObjectInfo> objects;
    std::vector<NormalSystemForm> systems;
};

struct World
{
    World();

    template <typename T>
    World& InitObject(T&& object)
    {
        storage.Add<T>(std::forward<T>(object));
        return *this;
    }

    template <typename Func>
    World& AddSystem(Func&& func)
    {
        storage.systems.push_back(Convert<Func>(std::forward<Func>(func)));
        return *this;
    }

    void Run();

    Storage storage;
    EventHandler eventHandler;
};

}