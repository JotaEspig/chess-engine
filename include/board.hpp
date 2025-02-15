#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "consts.hpp"
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

    Board makePseudoLegalMove(Move m);
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

    uint64_t _genPawnMoves(uint64_t from, bool isWhite) const;
    uint64_t _genKnightMoves(uint64_t from, bool isWhite) const;
    uint64_t _genBishopMoves(uint64_t from, bool isWhite) const;
    uint64_t _genRookMoves(uint64_t from, bool isWhite) const;
    uint64_t _genQueenMoves(uint64_t from, bool isWhite) const;
    uint64_t _genKingMoves(uint64_t from, bool isWhite) const;
    uint64_t _getPiecesMask(bool isWhite) const;
};
