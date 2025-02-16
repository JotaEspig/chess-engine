#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);
int getPieceIdx(char c);
char getPieceChar(int i, bool isWhite);
uint64_t lsb(uint64_t i);
int countZeros(unsigned long long x);
void initZobristKeys(uint64_t keys[5][6][64]);
