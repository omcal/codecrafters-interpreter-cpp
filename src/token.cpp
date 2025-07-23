#include <string>
#include "token.h"
#include <iostream>
#include <optional>

Token::Token(TokenType t, const std::string &lex, const std::optional<std::string> literal, int line)
    : type(t), lex(lex), literal(literal), line(line) {
        
    }

Token::~Token() {}

TokenType Token::get_type() const
{
    return type;
}
std::string Token::get_lexeme() const
{
    return lex;
}
std::string Token::get_literal() const
{
    return literal.has_value() ? literal.value() : "null";
}
int Token::get_line() const
{
    return line;
}

std::ostream & operator<<(std::ostream &os, const Token &token)
{
    os << TokentypeToString(token.type) << " " << token.get_lexeme() << " " << token.get_literal();
    return os;
}

std::string TokentypeToString(TokenType type)
{
    switch (type)
    {
    case LEFT_PAREN:
        return "LEFT_PAREN";
    case RIGHT_PAREN:
        return "RIGHT_PAREN";
    case RIGHT_BRACE:
        return "RIGHT_BRACE";
    case LEFT_BRACE:
        return "LEFT_BRACE";
    case COMMA:
        return "COMMA";
    case DOT:
        return "DOT";
    case MINUS:
        return "MINUS";
    case PLUS :
        return "PLUS";
    case SEMICOLON:
        return "SEMICOLON";
    case SLASH:
        return "SLASH";
    case STAR:
        return "STAR";
    case EOF_TOKEN:
        return "EOF";
    case EQUAL:
        return "EQUAL";
    case EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case BANG:
        return "BANG";
    case BANG_EQUAL:
        return "BANG_EQUAL";
    case LESS:
        return "LESS";
    case LESS_EQUAL:
        return "LESS_EQUAL";
    case GREATER:
        return "GREATER";
    case GREATER_EQUAL:
        return "GREATER_EQUAL";    

    default:
        return "UNKNOWN_TOKEN";
    }
}