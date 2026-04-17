#include <string_view>
#include <cstdint>

#include <fmt/base.h>
#include <vector>

#include "../include/regex.hpp"

enum class TokenType : uint8_t {
    Literal,     // 'a', 'b', etc.
    Dot,         // .
    Star,        // *
    Plus,        // +
    Question,    // ?
    Alternation, // |
    LParen,      // (
    RParen,      // )

    Concat
};

struct Token {
    TokenType type;
    char value = 0; // for literal

    int precedence = 0;
    bool right_associative = false;
};

int
main(int argc, char** argv) {
    if (argc < 3) {
        fmt::println("USAGE:\n\t{} <Pattern> <String>\n", argv[0]);
        return 1;
    }

    const std::string_view pattern{argv[1]};
    const std::string_view input{argv[2]};

    std::vector<Token> tokens;
    std::vector<Token> postfix;

    return 0;
}
