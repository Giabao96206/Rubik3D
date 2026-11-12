#pragma once

#include <array>
#include "Math/Vector2D.h"

namespace rubik {

class Matrix3x3 {
public:
    Matrix3x3();
    explicit Matrix3x3(const std::array<double, 9>& values);

    static Matrix3x3 identity();
    static Matrix3x3 translation(double tx, double ty);
    static Matrix3x3 rotation(double radians);
    static Matrix3x3 scaling(double sx, double sy);

    Matrix3x3 operator*(const Matrix3x3& rhs) const;
    Point2D transformPoint(const Point2D& point) const;
    double operator()(int row, int column) const { return values_[row * 3 + column]; }

private:
    std::array<double, 9> values_{}; // Row-major; vector cột: P' = M * P.
};

} // namespace rubik

