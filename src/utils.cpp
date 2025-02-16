#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

int getPieceIdx(char c) {
    c = std::tolower(c);
    int result = -1;
    switch (c) {
    case 'p':
        result = 0;
        break;
    case 'n':
        result = 1;
        break;
    case 'b':
        result = 2;
        break;
    case 'r':
        result = 3;
        break;
    case 'q':
        result = 4;
        break;
    case 'k':
        result = 5;
        break;
    default:
        break;
    }
    return result;
}

char getPieceChar(int i, bool isWhite) {
    char result = ' ';
    switch (i) {
    case 0:
        result = 'p';
        break;
    case 1:
        result = 'n';
        break;
    case 2:
        result = 'b';
        break;
    case 3:
        result = 'r';
        break;
    case 4:
        result = 'q';
        break;
    case 5:
        result = 'k';
        break;
    default:
        break;
    }
    if (isWhite) {
        return std::toupper(result);
    }
    return result;
}

uint64_t lsb(uint64_t i) { return i & -i; }

#if defined(_MSC_VER)
int countZeros(unsigned long long x) {
    unsigned long result;
    _BitScanForward64(&result, x);
    return result;
}
#else
int countZeros(unsigned long long x) { return __builtin_ctzll(x); }
#endif

void initZobristKeys(uint64_t keys[5][6][64]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 64; k++) {
                keys[i][j][k] = 0ULL;
                for (int l = 0; l < 64; l++) {
                    keys[i][j][k] |= (uint64_t{(uint64_t)rand()} & 1) << l;
                }
            }
        }
    }
}
