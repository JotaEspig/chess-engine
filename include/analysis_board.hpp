#pragma once

#include <mutex>

#include "board.hpp"

class AnalysisBoard : public Board {
  public:
    std::mutex mtx;

    AnalysisBoard(const Board &board) : Board(board) {}
};
