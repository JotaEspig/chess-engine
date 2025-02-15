#include <iostream>

#include "board.hpp"
#include "consts.hpp"
#include "move.hpp"
#include "perft.hpp"
#include "utils.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string fen;
    std::cout << "Enter FEN (leave it empty to use default start position): ";
    std::getline(std::cin, fen);
    if (fen == "") {
        fen = DEFAULT_FEN;
    }

    Board b{fen};
    std::string str = b.stringify();
    std::cout << str << std::endl;

    std::cout << "Is mate? " << b.isMate() << std::endl;
    std::cout << "Is draw? " << b.isDraw() << std::endl;

    std::string s;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, s);
        if (!std::cin.good() || s == "q" || s == "exit") {
            break;
        } else if (s.find("perft") != std::string::npos) {
            auto splitted = split(s, ' ');
            if (splitted.size() != 2) {
                continue;
            }
            auto pair = perft(b, std::stoi(splitted[1]));
            for (const NodesPerMove &n : pair.second) {
                std::cout << n.move.stringify() << ": " << n.nodes << std::endl;
            }
            std::cout << std::endl << pair.first << std::endl;
            continue;
        } else if (s == "") {
            continue;
        }

        Move m = Move::fromString(s);
        b = b.makePseudoLegalMove(m);
        std::cout << b.stringify() << std::endl;
    }

    return 0;
}
