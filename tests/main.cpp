#include <iostream>

#include "king_in_check.hpp"
#include "tests.hpp"

int main() {
    tests::Test *t = new tests::Test();

    ChecksTests(t);
    return 0;
}