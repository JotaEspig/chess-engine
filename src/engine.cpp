#include <algorithm>
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

AnalysisReport minimax(Board board, int depth) {
    if (board.ctx.whiteTurn) {
        return alphaBetaMax(board, depth, -10000, 10000);
    } else {
        return alphaBetaMin(board, depth, -10000, 10000);
    }
}

AnalysisReport alphaBetaMax(Board &board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return {evaluate(board), board};
    } else if (board.isMate()) {
        return {-10000, board};
    } else if (board.isDraw()) {
        return {0, board};
    }

    AnalysisReport bestReport = {-10000, board};
    auto moves = board.genMoves();
    std::sort(moves.begin(), moves.end(), sortMoves);
    for (auto &move : moves) {
        auto newBoard = board.makeAndSetMove(move);
        AnalysisReport report = alphaBetaMin(newBoard, depth - 1, alpha, beta);
        if (report.score > bestReport.score) {
            bestReport = report;
            if (report.score > alpha) {
                alpha = report.score;
            }
        }
        if (report.score >= beta) {
            return bestReport;
        }
    }
    return bestReport;
}

AnalysisReport alphaBetaMin(Board &board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return {evaluate(board), board};
    } else if (board.isMate()) {
        return {10000, board};
    } else if (board.isDraw()) {
        return {0, board};
    }

    AnalysisReport bestReport = {10000, board};
    auto moves = board.genMoves();
    std::sort(moves.begin(), moves.end(), sortMoves);
    for (auto &move : moves) {
        auto newBoard = board.makeAndSetMove(move);
        AnalysisReport report = alphaBetaMax(newBoard, depth - 1, alpha, beta);
        if (report.score < bestReport.score) {
            bestReport = report;
            if (report.score < beta) {
                beta = report.score;
            }
        }
        if (report.score <= alpha) {
            return bestReport;
        }
    }
    return bestReport;
}
