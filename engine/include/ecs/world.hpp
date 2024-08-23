#pragma once

#include "ecs/entity.hpp"
#include "core/type.hpp"
#include "entity.hpp"

#include <functional>
#include <unordered_map>

namespace scsr
{

struct ResourceData
{
    void* ptr = nullptr;
    std::function<void(void*)> deleter;

    ResourceData() = default;
    ~ResourceData()
    {
        if (ptr != nullptr)
        {
            deleter(ptr);
        }
    }
};

class Resources
{
public:
    template <typename T>
    bool Has() const
    {
        usize index = GenerateIndex<ResourceData, T>();
        auto it = m_Resources.find(index);
        return it != m_Resources.end() && it->second.ptr != nullptr;
    }

    template <typename T>
    T& Get()
    {
        usize index = GenerateIndex<ResourceData, T>();
        
    }
private:
    std::unordered_map<ComponentID, ResourceData> m_Resources;
};

struct Storage
{
    Resources resources;    
};

class World
{
public:
    World();
    ~World();

    template <typename T>
    void SetResource(T&& res)
    {
        
    }
private:
    Storage m_Storage;
};

}