#pragma once
#include <deque>
#include <optional>
#include "Rubik/RubiksCube.h"

namespace rubik {
class RotationAnimator {
public:
    void enqueue(const Move& move);
    void update(double deltaSeconds, RubiksCube& cube);
    void clear();
    bool busy() const { return active_.has_value() || !queue_.empty(); }
    const std::optional<Move>& activeMove() const { return active_; }
    double visualAngleDegrees() const;
    void setDuration(double seconds);
    double duration() const { return duration_; }
private:
    std::deque<Move> queue_;
    std::optional<Move> active_;
    double elapsed_{0.0};
    double duration_{0.24};
};
}

