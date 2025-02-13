#pragma once

#include <cstdint>
#include <string>

struct Move {
    uint64_t from = 0;
    uint64_t to = 0;

    std::string stringfy() const;
};