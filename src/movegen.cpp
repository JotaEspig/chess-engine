#include <vector>

#include "board.hpp"
#include "consts.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "utils.hpp"

const std::vector<Move> &Board::genMoves() {
    _preallocatedMoves.clear();

    int color = ctx.whiteTurn ? WHITE : BLACK;
    for (int pieceType = PAWN; pieceType <= KING; ++pieceType) {
        uint64_t pieces = bitboards[color][pieceType];
        while (pieces) {
            uint64_t from = lsb(pieces);
            uint64_t moves = 0;
            switch (pieceType) {
            case PAWN:
                moves = _genPawnMoves(*this, from, ctx.whiteTurn);
                break;
            case KNIGHT:
                moves = _genKnightMoves(*this, from, ctx.whiteTurn);
                break;
            case BISHOP:
                moves = _genBishopMoves(*this, from, ctx.whiteTurn);
                break;
            case ROOK:
                moves = _genRookMoves(*this, from, ctx.whiteTurn);
                break;
            case QUEEN:
                moves = _genQueenMoves(*this, from, ctx.whiteTurn);
                break;
            case KING:
                moves = _genKingMoves(*this, from, ctx.whiteTurn);
                break;
            }

            while (moves) {
                uint64_t to = lsb(moves);
                Move m;
                bool isCastling = pieceType == KING &&
                                  ((from == E1 && (to == C1 || to == G1)) ||
                                   (from == E8 && (to == C8 || to == G8)));
                if (pieceType == PAWN && (to & (RANK_1 | RANK_8))) {
                    for (int promotionPiece = KNIGHT; promotionPiece <= QUEEN;
                         ++promotionPiece) {
                        m = Move{from, to, false, promotionPiece};
                        if (makePseudoLegalMove(m).isValid()) {
                            _preallocatedMoves.push_back(m);
                        }
                    }
                    moves &= moves - 1;
                    continue; // Skip the rest of the loop because of promotion
                              // different type of move insertion (4 moves)
                } else if (isCastling) {
                    m = Move{from, to, true};
                } else {
                    m = Move{from, to};
                }

                // Check if the move is valid, then add it to the list
                if (makePseudoLegalMove(m).isValid()) {
                    _preallocatedMoves.push_back(m);
                }
                moves &= moves - 1;
            }
            pieces &= pieces - 1;
        }
    }
    return _preallocatedMoves;
}
