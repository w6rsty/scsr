#pragma once

#include "def.hpp"
#include "vector.hpp"

namespace scsr
{

/// 2x2 Column major matrix
struct Mat2
{
    union
    {
        struct
        {
            f32 m00, m10;
            f32 m01, m11;
        };
        struct
        {
            Vec2 col0, col1;
        };

        f32 data[4];
    };

    /// No initialization
    Mat2() = default;
    constexpr Mat2(f32 m00, f32 m01, f32 m10, f32 m11) : 
        m00(m00), m01(m01),
        m10(m10), m11(m11)
    {}
    /// From column vectors
    constexpr Mat2(const Vec2& col0, const Vec2& col1) : 
        col0(col0), col1(col1)
    {}
    /// From column array
    Mat2(f32* col[2]) :
        m00(col[0][0]), m01(col[1][0]),
        m10(col[0][1]), m11(col[1][1])
    {}
    /// From raw array
    Mat2(f32* raw) { memcpy(data, raw, sizeof(f32) * 4); }

    void Transpose()
    {
        std::swap(m01, m10);
    }
    Mat2 Transposed() const
    {
        return Mat2(
            m00, m10,
            m01, m11
        );
    }
    float Determinant() const
    {
        return m00 * m11 - m01 * m10;
    }
    float Trace() const
    {
        return m00 + m11;
    }
    Mat2 Inversed() const
    {
        float det = Determinant();
        if (det == 0) return Mat2::NANM();
        float invDet = 1.0f / det;
        return Mat2(
            m11 * invDet, -m01 * invDet,
            -m10 * invDet, m00 * invDet
        );
    }
    void Inverse()
    {
        *this = Inversed();
    }

    /// Mat2 add
    Mat2 operator + (const Mat2& other) const
    {
        return Mat2(
            col0 + other.col0,
            col1 + other.col1
        );
    }
    /// Mat2 sub
    Mat2 operator - (const Mat2& other) const
    {
        return Mat2(
            col0 - other.col0,
            col1 - other.col1
        );
    }
    /// Mat2 scalar multiply
    Mat2 operator * (f32 scalar) const
    {
        return Mat2(
            col0 * scalar,
            col1 * scalar
        );
    }
    /// Mat2 vector multiply
    Vec2 operator * (const Vec2& vec) const
    {
        return Vec2(
            m00 * vec.x + m01 * vec.y,
            m10 * vec.x + m11 * vec.y
        );
    }
    /// Mat2 matrix multiply
    Mat2 operator * (const Mat2& other) const
    {
        return Mat2(
            m00 * other.m00 + m01 * other.m10,
            m00 * other.m01 + m01 * other.m11,

            m10 * other.m00 + m11 * other.m10,
            m10 * other.m01 + m11 * other.m11
        );
    }
    float& operator [] (int index)
    {
        return data[index];
    }

    static constexpr Mat2 IDENTITY() { return Mat2(Vec2::X(), Vec2::Y()); }
    static constexpr Mat2 ZERO() { return Mat2(Vec2::ZERO(), Vec2::ZERO()); }
    static constexpr Mat2 NANM() { return Mat2(Vec2::NANM(), Vec2::NANM()); }
};

inline Mat2 Transpose(const Mat2& mat) { return mat.Transposed(); }
inline float Determinant(const Mat2& mat) { return mat.Determinant(); }
inline Mat2 Inverse(const Mat2& mat) { return mat.Inversed(); }

/// 3x3 Column major matrix
struct Mat3
{
    union
    {
        struct
        {
            f32 m00, m10, m20;
            f32 m01, m11, m21;
            f32 m02, m12, m22;
        };
        struct 
        {
            Vec3 col0, col1, col2;
        };

        f32 data[9];
    };

    /// No initialization
    Mat3() = default;
    constexpr Mat3(f32 m00, f32 m01, f32 m02, f32 m10, f32 m11, f32 m12, f32 m20, f32 m21, f32 m22) : 
        m00(m00), m01(m01), m02(m02),
        m10(m10), m11(m11), m12(m12),
        m20(m20), m21(m21), m22(m22)
    {}
    /// From column vectors
    constexpr Mat3(const Vec3& col0, const Vec3& col1, const Vec3& col2) : 
        col0(col0), col1(col1), col2(col2)
    {}
    /// From column array
    Mat3(f32* col[3]) :
        m00(col[0][0]), m01(col[1][0]), m02(col[2][0]),
        m10(col[0][1]), m11(col[1][1]), m12(col[2][1]),
        m20(col[0][2]), m21(col[1][2]), m22(col[2][2])
    {}
    /// From raw array
    Mat3(f32* raw) { memcpy(data, raw, sizeof(f32) * 9); }

    void Transpose()
    {
        std::swap(m01, m10);
        std::swap(m02, m20);
        std::swap(m12, m21);
    }
    Mat3 Transposed() const
    {
        return Mat3(
            m00, m10, m20,
            m01, m11, m21,
            m02, m12, m22
        );
    }
    float Determinant() const
    {
        return m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);
    }
    float Trace() const
    {
        return m00 + m11 + m22;
    }
    Mat3 Inversed() const
    {
        float det = Determinant();
        if (det == 0) return Mat3::NANM();
        float invDet = 1.0f / det;
        return Mat3(
            (m11 * m22 - m12 * m21) * invDet, (m02 * m21 - m01 * m22) * invDet, (m01 * m12 - m02 * m11) * invDet,
            (m12 * m20 - m10 * m22) * invDet, (m00 * m22 - m02 * m20) * invDet, (m02 * m10 - m00 * m12) * invDet,
            (m10 * m21 - m11 * m20) * invDet, (m01 * m20 - m00 * m21) * invDet, (m00 * m11 - m01 * m10) * invDet
        );
    }
    void Inverse()
    {
        *this = Inversed();
    }

    /// Mat3 add
    Mat3 operator + (const Mat3& other) const
    {
        return Mat3(
            col0 + other.col0,
            col1 + other.col1,
            col2 + other.col2
        );
    }
    /// Mat3 sub
    Mat3 operator - (const Mat3& other) const
    {
        return Mat3(
            col0 - other.col0,
            col1 - other.col1,
            col2 - other.col2
        );
    }
    /// Mat3 scalar multiply
    Mat3 operator * (f32 scalar) const
    {
        return Mat3(
            col0 * scalar,
            col1 * scalar,
            col2 * scalar
        );
    }
    /// Mat3 vector multiply
    Vec3 operator * (const Vec3& vec) const
    {
        return Vec3(
            m00 * vec.x + m01 * vec.y + m02 * vec.z,
            m10 * vec.x + m11 * vec.y + m12 * vec.z,
            m20 * vec.x + m21 * vec.y + m22 * vec.z
        );
    }
    /// Mat3 matrix multiply
    Mat3 operator * (const Mat3& other) const
    {
        return Mat3(
            m00 * other.m00 + m01 * other.m10 + m02 * other.m20,
            m00 * other.m01 + m01 * other.m11 + m02 * other.m21,
            m00 * other.m02 + m01 * other.m12 + m02 * other.m22,

            m10 * other.m00 + m11 * other.m10 + m12 * other.m20,
            m10 * other.m01 + m11 * other.m11 + m12 * other.m21,
            m10 * other.m02 + m11 * other.m12 + m12 * other.m22,

            m20 * other.m00 + m21 * other.m10 + m22 * other.m20,
            m20 * other.m01 + m21 * other.m11 + m22 * other.m21,
            m20 * other.m02 + m21 * other.m12 + m22 * other.m22
        );
    }
    float& operator [] (int index)
    {
        return data[index];
    }

    static constexpr Mat3 IDENTITY() { return Mat3(Vec3::X(), Vec3::Y(), Vec3::Z()); }
    static constexpr Mat3 ZERO() { return Mat3(Vec3::ZERO(), Vec3::ZERO(), Vec3::ZERO()); }
    static constexpr Mat3 NANM() { return Mat3(Vec3::NANM(), Vec3::NANM(), Vec3::NANM()); }
};

inline Mat3 Transpose(const Mat3& mat) { return mat.Transposed(); }
inline float Determinant(const Mat3& mat) { return mat.Determinant(); }
inline Mat3 Inverse(const Mat3& mat) { return mat.Inversed(); }

/// 4x4 Column major matrix
struct Mat4
{
    union
    {
        struct
        {
            f32 m00, m10, m20, m30;
            f32 m01, m11, m21, m31;
            f32 m02, m12, m22, m32;
            f32 m03, m13, m23, m33;
        };
        struct
        {
            Vec4 col0, col1, col2, col3;
        };

        f32 data[16];
    };

    /// No initialization
    Mat4() = default;
    constexpr Mat4(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23, f32 m30, f32 m31, f32 m32, f32 m33) : 
        m00(m00), m01(m01), m02(m02), m03(m03),
        m10(m10), m11(m11), m12(m12), m13(m13),
        m20(m20), m21(m21), m22(m22), m23(m23),
        m30(m30), m31(m31), m32(m32), m33(m33)
    {}
    /// From column vectors
    constexpr Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3) : 
        col0(col0), col1(col1), col2(col2), col3(col3)
    {}
    /// From column array
    Mat4(f32* col[4]) :
        m00(col[0][0]), m01(col[1][0]), m02(col[2][0]), m03(col[3][0]),
        m10(col[0][1]), m11(col[1][1]), m12(col[2][1]), m13(col[3][1]),
        m20(col[0][2]), m21(col[1][2]), m22(col[2][2]), m23(col[3][2]),
        m30(col[0][3]), m31(col[1][3]), m32(col[2][3]), m33(col[3][3])
    {}
    /// From raw array
    Mat4(f32* raw) { memcpy(data, raw, sizeof(f32) * 16); }
    /// From 3x3 matrix
    Mat4(Mat3 mat3) :
        m00(mat3.m00), m01(mat3.m01), m02(mat3.m02), m03(0),
        m10(mat3.m10), m11(mat3.m11), m12(mat3.m12), m13(0),
        m20(mat3.m20), m21(mat3.m21), m22(mat3.m22), m23(0),
        m30(0), m31(0), m32(0), m33(1)
    {}

    void Transpose()
    {
        std::swap(m01, m10);
        std::swap(m02, m20);
        std::swap(m03, m30);
        std::swap(m12, m21);
        std::swap(m13, m31);
        std::swap(m23, m32);
    }
    Mat4 Transposed() const
    {
        return Mat4(
            m00, m10, m20, m30,
            m01, m11, m21, m31,
            m02, m12, m22, m32,
            m03, m13, m23, m33
        );
    }
    float Determinant() const
    {
        return m00 * (m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m13 * m22 * m31 - m11 * m23 * m32 - m12 * m21 * m33) -
               m01 * (m10 * m22 * m33 + m12 * m23 * m30 + m13 * m20 * m32 - m13 * m22 * m30 - m10 * m23 * m32 - m12 * m20 * m33) +
               m02 * (m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31 - m13 * m21 * m30 - m10 * m23 * m31 - m11 * m20 * m33) -
               m03 * (m10 * m21 * m32 + m11 * m22 * m30 + m12 * m20 * m31 - m12 * m21 * m30 - m10 * m22 * m31 - m11 * m20 * m32);
    }
    float Trace() const
    {
        return m00 + m11 + m22 + m33;
    }
    Mat4 Inversed() const
    {
        float det = Determinant();
        if (det == 0) return Mat4::NANM();
        float invDet = 1.0f / det;
        return Mat4(
            (m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m13 * m22 * m31 - m11 * m23 * m32 - m12 * m21 * m33) * invDet,
            (m03 * m22 * m31 + m01 * m23 * m32 + m02 * m21 * m33 - m02 * m22 * m31 - m03 * m23 * m32 - m01 * m21 * m33) * invDet,
            (m03 * m12 * m31 + m01 * m13 * m32 + m02 * m11 * m33 - m02 * m12 * m31 - m03 * m13 * m32 - m01 * m11 * m33) * invDet,
            (m03 * m12 * m21 + m01 * m13 * m22 + m02 * m11 * m23 - m02 * m12 * m21 - m03 * m13 * m22 - m01 * m11 * m23) * invDet,

            (m10 * m22 * m33 + m12 * m23 * m30 + m13 * m20 * m32 - m13 * m22 * m30 - m10 * m23 * m32 - m12 * m20 * m33) * invDet,
            (m02 * m22 * m30 + m00 * m23 * m32 + m03 * m20 * m32 - m03 * m22 * m30 - m02 * m23 * m32 - m00 * m20 * m33) * invDet,
            (m03 * m12 * m30 + m00 * m13 * m32 + m02 * m10 * m33 - m02 * m12 * m30 - m03 * m13 * m32 - m00 * m10 * m33) * invDet,
            (m02 * m12 * m30 + m00 * m13 * m32 + m02 * m10 * m33 - m02 * m12 * m30 - m03 * m13 * m32 - m00 * m10 * m33) * invDet,

            (m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31 - m13 * m21 * m30 - m10 * m23 * m31 - m11 * m20 * m33) * invDet,
            (m03 * m21 * m30 + m01 * m23 * m31 + m03 * m20 * m31 - m03 * m21 * m30 - m01 * m23 * m30 - m01 * m20 * m33) * invDet,
            (m03 * m11 * m30 + m01 * m13 * m31 + m03 * m10 * m33 - m03 * m11 * m30 - m01 * m13 * m30 - m01 * m10 * m33) * invDet,
            (m03 * m11 * m20 + m01 * m13 * m21 + m02 * m10 * m23 - m02 * m11 * m20 - m03 * m13 * m21 - m01 * m10 * m23) * invDet,

            (m10 * m21 * m32 + m11 * m22 * m30 + m12 * m20 * m31 - m12 * m21 * m30 - m10 * m22 * m31 - m11 * m20 * m32) * invDet,
            (m02 * m21 * m30 + m00 * m22 * m31 + m01 * m20 * m32 - m01 * m21 * m30 - m02 * m22 * m31 - m00 * m20 * m32) * invDet,
            (m03 * m11 * m30 + m01 * m12 * m31 + m02 * m10 * m32 - m02 * m11 * m30 - m03 * m12 * m31 - m01 * m10 * m32) * invDet,
            (m02 * m11 * m20 + m00 * m12 * m21 + m01 * m10 * m22 - m01 * m11 * m20 - m02 * m12 * m21 - m00 * m10 * m22) * invDet
        );
    }
    void Inverse()
    {
        *this = Inversed();
    }

    Mat3 ToMat3() const
    {
        return Mat3(
            m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22
        );
    }

    /// Mat4 add
    Mat4 operator + (const Mat4& other) const
    {
        return Mat4(
            col0 + other.col0,
            col1 + other.col1,
            col2 + other.col2,
            col3 + other.col3
        );
    }
    /// Mat4 sub
    Mat4 operator - (const Mat4& other) const
    {
        return Mat4(
            col0 - other.col0,
            col1 - other.col1,
            col2 - other.col2,
            col3 - other.col3
        );
    }
    /// Mat4 scalar multiply
    Mat4 operator * (f32 scalar) const
    {
        return Mat4(
            col0 * scalar,
            col1 * scalar,
            col2 * scalar,
            col3 * scalar
        );
    }
    /// Mat4 vector multiply
    Vec4 operator * (const Vec4& vec) const
    {
        return Vec4(
            m00 * vec.x + m01 * vec.y + m02 * vec.z + m03 * vec.w,
            m10 * vec.x + m11 * vec.y + m12 * vec.z + m13 * vec.w,
            m20 * vec.x + m21 * vec.y + m22 * vec.z + m23 * vec.w,
            m30 * vec.x + m31 * vec.y + m32 * vec.z + m33 * vec.w
        );
    }
    /// Mat4 matrix multiply
    Mat4 operator * (const Mat4& other) const
    {
        return Mat4(
            m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30,
            m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31,
            m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32,
            m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33,

            m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30,
            m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31,
            m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32,
            m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33,

            m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30,
            m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31,
            m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32,
            m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33,

            m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30,
            m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31,
            m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32,
            m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33
        );
    }
    float& operator [] (int index)
    {
        return data[index];
    }

    static constexpr Mat4 IDENTITY() { return Mat4(Vec4::X(), Vec4::Y(), Vec4::Z(), Vec4::W()); }
    static constexpr Mat4 ZERO() { return Mat4(Vec4::ZERO(), Vec4::ZERO(), Vec4::ZERO(), Vec4::ZERO()); }
    static constexpr Mat4 NANM() { return Mat4(Vec4::NANM(), Vec4::NANM(), Vec4::NANM(), Vec4::NANM()); }
};

inline Mat4 Transpose(const Mat4& mat) { return mat.Transposed(); }
inline float Determinant(const Mat4& mat) { return mat.Determinant(); }
inline Mat4 Inverse(const Mat4& mat) { return mat.Inversed(); }

}