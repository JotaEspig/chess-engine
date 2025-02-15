#include <string>
#include <vector>

#include "board.hpp"
#include "game.hpp"
#include "move.hpp"

Game::Game() {}

Game::Game(std::string fen) { boards.push_back(Board(fen)); }

Board &Game::makePseudoLegalMove(Move m) {
    Board last = boards.back();
    Board b = last.makePseudoLegalMove(m);
    boards.push_back(b);
    return currentBoard();
}

Board Game::undoMove() {
    if (boards.size() > 1) {
        boards.pop_back();
        return boards.back();
    }
    return Board{};
}

Board &Game::currentBoard() {
    if (boards.size() > 1) {
        return boards.back();
    }
    return Board{};
}
