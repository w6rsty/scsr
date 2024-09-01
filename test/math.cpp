#include "core/math/math.hpp"
#include "core/core.hpp"

#include <iostream>

using namespace scsr;

int main() {
    Vec2 v2 = Vec2::ZERO();
    Vec3 v3 = Vec3::ZERO();
    Vec4 v4 = Vec4::ZERO();

    Mat2 m2 = Mat2::IDENTITY(); 
    Mat3 m3 = Mat3::IDENTITY();
    Mat4 m4 = Mat4::IDENTITY();

    std::cout << FormatMath(v2) << std::endl;
    std::cout << FormatMath(v3) << std::endl;
    std::cout << FormatMath(v4) << std::endl;

    std::cout << FormatMath(m2) << std::endl;
    std::cout << FormatMath(m3) << std::endl;
    std::cout << FormatMath(m4) << std::endl;
}