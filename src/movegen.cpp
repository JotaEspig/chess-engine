#include <vector>

#include "board.hpp"
#include "consts.hpp"
#include "move.hpp"
#include "utils.hpp"

const std::vector<Move> &Board::genMoves() {
    _preallocatedMoves.clear();
    int color = ctx.whiteTurn ? WHITE : BLACK;
    for (int pieceType = PAWN; pieceType < KING; ++pieceType) {
        uint64_t pieces = bitboards[color][pieceType];
        while (pieces) {
            uint64_t from = lsb(pieces);
            uint64_t moves = 0;
            switch (pieceType) {
            case PAWN:
                moves = _genPawnMoves(from, ctx.whiteTurn);
                break;
            case KNIGHT:
                moves = _genKnightMoves(from, ctx.whiteTurn);
                break;
            case BISHOP:
                moves = _genBishopMoves(from, ctx.whiteTurn);
                break;
            case ROOK:
                moves = _genRookMoves(from, ctx.whiteTurn);
                break;
            case QUEEN:
                moves = _genQueenMoves(from, ctx.whiteTurn);
                break;
            case KING:
                moves = _genKingMoves(from, ctx.whiteTurn);
                break;
            }
            while (moves) {
                uint64_t to = lsb(moves);
                _preallocatedMoves.push_back({from, to});
                moves &= moves - 1;
            }
            pieces &= pieces - 1;
        }
    }
    return _preallocatedMoves;
}

inline uint64_t Board::_genPawnMoves(uint64_t from, bool isWhite) {
    uint64_t moves = 0;
    uint64_t enemyPieces;
    uint64_t allPieces;
    if (isWhite) {
        enemyPieces = bitboards[BLACK][PAWN] | bitboards[BLACK][KNIGHT] |
                      bitboards[BLACK][BISHOP] | bitboards[BLACK][ROOK] |
                      bitboards[BLACK][QUEEN] | bitboards[BLACK][KING];
        allPieces = enemyPieces | bitboards[WHITE][PAWN] |
                    bitboards[WHITE][KNIGHT] | bitboards[WHITE][BISHOP] |
                    bitboards[WHITE][ROOK] | bitboards[WHITE][QUEEN] |
                    bitboards[WHITE][KING];
    } else {
        enemyPieces = bitboards[WHITE][PAWN] | bitboards[WHITE][KNIGHT] |
                      bitboards[WHITE][BISHOP] | bitboards[WHITE][ROOK] |
                      bitboards[WHITE][QUEEN] | bitboards[WHITE][KING];
        allPieces = enemyPieces | bitboards[BLACK][PAWN] |
                    bitboards[BLACK][KNIGHT] | bitboards[BLACK][BISHOP] |
                    bitboards[BLACK][ROOK] | bitboards[BLACK][QUEEN] |
                    bitboards[BLACK][KING];
    }
    allPieces &= ~from;

    if (isWhite) { // White
        uint64_t singlePush = (from << 8) & ~allPieces;
        if (singlePush) {
            moves |= singlePush;
            if ((from & RANK_2) && (singlePush << 8) & ~allPieces) {
                moves |= (singlePush << 8);
            }
        }
        uint64_t leftCapture =
            (from << 9) & ~FILE_H & (enemyPieces | ctx.enPassantSquare);
        if (leftCapture) {
            moves |= leftCapture;
        }
        uint64_t rightCapture =
            (from << 7) & ~FILE_A & (enemyPieces | ctx.enPassantSquare);
        if (rightCapture) {
            moves |= rightCapture;
        }
    } else { // Black
        uint64_t singlePush = (from >> 8) & ~allPieces;
        if (singlePush) {
            moves |= singlePush;
            if ((from & RANK_7) && (singlePush >> 8) & ~allPieces) {
                moves |= (singlePush >> 8);
            }
        }
        uint64_t leftCapture =
            (from >> 7) & ~FILE_A & (enemyPieces | ctx.enPassantSquare);
        if (leftCapture) {
            moves |= leftCapture;
        }
        uint64_t rightCapture =
            (from >> 9) & ~FILE_H & (enemyPieces | ctx.enPassantSquare);
        if (rightCapture) {
            moves |= rightCapture;
        }
    }
    return moves;
}

inline uint64_t Board::_genKnightMoves(uint64_t from, bool isWhite) {
    uint64_t ourPieces = _getOurPiecesMask(isWhite) & ~from;
    uint64_t moves = 0;
    uint64_t l1 = (from & ~FILE_A) << 6;
    uint64_t l2 = (from & ~FILE_A & ~FILE_B) << 15;
    uint64_t l3 = (from & ~FILE_H & ~FILE_G) << 17;
    uint64_t l4 = (from & ~FILE_H) << 10;
    uint64_t r1 = (from & ~FILE_H) >> 6;
    uint64_t r2 = (from & ~FILE_H & ~FILE_G) >> 15;
    uint64_t r3 = (from & ~FILE_A & ~FILE_B) >> 17;
    uint64_t r4 = (from & ~FILE_A) >> 10;
    moves |= l1 | l2 | l3 | l4 | r1 | r2 | r3 | r4;
    // Removes collision
    moves &= ~ourPieces;
    return moves;
}

inline uint64_t Board::_genBishopMoves(uint64_t from, bool isWhite) {
    uint64_t ourPieces = _getOurPiecesMask(isWhite) & ~from;
    uint64_t moves = 0;
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from << (7 * (multiplier - 1));
        // up right
        uint64_t l1 = (oneBefore & ~FILE_A & ~RANK_8) << 7;
        if (!l1 || l1 & ourPieces) {
            break;
        }
        moves |= l1;
    }
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from << (9 * (multiplier - 1));
        // up left
        uint64_t l2 = (oneBefore & ~FILE_H & ~RANK_8) << 9;
        if (!l2 || l2 & ourPieces) {
            break;
        }
        moves |= l2;
    }
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from >> (7 * (multiplier - 1));
        // down left
        uint64_t l3 = (oneBefore & ~FILE_H & ~RANK_1) >> 7;
        if (!l3 || l3 & ourPieces) {
            break;
        }
        moves |= l3;
    }
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from >> (9 * (multiplier - 1));
        // down right
        uint64_t l4 = (oneBefore & ~FILE_A & ~RANK_1) >> 9;
        if (!l4 || l4 & ourPieces) {
            break;
        }
        moves |= l4;
    }
    return moves;
}

inline uint64_t Board::_genRookMoves(uint64_t from, bool isWhite) {
    uint64_t ourPieces = _getOurPiecesMask(isWhite) & ~from;
    uint64_t moves = 0;
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from << (8 * (multiplier - 1));
        // up
        uint64_t l1 = (oneBefore & ~RANK_8) << 8;
        if (!l1 || l1 & ourPieces) {
            break;
        }
        moves |= l1;
    }
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from >> (8 * (multiplier - 1));
        // down
        uint64_t l2 = (oneBefore & ~RANK_1) >> 8;
        if (!l2 || l2 & ourPieces) {
            break;
        }
        moves |= l2;
    }
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from << multiplier;
        // left
        uint64_t l3 = (oneBefore & ~FILE_A) << 1;
        if (!l3 || l3 & ourPieces) {
            break;
        }
        moves |= l3;
    }
    for (int multiplier = 1; multiplier <= 7; ++multiplier) {
        uint64_t oneBefore = from >> multiplier;
        // right
        uint64_t l4 = (oneBefore & ~FILE_H) >> 1;
        if (!l4 || l4 & ourPieces) {
            break;
        }
        moves |= l4;
    }
    return moves;
}

inline uint64_t Board::_genQueenMoves(uint64_t from, bool isWhite) {
    return _genBishopMoves(from, isWhite) | _genRookMoves(from, isWhite);
}

inline uint64_t Board::_genKingMoves(uint64_t from, bool isWhite) {
    uint64_t ourPieces = _getOurPiecesMask(isWhite) & ~from;
    uint64_t moves = 0;
    uint64_t l1 = (from & ~FILE_A) << 7;
    uint64_t l2 = (from & ~FILE_A) << 8;
    uint64_t l3 = (from & ~FILE_H) << 9;
    uint64_t l4 = (from & ~FILE_H) << 1;
    uint64_t r1 = (from & ~FILE_H) >> 7;
    uint64_t r2 = (from & ~FILE_H) >> 8;
    uint64_t r3 = (from & ~FILE_A) >> 9;
    uint64_t r4 = (from & ~FILE_A) >> 1;
    moves |= l1 | l2 | l3 | l4 | r1 | r2 | r3 | r4;
    // Removes collision
    moves &= ~ourPieces;
    return moves;
}

inline uint64_t Board::_getOurPiecesMask(bool isWhite) {
    uint64_t ourPieces;
    if (isWhite) {
        ourPieces = bitboards[WHITE][PAWN] | bitboards[WHITE][KNIGHT] |
                    bitboards[WHITE][BISHOP] | bitboards[WHITE][ROOK] |
                    bitboards[WHITE][QUEEN] | bitboards[WHITE][KING];
    } else {
        ourPieces = bitboards[BLACK][PAWN] | bitboards[BLACK][KNIGHT] |
                    bitboards[BLACK][BISHOP] | bitboards[BLACK][ROOK] |
                    bitboards[BLACK][QUEEN] | bitboards[BLACK][KING];
    }
    return ourPieces;
}
