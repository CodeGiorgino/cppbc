#pragma once
#include <string>
#include <exception>

#define FMT_HEADER_ONLY

#include <fmt/base.h>
#include <fmt/core.h>

class bc_parse_exception : public std::exception {
    public:
        bc_parse_exception(size_t pos, std::string text) : m_pos(pos), m_text(text) {
            m_message = fmt::format("ERROR: Cannot parse source at position {}: {}\n\0",
                    m_pos, m_text);
        }

        char *what() { return strdup(m_message.c_str()); }

    private:
        size_t m_pos;
        std::string m_text;
        std::string m_message;
};

float bc(std::string source);
