#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);
int getPieceIdx(char c);
char getPieceChar(int i, bool isWhite);
uint64_t lsb(uint64_t i);
