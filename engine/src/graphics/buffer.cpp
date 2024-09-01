#include "graphics/buffer.hpp"
#include "core/log.hpp"

namespace scsr
{

BufferAllocator::BufferAllocator(usize cap) :
    m_Capacity(cap),
    m_Ptr(nullptr)
{
    m_Ptr = new u8[m_Capacity];
    if (!m_Ptr)
    {
        LOG_ERROR("Failed to allocate buffer ({} bytes)", m_Capacity);
    }
    else
    {
        LOG_INFO("Buffer allocated ({} bytes) at {}", m_Capacity, (void*)m_Ptr);
    }
}

BufferAllocator::~BufferAllocator()
{
    delete[] m_Ptr;
}

}