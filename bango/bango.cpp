#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

enum class TokenType {
    FUNCTION,
    PRINT,
    STRING_LITERAL,
    SEMI,
    OPEN_PAREN, 
    CLOSE_PAREN,
    OPEN_BRACE,
    CLOSE_BRACE,
    END_OF_FILE,
    UNKNOWN
};


struct Token {
    TokenType type;
    std::string text;
};

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

int main() {
    try {
        std::ifstream file("main.bango");
        if (!file.is_open()) {
            std::cerr << "Could not open file." << std::endl;
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string code = buffer.str();

        Tokenizer tokenizer(code);
        std::vector<Token> tokens = tokenizer.tokenize();

     //   for (const auto& token : tokens) {
     //       std::cout << "Token type: " << static_cast<int>(token.type)
     //           << ", text: '" << token.text << "'" << std::endl;
     //   }
        Parser parser(tokens);
        parser.parse();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
     
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Press ENTER to exit...";
        std::cin.get();
        return 1;
    }


    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press ENTER to exit...";
    std::cin.get();

    return 0;
}
