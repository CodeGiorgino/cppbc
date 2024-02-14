#include "../include/cppbc.hpp"
#include <cmath>
#include <fmt/base.h>
#include <stdlib.h>
#include <cstddef>
#include <string>
#include <vector>

#define LOG_TRACE

#define BinaryOpsLiteral std::string(" +-*/")
enum BinaryOps { NOP, PLUS, DASH, STAR, SLASH };

float bop_apply(float lhs, float rhs, BinaryOps bop, size_t pos) {
    (void)pos;

    switch (bop) {
        case NOP:   return rhs;
        case PLUS:  return lhs + rhs;
        case DASH:  return lhs + rhs;
        case STAR:  return lhs * rhs;
        case SLASH: return lhs / rhs;
        default:    return 0.0f;
    }
}

typedef struct TreeNode {
    double value = NAN;
    BinaryOps bop = BinaryOps::NOP;
} TreeNode;

void tree_dump(std::vector<TreeNode> const& root) {
    size_t index = 0;

    printf("\n===[ Tree dump ]===\n");
    for (const auto& node : root) {
        fmt::println("(#{})", index);
        fmt::println(".value = {}", node.value);
        fmt::println(".bop   = {}\n", BinaryOpsLiteral[node.bop]);
        index++;
    }
}

double bc_implementation(std::vector<TreeNode> const& root) {
    std::vector<TreeNode> collapsed = root;

    for (size_t i = 0; i < collapsed.size(); ++i) {
        auto& node = collapsed[i];

        if (node.bop == BinaryOps::STAR) {
            auto& next = collapsed[i + 1];

            next.value = node.value * next.value;
            node.value = NAN;
        }

        else if (node.bop == BinaryOps::SLASH) {
            auto& next = collapsed[i + 1];

            next.value = node.value / next.value;
            node.value = NAN;
        }
    }

    for (size_t i = 0; i < collapsed.size(); ++i) {
        auto& node = collapsed[i];

        if (node.bop == BinaryOps::PLUS) {
            size_t j = i + 1;
            TreeNode* next = nullptr;

            for (; j < collapsed.size(); ++j) {
                if (!std::isnan(collapsed[j].value)) {
                    next = &collapsed[j];
                    break;
                }
            }

            if (next == nullptr)
                throw bc_parse_exception(i, "The node is missing an rvalue");

            next->value = node.value + next->value;
            i = j - 1;
        }

        else if (node.bop == BinaryOps::DASH) {
            size_t j = i + 1;
            TreeNode* next = nullptr;

            for (; j < collapsed.size(); ++j) {
                if (!std::isnan(collapsed[j].value)) {
                    next = &collapsed[j];
                    break;
                }
            }

            if (next == nullptr)
                throw bc_parse_exception(i, "The node is missing an rvalue");

            next->value = node.value - next->value;
            i = j - 1;
        }
    }

    return collapsed.back().value;
}

double bc(std::string source) {
    size_t pos = 0;
    std::vector<TreeNode> root;

    std::string literal = "";

    for (const char& ch : source) {
        if ('0' <= ch && ch <= '9')
            literal += ch;

        else if (ch == '.') {
            if (literal.find('.') != std::string::npos)
                throw bc_parse_exception(pos, "Numeric value cannot contains multiple floating points");

            literal += ch;
        }

        else if (ch != ' ' && BinaryOpsLiteral.find(ch) != std::string::npos) {
            BinaryOps bop = BinaryOps::NOP;

            switch (ch) {
                case '+':
                    bop = BinaryOps::PLUS;
                    break;
                case '-':
                    bop = BinaryOps::DASH;
                    break;
                case '*':
                    bop = BinaryOps::STAR;
                    break;
                case '/':
                    bop = BinaryOps::SLASH;
                    break;
                default: 
                    throw bc_parse_exception(pos, "Binary operator not implemented yet");
            }

            root.push_back(TreeNode { .value = atof(literal.c_str()), .bop = bop });
            literal = "";
        }

        else if (ch == ' ') { /* Skip spaces */ }
        else throw bc_parse_exception(pos, "Unknown character found");

        pos++;
    }

    root.push_back(TreeNode { .value = atof(literal.c_str()) });

#ifdef LOG_TRACE
    tree_dump(root);
#endif

    return bc_implementation(root);
}
