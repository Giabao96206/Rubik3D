#include "Math/Matrix4x4.h"

#include <cmath>
#include <stdexcept>

namespace rubik {

	Matrix4x4::Matrix4x4() = default;
	Matrix4x4::Matrix4x4(const std::array<double, 16>& values) : values_(values) {}

	Matrix4x4 Matrix4x4::identity() { return Matrix4x4({ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }); }
	Matrix4x4 Matrix4x4::translation(double tx, double ty, double tz) { return Matrix4x4({ 1,0,0,tx, 0,1,0,ty, 0,0,1,tz, 0,0,0,1 }); }
	Matrix4x4 Matrix4x4::scaling(double sx, double sy, double sz) { return Matrix4x4({ sx,0,0,0, 0,sy,0,0, 0,0,sz,0, 0,0,0,1 }); }
	Matrix4x4 Matrix4x4::rotationX(double a) { const double c = std::cos(a), s = std::sin(a); return Matrix4x4({ 1,0,0,0, 0,c,-s,0, 0,s,c,0, 0,0,0,1 }); }
	Matrix4x4 Matrix4x4::rotationY(double a) { const double c = std::cos(a), s = std::sin(a); return Matrix4x4({ c,0,s,0, 0,1,0,0, -s,0,c,0, 0,0,0,1 }); }
	Matrix4x4 Matrix4x4::rotationZ(double a) { const double c = std::cos(a), s = std::sin(a); return Matrix4x4({ c,-s,0,0, s,c,0,0, 0,0,1,0, 0,0,0,1 }); }

	Matrix4x4 Matrix4x4::perspective(double fov, double aspect, double nearP, double farP) {
		if (aspect <= 0 || nearP <= 0 || farP <= nearP) throw std::invalid_argument("Invalid perspective parameters");
		const double f = 1.0 / std::tan(fov * 0.5);
		return Matrix4x4({ f / aspect,0,0,0, 0,f,0,0, 0,0,(farP + nearP) / (nearP - farP),(2 * farP * nearP) / (nearP - farP), 0,0,-1,0 });
	}

	Matrix4x4 Matrix4x4::orthographic(double l, double r, double b, double t, double n, double f) {
		if (r == l || t == b || f == n) throw std::invalid_argument("Invalid orthographic parameters");
		return Matrix4x4({ 2 / (r - l),0,0,-(r + l) / (r - l), 0,2 / (t - b),0,-(t + b) / (t - b), 0,0,-2 / (f - n),-(f + n) / (f - n), 0,0,0,1 });
	}

	Matrix4x4 Matrix4x4::lookAt(const Point3D& eye, const Point3D& target, const Vector3D& up) {
		// f hướng từ mắt tới tâm; s,u tạo cơ sở trực chuẩn của camera.
		const Vector3D f = (target - eye).normalized();
		const Vector3D s = f.cross(up).normalized();
		const Vector3D u = s.cross(f);
		const Vector3D e{ eye.x, eye.y, eye.z };
		return Matrix4x4({ s.x,s.y,s.z,-s.dot(e), u.x,u.y,u.z,-u.dot(e), -f.x,-f.y,-f.z,f.dot(e), 0,0,0,1 });
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
		Matrix4x4 result;
		for (int r = 0;r < 4;++r) for (int c = 0;c < 4;++c) for (int k = 0;k < 4;++k)
			result.values_[r * 4 + c] += values_[r * 4 + k] * rhs.values_[k * 4 + c];
		return result;
	}

	Point3D Matrix4x4::transformPoint(const Point3D& p) const {
		const double x = values_[0] * p.x + values_[1] * p.y + values_[2] * p.z + values_[3];
		const double y = values_[4] * p.x + values_[5] * p.y + values_[6] * p.z + values_[7];
		const double z = values_[8] * p.x + values_[9] * p.y + values_[10] * p.z + values_[11];
		const double w = values_[12] * p.x + values_[13] * p.y + values_[14] * p.z + values_[15];
		return std::abs(w) > 1e-12 ? Point3D{ x / w,y / w,z / w } : Point3D{ x,y,z };
	}

	Vector3D Matrix4x4::transformVector(const Vector3D& v) const {
		return { values_[0] * v.x + values_[1] * v.y + values_[2] * v.z,
				values_[4] * v.x + values_[5] * v.y + values_[6] * v.z,
				values_[8] * v.x + values_[9] * v.y + values_[10] * v.z };
	}

	std::array<float, 16> Matrix4x4::toColumnMajorFloat() const {
		std::array<float, 16> out{};
		for (int r = 0;r < 4;++r) for (int c = 0;c < 4;++c) out[c * 4 + r] = static_cast<float>(values_[r * 4 + c]);
		return out;
	}

} // namespace rubik
