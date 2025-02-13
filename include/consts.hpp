#pragma once

// Fens
#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Memory allocations
#define MAX_MOVES 256

// Colors
#define WHITE 0
#define BLACK 1

// Piece type
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

// Board Squares
#define RANK_1 0xFFULL
#define RANK_2 0xFF00ULL
#define RANK_7 0xFF000000000000ULL
#define RANK_8 0xFF00000000000000ULL
#define FILE_A 0x8080808080808080ULL
#define FILE_B 0x4040404040404040ULL
#define FILE_G 0x2020202020202020ULL
#define FILE_H 0x0101010101010101ULL
#define H1 1
#define G1 H1 << 1
#define F1 G1 << 1
#define E1 F1 << 1
#define D1 E1 << 1
#define C1 D1 << 1
#define B1 C1 << 1
#define A1 B1 << 1
#define H2 A1 << 1
#define G2 H2 << 1
#define F2 G2 << 1
#define E2 F2 << 1
#define D2 E2 << 1
#define C2 D2 << 1
#define B2 C2 << 1
#define A2 B2 << 1
#define H3 A2 << 1
#define G3 H3 << 1
#define F3 G3 << 1
#define E3 F3 << 1
#define D3 E3 << 1
#define C3 D3 << 1
#define B3 C3 << 1
#define A3 B3 << 1
#define H4 A3 << 1
#define G4 H4 << 1
#define F4 G4 << 1
#define E4 F4 << 1
#define D4 E4 << 1
#define C4 D4 << 1
#define B4 C4 << 1
#define A4 B4 << 1
#define H5 A4 << 1
#define G5 H5 << 1
#define F5 G5 << 1
#define E5 F5 << 1
#define D5 E5 << 1
#define C5 D5 << 1
#define B5 C5 << 1
#define A5 B5 << 1
#define H6 A5 << 1
#define G6 H6 << 1
#define F6 G6 << 1
#define E6 F6 << 1
#define D6 E6 << 1
#define C6 D6 << 1
#define B6 C6 << 1
#define A6 B6 << 1
#define H7 A6 << 1
#define G7 H7 << 1
#define F7 G7 << 1
#define E7 F7 << 1
#define D7 E7 << 1
#define C7 D7 << 1
#define B7 C7 << 1
#define A7 B7 << 1
#define H8 A7 << 1
#define G8 H8 << 1
#define F8 G8 << 1
#define E8 F8 << 1
#define D8 E8 << 1
#define C8 D8 << 1
#define B8 C8 << 1
#define A8 B8 << 1
