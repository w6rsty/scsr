#pragma once

#include "core/type.hpp"
#include <atomic>

namespace scsr
{

using Entity = u32;
using ComponentID = u32;

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

    std::atomic<Entity> m_Next;
};

template <typename Category, typename T>
usize GenerateIndex()
{
    return IndexGenerator<Category>::Instance().template Generate<T>();
}

}