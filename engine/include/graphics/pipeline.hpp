#pragma once

#include "core/type.hpp"

namespace scsr
{

using AttachementID = usize;

class Pipeline
{
public:
    void Perform(void* ptr, usize pitch);
};

}