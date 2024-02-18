#include "../include/cppbc.hpp"
#include <vector>

// #define LOG_TRACE

#define BinaryOpsLiteral std::string(" +-*/")
enum BinaryOps { NOP, PLUS, DASH, STAR, SLASH };

typedef struct TreeNode {
    double value = NAN;
    BinaryOps bop = BinaryOps::NOP;
} TreeNode;

/**
 * @brief Dump the tree structure on the stdout
 *
 * @param root The tree to dump
 */
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

/**
 * @brief Implementation of the basic calculator
 *
 * @param root The tree representing the source
 * @return The result of the operation
 */
double bc_implementation(std::string source, std::vector<TreeNode> const& root) {
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
                throw bc_parse_exception(source, i, "The node is missing an rvalue");

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
                throw bc_parse_exception(source, i, "The node is missing an rvalue");

            next->value = node.value - next->value;
            i = j - 1;
        }
    }

    return collapsed.back().value;
}

double bc(std::string source) {
    std::vector<TreeNode> root;
    std::string literal = "";

    for (size_t pos = 0; pos < source.length(); ++pos) {
        char ch = source[pos];

        if ('0' <= ch && ch <= '9')
            literal += ch;

        else if (ch == '.') {
            if (literal.find('.') != std::string::npos)
                throw bc_parse_exception(source, pos, "Numeric value cannot contains multiple floating points");

            literal += ch;
        }

        else if (ch != ' ' && BinaryOpsLiteral.find(ch) != std::string::npos) {
            if (literal == "")
                throw bc_parse_exception(source, pos, "Cannot declare a binary operator without a previous numeric value");

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
                    throw bc_parse_exception(source, pos, "Binary operator not implemented yet");
            }

            root.push_back(TreeNode { .value = atof(literal.c_str()), .bop = bop });
            literal = "";
        }

        else if (ch == '(') {
            size_t brackets_count = 1;
            size_t i = pos + 1;

            for (; i < source.length(); ++i) {
                if (source[i] == '(') 
                    brackets_count++;
                
                else if (source[i] == ')') 
                    brackets_count--;

                if (brackets_count == 0) 
                    break;
            }

            if (brackets_count > 0)
                throw bc_parse_exception(source, pos, "Brackets mismatch");

            std::string exp_result = std::to_string(bc(source.substr(pos + 1, i - pos - 1)));
            source.erase(pos, i - pos + 1);
            source.insert(pos, exp_result);
            pos--;
        }

        else if (ch == ' ') { /* Skip spaces */ }
        else {
            fmt::println("[info] ch: '{}'", ch);
            throw bc_parse_exception(source, pos, "Unknown character found");
        }
    }

    root.push_back(TreeNode { .value = atof(literal.c_str()) });

#ifdef LOG_TRACE
    fmt::println("[INFO] (cppbc) Source: {}", source);
    tree_dump(root);
#endif

    return bc_implementation(source, root);
}
