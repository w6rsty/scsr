#pragma once

#include "core/type.hpp"

namespace scsr
{

enum class BufferUsage
{
    Static,
    Dynamic,
    Stream,
};

enum class BufferStorageFormat
{
    RU8,        // Grayscale, 8-bit  integer
    RGB888,     // RGB,       8-bit  integer
    RGBA888,    // RGBA,      8-bit  integer

    D32,        // Depth,     24-bit integer
    D24S8,      // Depth,     24-bit integer, Stencil, 8-bit integer
};

usize BufferStorageFormatSize(BufferStorageFormat format);

class FrameBuffer final
{
public:
    FrameBuffer(usize width, usize height, BufferStorageFormat format);
    ~FrameBuffer();

    usize Width() const;
    usize Height() const;
    BufferStorageFormat Format() const;
    u8* Data();
private:
    usize m_Width;
    usize m_Height;
    BufferStorageFormat m_Format;
    u8* m_Data;
};
}