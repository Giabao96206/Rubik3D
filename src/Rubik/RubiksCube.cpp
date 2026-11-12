#include "Rubik/RubiksCube.h"

#include <algorithm>
#include <array>
#include <random>
#include <set>
#include <stdexcept>

namespace rubik {
namespace {
int axisValue(const Int3& p, Axis a) { return a==Axis::X?p.x:a==Axis::Y?p.y:p.z; }
Int3 rotateOnce(Int3 p, Axis axis, int sign) {
    if (axis==Axis::X) return sign>0 ? Int3{p.x,-p.z,p.y} : Int3{p.x,p.z,-p.y};
    if (axis==Axis::Y) return sign>0 ? Int3{p.z,p.y,-p.x} : Int3{-p.z,p.y,p.x};
    return sign>0 ? Int3{-p.y,p.x,p.z} : Int3{p.y,-p.x,p.z};
}
Int3 directionVector(Direction d) {
    switch(d) {
        case Direction::PosX:return {1,0,0}; case Direction::NegX:return {-1,0,0};
        case Direction::PosY:return {0,1,0}; case Direction::NegY:return {0,-1,0};
        case Direction::PosZ:return {0,0,1}; case Direction::NegZ:return {0,0,-1};
    }
    return {};
}
Direction vectorDirection(const Int3& p) {
    if(p.x==1)return Direction::PosX; if(p.x==-1)return Direction::NegX;
    if(p.y==1)return Direction::PosY; if(p.y==-1)return Direction::NegY;
    if(p.z==1)return Direction::PosZ; return Direction::NegZ;
}
Direction outward(Face f) {
    switch(f){case Face::R:return Direction::PosX;case Face::L:return Direction::NegX;
      case Face::U:return Direction::PosY;case Face::D:return Direction::NegY;
      case Face::F:return Direction::PosZ;case Face::B:return Direction::NegZ;}
    return Direction::PosY;
}
}

RubiksCube::RubiksCube() { reset(); }

void RubiksCube::reset() {
    cubies_.clear();
    int id=0;
    for(int x=-1;x<=1;++x) for(int y=-1;y<=1;++y) for(int z=-1;z<=1;++z) {
        Cubie c; c.id=id++; c.home=c.position={x,y,z}; c.visible=(x||y||z);
        c.stickers.fill(StickerColor::None);
        if(x== 1)c.stickers[0]=StickerColor::Red;
        if(x==-1)c.stickers[1]=StickerColor::Orange;
        if(y== 1)c.stickers[2]=StickerColor::White;
        if(y==-1)c.stickers[3]=StickerColor::Yellow;
        if(z== 1)c.stickers[4]=StickerColor::Green;
        if(z==-1)c.stickers[5]=StickerColor::Blue;
        c.homeStickers=c.stickers;
        cubies_.push_back(c);
    }
    history_.clear();
}

void RubiksCube::rotateDiscrete(Axis axis, int layer, int signedTurns) {
    int turns=signedTurns%4; if(turns<0)turns+=4;
    for(int t=0;t<turns;++t) for(auto& c:cubies_) if(axisValue(c.position,axis)==layer) {
        c.position=rotateOnce(c.position,axis,1);
        std::array<StickerColor,6> next{}; next.fill(StickerColor::None);
        for(int i=0;i<6;++i) if(c.stickers[i]!=StickerColor::None) {
            const Direction target=vectorDirection(rotateOnce(directionVector(static_cast<Direction>(i)),axis,1));
            next[static_cast<int>(target)]=c.stickers[i];
        }
        c.stickers=next;
    }
}

void RubiksCube::rotateFace(Face face, int quarterTurns) { applyMove({face,quarterTurns}); }

void RubiksCube::applyMove(const Move& move, bool record) {
    if(move.quarterTurns==0 || std::abs(move.quarterTurns)>2) throw std::invalid_argument("quarterTurns must be -2..2 excluding 0");
    rotateDiscrete(axisOf(move.face),layerOf(move.face),signedAxisTurns(move));
    if(record) { history_.done.push_back(move); history_.redo.clear(); }
}

void RubiksCube::applyAlgorithm(const std::string& algorithm) { for(const auto& m:parseAlgorithm(algorithm)) applyMove(m); }

bool RubiksCube::isSolved() const {
    return std::all_of(cubies_.begin(),cubies_.end(),[](const Cubie& c){return c.position==c.home && c.stickers==c.homeStickers;});
}

bool RubiksCube::isValid() const {
    if(cubies_.size()!=27)return false;
    std::set<std::tuple<int,int,int>> positions;
    std::array<int,7> counts{};
    for(const auto& c:cubies_) {
        if(std::abs(c.position.x)>1||std::abs(c.position.y)>1||std::abs(c.position.z)>1)return false;
        positions.emplace(c.position.x,c.position.y,c.position.z);
        for(auto color:c.stickers) ++counts[static_cast<int>(color)];
    }
    if(positions.size()!=27)return false;
    for(int i=1;i<=6;++i) if(counts[i]!=9)return false;
    return true;
}

std::vector<Move> RubiksCube::scramble(int count, std::uint32_t seed) {
    if(count<0)throw std::invalid_argument("Scramble count cannot be negative");
    std::mt19937 rng(seed?seed:std::random_device{}());
    std::uniform_int_distribution<int> faceDist(0,5), turnDist(0,2);
    const int turns[]={1,-1,2}; std::vector<Move> result;
    Face previous=Face::U; bool hasPrevious=false;
    while(static_cast<int>(result.size())<count){
        Move m{static_cast<Face>(faceDist(rng)),turns[turnDist(rng)]};
        if(hasPrevious && axisOf(m.face)==axisOf(previous))continue;
        applyMove(m); result.push_back(m); previous=m.face; hasPrevious=true;
    }
    return result;
}

bool RubiksCube::undo() {
    if(history_.done.empty())return false;
    const Move m=history_.done.back(); history_.done.pop_back();
    applyMove(inverse(m),false); history_.redo.push_back(m); return true;
}
bool RubiksCube::redo() {
    if(history_.redo.empty())return false;
    const Move m=history_.redo.back(); history_.redo.pop_back();
    applyMove(m,false); history_.done.push_back(m); return true;
}

StickerColor RubiksCube::colorAt(Face face,int row,int column) const {
    if(row<0||row>2||column<0||column>2)throw std::out_of_range("Net coordinates");
    Int3 wanted{};
    switch(face) {
      case Face::F:wanted={column-1,1-row,1};break; case Face::B:wanted={1-column,1-row,-1};break;
      case Face::U:wanted={column-1,1,row-1};break; case Face::D:wanted={column-1,-1,1-row};break;
      case Face::R:wanted={1,1-row,1-column};break; case Face::L:wanted={-1,1-row,column-1};break;
    }
    const auto it=std::find_if(cubies_.begin(),cubies_.end(),[&](const Cubie& c){return c.position==wanted;});
    return it==cubies_.end()?StickerColor::None:it->stickers[static_cast<int>(outward(face))];
}

} // namespace rubik

