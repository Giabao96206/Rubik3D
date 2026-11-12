#pragma once
#include <vector>
#include "Math/Vector3D.h"

namespace rubik {
Point3D deCasteljau(const std::vector<Point3D>& controlPoints,double t);
std::vector<Point3D> sampleBezier(const std::vector<Point3D>& controlPoints,int segments);
}

