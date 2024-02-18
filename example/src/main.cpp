#include "../../include/cppbc.hpp"
#include <iostream>

#define FMT_HEADER_ONLY

#include <fmt/base.h>
#include <fmt/core.h>

int main(void) {
    fmt::println("{:=^40}", "[ example.out ]");
    std::string source = "2 + (3 * (2 + 2)) + 2 * 3";

    try {
        float result = bc(source);
        fmt::println("Result: {}", result);
    }

    catch (bc_parse_exception& ex) {
        std::cerr << ex.what();
    }

    return 0;
}
