#pragma once

#include "board.hpp"
#include "tests.hpp"
#include "consts.hpp"

void pawnChecksTests(tests::Test *t) {
    Board b{DEFAULT_FEN};
    const std::vector<Move> &moves = b.genMoves();
    t->Equal((int)moves.size(), 20);

    b = Board{"rnbq1bnr/pppp1ppp/8/4pk2/4P3/8/PPPP1PPP/RNBQKBNR b KQ - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(WHITE));

    b = Board{"rnbqkbnr/ppp1pppp/8/3p4/4K3/4P3/PPPP1PPP/RNBQ1BNR w kq - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(BLACK));

    // Check for file limitations
    // (e.g. black pawn at file A cannot capture to the left)
    b = Board{"rnbqkbnr/ppp1pppp/7p/8/K7/4P3/PPPP1PPP/RNBQ1BNR w HAkq - 0 1"};
    t->False(b.isKingInCheck(), "file limitations for pawn check failed");
    t->False(b.isKingInCheck(BLACK));

    t->Resume();
}

void knightChecksTests(tests::Test *t) {
    Board b{"r1bqkbnr/pppppppp/3n4/8/4K3/4P3/PPPP1PPP/RNBQ1BNR w kq - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(BLACK));
    b = Board{"rnbq1bnr/ppppkppp/4p3/5N2/8/4P3/PPPP1PPP/RNBQKB1R b ha - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(WHITE));

    t->Resume();
}

void bishopChecksTests(tests::Test *t) {
    Board b{"rn1qkbnr/ppp2ppp/3p4/4pb2/4K3/4P3/PPPP1PPP/RNBQ1BNR w - - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(BLACK));
    b = Board{"rnbq1bnr/pppp1ppp/4k3/4p3/2B5/4P3/PPPP1PPP/RNBQK1NR b KQ - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(WHITE));

    t->Resume();
}

void rookChecksTests(tests::Test *t) {
    Board b{"1nbqkbnr/ppp2ppp/3p4/4p3/r3K3/4P3/PPPP1PPP/RNBQ1BNR w - - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(BLACK));
    b = Board{"rnbq1bnr/ppp2ppp/3pk2R/4p3/8/4P3/PPPP1PPP/RNBQKBN1 b Q - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(WHITE));

    t->Resume();
}

void queenChecksTests(tests::Test *t) {
    Board b{"rnb1kbnr/ppp2ppp/3p4/4p3/4K2q/4P3/PPPP1PPP/RNBQ1BNR w - - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(BLACK));
    b = Board{"rnbq1bnr/ppp2ppp/3pk3/4p3/6Q1/4P3/PPPP1PPP/RNB1KBNR b KQ - 0 1"};
    t->True(b.isKingInCheck());
    t->False(b.isKingInCheck(WHITE));

    t->Resume();
}

void ChecksTests(tests::Test *t) {
    std::cout << "pawn\n";
    pawnChecksTests(t);
    std::cout << "knight\n";
    knightChecksTests(t);
    std::cout << "bishop\n";
    bishopChecksTests(t);
    std::cout << "rook\n";
    rookChecksTests(t);
    std::cout << "queen\n";
    queenChecksTests(t);
}
