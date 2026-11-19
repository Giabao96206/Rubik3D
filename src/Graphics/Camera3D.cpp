
#include "Graphics/Camera3D.h"
#include "Algorithms/Bezier.h"

#include <algorithm>
#include <cmath>

using namespace std;

namespace rubik {

    namespace {
        constexpr double pi = 3.14159265358979323846;

        double rad(double d) {
            return d * pi / 180.0;
        }
    }

    // ==========================================
    // 1. XOAY CAMERA
    // ==========================================

    void Camera3D::orbit(double dyaw, double dpitch) {
        yawDegrees_ += dyaw;

        pitchDegrees_ = clamp(
            pitchDegrees_ + dpitch,
            -85.0,
            85.0
        );
    }

    // ==========================================
    // 2. PHÓNG TO / THU NHỎ CAMERA
    // ==========================================

    void Camera3D::zoom(double delta) {
        radius_ = clamp(radius_ + delta, 4.2, 16.0);
    }

    // ==========================================
    // 3. ĐẶT LẠI CAMERA
    // ==========================================

    void Camera3D::reset() {
        yawDegrees_ = 40;
        pitchDegrees_ = 25;
        radius_ = 8;
        fovDegrees_ = 45;

        mode_ = ProjectionMode::Perspective;

        tourActive_ = false;
        tourParameter_ = 0;
    }

    // ==========================================
    // 4. BẮT ĐẦU CAMERA TOUR
    // ==========================================

    void Camera3D::startTour() {
        tourStart_ = position();
        tourActive_ = true;
        tourParameter_ = 0;
    }

    // ==========================================
    // 5. CẬP NHẬT CAMERA TOUR
    // ==========================================

    void Camera3D::update(double dt) {
        if (!tourActive_)
            return;

        tourParameter_ += max(0.0, dt) / 8.0;

        if (tourParameter_ >= 1.0) {
            tourParameter_ = 1.0;
            tourActive_ = false;

            const Point3D end{ 7, 3, 7 };

            radius_ = sqrt(
                end.x * end.x +
                end.y * end.y +
                end.z * end.z
            );

            pitchDegrees_ = asin(end.y / radius_) * 180.0 / pi;
            yawDegrees_ = atan2(end.x, end.z) * 180.0 / pi;
        }
    }

    // ==========================================
    // 6. CHUYỂN ĐỔI PHÉP CHIẾU
    // ==========================================

    void Camera3D::toggleProjection() {
        mode_ = mode_ == ProjectionMode::Perspective
            ? ProjectionMode::Orthographic
            : ProjectionMode::Perspective;
    }

    // ==========================================
    // 7. TÍNH VỊ TRÍ CAMERA
    // ==========================================

    Point3D Camera3D::position() const {
        if (tourActive_) {
            return deCasteljau(
                {
                    tourStart_,
                    {-7, 6, 5},
                    {-6, -2, -7},
                    {6, 4, -6},
                    {7, 3, 7}
                },
                tourParameter_
            );
        }

        const double y = rad(yawDegrees_);
        const double p = rad(pitchDegrees_);
        const double cp = cos(p);

        return {
            radius_ * cp * sin(y),
            radius_ * sin(p),
            radius_ * cp * cos(y)
        };
    }

    // ==========================================
    // 8. MA TRẬN QUAN SÁT (VIEW MATRIX)
    // ==========================================

    Matrix4x4 Camera3D::viewMatrix() const {
        return Matrix4x4::lookAt(
            position(),
            { 0, 0, 0 },
            { 0, 1, 0 }
        );
    }

    // ==========================================
    // 9. MA TRẬN PHÉP CHIẾU
    // ==========================================

    Matrix4x4 Camera3D::projectionMatrix(double aspect) const {
        return mode_ == ProjectionMode::Perspective
            ? Matrix4x4::perspective(
                rad(fovDegrees_),
                aspect,
                0.1,
                100
            )
            : Matrix4x4::orthographic(
                -4 * aspect,
                4 * aspect,
                -4,
                4,
                0.1,
                100
            );
    }

} // namespace rubik
