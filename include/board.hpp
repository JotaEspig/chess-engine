#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "context.hpp"
#include "move.hpp"
#include "utils.hpp"

extern uint64_t ZOBRIST_KEYS[5][6][64];

class Board {
  public:
    // 0: White, 1: Black
    // 0: Pawns, 1: Knights, 2: Bishops, 3: Rooks, 4: Queens, 5: Kings
    // each bitboard is 64 bits, with the MSB representing a8 and the LSB
    // representing h1
    uint64_t bitboards[2][6] = {{0}};
    Context ctx{false, 0, 0ULL, 0, 0};
    uint64_t hash = 0ULL;

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

    void _setHash();
    void _initZobristKeys();
};

inline Board Board::copy() { return *this; }

inline void Board::_setHash() {
    hash = 0ULL;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            uint64_t b = bitboards[i][j];
            while (b) {
                int index = __builtin_ctzll(b);
                hash ^= ZOBRIST_KEYS[i][j][index];
                b &= b - 1;
            }
        }
    }
    if (ctx.enPassantSquare != 0ULL) {
        hash ^= ZOBRIST_KEYS[2][0][countZeros(ctx.enPassantSquare)];
    }
    if (ctx.castlingRights != 0) {
        hash ^= ZOBRIST_KEYS[3][1][(int)ctx.castlingRights];
    }
    hash ^= ZOBRIST_KEYS[4][2][ctx.whiteTurn];
}
