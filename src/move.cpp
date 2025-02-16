#include <cassert>
#include <stdexcept>
#include <string>

#include "consts.hpp"
#include "move.hpp"
#include "utils.hpp"

Move Move::fromString(std::string s) {
    if (s.length() < 4) {
        throw std::invalid_argument("Invalid move string");
    }
    if (s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8' || s[2] < 'a' ||
        s[2] > 'h' || s[3] < '1' || s[3] > '8') {
        throw std::invalid_argument("Invalid move string");
    }

    if (s.length() == 5) {
        std::string promotion = "nbrq";
        uint64_t from = 1ULL << (8 * (s[1] - '1') + (7 - s[0] + 'a'));
        uint64_t to = 1ULL << (8 * (s[3] - '1') + (7 - s[2] + 'a'));
        int promotionType = promotion.find(s[4]) + KNIGHT;
        return Move{from, to, false, promotionType};
    }
    uint64_t from = 1ULL << (8 * (s[1] - '1') + (7 - s[0] + 'a'));
    uint64_t to = 1ULL << (8 * (s[3] - '1') + (7 - s[2] + 'a'));
    return Move{from, to};
}

std::string Move::stringify() const {
    std::string result = "";
    int fromCount = countZeros(from);
    result += 'a' + 7 - ((fromCount) % 8);
    result += '1' + ((fromCount) / 8);
    int toCount = countZeros(to);
    result += 'a' + 7 - ((toCount) % 8);
    result += '1' + ((toCount) / 8);
    if (promotionPiece != -1) {
        result += "nbrq"[promotionPiece - KNIGHT]; // nasty thing lol
    }
    return result;
}
