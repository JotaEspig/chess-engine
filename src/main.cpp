#include <iostream>

#include "board.hpp"
#include "consts.hpp"
#include "move.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Board b{DEFAULT_FEN};
    std::string s = b.stringify();
    std::cout << s << std::endl;

    const std::vector<Move> &moves = b.genMoves();
    for (const Move &m : moves) {
        std::cout << m.stringfy() << std::endl;
        // Board nb = b.makePseudoLegalMove(m);
        // std::string ns = nb.stringify();
        // std::cout << ns << std::endl;
    }
    std::cout << moves.size() << std::endl;

    return 0;
}