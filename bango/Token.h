#pragma once
#include "TokenType.h"
#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
struct Token {
    TokenType type;
    std::string text;
};