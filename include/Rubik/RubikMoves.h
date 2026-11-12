#pragma once
#include <optional>
#include <string>
#include <vector>

namespace rubik {

// M/E/S là ba lát giữa, lần lượt song song với L/R, U/D và F/B.
enum class Face { U, D, L, R, F, B, M, E, S };
enum class Axis { X, Y, Z };

struct Move {
    Face face{Face::U};
    int quarterTurns{1}; // 1=thuận chiều kim đồng hồ khi nhìn trực diện mặt; -1=ngược; 2=nửa vòng.
    bool operator==(const Move& other) const { return face == other.face && quarterTurns == other.quarterTurns; }
};

std::string toString(const Move& move);
std::optional<Move> parseMove(const std::string& token);
std::vector<Move> parseAlgorithm(const std::string& text);
Move inverse(const Move& move);
Axis axisOf(Face face);
int layerOf(Face face);
int signedAxisTurns(const Move& move);

} // namespace rubik

