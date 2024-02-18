#pragma once
#include <exception>

#define FMT_HEADER_ONLY
#include <fmt/base.h>
#include <fmt/color.h>

/**
 * @brief Custom exception for API errors
 */
class bc_parse_exception : public std::exception {
    public:
        bc_parse_exception(std::string source, size_t pos, std::string text) 
            : m_source(source), m_pos(pos), m_text(text) {
            m_message = fmt::format("{}: Cannot parse source at position {}: {}\n"
                    "  Source: {}\n"
                    "          {:>{}}\n\0",
                    fmt::format(fmt::emphasis::bold | fg(fmt::color::red), "ERROR"),
                    m_pos, 
                    m_text, 
                    m_source, 
                    "^", pos + 1);
        }

        char *what() { return strdup(m_message.c_str()); }

    private:
        std::string m_source;
        size_t m_pos;
        std::string m_text;
        std::string m_message;
};

/**
 * @brief The basic calculator exposed API function
 *
 * @param source The string representation of the operation to perform
 * @return The result of the operation
 */
double bc(std::string source);
