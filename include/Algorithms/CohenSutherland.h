#pragma once
#include <optional>
#include "Math/Vector2D.h"

namespace rubik {
struct Rect2D { double xmin{},ymin{},xmax{},ymax{}; };
struct LineSegment2D { Point2D a,b; };
std::optional<LineSegment2D> cohenSutherlandClip(LineSegment2D line,const Rect2D& window);
}

