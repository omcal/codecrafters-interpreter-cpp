#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "token.h"

std::string read_file_contents(const std::string& filename);
void add_token(TokenType type, std::string lexeme, std::optional<std::string> literal = std::nullopt);
std::vector<Token> tokens;
int line_number = 1;

int main(int argc, char *argv[]) {
    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        int ret_val = 0;
        
        for (size_t i = 0; i < file_contents.length(); i++) {
            char c = file_contents[i];
            
            switch (c) {
                case ' ':
                case '\r':
                case '\t':
                    // Ignore whitespace
                    break;
                    
                case '\n':
                    line_number++;
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
                case '*':
                    add_token(TokenType::STAR, "*");
                    break;
                    
                case '!':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::BANG_EQUAL, "!=");
                        i++; // Skip the '='
                    } else {
                        add_token(TokenType::BANG, "!");
                    }
                    break;
                    
                case '=':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::EQUAL_EQUAL, "==");
                        i++; // Skip the '='
                    } else {
                        add_token(TokenType::EQUAL, "=");
                    }
                    break;
                    
                case '<':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::LESS_EQUAL, "<=");
                        i++; // Skip the '='
                    } else {
                        add_token(TokenType::LESS, "<");
                    }
                    break;
                    
                case '>':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::GREATER_EQUAL, ">=");
                        i++; // Skip the '='
                    } else {
                        add_token(TokenType::GREATER, ">");
                    }
                    break;
                    
                case '/':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '/') {
                        // Skip comment until end of line
                        i++; // Skip second '/'
                        while (i + 1 < file_contents.length() && file_contents[i + 1] != '\n') {
                            i++;
                        }
                    } else {
                        add_token(TokenType::SLASH, "/");
                    }
                    break;
                    
                case '"': {
                    std::string value = "";
                    std::string lexeme = "\"";
                    i++; // Skip opening quote
                    
                    while (i < file_contents.length() && file_contents[i] != '"') {
                        if (file_contents[i] == '\n') {
                            line_number++;
                        }
                        value += file_contents[i];
                        lexeme += file_contents[i];
                        i++;
                    }
                    
                    if (i >= file_contents.length()) {
                        // Unterminated string
                        std::cerr << "[line " << line_number << "] Error: Unterminated string." << std::endl;
                        ret_val = 65;
                        break;
                    }
                    
                    // Add closing quote to lexeme
                    lexeme += "\"";
                    
                    // Create token with literal value
                    Token token = Token(TokenType::STRING, lexeme, value, line_number);
                    tokens.push_back(token);
                    std::cout << token << std::endl;
                    break;
                }
                
                default:
                    std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl;
                    ret_val = 65;
                    break;
            }
        }

        std::cout << "EOF  null" << std::endl;
        return ret_val;
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

void add_token(TokenType type, std::string lexeme, std::optional<std::string> literal) {
    Token token = Token(type, lexeme, literal, line_number);
    tokens.push_back(token);
    std::cout << token << std::endl;
}
