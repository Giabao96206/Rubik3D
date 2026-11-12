#include "Graphics/Lighting.h"
#include <algorithm>

namespace rubik {
double lambertIntensity(const Vector3D& n,const Point3D&p,const Light& l,double ka,double kd){const Vector3D L=(l.position-p).normalized();return std::clamp(l.ambient*ka+l.diffuse*kd*std::max(0.0,n.normalized().dot(L)),0.0,1.0);}
std::array<float,3> shadeColor(std::array<float,3>b,double i){return {static_cast<float>(b[0]*i),static_cast<float>(b[1]*i),static_cast<float>(b[2]*i)};}
bool isFrontFacing(const Point3D&a,const Point3D&b,const Point3D&c,const Point3D&eye){const Vector3D n=(b-a).cross(c-a);return n.dot(eye-a)>=0;}
}
