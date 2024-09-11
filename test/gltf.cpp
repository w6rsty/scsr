#include "graphics/gltf.hpp"
#include "core/log.hpp"

using namespace scsr;

int main()
{
    GLTF gltf("assets/meshes/Katana.glb");
    gltf.Load();

    for (const auto& scene : gltf.Scenes())
    {
        PRINT("Scene: {}", scene.name);
        for (const auto& nodeIdx : scene.nodes)
        {
            auto node = gltf.Nodes()[nodeIdx];
            PRINT("Node: {}", node.name);

            if (node.mesh < -1)
            {
                for (const auto& childIdx : node.children)
                {
                    auto child = gltf.Nodes()[childIdx];
                    PRINT("Child: {}", child.name);
                    
                    auto mesh = gltf.Meshes()[child.mesh];
                    PRINT("Mesh: {}", mesh.name);
                    for (const auto& primitive : mesh.primitives)
                    {
                        PRINT("indices: {}", primitive.indices);
                    }
                }
            }
        }
    }


    return 0;
}