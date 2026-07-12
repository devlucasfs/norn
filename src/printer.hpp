#pragma once

#include "tokenizer/token.hpp"
#include "tokenizer/token_kind.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>

std::string printer(std::vector<Token> result) {
    std::stringstream ss;

    int tabs = 0;
    for( int i = 0; i < result.size(); i++ ) {
        auto token = result.at(i);
        int k;
        if( i == 0 || (k = result.at(i - 1).kind) == SEMICOLON || k == LEFT_BRACE || token.kind == RIGHT_BRACE ) {
            ss << std::string(
                token.kind != RIGHT_BRACE
                ? (  tabs < 0       ? 0 : tabs     )
                : ( (tabs - 1) < 0  ? 0 : tabs - 1 ),

                '\t'
            );
        }
        else ss << " ";
        #define X(kind, val) case kind: { ss << val; }break;
        switch(token.kind) {
            case LEFT_BRACE : { tabs++; ss << "{\n"; } break;
            case RIGHT_BRACE: { tabs--; ss << "}"; }   break;
            case SEMICOLON  : { ss << ";\n"; }         break;

            case STRING : case IDENTIFIER:
            case INTEGER: case _FLOAT: {
                ss << token.lexeme;
            } break;
            case CARLA_EOF: case UNKNOWN_TK: break;
            default: switch (token.kind) {
                CARLA_FIELDS_TOKENS_KIND
            } break;
        }

        #undef X
        if( i + 1 < result.size() && (k = result.at(i + 1).kind) == RIGHT_BRACE ) {
            ss << '\n';
        }
    }

    return ss.str();
}
