#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "Rubik/Cubie.h"
#include "Rubik/MoveHistory.h"

namespace rubik {

class RubiksCube {
public:
    RubiksCube();
    void rotateFace(Face face, int quarterTurns);
    void applyMove(const Move& move, bool recordHistory=true);
    void applyAlgorithm(const std::string& algorithm);
    bool isSolved() const;
    bool isValid() const;
    void reset();
    std::vector<Move> scramble(int moves, std::uint32_t seed=0);
    bool undo();
    bool redo();

    const std::vector<Cubie>& cubies() const { return cubies_; }
    const MoveHistory& history() const { return history_; }
    StickerColor colorAt(Face face, int row, int column) const;

private:
    std::vector<Cubie> cubies_;
    MoveHistory history_;
    void rotateDiscrete(Axis axis, int layer, int signedTurns);
};

} // namespace rubik

