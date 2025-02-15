#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "context.hpp"
#include "move.hpp"

class Board {
  public:
    // 0: White, 1: Black
    // 0: Pawns, 1: Knights, 2: Bishops, 3: Rooks, 4: Queens, 5: Kings
    // each bitboard is 64 bits, with the MSB representing a8 and the LSB
    // representing h1
    uint64_t bitboards[2][6] = {{0}};
    Context ctx{false, 0, 0ULL, 0, 0};

    Board();
    Board(std::string fen);

    Board makeAndSetMove(Move &m);
    const std::vector<Move> &genMoves();
    bool isKingInCheck(int color = -1) const;
    bool isMate();
    bool isDraw();
    bool isValid() const;
    std::string stringify() const;
    /**
     * @brief Copy the current board with isMovesGenerated == false
     */
    Board copy();

  private:
    bool _isValidFlag = false;
    static std::vector<Move> _preallocatedMoves;
};
