#pragma once

#include "core/type.hpp"

#include <atomic>
#include <functional>
#include <unordered_map>

namespace scsr
{

/// Creates a unique index for singleton instances
/// T is a type in Category 
template <typename Category>
class IndexGenerator
{
public:
    static IndexGenerator& Instance()
    {
        static IndexGenerator instance;
        return instance;
    }

    template <typename T>
    usize Generate()
    {
        static usize index = m_Next.fetch_add(1, std::memory_order_relaxed);
        return index;
    }
private:
    IndexGenerator() : m_Next(0) {}

    IndexGenerator(const IndexGenerator&) = delete;
    IndexGenerator& operator=(const IndexGenerator&) = delete;

    std::atomic<u32> m_Next;
};

template <typename Category, typename T>
usize GenerateIndex()
{
    return IndexGenerator<Category>::Instance().template Generate<T>();
}



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
    std::unordered_map<u32, ResourceData> m_Resources;
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