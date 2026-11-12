#include "Rubik/RubikMoves.h"

#include <sstream>
#include <stdexcept>

namespace rubik {

	std::string toString(const Move& move) {
		const char names[] = { 'U','D','L','R','F','B' };
		std::string text(1, names[static_cast<int>(move.face)]);
		if (move.quarterTurns == -1) text += "'";
		else if (move.quarterTurns == 2 || move.quarterTurns == -2) text += "2";
		return text;
	}

	std::optional<Move> parseMove(const std::string& token) {
		if (token.empty() || token.size() > 2) return std::nullopt;
		Face face;
		switch (token[0]) {
		case 'U': face = Face::U; break; case 'D': face = Face::D; break;
		case 'L': face = Face::L; break; case 'R': face = Face::R; break;
		case 'F': face = Face::F; break; case 'B': face = Face::B; break;
		default: return std::nullopt;
		}
		int turns = 1;
		if (token.size() == 2) {
			if (token[1] == '\'') turns = -1;
			else if (token[1] == '2') turns = 2;
			else return std::nullopt;
		}
		return Move{ face,turns };
	}

	std::vector<Move> parseAlgorithm(const std::string& text) {
		std::istringstream stream(text);
		std::vector<Move> moves;
		std::string token;
		while (stream >> token) {
			const auto move = parseMove(token);
			if (!move) throw std::invalid_argument("Invalid Rubik move: " + token);
			moves.push_back(*move);
		}
		return moves;
	}

	Move inverse(const Move& move) { return { move.face, std::abs(move.quarterTurns) == 2 ? 2 : -move.quarterTurns }; }
	Axis axisOf(Face f) { return (f == Face::L || f == Face::R) ? Axis::X : (f == Face::U || f == Face::D) ? Axis::Y : Axis::Z; }
	int layerOf(Face f) { return (f == Face::R || f == Face::U || f == Face::F) ? 1 : -1; }
	int signedAxisTurns(const Move& m) {
		const int base = (m.face == Face::R || m.face == Face::U || m.face == Face::F) ? -1 : 1;
		return base * m.quarterTurns;
	}

} // namespace rubik

