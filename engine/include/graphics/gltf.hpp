#pragma once

#include "core/type.hpp"
#include "core/math/math.hpp"

#include <string>
#include <vector>

namespace scsr
{

struct GLBHeader
{
    u32 magic;
    u32 version;
    u32 length;
};

struct GLBChunk
{
    u32 chunkLength;
    u32 chunkType;
    std::vector<u8> chunkData;
};

struct GLTFScene
{
    std::string name;
    std::vector<u32> nodes;
};

struct GLTFBuffer
{
    u32 byteLength;
};

struct GLTFBufferView
{
    u32 buffer;
    u32 byterLength;
    u32 byteOffset;
    u32 target;
};

struct GLTFAccessor
{
    enum class Type
    {
        SCALAR = 0,
        VEC2 = 1,
        VEC3 = 2,
        VEC4 = 3,
        MAT2 = 4,
        MAT3 = 5,
        MAT4 = 6
    };

    u32 componentType;
    Type type;
    u32 count;
    u32 bufferView;
    u32 byteOffset;
    Vec3 min;
    Vec3 max;
};

struct GLTFMesh
{
    struct Primitive
    {
        struct Attribute
        {
            u32 normal;
            u32 position;
            u32 texcoord_0;
        };

        u32 material;
        u32 mode;
        Attribute attributes;
        u32 indices;
    };

    std::string name;
    std::vector<Primitive> primitives;
};

struct GLTFNode
{
    std::string name;
    Vec3 translation;
    Quat rotation;
    Vec3 scale;
    u32 mesh;
    std::vector<u32> children;
};

class GLTF
{
public:
    static constexpr u32 Magic = 0x46546C67;
    static constexpr u32 Bin = 0x004E4942;

    GLTF(const std::string& path);
    void Load();

    const std::vector<GLTFScene>& Scenes() const { return m_scenes; }
    const std::vector<GLTFNode>& Nodes() const { return m_nodes; }
    const std::vector<GLTFBuffer>& Buffers() const { return m_buffers; }
    const std::vector<GLTFBufferView>& BufferViews() const { return m_bufferViews; }
    const std::vector<GLTFAccessor>& Accessors() const { return m_accessors; }
    const std::vector<GLTFMesh>& Meshes() const { return m_meshes; }
private:
    std::string m_path;

    std::vector<GLTFScene> m_scenes;
    std::vector<GLTFNode> m_nodes;
    std::vector<GLTFBuffer> m_buffers;
    std::vector<GLTFBufferView> m_bufferViews;

    std::vector<GLTFAccessor> m_accessors;
    std::vector<GLTFMesh> m_meshes;
};

}