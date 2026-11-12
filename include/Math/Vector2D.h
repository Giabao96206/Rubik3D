#pragma once

#include <cmath>

namespace rubik {

struct Point2D {
    double x{0.0};
    double y{0.0};
};

struct Vector2D {
    double x{0.0};
    double y{0.0};

    constexpr Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }
    constexpr Vector2D operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }
    constexpr Vector2D operator*(double scalar) const { return {x * scalar, y * scalar}; }
    constexpr double dot(const Vector2D& other) const { return x * other.x + y * other.y; }
    double length() const { return std::sqrt(dot(*this)); }
    Vector2D normalized() const {
        const double len = length();
        return len > 1e-12 ? (*this) * (1.0 / len) : Vector2D{};
    }
};

} // namespace rubik

