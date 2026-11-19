
#include "Math/Matrix3x3.h"

#include <cmath>
#include <array>

using namespace std;

namespace rubik {

    Matrix3x3::Matrix3x3() = default;

    Matrix3x3::Matrix3x3(const array<double, 9>& values)
        : values_(values) {
    }

    // ==========================================
    // 1. MA TRẬN ĐƠN VỊ
    // ==========================================

    Matrix3x3 Matrix3x3::identity() {
        return Matrix3x3({
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
            });
    }

    // ==========================================
    // 2. MA TRẬN TỊNH TIẾN
    // ==========================================

    Matrix3x3 Matrix3x3::translation(double tx, double ty) {
        return Matrix3x3({
            1, 0, tx,
            0, 1, ty,
            0, 0, 1
            });
    }

    // ==========================================
    // 3. MA TRẬN TỶ LỆ
    // ==========================================

    Matrix3x3 Matrix3x3::scaling(double sx, double sy) {
        return Matrix3x3({
            sx, 0,  0,
            0,  sy, 0,
            0,  0,  1
            });
    }

    // ==========================================
    // 4. MA TRẬN QUAY
    // ==========================================

    Matrix3x3 Matrix3x3::rotation(double radians) {
        const double c = cos(radians);
        const double s = sin(radians);

        return Matrix3x3({
             c, -s, 0,
             s,  c, 0,
             0,  0, 1
            });
    }

    // ==========================================
    // 5. PHÉP NHÂN HAI MA TRẬN 3x3
    // ==========================================

    Matrix3x3 Matrix3x3::operator*(const Matrix3x3& rhs) const {
        Matrix3x3 result;

        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                for (int k = 0; k < 3; ++k) {
                    result.values_[r * 3 + c] +=
                        values_[r * 3 + k] *
                        rhs.values_[k * 3 + c];
                }
            }
        }

        return result;
    }

    // ==========================================
    // 6. BIẾN ĐỔI ĐIỂM 2D
    // ==========================================

    Point2D Matrix3x3::transformPoint(const Point2D& p) const {
        const double x =
            values_[0] * p.x +
            values_[1] * p.y +
            values_[2];

        const double y =
            values_[3] * p.x +
            values_[4] * p.y +
            values_[5];

        const double w =
            values_[6] * p.x +
            values_[7] * p.y +
            values_[8];

        return abs(w) > 1e-12
            ? Point2D{ x / w, y / w }
        : Point2D{ x, y };
    }

} // namespace rubik
