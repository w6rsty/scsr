#include "core/log.hpp"
#include "ecs/entity.hpp"
#include "ecs/world.hpp"
#include <cassert>

using namespace scsr;

struct Foo {};

int main()
{
    LOG_INFO("Index test");

    auto a =IndexGenerator<ResourceInfo>::Instance().Generate<Foo>();
    auto b =IndexGenerator<ResourceInfo>::Instance().Generate<Foo>();

    assert(a == b);
}