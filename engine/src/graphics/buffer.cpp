#include "graphics/buffer.hpp"
#include "core/log.hpp"

namespace scsr
{

usize BufferStorageFormatSize(BufferStorageFormat format)
{
    switch (format)
    {
    case BufferStorageFormat::RU8:      return 1;
    case BufferStorageFormat::RGB888:   return 3;
    case BufferStorageFormat::RGBA888:  return 4;
    case BufferStorageFormat::D32:      return 4;
    case BufferStorageFormat::D24S8:    return 4;
    default:
        LOG_WARN("Unexpected BufferStorageFormat");
        return 0;
    }
}

FrameBuffer::FrameBuffer(usize width, usize height, BufferStorageFormat format) :
    m_Width(width),
    m_Height(height),
    m_Format(format)
{
    m_Data = new u8[width * height * BufferStorageFormatSize(format)];
}

FrameBuffer::~FrameBuffer()
{
    delete[] m_Data;
}

usize FrameBuffer::Width() const { return m_Width; }
usize FrameBuffer::Height() const { return m_Height; }
BufferStorageFormat FrameBuffer::Format() const { return m_Format; }
u8* FrameBuffer::Data() { return m_Data; }

}