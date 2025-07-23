#pragma once

#include <string>
#include <optional>

enum TokenType
{
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    EQUAL_EQUAL,
    EQUAL,
    BANG,
    BANG_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    STRING,
    NUMBER,
    IDENTIFIER,
    EOF_TOKEN
};

class Token
{
private:
    const TokenType type;
    const std::string lex;
    const std::optional<std::string> literal;
    const int line;

public:
    Token(TokenType type, const std::string &lexeme, const std::optional<std::string> literal, int line);
    ~Token();
    TokenType get_type() const;
    std::string get_lexeme() const;
    std::string get_literal() const;
    int get_line() const;
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

std::string TokentypeToString(TokenType type);