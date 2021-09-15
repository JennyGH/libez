#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <base/container_ex.hpp>

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main(int argc, char** argv)
{
    {
        std::map<int, std::string> test = {{0, "a"}, {1, "b"}};

        const auto filtered = ez::base::filter(test, [](const std::pair<int, std::string>& e) {
            return e.first != 0;
        });
        const auto mapped   = ez::base::map<std::map<std::string, std::string>>(filtered, [](const std::pair<int, std::string>& e) {
            return std::pair<std::string, std::string> {std::to_string(e.first), e.second};
        });
        const auto joined   = ez::base::join(mapped, [](const std::pair<std::string, std::string>& e) {
            return "{" + e.first + ", " + e.second + "}";
        });
    }

    {
        std::vector<int> test = {0, 1, 2, 3};

        const auto filtered = ez::base::filter(test, [](const int& e) {
            return e != 0;
        });
        const auto mapped   = ez::base::map<std::vector<std::string>>(filtered, [](const int& e) {
            return std::to_string(e);
        });
        const auto joined   = ez::base::join(mapped);
    }

    return 0;
}