#include "core/core.hpp"
#include "core/math/ext.hpp"
#include "core/math/vector.hpp"

using namespace scsr;

int main() {
    // {
    //     Quat a(1.0f, 2.0f, 3.0f, 4.0f);
    //     Quat b = a.Inversed();

    //     LOG_INFO("{}", FormatMath(a * b));
    // }
    // {
    //     Quat rotation = Quat::FromAxisAngle(Vec3::Y(), Radians(90.0f));

    //     Quat a(0.0f, Vec3(1, 0, 0));

    //     Quat result = rotation * a * rotation.Conjugated();
    //     LOG_INFO("before\t{}", FormatMath(a));
    //     LOG_INFO("after\t{}", FormatMath(result));
    // }

    // {
    //     Mat3 rotation_mat = Quat::FromAxisAngle(Vec3::Y(), Radians(90.0f)).ToMat3();
    //     Quat rotation_quat = Quat::FromMat3(rotation_mat);

    //     Quat a(0.0f, Vec3(1, 0, 0));
    //     Quat result = rotation_quat * a * rotation_quat.Conjugated();

    //     LOG_INFO("before\t{}", FormatMath(a));
    //     LOG_INFO("after\t{}", FormatMath(result));
    // }

    // {
    //     Vec3 a(1, 0, 0);
    //     Mat3 rotation = Quat::FromAxisAngle(Vec3::Y(), Radians(90.0f)).ToMat3();

    //     Vec3 result = rotation * a;

    //     LOG_INFO("before\t{}", FormatMath(a));
    //     LOG_INFO("after\t{}", FormatMath(result));
    // }
    // {
    //     // test SRT decomposition
    //     Vec3 scale(1.0f, 2.0f, 3.0f);
    //     Quat rotation = Quat::FromRotationY(Radians(90.0f));
    //     Vec3 translation(4.0f, 5.0f, 6.0f);

    //     Mat4 transform = Mat4FromScaleRotationTranslation(scale, rotation, translation);

    //     LOG_INFO("transform\t{}", FormatMath(transform));

    //     auto [s, r, t] = Mat4ToScaleRotationTranslation(transform);
    //     LOG_INFO("scale\t{}", FormatMath(s));
    //     LOG_INFO("rotation\t{}", FormatMath(r));
    //     LOG_INFO("translation\t{}", FormatMath(t));
    // }
    {
        auto lookto = Mat4LookTo(Vec3::ZERO(), Vec3 {1, 2, 3});

        PRINT("{}", FormatMath(lookto));
    }
}