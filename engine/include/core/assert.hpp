#pragma once

#ifdef SCSR_RT_ASSERT

#include "core/log.hpp"
#include <cassert>

#endif

namespace scsr
{

#ifdef SCSR_RT_ASSERT

#define RT_ASSERT(x, ...) \
    if (!(x)) \
    { \
        LOG_RT_ERROR("Assertion failed: ({})\n{}", #x, __VA_ARGS__); \
        assert(false); \
    }

#define TODO(...) \
    LOG_WARN(__VA_ARGS__);

#else

#define RT_ASSERT(x, ...)
#define TODO(x, ...)

#endif



}