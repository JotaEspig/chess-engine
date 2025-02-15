#pragma once

#include <cstdint>
#include <string>

struct Move {
    uint64_t from = 0;
    uint64_t to = 0;
    bool isCastling = false;
    int capturedPiece = -1;
    int promotionPiece = -1;
    bool isCheck = false;

    static Move fromString(std::string s);

    std::string stringify() const;
};
