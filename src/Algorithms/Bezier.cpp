#include "Algorithms/Bezier.h"
#include <algorithm>
#include <stdexcept>

namespace rubik {
Point3D deCasteljau(const std::vector<Point3D>& points,double t){
    if(points.empty())throw std::invalid_argument("Bezier needs at least one control point");
    t=std::clamp(t,0.0,1.0);auto work=points;
    for(std::size_t level=1;level<work.size();++level)for(std::size_t i=0;i<work.size()-level;++i)
        work[i]={work[i].x*(1-t)+work[i+1].x*t,work[i].y*(1-t)+work[i+1].y*t,work[i].z*(1-t)+work[i+1].z*t};
    return work[0];
}
std::vector<Point3D> sampleBezier(const std::vector<Point3D>& p,int segments){
    if(segments<1)throw std::invalid_argument("Bezier segments must be positive");std::vector<Point3D> out;out.reserve(segments+1);
    for(int i=0;i<=segments;++i)out.push_back(deCasteljau(p,(double)i/segments));return out;
}
}

