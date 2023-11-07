#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include "Tokenizer.h"
#include "Parser.h"



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
