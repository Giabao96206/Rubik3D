#pragma once

#include <array>
#include "Math/Vector3D.h"

namespace rubik {

class Matrix4x4 {
public:
    Matrix4x4();
    explicit Matrix4x4(const std::array<double, 16>& values);

    static Matrix4x4 identity();
    static Matrix4x4 translation(double tx, double ty, double tz);
    static Matrix4x4 scaling(double sx, double sy, double sz);
    static Matrix4x4 rotationX(double radians);
    static Matrix4x4 rotationY(double radians);
    static Matrix4x4 rotationZ(double radians);
    static Matrix4x4 perspective(double fovYRadians, double aspect, double nearPlane, double farPlane);
    static Matrix4x4 orthographic(double left, double right, double bottom, double top, double nearPlane, double farPlane);
    static Matrix4x4 lookAt(const Point3D& eye, const Point3D& target, const Vector3D& up);

    Matrix4x4 operator*(const Matrix4x4& rhs) const;
    Point3D transformPoint(const Point3D& point) const;
    Vector3D transformVector(const Vector3D& vector) const;
    std::array<float, 16> toColumnMajorFloat() const;
    double operator()(int row, int column) const { return values_[row * 4 + column]; }

private:
    std::array<double, 16> values_{}; // Row-major nội bộ, nhân vector cột.
};

} // namespace rubik

