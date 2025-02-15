#pragma once

#include <atomic>
#include <bitset>
#include <chrono>
#include <ctime>
#include <mutex>
#include <unordered_map>

#include "analysis_board.hpp"
#include "analysis_report.hpp"
#include "consts.hpp"
#include "piecesquaretable.hpp"

extern std::unordered_map<int, int> pieceValues;

class Engine {
  public:
    AnalysisReport bestReport;
    std::atomic<bool> finished{false};

    Engine() = default;

    // Call using thread_obj;
    void analysisByDepth(AnalysisBoard &board, int depth);
    int evaluate(AnalysisBoard &board);
    float nodesPerSecond();
    std::chrono::milliseconds totalTime();

  private:
    std::chrono::steady_clock::time_point _startTime;
    uint64_t _nodes = 0;
    std::mutex _npsMtx;

    AnalysisReport minimax(AnalysisBoard &board, int depth);
    AnalysisReport alphaBetaMax(AnalysisBoard &board, int depth, int alpha,
                                int beta);
    AnalysisReport alphaBetaMin(AnalysisBoard &board, int depth, int alpha,
                                int beta);
    void _incNodes();
};

inline int Engine::evaluate(AnalysisBoard &board) {
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

    int score = whiteMaterial - blackMaterial;
    for (int pieceType = PAWN; pieceType < KING; pieceType++) {
        auto whiteBB = board.bitboards[WHITE][pieceType];
        while (whiteBB) {
            int square = __builtin_ctzll(whiteBB);
            score += getPieceSquareValue(pieceType, WHITE, square);
            whiteBB &= whiteBB - 1;
        }

        auto blackBB = board.bitboards[BLACK][pieceType];
        while (blackBB) {
            int square = __builtin_ctzll(blackBB);
            score -= getPieceSquareValue(pieceType, BLACK, square);
            blackBB &= blackBB - 1;
        }
    }

    return score;
}
