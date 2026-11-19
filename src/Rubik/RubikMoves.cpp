
#include "Rubik/RubikMoves.h"

#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <cmath>

using namespace std;

namespace rubik {

    // ==========================================
    // 1. CHUYỂN LỆNH XOAY THÀNH CHUỖI
    // ==========================================

    string toString(const Move& move) {
        const char names[] = {
            'U', 'D', 'L', 'R', 'F', 'B', 'M', 'E', 'S', 'x', 'y'
        };

        string text(1, names[static_cast<int>(move.face)]);

        if (move.quarterTurns == -1) {
            text += "'";
        }
        else if (move.quarterTurns == 2 || move.quarterTurns == -2) {
            text += "2";
        }

        return text;
    }

    // ==========================================
    // 2. PHÂN TÍCH MỘT LỆNH XOAY
    // ==========================================

    optional<Move> parseMove(const string& token) {
        if (token.empty() || token.size() > 2)
            return nullopt;

        Face face;

        switch (toupper(static_cast<unsigned char>(token[0]))) {
        case 'U':
            face = Face::U;
            break;

        case 'D':
            face = Face::D;
            break;

        case 'L':
            face = Face::L;
            break;

        case 'R':
            face = Face::R;
            break;

        case 'F':
            face = Face::F;
            break;

        case 'B':
            face = Face::B;
            break;

        case 'M':
            face = Face::M;
            break;

        case 'E':
            face = Face::E;
            break;

        case 'S':
            face = Face::S;
            break;

        case 'X':
            face = Face::X;
            break;

        case 'Y':
            face = Face::Y;
            break;

        default:
            return nullopt;
        }

        int turns = 1;

        if (token.size() == 2) {
            if (token[1] == '\'') {
                turns = -1;
            }
            else if (token[1] == '2') {
                turns = 2;
            }
            else {
                return nullopt;
            }
        }

        return Move{ face, turns };
    }

    // ==========================================
    // 3. PHÂN TÍCH CHUỖI THUẬT TOÁN RUBIK
    // ==========================================

    vector<Move> parseAlgorithm(const string& text) {
        vector<Move> moves;
        for (size_t i = 0; i < text.size();) {
            if (isspace(static_cast<unsigned char>(text[i]))) {
                ++i;
                continue;
            }
            const size_t start = i++;
            if (i < text.size() && (text[i] == '\'' || text[i] == '2')) {
                ++i;
            }
            const string token = text.substr(start, i - start);
            const auto move = parseMove(token);

            if (!move) {
                throw invalid_argument(
                    "Invalid Rubik move: " + token
                );
            }

            moves.push_back(*move);
        }

        return moves;
    }

    // ==========================================
    // 4. TÌM LỆNH XOAY NGƯỢC
    // ==========================================

    Move inverse(const Move& move) {
        return {
            move.face,
            abs(move.quarterTurns) == 2
                ? 2
                : -move.quarterTurns
        };
    }

    // ==========================================
    // 5. XÁC ĐỊNH TRỤC XOAY
    // ==========================================

    Axis axisOf(Face f) {
        if (f == Face::L || f == Face::R || f == Face::M || f == Face::X)
            return Axis::X;

        if (f == Face::U || f == Face::D || f == Face::E || f == Face::Y)
            return Axis::Y;

        return Axis::Z;
    }

    // ==========================================
    // 6. XÁC ĐỊNH LỚP XOAY
    // ==========================================

    int layerOf(Face f) {
        if (f == Face::M || f == Face::E || f == Face::S)
            return 0;

        return (f == Face::R || f == Face::U || f == Face::F)
            ? 1
            : -1;
    }

    bool isWholeCubeRotation(Face face) {
        return face == Face::X || face == Face::Y;
    }

    // ==========================================
    // 7. XÁC ĐỊNH CHIỀU XOAY THEO TRỤC
    // ==========================================

    int signedAxisTurns(const Move& m) {
        // M quay như L, E quay như D,
        // S quay như F theo ký hiệu Rubik chuẩn.

        const int base =
            (m.face == Face::R ||
                m.face == Face::U ||
                m.face == Face::F ||
                m.face == Face::S ||
                isWholeCubeRotation(m.face))
            ? -1
            : 1;

        return base * m.quarterTurns;
    }

} // namespace rubik
