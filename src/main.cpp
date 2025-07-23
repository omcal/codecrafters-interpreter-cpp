#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "token.h"

std::string read_file_contents(const std::string& filename);
void add_token(TokenType type, std::string lexeme, std::optional<std::string> literal = std::nullopt);
bool is_digit(char c);
bool is_alpha(char c);
bool is_alnum(char c);
std::string normalize_number(const std::string& number);
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
                        i++; 
                    } else {
                        add_token(TokenType::BANG, "!");
                    }
                    break;
                    
                case '=':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::EQUAL_EQUAL, "==");
                        i++; 
                    } else {
                        add_token(TokenType::EQUAL, "=");
                    }
                    break;
                    
                case '<':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::LESS_EQUAL, "<=");
                        i++; 
                    } else {
                        add_token(TokenType::LESS, "<");
                    }
                    break;
                    
                case '>':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                        add_token(TokenType::GREATER_EQUAL, ">=");
                        i++; 
                    } else {
                        add_token(TokenType::GREATER, ">");
                    }
                    break;
                    
                case '/':
                    if (i + 1 < file_contents.length() && file_contents[i + 1] == '/') {
                        i++; 
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
                    i++; 
                    
                    while (i < file_contents.length() && file_contents[i] != '"') {
                        if (file_contents[i] == '\n') {
                            line_number++;
                        }
                        value += file_contents[i];
                        lexeme += file_contents[i];
                        i++;
                    }
                    
                    if (i >= file_contents.length()) {
                        std::cerr << "[line " << line_number << "] Error: Unterminated string." << std::endl;
                        ret_val = 65;
                        break;
                    }
                    
                    lexeme += "\"";
                    
                    Token token = Token(TokenType::STRING, lexeme, value, line_number);
                    tokens.push_back(token);
                    std::cout << token << std::endl;
                    break;
                }
                
                default:
                    if (is_digit(c)) {
                        // Handle number literals
                        std::string number = "";
                        size_t start = i;
                        
                        // Consume all consecutive digits
                        while (i < file_contents.length() && is_digit(file_contents[i])) {
                            number += file_contents[i];
                            i++;
                        }
                        
                        // Check for decimal point
                        if (i < file_contents.length() && file_contents[i] == '.' && 
                            i + 1 < file_contents.length() && is_digit(file_contents[i + 1])) {
                            number += ".";
                            i++; 
                            
                            while (i < file_contents.length() && is_digit(file_contents[i])) {
                                number += file_contents[i];
                                i++;
                            }
                        }
                        
                        i--;
                        
                        std::string literal = normalize_number(number);
                        
                        Token token = Token(TokenType::NUMBER, number, literal, line_number);
                        tokens.push_back(token);
                        std::cout << token << std::endl;
                    } else if (is_alpha(c)) {
                        // Handle identifiers
                        std::string identifier = "";
                        size_t start = i;
                        
                        // Consume all consecutive alphanumeric characters
                        while (i < file_contents.length() && is_alnum(file_contents[i])) {
                            identifier += file_contents[i];
                            i++;
                        }
                        
                        i--;
                        
                        Token token = Token(TokenType::IDENTIFIER, identifier, std::nullopt, line_number);
                        tokens.push_back(token);
                        std::cout << token << std::endl;
                    } else {
                        std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl;
                        ret_val = 65;
                    }
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

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

std::string normalize_number(const std::string& number) {
    // If it's an integer (no decimal point), add ".0"
    if (number.find('.') == std::string::npos) {
        return number + ".0";
    }
    
    // If it has a decimal point, remove trailing zeros but keep at least one digit after decimal
    std::string result = number;
    
    // Remove trailing zeros after decimal point
    while (result.length() > 2 && result.back() == '0' && result[result.length() - 2] != '.') {
        result.pop_back();
    }
    
    return result;
}
