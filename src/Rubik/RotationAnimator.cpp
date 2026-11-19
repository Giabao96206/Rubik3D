
#include "Rubik/RotationAnimator.h"

#include <algorithm>
#include <cmath>

using namespace std;

namespace rubik {

    // ==========================================
    // 1. THÊM LỆNH XOAY VÀO HÀNG ĐỢI
    // ==========================================

    void RotationAnimator::enqueue(const Move& m) {
        queue_.push_back(m);
    }

    // ==========================================
    // 2. THIẾT LẬP THỜI GIAN XOAY
    // ==========================================

    void RotationAnimator::setDuration(double s) {
        duration_ = clamp(s, 0.05, 2.0);
    }

    // ==========================================
    // 3. XÓA HÀNG ĐỢI VÀ TRẠNG THÁI XOAY
    // ==========================================

    void RotationAnimator::clear() {
        queue_.clear();
        active_.reset();
        elapsed_ = 0;
    }

    // ==========================================
    // 4. TÍNH GÓC XOAY HIỂN THỊ
    // ==========================================

    double RotationAnimator::visualAngleDegrees() const {
        if (!active_)
            return 0;

        double t = clamp(elapsed_ / duration_, 0.0, 1.0);

        // Smoothstep: vận tốc đầu và cuối bằng 0
        t = t * t * (3.0 - 2.0 * t);

        return 90.0 *
            static_cast<double>(signedAxisTurns(*active_)) * t;
    }

    // ==========================================
    // 5. CẬP NHẬT HOẠT ẢNH XOAY RUBIK
    // ==========================================

    void RotationAnimator::update(double dt, RubiksCube& cube) {

        // Lấy lệnh xoay tiếp theo từ hàng đợi
        if (!active_ && !queue_.empty()) {
            active_ = queue_.front();
            queue_.pop_front();
            elapsed_ = 0;
        }

        // Không có lệnh xoay thì thoát
        if (!active_)
            return;

        // Cập nhật thời gian hoạt ảnh
        elapsed_ += max(0.0, dt);

        // Hoàn thành hoạt ảnh
        if (elapsed_ >= duration_) {
            cube.applyMove(*active_);
            active_.reset();
            elapsed_ = 0;
        }
    }

} // namespace rubik
