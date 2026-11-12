#pragma once
#include <array>
#include "Math/Vector3D.h"
namespace rubik { struct PolygonFace { std::array<int,4> vertices{}; Vector3D normal{}; int direction{}; }; }

