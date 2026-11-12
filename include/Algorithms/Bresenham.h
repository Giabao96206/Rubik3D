#pragma once
#include <vector>

namespace rubik {
struct PixelPoint { int x{}; int y{}; bool operator==(const PixelPoint& p) const {return x==p.x&&y==p.y;} };
std::vector<PixelPoint> bresenhamLine(PixelPoint start, PixelPoint end);
}

