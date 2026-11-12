#pragma once
#include <vector>
#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"
#include "Geometry/Face.h"

namespace rubik {
struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<PolygonFace> faces;
    static Mesh unitCube();
};
}

