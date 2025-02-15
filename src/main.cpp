#include <atomic>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "board.hpp"
#include "consts.hpp"
#include "engine.hpp"
#include "move.hpp"
#include "perft.hpp"
#include "utils.hpp"

std::string rjust(std::string s, int width, char fillchar = ' ') {
    if (s.length() >= width) {
        return s;
    }
    return std::string(width - s.length(), fillchar) + s;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string fen;
    std::cout << "Enter FEN (leave it empty to use default start position): ";
    std::getline(std::cin, fen);
    if (fen == "") {
        fen = DEFAULT_FEN;
    }

    Board b{fen};
    std::cout << "Is mate? " << b.isMate() << std::endl;
    std::cout << "Is draw? " << b.isDraw() << std::endl;

    std::string s;
    while (true) {
        std::string str = b.stringify();
        std::cout << str << std::endl;
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
        } else if (s.find("eval") != std::string::npos) {
            auto splitted = split(s, ' ');
            if (splitted.size() != 2) {
                continue;
            }
            Engine e;
            AnalysisBoard ab{b};
            std::thread t(&Engine::analysisByDepth, &e, std::ref(ab),
                          std::stoi(splitted[1]));

            while (!e.finished.load(std::memory_order_acquire)) {
                double knps = e.nodesPerSecond() / 1000.0;
                std::ostringstream oss;
                oss << "NPS: " << std::setw(10) << std::fixed
                    << std::setprecision(1) << knps << "k";

                std::cout << oss.str() << "\r" << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            std::cout << std::endl;
            t.join();

            e.analysisByDepth(ab, std::stoi(splitted[1]));
            auto report = e.bestReport;
            std::cout << report.board.stringify() << ": " << report.score
                      << std::endl;
            std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::seconds>(e.totalTime()) << std::endl;
            continue;
        } else if (s == "") {
            continue;
        }

        Move m = Move::fromString(s);
        b = b.makeAndSetMove(m);
        std::cout << b.stringify() << std::endl;
    }

    return 0;
}
