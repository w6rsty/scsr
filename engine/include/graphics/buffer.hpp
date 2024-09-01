#pragma once

#include "core/type.hpp"

namespace scsr
{

const usize DefaultBufferSize = 0x1000000;

class BufferAllocator
{
public:
    BufferAllocator(usize cap = DefaultBufferSize);
    ~BufferAllocator();

    void* Allocate(usize bytes);
    void Release();
private:
    usize m_Capacity;
    u8* m_Ptr;
};

class Buffer
{
    PIN(Buffer)
    SIG(Buffer)
public:
    Buffer(usize size) :
        m_Size(size)
    {
        m_Ptr = new u8[m_Size];
    }

    ~Buffer()
    {
        delete[] m_Ptr;
    }
protected:
    u8* m_Ptr;
    usize m_Size;
};

class FrameBuffer final : public Buffer
{
public:

private:

};

}