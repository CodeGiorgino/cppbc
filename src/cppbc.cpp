#include "../include/cppbc.hpp"
#include <cmath>
#include <string>

#define BinaryOpsLiteral std::string("+-*/")
enum BinaryOps { NOP, PLUS, DASH, STAR, SLASH };

float bop_apply(float lhs, float rhs, BinaryOps bop, size_t pos) {
    (void)pos;

    switch (bop) {
        // throw bc_parse_exception(pos, "Cannot apply NOP binary operator");
        case NOP:   return rhs;
        case PLUS:  return lhs + rhs;
        case DASH:  return lhs + rhs;
        case STAR:  return lhs * rhs;
        case SLASH: return lhs / rhs;
        default:    return 0.0f;
    }
}

float bc(std::string source) {
    float retval = NAN;

    BinaryOps current_bop = BinaryOps::NOP;
    std::string literal = "";
    size_t pos = 0;

    for (char ch : source) {
        /* Number parsing */
        if (std::isdigit(ch))
            literal += ch;

        else if (ch == '.') {
            if (literal.find(".") == std::string::npos)
                throw bc_parse_exception(pos, "Value cannot contains multiple dots");

            literal += ch;
        }

        /* Operation token parsing */
        else if (BinaryOpsLiteral.find(ch) != std::string::npos) {
            if (current_bop != BinaryOps::NOP && literal == "")
                throw bc_parse_exception(pos, "Cannot concatenate binary operators");

            retval = bop_apply(retval, atof(literal.c_str()), current_bop, pos);
            literal = "";

            switch (ch) {
                case '+':
                    current_bop = BinaryOps::PLUS;
                    break;

                case '-':
                    current_bop = BinaryOps::DASH;
                    break;

                case '*':
                    current_bop = BinaryOps::STAR;
                    break;

                case '/':
                    current_bop = BinaryOps::SLASH;
                    break;

                default:
                    throw bc_parse_exception(pos, "Binary operator not implemented");
            }
        }

        /* Extra */
        else if (ch == ' ') {
            /* Skip for now, implements nop check */
        }

        else throw bc_parse_exception(pos, "Unknown character found");

        pos++;
    }

    retval = bop_apply(retval, atof(literal.c_str()), current_bop, pos);
    return retval;
}
