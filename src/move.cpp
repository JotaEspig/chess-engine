#include <string>

#include "move.hpp"

#if defined(_MSC_VER)
int __builtin_ctz(unsigned long long x) {
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
    int fromCount = __builtin_ctz(from);
    result += 'a' + 7 - ((fromCount) % 8);
    result += '1' + ((fromCount) / 8);
    int toCount = __builtin_ctz(to);
    result += 'a' + 7 - ((toCount) % 8);
    result += '1' + ((toCount) / 8);
    return result;
}