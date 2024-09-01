#include "core/core.hpp"

using namespace scsr;

void test(Storage& storagae)
{
    storagae.Get<Window>().OnUpdate();
}
int runtime(int argc, char* argv[])
{

    World world;

    WindowProp prop { .Title = "scsr", .Width = 600, .Height = 600 };

    world
        .InitObject(Window {prop})
        .AddSystem(test)
        .Run();

    return 0;
}