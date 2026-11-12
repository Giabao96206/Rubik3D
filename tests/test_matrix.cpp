#include "TestHarness.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"

using namespace rubik;
namespace {constexpr double pi=3.14159265358979323846;}

TEST_CASE("Matrix identity and translation"){
    const Point3D p{1,2,3};const auto i=Matrix4x4::identity().transformPoint(p);
    REQUIRE_NEAR(i.x,1,1e-9);REQUIRE_NEAR(i.y,2,1e-9);REQUIRE_NEAR(i.z,3,1e-9);
    const auto q=Matrix4x4::translation(4,-2,1).transformPoint(p);
    REQUIRE_NEAR(q.x,5,1e-9);REQUIRE_NEAR(q.y,0,1e-9);REQUIRE_NEAR(q.z,4,1e-9);
}
TEST_CASE("Matrix rotations 90 degrees"){
    auto x=Matrix4x4::rotationX(pi/2).transformPoint({0,1,0});REQUIRE_NEAR(x.z,1,1e-9);
    auto y=Matrix4x4::rotationY(pi/2).transformPoint({0,0,1});REQUIRE_NEAR(y.x,1,1e-9);
    auto z=Matrix4x4::rotationZ(pi/2).transformPoint({1,0,0});REQUIRE_NEAR(z.y,1,1e-9);
}
TEST_CASE("Combined transform follows T R S P"){
    const auto m=Matrix4x4::translation(5,0,0)*Matrix4x4::rotationZ(pi/2)*Matrix4x4::scaling(2,2,2);
    const auto q=m.transformPoint({1,0,0});REQUIRE_NEAR(q.x,5,1e-9);REQUIRE_NEAR(q.y,2,1e-9);
}
TEST_CASE("2D homogeneous transform"){
    const auto m=Matrix3x3::translation(2,3)*Matrix3x3::rotation(pi/2);
    const auto p=m.transformPoint({1,0});REQUIRE_NEAR(p.x,2,1e-9);REQUIRE_NEAR(p.y,4,1e-9);
}

