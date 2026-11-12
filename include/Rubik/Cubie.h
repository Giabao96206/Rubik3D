#pragma once
#include <array>

namespace rubik {

enum class StickerColor { None, White, Yellow, Red, Orange, Green, Blue };
enum class Direction { PosX=0, NegX=1, PosY=2, NegY=3, PosZ=4, NegZ=5 };

struct Int3 {
    int x{}; int y{}; int z{};
    bool operator==(const Int3& p) const { return x==p.x && y==p.y && z==p.z; }
};

struct Cubie {
    int id{};
    Int3 home{};
    Int3 position{};
    std::array<StickerColor,6> stickers{};
    std::array<StickerColor,6> homeStickers{};
    bool visible{true};
};

} // namespace rubik

