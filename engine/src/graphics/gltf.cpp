#include "graphics/gltf.hpp"

#include <json.hpp>

#include <fstream>

namespace scsr
{

GLTF::GLTF(const std::string& path) :
    m_path(path)
{}

void GLTF::Load()
{
    GLBHeader header;
    std::string json;
    std::vector<GLBChunk> chunks;

    std::ifstream in;
    in.open(m_path, std::ios::binary | std::ios::in);
    if (in.fail()) { return; }

    in.read(reinterpret_cast<byte*>(&header), sizeof(GLBHeader));
    if (header.magic != Magic) { 
        in.close();
        return; // Not match glTF magic number 
    }

    uint32_t jsonLength;
    uint32_t jsonType;
    in.read(reinterpret_cast<byte*>(&jsonLength), sizeof(u32));
    in.read(reinterpret_cast<byte*>(&jsonType), sizeof(u32));
    json.resize(jsonLength);
    in.read(reinterpret_cast<byte*>(json.data()), jsonLength);

    while (in.tellg() < header.length) {
        GLBChunk chunk;
        in.read(reinterpret_cast<byte*>(&chunk.chunkLength), sizeof(u32));
        in.read(reinterpret_cast<byte*>(&chunk.chunkType), sizeof(u32));
        if (chunk.chunkType != Bin) { continue; } // Not BIN chunk
        chunk.chunkData.resize(chunk.chunkLength);
        in.read(reinterpret_cast<byte*>(chunk.chunkData.data()), chunk.chunkLength);

        chunks.push_back(chunk);
    }

    nlohmann::json gltf = nlohmann::json::parse(json);

    if (gltf.contains("scenes")) {
        auto scenes = gltf["scenes"];

        for (const auto& scene : scenes)
        {
            GLTFScene s;
            if (scene.contains("name")) { s.name = scene["name"]; }
            if (scene.contains("nodes")) {
                auto nodes = scene["nodes"];
                for (const auto& node : nodes) {
                    s.nodes.push_back(node);
                }
            }
            m_scenes.push_back(s);
        }
    }

    if (gltf.contains("buffers"))
    {
        auto buffers = gltf["buffers"];
        for (const auto& buffer : buffers)
        {
            GLTFBuffer b;
            if (buffer.contains("byteLength")) { b.byteLength = buffer["byteLength"]; }
            m_buffers.push_back(b);
        }
    }

    if (gltf.contains("bufferViews"))
    {
        auto bufferViews = gltf["bufferViews"];
        for (const auto& bufferView : bufferViews)
        {
            GLTFBufferView bv;
            if (bufferView.contains("buffer")) { bv.buffer = bufferView["buffer"]; }
            if (bufferView.contains("byteLength")) { bv.byterLength = bufferView["byteLength"]; }
            if (bufferView.contains("byteOffset")) { bv.byteOffset = bufferView["byteOffset"]; }
            if (bufferView.contains("target")) { bv.target = bufferView["target"]; }
            m_bufferViews.push_back(bv);
        }
    }

    if (gltf.contains("accessors"))
    {
        auto accessors = gltf["accessors"];
        for (const auto& accessor : accessors)
        {
            GLTFAccessor a;
            if (accessor.contains("componentType")) { a.componentType = accessor["componentType"]; }
            if (accessor.contains("type")) {
                auto type = accessor["type"];
                if (type == "SCALAR") { a.type = GLTFAccessor::Type::SCALAR; }
                else if (type == "VEC2") { a.type = GLTFAccessor::Type::VEC2; }
                else if (type == "VEC3") { a.type = GLTFAccessor::Type::VEC3; }
                else if (type == "VEC4") { a.type = GLTFAccessor::Type::VEC4; }
                else if (type == "MAT2") { a.type = GLTFAccessor::Type::MAT2; }
                else if (type == "MAT3") { a.type = GLTFAccessor::Type::MAT3; }
                else if (type == "MAT4") { a.type = GLTFAccessor::Type::MAT4; }
            }
            if (accessor.contains("count")) { a.count = accessor["count"]; }
            if (accessor.contains("bufferView")) { a.bufferView = accessor["bufferView"]; }
            if (accessor.contains("byteOffset")) { a.byteOffset = accessor["byteOffset"]; }
            if (accessor.contains("min")) {
                auto min = accessor["min"];
                a.min = { min[0], min[1], min[2] };
            }
            if (accessor.contains("max")) {
                auto max = accessor["max"];
                a.max = { max[0], max[1], max[2] };
            }
            m_accessors.push_back(a);
        }
    }

    if (gltf.contains("nodes"))
    {
        auto nodes = gltf["nodes"];
        for (const auto& node : nodes)
        {
            GLTFNode n;
            if (node.contains("name")) { n.name = node["name"]; }
            if (node.contains("translation")) {
                auto translation = node["translation"];
                n.translation = { translation[0], translation[1], translation[2] };
            }
            if (node.contains("rotation")) {
                auto rotation = node["rotation"];
                n.rotation = { rotation[3], rotation[0], rotation[1], rotation[2] };
            }
            if (node.contains("scale")) {
                auto scale = node["scale"];
                n.scale = { scale[0], scale[1], scale[2] };
            }
            if (node.contains("mesh")) { n.mesh = node["mesh"]; }
            else { n.mesh = -1; }
            if (node.contains("children")) {
                auto children = node["children"];
                for (const auto& child : children) {
                    n.children.push_back(child);
                }
            }
            m_nodes.push_back(n);
        }
    }

    if (gltf.contains("meshes"))
    {
        auto meshes = gltf["meshes"];
        for (const auto& mesh : meshes)
        {
            GLTFMesh m;
            if (mesh.contains("name")) { m.name = mesh["name"]; }
            if (mesh.contains("primitives")) {
                auto primitives = mesh["primitives"];
                for (const auto& primitive : primitives)
                {
                    GLTFMesh::Primitive p;
                    if (primitive.contains("material")) { p.material = primitive["material"]; }
                    if (primitive.contains("mode")) { p.mode = primitive["mode"]; }
                    if (primitive.contains("attributes")) {
                        auto attributes = primitive["attributes"];
                        if (attributes.contains("NORMAL")) { p.attributes.normal = attributes["NORMAL"]; }
                        if (attributes.contains("POSITION")) { p.attributes.position = attributes["POSITION"]; }
                        if (attributes.contains("TEXCOORD_0")) { p.attributes.texcoord_0 = attributes["TEXCOORD_0"]; }
                    }
                    if (primitive.contains("indices")) { p.indices = primitive["indices"]; }
                    m.primitives.push_back(p);
                }
            }
            m_meshes.push_back(m);
        }
    }

    in.close();
}

}