#include "core/core.hpp"

using namespace scsr;

int main() {
    {
        Quat a(1.0f, 2.0f, 3.0f, 4.0f);
        Quat b = a.Inversed();

        LOG_INFO("{}", FormatMath(a * b));
    }
    {
        Quat rotation = Quat::FromAxisAngle(Vec3::Y(), Radians(90.0f));

        Quat a(0.0f, Vec3(1, 0, 0));

        Quat result = rotation * a * rotation.Conjugated();
        LOG_INFO("before\t{}", FormatMath(a));
        LOG_INFO("after\t{}", FormatMath(result));
    }

    {
        Mat3 rotation_mat = Quat::FromAxisAngle(Vec3::Y(), Radians(90.0f)).ToMat3();
        Quat rotation_quat = Quat::FromMat3(rotation_mat);

        Quat a(0.0f, Vec3(1, 0, 0));
        Quat result = rotation_quat * a * rotation_quat.Conjugated();

        LOG_INFO("before\t{}", FormatMath(a));
        LOG_INFO("after\t{}", FormatMath(result));
    }

    {
        Vec3 a(1, 0, 0);
        Mat3 rotation = Quat::FromAxisAngle(Vec3::Y(), Radians(90.0f)).ToMat3();

        Vec3 result = rotation * a;

        LOG_INFO("before\t{}", FormatMath(a));
        LOG_INFO("after\t{}", FormatMath(result));
    }
    
}