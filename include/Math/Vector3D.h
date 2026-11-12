#pragma once

#include <cmath>

namespace rubik {

struct Point3D {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct Vector3D {
    double x{0.0};
    double y{0.0};
    double z{0.0};

    constexpr Vector3D operator+(const Vector3D& v) const { return {x + v.x, y + v.y, z + v.z}; }
    constexpr Vector3D operator-(const Vector3D& v) const { return {x - v.x, y - v.y, z - v.z}; }
    constexpr Vector3D operator-() const { return {-x, -y, -z}; }
    constexpr Vector3D operator*(double s) const { return {x * s, y * s, z * s}; }
    constexpr Vector3D operator/(double s) const { return {x / s, y / s, z / s}; }
    Vector3D& operator+=(const Vector3D& v) { x += v.x; y += v.y; z += v.z; return *this; }

    constexpr double dot(const Vector3D& v) const { return x * v.x + y * v.y + z * v.z; }
    constexpr Vector3D cross(const Vector3D& v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }
    double length() const { return std::sqrt(dot(*this)); }
    Vector3D normalized() const {
        const double len = length();
        return len > 1e-12 ? (*this) / len : Vector3D{};
    }
};

inline constexpr Vector3D operator-(const Point3D& a, const Point3D& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline constexpr Point3D operator+(const Point3D& p, const Vector3D& v) {
    return {p.x + v.x, p.y + v.y, p.z + v.z};
}

inline constexpr Vector3D operator*(double scalar, const Vector3D& v) { return v * scalar; }

} // namespace rubik

