#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace tests {

class Test {
  public:
    Test() = default;

    template <typename T>
    void Equal(T actual, T expected, std::string message = "") {
        if (actual != expected) {
            if (message != "") {
                message += '\n';
            }
            message += "Expected " + std::to_string(expected) + ", got " +
                       std::to_string(actual);

            _errors.push_back(message);
        }
    }

    void True(bool x, std::string message = "") {
        if (!x) {
            if (message == "") {
                message = "Is not true.";
            }
            _errors.push_back(message);
        }
    }

    void False(bool x, std::string message = "") {
        if (x) {
            if (message == "") {
                message = "Is not false.";
            }
            _errors.push_back(message);
        }
    }

    void Resume() {
        for (const std::string &error : _errors) {
            std::cerr << error << std::endl;
        }
        if (_errors.size() == 0) {
            std::cout << "All tests passed!" << std::endl;
        }
        _errors.clear();
    }

  private:
    std::vector<std::string> _errors;
};

} // namespace tests
