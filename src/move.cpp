#include <string>

#include "move.hpp"

#if defined(_MSC_VER)
int __builtin_ctz(unsigned long long x) {
    unsigned long result;
    _BitScanForward64(&result, x);
    return result;
}
#endif

std::string Move::stringfy() const {
    std::string result = "";
    int fromCount = __builtin_ctz(from);
    result += 'a' + ((fromCount) % 8);
    result += '1' + ((fromCount) / 8);
    int toCount = __builtin_ctz(to);
    result += 'a' + ((toCount) % 8);
    result += '1' + ((toCount) / 8);
    return result;
}