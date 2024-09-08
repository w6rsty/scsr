#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace scsr {

using usize = size_t;

using u8    = uint8_t;
using u16   = uint16_t; 
using u32   = uint32_t; 
using u64   = uint64_t;

using i8    = int8_t;
using i16   = int16_t; 
using i32   = int32_t; 
using i64   = int64_t;

using f32   = float;
using f64   = double;

/// Unique pointer alias
template <typename T>
using Scp = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scp<T> MakeScp(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

/// Shared pointer alias
template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> MakeRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#define BIT(x) (1 << x)

using StringHash = usize;
// djb2 hash
static StringHash StrToHash(const std::string& str)
{
    usize hash = 5381;
    for (char c : str)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/// Not allow to move
#define PIN(x) \
x(x&&) = delete; \
x& operator=(x&&) = delete;

#define SIG(x) \
x(const x&) = delete; \
x& operator=(const x&) = delete;

}