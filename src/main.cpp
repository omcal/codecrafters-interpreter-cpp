#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "token.h"

std::string read_file_contents(const std::string& filename);
void add_token(TokenType type,std::string lexeme);
std::vector<Token> tokens;
int main(int argc, char *argv[]) {
    // // Disable output buffering
    // std::cout << std::unitbuf;
    // std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

if (command == "tokenize") {
    std::string file_contents = read_file_contents(argv[2]);
    int index = 0;
    if (!file_contents.empty()) {
        for (auto it = file_contents.begin(); it != file_contents.end(); ++it, index++) {
            switch (*it) {
                case ',':
                    add_token(TokenType::COMMA, ",");
                    break;
                case '.':
                    add_token(TokenType::DOT, ".");
                    break;
                case '-':
                    add_token(TokenType::MINUS, "-");
                    break;
                case '+':
                    add_token(TokenType::PLUS, "+");
                    break;
                case ';':
                    add_token(TokenType::SEMICOLON, ";");
                    break;
                case '/':
                    add_token(TokenType::SLASH, "/");
                    break;
                case '*':
                    add_token(TokenType::STAR, "*");
                    break;
                case '(':
                    add_token(TokenType::LEFT_PAREN, "(");
                    break;
                case ')':
                    add_token(TokenType::RIGHT_PAREN, ")");
                    break;
                case '{':
                    add_token(TokenType::LEFT_BRACE, "{");
                    break;
                case '}':
                    add_token(TokenType::RIGHT_BRACE, "}");
                    break;
                default:
                    break;
            }

            if (!tokens.empty()) {
                std::cout << tokens[index] << std::endl;
            }
        }
    }

    std::cout << "EOF  null" << std::endl;
    return 0; 
}
std::cerr << "Unknown command: " << command << std::endl;
return 1;

}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void add_token(TokenType type,std::string lexeme) {
    Token token = Token(type, lexeme, std::nullopt, 0); 
    tokens.push_back(token);
}
