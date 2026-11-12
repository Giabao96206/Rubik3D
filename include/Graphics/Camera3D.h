#pragma once
#include "Graphics/Projection.h"
#include "Math/Matrix4x4.h"

namespace rubik {
class Camera3D {
public:
    void orbit(double deltaYawDegrees,double deltaPitchDegrees);
    void zoom(double delta);
    void reset();
    void startTour();
    void update(double deltaSeconds);
    void toggleProjection();
    Matrix4x4 viewMatrix()const;
    Matrix4x4 projectionMatrix(double aspect)const;
    Point3D position()const;
    double yaw()const{return yawDegrees_;} double pitch()const{return pitchDegrees_;}
    double radius()const{return radius_;} double fov()const{return fovDegrees_;}
    ProjectionMode projectionMode()const{return mode_;}
    bool tourActive()const{return tourActive_;}
private:
    double yawDegrees_{40},pitchDegrees_{25},radius_{8},fovDegrees_{45};
    ProjectionMode mode_{ProjectionMode::Perspective};
    bool tourActive_{false};
    double tourParameter_{0.0};
    Point3D tourStart_{};
};
}

