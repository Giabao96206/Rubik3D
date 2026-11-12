#include "Rubik/RotationAnimator.h"
#include <algorithm>
#include <cmath>

namespace rubik {
void RotationAnimator::enqueue(const Move& m){queue_.push_back(m);}
void RotationAnimator::setDuration(double s){duration_=std::clamp(s,0.05,2.0);}
void RotationAnimator::clear(){queue_.clear();active_.reset();elapsed_=0;}
double RotationAnimator::visualAngleDegrees() const {
    if(!active_)return 0;
    double t=std::clamp(elapsed_/duration_,0.0,1.0);
    t=t*t*(3.0-2.0*t); // smoothstep: vận tốc đầu/cuối bằng 0.
    return 90.0*static_cast<double>(signedAxisTurns(*active_))*t;
}
void RotationAnimator::update(double dt,RubiksCube& cube){
    if(!active_&&!queue_.empty()){active_=queue_.front();queue_.pop_front();elapsed_=0;}
    if(!active_)return;
    elapsed_+=std::max(0.0,dt);
    if(elapsed_>=duration_){cube.applyMove(*active_);active_.reset();elapsed_=0;}
}
} // namespace rubik
