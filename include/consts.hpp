#pragma once

// Fens
#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Memory allocations
#define MAX_MOVES 256

// Colors
#define WHITE 0
#define BLACK 1

// Castling
#define NO_CASTLING 0
#define WHITE_KING_SIDE 1
#define WHITE_QUEEN_SIDE 2
#define WHITE_BOTH 3
#define BLACK_KING_SIDE 4
#define BLACK_QUEEN_SIDE 8
#define BLACK_BOTH 12

// Piece type
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

// Board Squares
constexpr auto RANK_1 = 0xFFULL;
constexpr auto RANK_2 = 0xFF00ULL;
constexpr auto RANK_4 = 0xFF000000ULL;
constexpr auto RANK_5 = 0xFF00000000ULL;
constexpr auto RANK_7 = 0xFF000000000000ULL;
constexpr auto RANK_8 = 0xFF00000000000000ULL;
constexpr auto FILE_A = 0x8080808080808080ULL;
constexpr auto FILE_B = 0x4040404040404040ULL;
constexpr auto FILE_G = 0x0202020202020202ULL;
constexpr auto FILE_H = 0x0101010101010101ULL;
constexpr auto H1 = 1ULL;
constexpr auto G1 = H1 << 1;
constexpr auto F1 = G1 << 1;
constexpr auto E1 = F1 << 1;
constexpr auto D1 = E1 << 1;
constexpr auto C1 = D1 << 1;
constexpr auto B1 = C1 << 1;
constexpr auto A1 = B1 << 1;
constexpr auto H2 = A1 << 1;
constexpr auto G2 = H2 << 1;
constexpr auto F2 = G2 << 1;
constexpr auto E2 = F2 << 1;
constexpr auto D2 = E2 << 1;
constexpr auto C2 = D2 << 1;
constexpr auto B2 = C2 << 1;
constexpr auto A2 = B2 << 1;
constexpr auto H3 = A2 << 1;
constexpr auto G3 = H3 << 1;
constexpr auto F3 = G3 << 1;
constexpr auto E3 = F3 << 1;
constexpr auto D3 = E3 << 1;
constexpr auto C3 = D3 << 1;
constexpr auto B3 = C3 << 1;
constexpr auto A3 = B3 << 1;
constexpr auto H4 = A3 << 1;
constexpr auto G4 = H4 << 1;
constexpr auto F4 = G4 << 1;
constexpr auto E4 = F4 << 1;
constexpr auto D4 = E4 << 1;
constexpr auto C4 = D4 << 1;
constexpr auto B4 = C4 << 1;
constexpr auto A4 = B4 << 1;
constexpr auto H5 = A4 << 1;
constexpr auto G5 = H5 << 1;
constexpr auto F5 = G5 << 1;
constexpr auto E5 = F5 << 1;
constexpr auto D5 = E5 << 1;
constexpr auto C5 = D5 << 1;
constexpr auto B5 = C5 << 1;
constexpr auto A5 = B5 << 1;
constexpr auto H6 = A5 << 1;
constexpr auto G6 = H6 << 1;
constexpr auto F6 = G6 << 1;
constexpr auto E6 = F6 << 1;
constexpr auto D6 = E6 << 1;
constexpr auto C6 = D6 << 1;
constexpr auto B6 = C6 << 1;
constexpr auto A6 = B6 << 1;
constexpr auto H7 = A6 << 1;
constexpr auto G7 = H7 << 1;
constexpr auto F7 = G7 << 1;
constexpr auto E7 = F7 << 1;
constexpr auto D7 = E7 << 1;
constexpr auto C7 = D7 << 1;
constexpr auto B7 = C7 << 1;
constexpr auto A7 = B7 << 1;
constexpr auto H8 = A7 << 1;
constexpr auto G8 = H8 << 1;
constexpr auto F8 = G8 << 1;
constexpr auto E8 = F8 << 1;
constexpr auto D8 = E8 << 1;
constexpr auto C8 = D8 << 1;
constexpr auto B8 = C8 << 1;
constexpr auto A8 = B8 << 1;
