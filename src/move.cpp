#include <string>

#include "consts.hpp"
#include "move.hpp"

#if defined(_MSC_VER)
int __builtin_ctzll(unsigned long long x) {
    unsigned long result;
    _BitScanForward64(&result, x);
    return result;
}
#endif

Move Move::fromString(std::string s) {
    uint64_t from = 1ULL << (8 * (s[1] - '1') + (7 - s[0] + 'a'));
    uint64_t to = 1ULL << (8 * (s[3] - '1') + (7 - s[2] + 'a'));
    return Move{from, to};
}

std::string Move::stringify() const {
    std::string result = "";
    int fromCount = __builtin_ctzll(from);
    result += 'a' + 7 - ((fromCount) % 8);
    result += '1' + ((fromCount) / 8);
    int toCount = __builtin_ctzll(to);
    result += 'a' + 7 - ((toCount) % 8);
    result += '1' + ((toCount) / 8);
    if (promotionPiece != -1) {
        result += "nbrq"[promotionPiece - KNIGHT]; // nasty thing lol
    }
    return result;
}
