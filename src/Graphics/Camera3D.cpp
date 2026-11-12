#include "Graphics/Camera3D.h"
#include "Algorithms/Bezier.h"
#include <algorithm>
#include <cmath>

namespace rubik {
	namespace { constexpr double pi = 3.14159265358979323846; double rad(double d) { return d * pi / 180.0; } }
	void Camera3D::orbit(double dyaw, double dpitch) { yawDegrees_ += dyaw;pitchDegrees_ = std::clamp(pitchDegrees_ + dpitch, -85.0, 85.0); }
	void Camera3D::zoom(double delta) { radius_ = std::clamp(radius_ + delta, 4.2, 16.0); }
	void Camera3D::reset() { yawDegrees_ = 40;pitchDegrees_ = 25;radius_ = 8;fovDegrees_ = 45;mode_ = ProjectionMode::Perspective;tourActive_ = false;tourParameter_ = 0; }
	void Camera3D::startTour() { tourStart_ = position();tourActive_ = true;tourParameter_ = 0; }
	void Camera3D::update(double dt) { if (!tourActive_)return;tourParameter_ += std::max(0.0, dt) / 8.0;if (tourParameter_ >= 1.0) { tourParameter_ = 1.0;tourActive_ = false;const Point3D end{ 7,3,7 };radius_ = std::sqrt(end.x * end.x + end.y * end.y + end.z * end.z);pitchDegrees_ = std::asin(end.y / radius_) * 180.0 / pi;yawDegrees_ = std::atan2(end.x, end.z) * 180.0 / pi; } }
	void Camera3D::toggleProjection() { mode_ = mode_ == ProjectionMode::Perspective ? ProjectionMode::Orthographic : ProjectionMode::Perspective; }
	Point3D Camera3D::position()const { if (tourActive_)return deCasteljau({ tourStart_,{-7,6,5},{-6,-2,-7},{6,4,-6},{7,3,7} }, tourParameter_);const double y = rad(yawDegrees_), p = rad(pitchDegrees_), cp = std::cos(p);return { radius_ * cp * std::sin(y),radius_ * std::sin(p),radius_ * cp * std::cos(y) }; }
	Matrix4x4 Camera3D::viewMatrix()const { return Matrix4x4::lookAt(position(), { 0,0,0 }, { 0,1,0 }); }
	Matrix4x4 Camera3D::projectionMatrix(double aspect)const { return mode_ == ProjectionMode::Perspective ? Matrix4x4::perspective(rad(fovDegrees_), aspect, .1, 100) : Matrix4x4::orthographic(-4 * aspect, 4 * aspect, -4, 4, .1, 100); }
}

