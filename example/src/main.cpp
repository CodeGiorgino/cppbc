#include "../../include/cppbc.hpp"
#include <iostream>

#define FMT_HEADER_ONLY

#include <fmt/base.h>
#include <fmt/core.h>

int main(void) {
    fmt::println("{:=^40}", "[ example.out ]");

    try {
        float retval = bc("1 + 1 * 2 + 3");
        fmt::println("retval: {}", retval);
    }

    catch (bc_parse_exception& ex) {
        std::cerr << ex.what();
    }

    return 0;
}
