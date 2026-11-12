#pragma once
#include <vector>
#include "Rubik/RubikMoves.h"

namespace rubik {
struct MoveHistory {
    std::vector<Move> done;
    std::vector<Move> redo;
    void clear() { done.clear(); redo.clear(); }
};
}

