#include "core/core.hpp" // IWYU pragma: keep

using namespace scsr;

int main()
{
    Uniforms uniforms;

    uniforms.Set("camera_position", Vec3::ONE());

    auto pos = uniforms.Get<Vec3>("camera_position");

    LOG_INFO("Get {}", FormatMath(pos));

    return 0;
}