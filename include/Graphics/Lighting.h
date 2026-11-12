#pragma once
#include <array>
#include "Math/Vector3D.h"
namespace rubik {
struct Light { Point3D position{4,6,5}; double ambient{0.34}; double diffuse{0.82}; };
double lambertIntensity(const Vector3D& normal,const Point3D& point,const Light& light,double ka=1.0,double kd=1.0);
std::array<float,3> shadeColor(std::array<float,3> base,double intensity);
bool isFrontFacing(const Point3D& a,const Point3D& b,const Point3D& c,const Point3D& eye);
}

