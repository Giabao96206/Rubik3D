#include "TestHarness.h"
#include "Rubik/RubiksCube.h"
#include "Rubik/RotationAnimator.h"

using namespace rubik;
TEST_CASE("Solved cube has 27 cubies and valid stickers"){RubiksCube c;REQUIRE(c.cubies().size()==27);REQUIRE(c.isSolved());REQUIRE(c.isValid());}
TEST_CASE("U four times restores state"){RubiksCube c;for(int i=0;i<4;++i)c.rotateFace(Face::U,1);REQUIRE(c.isSolved());REQUIRE(c.isValid());}
TEST_CASE("R then inverse restores state"){RubiksCube c;c.applyAlgorithm("R R'");REQUIRE(c.isSolved());}
TEST_CASE("F2 twice restores state"){RubiksCube c;c.applyAlgorithm("F2 F2");REQUIRE(c.isSolved());}
TEST_CASE("Middle slices return after four quarter turns"){RubiksCube c;c.applyAlgorithm("M M M M E E E E S S S S");REQUIRE(c.isSolved());REQUIRE(c.isValid());}
TEST_CASE("Middle slice inverse and double moves are exact"){RubiksCube c;c.applyAlgorithm("M M' E2 E2 S S'");REQUIRE(c.isSolved());REQUIRE(c.isValid());}
TEST_CASE("M rotates only the visible cubies around the center core"){
    RubiksCube c;const auto before=c.cubies();c.rotateFace(Face::M,1);int changedVisible=0;
    for(std::size_t i=0;i<c.cubies().size();++i){const auto& oldCubie=before[i];const auto& newCubie=c.cubies()[i];if(oldCubie.home.x==0&&oldCubie.visible){if(!(oldCubie.position==newCubie.position)||oldCubie.stickers!=newCubie.stickers)++changedVisible;}else if(oldCubie.home.x!=0){REQUIRE(oldCubie.position==newCubie.position);REQUIRE(oldCubie.stickers==newCubie.stickers);}}
    REQUIRE(changedVisible==8);REQUIRE(c.isValid());
}
TEST_CASE("Algorithm and inverse restore state"){
    RubiksCube c;const auto moves=parseAlgorithm("R U F2 L D' B");for(const auto&m:moves)c.applyMove(m);for(auto it=moves.rbegin();it!=moves.rend();++it)c.applyMove(inverse(*it));REQUIRE(c.isSolved());REQUIRE(c.isValid());
}
TEST_CASE("Undo redo are exact transactions"){RubiksCube c;c.applyAlgorithm("R U");REQUIRE(c.undo());REQUIRE(c.undo());REQUIRE(c.isSolved());REQUIRE(c.redo());REQUIRE(!c.isSolved());}
TEST_CASE("Scramble preserves cubies and stickers"){RubiksCube c;c.scramble(50,42);REQUIRE(c.isValid());REQUIRE(c.cubies().size()==27);}
TEST_CASE("Animation commits logic only at transaction end"){
    RubiksCube c;RotationAnimator animator;animator.setDuration(.2);animator.enqueue({Face::R,1});animator.update(.05,c);REQUIRE(c.isSolved());animator.update(.2,c);REQUIRE(!c.isSolved());REQUIRE(c.history().done.size()==1);
}

