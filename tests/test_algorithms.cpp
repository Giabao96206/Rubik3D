#include "TestHarness.h"
#include "Algorithms/Bezier.h"
#include "Algorithms/Bresenham.h"
#include "Algorithms/CohenSutherland.h"
#include "Algorithms/ScanlineFill.h"
#include "Graphics/Lighting.h"
#include "Graphics/Camera3D.h"

#include <algorithm>
using namespace rubik;
TEST_CASE("Bresenham supports every octant"){
    const PixelPoint origin{0,0};const PixelPoint ends[]={{5,2},{2,5},{-2,5},{-5,2},{-5,-2},{-2,-5},{2,-5},{5,-2}};
    for(const auto&e:ends){const auto p=bresenhamLine(origin,e);REQUIRE(p.front()==origin);REQUIRE(p.back()==e);REQUIRE(p.size()==static_cast<std::size_t>(std::max(std::abs(e.x),std::abs(e.y))+1));}
}
TEST_CASE("Cohen Sutherland accepts rejects and clips"){
    const Rect2D r{0,0,10,10};REQUIRE(cohenSutherlandClip({{2,2},{8,8}},r).has_value());REQUIRE(!cohenSutherlandClip({{-3,2},{-1,8}},r));
    const auto c=cohenSutherlandClip({{-5,5},{15,5}},r);REQUIRE(c.has_value());REQUIRE_NEAR(c->a.x,0,1e-9);REQUIRE_NEAR(c->b.x,10,1e-9);
}
TEST_CASE("Scanline fills a rectangle"){
    const auto p=scanlineFill({{1,1},{5,1},{5,4},{1,4}});REQUIRE(!p.empty());REQUIRE(std::find(p.begin(),p.end(),PixelPoint{3,2})!=p.end());
}
TEST_CASE("de Casteljau keeps endpoints and midpoint"){
    const std::vector<Point3D> p={{0,0,0},{1,2,0},{2,0,0}};auto a=deCasteljau(p,0),b=deCasteljau(p,1),m=deCasteljau(p,.5);REQUIRE_NEAR(a.x,0,1e-9);REQUIRE_NEAR(b.x,2,1e-9);REQUIRE_NEAR(m.x,1,1e-9);REQUIRE_NEAR(m.y,1,1e-9);
}
TEST_CASE("Lambert intensity stays normalized"){
    Light l;l.position={0,0,10};const double lit=lambertIntensity({0,0,1},{0,0,0},l);const double dark=lambertIntensity({0,0,-1},{0,0,0},l);REQUIRE(lit<=1&&lit>dark);REQUIRE(dark>=0);
}
TEST_CASE("Bezier camera tour starts continuously and ends at its endpoint"){
    Camera3D camera;const Point3D before=camera.position();camera.startTour();const Point3D start=camera.position();REQUIRE_NEAR(start.x,before.x,1e-9);REQUIRE_NEAR(start.y,before.y,1e-9);REQUIRE_NEAR(start.z,before.z,1e-9);camera.update(8.1);const Point3D end=camera.position();REQUIRE(!camera.tourActive());REQUIRE_NEAR(end.x,7,1e-9);REQUIRE_NEAR(end.y,3,1e-9);REQUIRE_NEAR(end.z,7,1e-9);
}
