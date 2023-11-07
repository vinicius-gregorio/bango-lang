#include "TokenType.h"
#include "Token.h"
#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#pragma once
class Parser {
private:
    void error(const std::string& message) {
        std::cerr << message << std::endl;

        throw std::runtime_error(message);
    }


    std::vector<Token> tokens;
    size_t position = 0;

    void mainFunction() {
        eat(TokenType::FUNCTION);
        eat(TokenType::OPEN_PAREN);
        eat(TokenType::CLOSE_PAREN);
        eat(TokenType::OPEN_BRACE);


        while (tokens[position].type != TokenType::CLOSE_BRACE) {
            printStatement();
        }

        eat(TokenType::CLOSE_BRACE);
    }


    void printStatement() {
        eat(TokenType::PRINT);
        eat(TokenType::OPEN_PAREN);
        Token token = tokens[position];
        if (token.type == TokenType::STRING_LITERAL) {
            std::cout << token.text << std::endl;
            eat(TokenType::STRING_LITERAL);
            eat(TokenType::CLOSE_PAREN);
            eat(TokenType::SEMI);
        }
        else {
            error("Error: Expected a string literal.");
        }
    }
    void functionDeclaration() {
        eat(TokenType::FUNCTION);
        eat(TokenType::OPEN_BRACE);
        eat(TokenType::CLOSE_BRACE);
        printStatement();
    }

public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    void parse() {
        if (tokens[position].type == TokenType::FUNCTION &&
            tokens[position].text == "bango") {
            mainFunction();
        }
        else {
            error("Error: Unexpected token. Expected the 'bango()' function declaration.");
        }
    }


    void eat(TokenType type) {
        if (tokens[position].type == type) {
            position++;
        }
        else {
            error("Error: Expected token type " +
                std::to_string(static_cast<int>(type)) +
                " but found " +
                std::to_string(static_cast<int>(tokens[position].type)));
        }
    }

};