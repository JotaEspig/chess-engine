#include <algorithm>
#include <atomic>
#include <chrono>
#include <ctime>
#include <limits>
#include <unordered_map>

#include "analysis_report.hpp"
#include "engine.hpp"

std::unordered_map<int, int> pieceValues = {
    {PAWN, 100}, {KNIGHT, 320}, {BISHOP, 330}, {ROOK, 500}, {QUEEN, 900}};

int moveScore(const Move &move) {
    int score = 0;
    if (move.capturedPiece != -1) {
        score += pieceValues[move.capturedPiece];
    }
    if (move.promotionPiece != -1) {
        score += pieceValues[move.promotionPiece];
    }
    if (move.isCastling) {
        score += 50;
    }
    if (move.isCheck) {
        score += 100;
    }
    return score;
}

bool sortMoves(Move &a, Move &b) { return moveScore(a) > moveScore(b); }

void Engine::analysisByDepth(AnalysisBoard &board, int depth) {
    _startTime = std::chrono::steady_clock::now();
    bestReport = minimax(board, depth);
    finished.store(true, std::memory_order_release);
}

float Engine::nodesPerSecond() {
    _npsMtx.lock();
    float nps = (float)_nodes / ((float)totalTime().count() / 1000);
    // Reset if overflow for uint32_t (_nodes is uint64_t, so it won't overflow
    // at this point)
    if (_nodes > std::numeric_limits<uint>::max()) {
        _nodes = 0;
        _startTime = std::chrono::steady_clock::now();
    }
    _npsMtx.unlock();

    return nps;
}

std::chrono::milliseconds Engine::totalTime() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                 _startTime);
}

AnalysisReport Engine::minimax(AnalysisBoard &board, int depth) {
    if (board.ctx.whiteTurn) {
        return alphaBetaMax(board, depth, -10000, 10000);
    } else {
        return alphaBetaMin(board, depth, -10000, 10000);
    }
}

AnalysisReport Engine::alphaBetaMax(AnalysisBoard &board, int depth, int alpha,
                                    int beta) {
    if (depth == 0) {
        _incNodes();
        return {evaluate(board), board};
    } else if (board.isMate()) {
        _incNodes();
        return {-10000, board};
    } else if (board.isDraw()) {
        _incNodes();
        return {0, board};
    }

    AnalysisReport bestReport = {-10000, board};
    auto moves = board.genMoves();
    std::sort(moves.begin(), moves.end(), sortMoves);
    for (auto &move : moves) {
        AnalysisBoard newBoard = board.makeAndSetMove(move);
        AnalysisReport report = alphaBetaMin(newBoard, depth - 1, alpha, beta);
        if (report.score > bestReport.score) {
            bestReport = report;
            if (report.score > alpha) {
                alpha = report.score;
            }
        }
        if (report.score >= beta) {
            return report;
        }
    }
    return bestReport;
}

AnalysisReport Engine::alphaBetaMin(AnalysisBoard &board, int depth, int alpha,
                                    int beta) {
    if (depth == 0) {
        _incNodes();
        return {evaluate(board), board};
    } else if (board.isMate()) {
        _incNodes();
        return {10000, board};
    } else if (board.isDraw()) {
        _incNodes();
        return {0, board};
    }

    AnalysisReport bestReport = {10000, board};
    auto moves = board.genMoves();
    std::sort(moves.begin(), moves.end(), sortMoves);
    for (auto &move : moves) {
        AnalysisBoard newBoard = board.makeAndSetMove(move);
        AnalysisReport report = alphaBetaMax(newBoard, depth - 1, alpha, beta);
        if (report.score < bestReport.score) {
            bestReport = report;
            if (report.score < beta) {
                beta = report.score;
            }
        }
        if (report.score <= alpha) {
            return report;
        }
    }
    return bestReport;
}

void Engine::_incNodes() {
    _npsMtx.lock();
    ++_nodes;
    _npsMtx.unlock();
}
