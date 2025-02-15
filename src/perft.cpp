#include <cstdint>
#include <utility>

#include "board.hpp"
#include "move.hpp"
#include "perft.hpp"

std::pair<uint64_t, std::vector<NodesPerMove>> perft(Board &b, uint16_t depth) {
    std::vector<NodesPerMove> nodesPerMoveVec;
    if (depth == 0) {
        return std::make_pair(1, nodesPerMoveVec);
    }

    uint64_t nodes = 0;
    auto moves = b.genMoves();
    for (const Move &move : moves) {
        Board newB = b.makePseudoLegalMove(move);
        auto pair = perft(newB, depth - 1);
        nodes += pair.first;
        NodesPerMove nodesPerMove{move, pair.first};
        nodesPerMoveVec.push_back(nodesPerMove);
    }
    return std::make_pair(nodes, nodesPerMoveVec);
}