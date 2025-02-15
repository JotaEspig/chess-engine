#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "board.hpp"
#include "move.hpp"

struct NodesPerMove {
    Move move;
    uint64_t nodes;
};

std::pair<uint64_t, std::vector<NodesPerMove>> perft(Board &b, uint16_t depth);
