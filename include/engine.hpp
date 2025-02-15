#pragma once

#include <bitset>
#include <unordered_map>

#include "analysis_report.hpp"
#include "board.hpp"
#include "consts.hpp"

extern std::unordered_map<int, int> pieceValues;

AnalysisReport minimax(Board board, int depth);
AnalysisReport alphaBetaMax(Board &board, int depth, int alpha, int beta);
AnalysisReport alphaBetaMin(Board &board, int depth, int alpha, int beta);
inline int evaluate(Board &board) {
    if (board.isMate()) {
        return board.ctx.whiteTurn ? -10000 : 10000;
    } else if (board.isDraw()) {
        return 0;
    }

    // count material
    int whiteMaterial = 0;
    int blackMaterial = 0;
    for (int pieceType = PAWN; pieceType < KING; pieceType++) {
        auto whiteCount =
            std::bitset<64>(board.bitboards[WHITE][pieceType]).count();
        auto blackCount =
            std::bitset<64>(board.bitboards[BLACK][pieceType]).count();
        whiteMaterial += whiteCount * pieceValues[pieceType];
        blackMaterial += blackCount * pieceValues[pieceType];
    }

    return whiteMaterial - blackMaterial;
}
