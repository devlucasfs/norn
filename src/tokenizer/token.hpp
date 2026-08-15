#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "token_kind.hpp"

struct Token {
  public:
    TokenKind kind;
    std::string lexeme;
    std::string typement;
    int line;

    static Token build(TokenKind kind, std::string lexeme, std::string typement, int line);
    std::string to_string();

    static int needsLineFeed(int pos, std::vector<Token> tokens) {
        if( tokens.size() <= pos + 1 ) return 0;

        auto current = tokens.at(pos), next = tokens.at(pos + 1);

        if( next.kind == PUSH_F || next.kind == CARLA_LNREPEAT_LITERAL ) return 1;
        return next.line - current.line;
    }
};

Token
Token::build(TokenKind kind, std::string lexeme, std::string typement, int line) {
    return (Token) { kind, lexeme, typement, line };
}

std::string
Token::to_string() {
    std::stringstream stream;
    stream << "Token { buffer: `" << this->lexeme << "` kind: " << tokenKindToString(this->kind) << " : " << this->kind <<  " line: " << this->line << " }";
    return stream.str();
}
