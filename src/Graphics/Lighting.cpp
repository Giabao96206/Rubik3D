
#include "Graphics/Lighting.h"
#include <algorithm>
#include <array>

using namespace std;

namespace rubik {

    // ==========================================
    // 1. TÍNH CƯỜNG ĐỘ ÁNH SÁNG LAMBERT
    // ==========================================

    double lambertIntensity(
        const Vector3D& n,
        const Point3D& p,
        const Light& l,
        double ka,
        double kd
    ) {
        const Vector3D L = (l.position - p).normalized();

        return clamp(
            l.ambient * ka +
            l.diffuse * kd * max(0.0, n.normalized().dot(L)),
            0.0,
            1.0
        );
    }

    // ==========================================
    // 2. TÍNH MÀU SAU KHI CHIẾU SÁNG
    // ==========================================

    array<float, 3> shadeColor(array<float, 3> b, double i) {
        return {
            static_cast<float>(b[0] * i),
            static_cast<float>(b[1] * i),
            static_cast<float>(b[2] * i)
        };
    }

    // ==========================================
    // 3. XÁC ĐỊNH MẶT TRƯỚC CỦA ĐA GIÁC
    // ==========================================

    bool isFrontFacing(
        const Point3D& a,
        const Point3D& b,
        const Point3D& c,
        const Point3D& eye
    ) {
        const Vector3D n = (b - a).cross(c - a);

        return n.dot(eye - a) >= 0;
    }

} // namespace rubik
