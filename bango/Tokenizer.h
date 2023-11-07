#include "TokenType.h"
#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Token.h"

class Tokenizer {
private:
    std::string source;
    size_t position = 0;

    char advance() {
        return source[position++];
    }
    Token string() {
        std::string value;
        // The opening quote has already been read to determine that it's a string, so we include it
        value += '"';
        while (position < source.size() && source[position] != '"') {
            value += advance(); // Append the characters of the string literal
        }
        if (position < source.size()) {
            value += advance(); // Append the closing quote and advance past it
        }
        else {
            // Handle the error - the string was not closed before the file ended
            throw std::runtime_error("String literal not closed.");
        }
        return { TokenType::STRING_LITERAL, value };
    }



    Token nextToken() {
        while (position < source.size()) {
            char current = advance();
            switch (current) {
            case ' ': case '\r': case '\t': case '\n': // Skip whitespace
                continue;
            case '(':
                return { TokenType::OPEN_PAREN, "(" };
            case ')':
                return { TokenType::CLOSE_PAREN, ")" };
            case '{':
                return { TokenType::OPEN_BRACE, "{" };
            case '}':
                return { TokenType::CLOSE_BRACE, "}" };
            case ';':
                return { TokenType::SEMI, ";" };
            case '"':
                return string();
            default:
                if (std::isalpha(current)) {
                    std::string text(1, current);
                    while (position < source.size() && std::isalnum(source[position])) {
                        text += advance();
                    }
                    if (text == "bango") {
                        return { TokenType::FUNCTION, text };
                    }
                    else if (text == "print") {
                        return { TokenType::PRINT, text };
                    }
                }
                return { TokenType::UNKNOWN, std::string(1, current) };
            }
        }

        return { TokenType::END_OF_FILE, "" };
    }

public:
    explicit Tokenizer(const std::string& source) : source(source) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token;
        do {
            token = nextToken();
            tokens.push_back(token);
        } while (token.type != TokenType::END_OF_FILE);
        return tokens;
    }
};