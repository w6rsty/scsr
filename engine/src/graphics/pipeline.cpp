#include "graphics/pipeline.hpp"

namespace scsr
{

void Pipeline::Perform(void* ptr, usize pitch)
{
    usize width = 600;
    usize height = 600;
    usize pixelSize = 4;
    for (usize y = 0; y < height; ++y)
    {
        for (usize x = 0; x < width; ++x)
        {
            u32* pixel = reinterpret_cast<u32*>(static_cast<u8*>(ptr) +  y * pitch + x * pixelSize);
            float r = static_cast<float>(x) / (width - 1);
            float g = static_cast<float>(y) / (height - 1);
            float b = 0;
            *pixel = (u32)(255) << 24 | (u32)(b * 255) << 16 | (u32)(g * 255) << 8 | (u32)(r * 255);
        }
    }
    
}

}