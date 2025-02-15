#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "board.hpp"
#include "consts.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "utils.hpp"

std::vector<Move> Board::_preallocatedMoves;

Board::Board() : bitboards{{0}}, ctx{false, 0, 0ULL, 0, 0} {
    _preallocatedMoves.reserve(MAX_MOVES);
}

Board::Board(std::string fen) : Board{} {
    auto splitted = split(fen, ' ');
    if (splitted.size() != 6) {
        return;
    }

    std::memset(bitboards[0], 0ULL, 6);
    std::memset(bitboards[1], 0ULL, 6);
    int rank = 7;
    int file = 0;
    for (char c : splitted[0]) {
        if (c == '/') {
            --rank;
            file = 0;
            continue;
        }
        if (c >= '1' && c <= '8') {
            file += c - '0'; // skip empty squares
        } else {
            int boardIdx = 0;
            if (c >= 'A' && c <= 'Z') {
                boardIdx = 0;
            } else if (c >= 'a' && c <= 'z') {
                boardIdx = 1;
            }
            // 7 - x because of MSB and LSB.
            // See bitboards comment
            int pieceIdx = getPieceIdx(c);
            bitboards[boardIdx][pieceIdx] |= 1ULL << (rank * 8 + (7 - file));
            ++file;
        }
    }

    // Set context
    // White turn
    if (splitted[1] == "w") {
        ctx.whiteTurn = true;
    } else {
        ctx.whiteTurn = false;
    }
    // Castling rights
    if (splitted[2].find('K') != std::string::npos) {
        ctx.castlingRights |= 1;
    }
    if (splitted[2].find('Q') != std::string::npos) {
        ctx.castlingRights |= 1 << 1;
    }
    if (splitted[2].find('k') != std::string::npos) {
        ctx.castlingRights |= 1 << 2;
    }
    if (splitted[2].find('q') != std::string::npos) {
        ctx.castlingRights |= 1 << 3;
    }
    // En passant square
    if (splitted[3] != "-") {
        // 7 - x because of MSB and LSB.
        // See bitboards comment
        ctx.enPassantSquare =
            1ULL << (8 * (splitted[3][1] - '1') + (7 - splitted[3][0] - 'a'));
    }
    // Half move clock
    ctx.halfMoveClock = std::stoi(splitted[4]);
    // Full move number
    ctx.fullMoveNumber = std::stoi(splitted[5]);

    _isValidFlag = true;
}

Board Board::makeAndSetMove(Move &m) {
    int color = -1;
    int foundPieceType = -1;
    int capturedColor = -1;
    int capturedPieceType = -1;
    Board newBoard = *this;

    // Update bitboards
    for (int pieceType = PAWN; pieceType <= KING; ++pieceType) {
        if (bitboards[WHITE][pieceType] & m.from) {
            newBoard.bitboards[WHITE][pieceType] &= ~m.from;
            newBoard.bitboards[WHITE][pieceType] |= m.to;
            foundPieceType = pieceType;
            color = WHITE;
        } else if (bitboards[BLACK][pieceType] & m.from) {
            newBoard.bitboards[BLACK][pieceType] &= ~m.from;
            newBoard.bitboards[BLACK][pieceType] |= m.to;
            color = BLACK;
            foundPieceType = pieceType;
        }
        // Check for captures
        if (bitboards[WHITE][pieceType] & m.to) {
            capturedPieceType = pieceType;
            capturedColor = WHITE;
        } else if (bitboards[BLACK][pieceType] & m.to) {
            capturedPieceType = pieceType;
            capturedColor = BLACK;
        }
    }
    if (foundPieceType == PAWN && m.to == ctx.enPassantSquare) {
        if (color == WHITE) {
            newBoard.bitboards[BLACK][PAWN] &= ~(m.to >> 8);
        } else {
            newBoard.bitboards[WHITE][PAWN] &= ~(m.to << 8);
        }
        m.capturedPiece = PAWN;
    } else if (capturedColor != -1) {
        newBoard.bitboards[capturedColor][capturedPieceType] &= ~m.to;
        m.capturedPiece = capturedPieceType;
    }
    if (m.isCastling) {
        uint64_t rookFrom = 0;
        uint64_t rookTo = 0;
        if (m.to == G1) {
            rookFrom = H1;
            rookTo = F1;
            newBoard.ctx.castlingRights &= ~WHITE_BOTH;
        } else if (m.to == C1) {
            rookFrom = A1;
            rookTo = D1;
            newBoard.ctx.castlingRights &= ~WHITE_BOTH;
        } else if (m.to == G8) {
            rookFrom = H8;
            rookTo = F8;
            newBoard.ctx.castlingRights &= ~BLACK_BOTH;
        } else if (m.to == C8) {
            rookFrom = A8;
            rookTo = D8;
            newBoard.ctx.castlingRights &= ~BLACK_BOTH;
        }

        newBoard.bitboards[color][ROOK] &= ~rookFrom;
        newBoard.bitboards[color][ROOK] |= rookTo;
    }

    if (color == -1 || foundPieceType == -1 || color == capturedColor) {
        newBoard._isValidFlag = false;
        return newBoard;
    }

    bool isDoublePush = false;
    if (foundPieceType == ROOK) {
        // update castling rights
        if (m.from == H1) {
            newBoard.ctx.castlingRights &= ~WHITE_KING_SIDE;
        } else if (m.from == A1) {
            newBoard.ctx.castlingRights &= ~WHITE_QUEEN_SIDE;
        } else if (m.from == H8) {
            newBoard.ctx.castlingRights &= ~BLACK_KING_SIDE;
        } else if (m.from == A8) {
            newBoard.ctx.castlingRights &= ~BLACK_QUEEN_SIDE;
        }
    } else if (foundPieceType == PAWN) {
        if (color == WHITE) {
            if (m.from & RANK_2 && m.to & RANK_4) {
                isDoublePush = true;
            }
        } else {
            if (m.from & RANK_7 && m.to & RANK_5) {
                isDoublePush = true;
            }
        }
    }

    if (isDoublePush) {
        newBoard.ctx.enPassantSquare = color == WHITE ? m.to >> 8 : m.to << 8;
    } else {
        newBoard.ctx.enPassantSquare = 0ULL;
    }

    // Update context
    newBoard.ctx.whiteTurn = !ctx.whiteTurn;
    if (capturedColor == -1 || foundPieceType != PAWN) {
        newBoard.ctx.halfMoveClock = ctx.halfMoveClock + 1;
    } else {
        newBoard.ctx.halfMoveClock = 0;
    }
    newBoard.ctx.fullMoveNumber += newBoard.ctx.whiteTurn;
    // Setup m.isCheck for move score calculation
    if (newBoard.isKingInCheck()) {
        m.isCheck = true;
    }
    return newBoard;
}

bool Board::isKingInCheck(int color) const {
    if (color == -1) {
        color = ctx.whiteTurn ? WHITE : BLACK;
    }

    assert(color == WHITE || color == BLACK);
    uint64_t enemyColor = color == WHITE ? BLACK : WHITE;

    uint64_t king = bitboards[color][KING];
    uint64_t ourPieces = _getPiecesMask(*this, color == WHITE);
    uint64_t enemyPieces = _getPiecesMask(*this, color != WHITE);

    // Check for pawns
    if (color == WHITE) {
        uint64_t rightCapture =
            (king << 7) & ~FILE_A &
            bitboards[BLACK][PAWN]; // black pawns in A file cannot capture
                                    // to the right
        uint64_t leftCapture =
            (king << 9) & ~FILE_H &
            bitboards[BLACK][PAWN]; // black pawns in H file cannot capture
                                    // to the left
        if (leftCapture || rightCapture) {
            return true;
        }
    } else {
        uint64_t leftCapture =
            (king >> 9) & ~FILE_A &
            bitboards[WHITE][PAWN]; // white pawns in A file cannot capture
                                    // to the left
        uint64_t rightCapture =
            (king >> 7) & ~FILE_H &
            bitboards[WHITE][PAWN]; // white pawns in H file cannot capture
                                    // to the right
        if (leftCapture || rightCapture) {
            return true;
        }
    }

    uint64_t knights = bitboards[enemyColor][KNIGHT];
    uint64_t kingKnightMoves = _genKnightMoves(*this, king, color == WHITE);
    if (knights & kingKnightMoves) {
        return true;
    }

    uint64_t bishopsAndQueens =
        bitboards[enemyColor][BISHOP] | bitboards[enemyColor][QUEEN];
    uint64_t kingDiagonalMoves = _genBishopMoves(*this, king, color == WHITE);
    if (bishopsAndQueens & kingDiagonalMoves) {
        return true;
    }

    uint64_t rooksAndQueens =
        bitboards[enemyColor][ROOK] | bitboards[enemyColor][QUEEN];
    uint64_t kingOrthogonalMoves = _genRookMoves(*this, king, color == WHITE);
    if (rooksAndQueens & kingOrthogonalMoves) {
        return true;
    }

    uint64_t enemyKing = bitboards[enemyColor][KING];
    uint64_t kingMoves = _genKingMoves(*this, king, color == WHITE);
    if (enemyKing & kingMoves) {
        return true;
    }

    return false;
}

bool Board::isMate() { return isKingInCheck() && genMoves().size() == 0; }

bool Board::isDraw() {
    // Fifty move rule
    if (ctx.halfMoveClock >= 100) {
        return true;
    }

    // TODO Implement threefold repetition
    // TODO Implement insufficient material

    return !isKingInCheck() && genMoves().size() == 0;
}

bool Board::isValid() const {
    if (!_isValidFlag) {
        return false;
    }

    // Inverted colors to check if white can capture the king
    int color = ctx.whiteTurn ? BLACK : WHITE;
    return !isKingInCheck(color);
}

std::string Board::stringify() const {
    char visualBoard[64] = {0};
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 64; ++j) {
            if (bitboards[0][i] & (1ULL << j)) {
                visualBoard[j] = getPieceChar(i, true);
            } else if (bitboards[1][i] & (1ULL << j)) {
                visualBoard[j] = getPieceChar(i, false);
            }
        }
    }

    std::string result = "-";
    for (int i = 0; i < 8; ++i) {
        result += "----";
    }
    result += "\n";
    // Reverse because the bits are stored from a8 to h1
    for (int i = 63; i >= 0; --i) {
        result += "| ";
        if (visualBoard[i]) {
            result += visualBoard[i];
        } else {
            result += ' '; // two spaces for empty squares
        }
        result += ' ';
        if (i % 8 == 0) {
            result += "|\n-";
            for (int j = 0; j < 8; ++j) {
                result += "----";
            }
            result += '\n';
        }
    }

    return result;
}

inline Board Board::copy() { return *this; }
