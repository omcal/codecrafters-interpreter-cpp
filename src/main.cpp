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
TokenType get_keyword_type(const std::string& text);
std::string normalize_number(const std::string& number);
std::vector<Token> tokens;
int line_number = 1;

void parse_file(const std::string& filename);

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
                        std::string number = "";
                        size_t start = i;
                        
                        while (i < file_contents.length() && is_digit(file_contents[i])) {
                            number += file_contents[i];
                            i++;
                        }
                        
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
                        std::string identifier = "";
                        size_t start = i;
                        
                        while (i < file_contents.length() && is_alnum(file_contents[i])) {
                            identifier += file_contents[i];
                            i++;
                        }
                        
                        i--;
                        
                        TokenType tokenType = get_keyword_type(identifier);
                        if (tokenType == TokenType::IDENTIFIER) {
                            Token token = Token(TokenType::IDENTIFIER, identifier, std::nullopt, line_number);
                            tokens.push_back(token);
                            std::cout << token << std::endl;
                        } else {
                            Token token = Token(tokenType, identifier, std::nullopt, line_number);
                            tokens.push_back(token);
                            std::cout << token << std::endl;
                        }
                    } else {
                        std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl;
                        ret_val = 65;
                    }
                    break;
            }
        }

        std::cout << "EOF  null" << std::endl;
        return ret_val;
    } else if (command == "parse") {
        parse_file(argv[2]);
        return 0;
    }
    
    std::cerr << "Unknown command: " << command << std::endl;
    return 1;
}

std::string parse_expression(size_t& index);

void parse_file(const std::string& filename) {
    std::string file_contents = read_file_contents(filename);
    
    // Reset tokens and line number for parsing
    tokens.clear();
    line_number = 1;
    
    // Tokenize the input first
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
                
            case '(': {
                tokens.push_back(Token(TokenType::LEFT_PAREN, "(", std::nullopt, line_number));
                break;
            }
            case ')': {
                tokens.push_back(Token(TokenType::RIGHT_PAREN, ")", std::nullopt, line_number));
                break;
            }
            
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
                    // Handle unterminated string error if needed
                    break;
                }
                
                lexeme += "\"";
                
                Token token = Token(TokenType::STRING, lexeme, value, line_number);
                tokens.push_back(token);
                break;
            }
                
            default:
                if (is_digit(c)) {
                    std::string number = "";
                    size_t start = i;
                    
                    while (i < file_contents.length() && is_digit(file_contents[i])) {
                        number += file_contents[i];
                        i++;
                    }
                    
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
                } else if (is_alpha(c)) {
                    std::string identifier = "";
                    size_t start = i;
                    
                    while (i < file_contents.length() && is_alnum(file_contents[i])) {
                        identifier += file_contents[i];
                        i++;
                    }
                    
                    i--;
                    
                    TokenType tokenType = get_keyword_type(identifier);
                    Token token = Token(tokenType, identifier, std::nullopt, line_number);
                    tokens.push_back(token);
                }
                break;
        }
    }
    
    // Parse the tokens and output the result
    if (!tokens.empty()) {
        size_t index = 0;
        std::string result = parse_expression(index);
        std::cout << result << std::endl;
    }
}

std::string parse_expression(size_t& index) {
    if (index >= tokens.size()) {
        return "";
    }
    
    Token token = tokens[index];
    
    if (token.get_type() == TokenType::LEFT_PAREN) {
        index++; // consume '('
        std::string inner = parse_expression(index);
        if (index < tokens.size() && tokens[index].get_type() == TokenType::RIGHT_PAREN) {
            index++; // consume ')'
        }
        return "(group " + inner + ")";
    } else if (token.get_type() == TokenType::TRUE) {
        index++;
        return "true";
    } else if (token.get_type() == TokenType::FALSE) {
        index++;
        return "false";
    } else if (token.get_type() == TokenType::NIL) {
        index++;
        return "nil";
    } else if (token.get_type() == TokenType::NUMBER) {
        index++;
        return token.get_literal();
    } else if (token.get_type() == TokenType::STRING) {
        index++;
        return token.get_literal();
    }
    
    index++;
    return "";
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

TokenType get_keyword_type(const std::string& text) {
    if (text == "and") return TokenType::AND;
    if (text == "class") return TokenType::CLASS;
    if (text == "else") return TokenType::ELSE;
    if (text == "false") return TokenType::FALSE;
    if (text == "for") return TokenType::FOR;
    if (text == "fun") return TokenType::FUN;
    if (text == "if") return TokenType::IF;
    if (text == "nil") return TokenType::NIL;
    if (text == "or") return TokenType::OR;
    if (text == "print") return TokenType::PRINT;
    if (text == "return") return TokenType::RETURN;
    if (text == "super") return TokenType::SUPER;
    if (text == "this") return TokenType::THIS;
    if (text == "true") return TokenType::TRUE;
    if (text == "var") return TokenType::VAR;
    if (text == "while") return TokenType::WHILE;
    
    // If it's not a keyword, return IDENTIFIER
    return TokenType::IDENTIFIER;
}

std::string normalize_number(const std::string& number) {
    if (number.find('.') == std::string::npos) {
        return number + ".0";
    }
    
    std::string result = number;
    
    while (result.length() > 2 && result.back() == '0' && result[result.length() - 2] != '.') {
        result.pop_back();
    }
    
    return result;
}
