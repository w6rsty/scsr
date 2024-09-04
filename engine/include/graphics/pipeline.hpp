#pragma once

#include "core/type.hpp"
#include "core/math/math.hpp"

#include <map>
#include <functional>

namespace scsr
{

using AttachementID = usize;

struct Uniforms
{
    template <typename T>
    struct UniformMap;

    template <>
    struct UniformMap<i32> { static auto& GetMap(Uniforms& u) { return u.integer; } };

    template <>
    struct UniformMap<f32> { static auto& GetMap(Uniforms& u) { return u.floating; } };

    template <>
    struct UniformMap<Vec2> { static auto& GetMap(Uniforms& u) { return u.vec2; } };

    template <>
    struct UniformMap<Vec3> { static auto& GetMap(Uniforms& u) { return u.vec3; } };

    template <>
    struct UniformMap<Vec4> { static auto& GetMap(Uniforms& u) { return u.vec4; } };

    template <>
    struct UniformMap<Mat3> { static auto& GetMap(Uniforms& u) { return u.mat3; } };

    template <>
    struct UniformMap<Mat4> { static auto& GetMap(Uniforms& u) { return u.mat4; } };

    template <typename T>
    void Set(const std::string& name, T value)
    {
        StringHash hash = StrToHash(name);
        UniformMap<T>::GetMap(*this)[hash] = value;
    }

    template <typename T>
    T Get(const std::string& name)
    {
        StringHash hash = StrToHash(name);
        return UniformMap<T>::GetMap(*this)[hash];
    }

    std::map<StringHash, i32> integer;
    std::map<StringHash, f32> floating;
    std::map<StringHash, Vec2> vec2;
    std::map<StringHash, Vec3> vec3;
    std::map<StringHash, Vec4> vec4;
    std::map<StringHash, Mat3> mat3;
    std::map<StringHash, Mat4> mat4;
};


struct VertexOutput
{
    Vec4 position;
    void* userdata;
};

using VertexShader = std::function<VertexOutput(usize, Uniforms&)>;

class Pipeline
{
public:
    void Perform(void* ptr, usize pitch)
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
};

}