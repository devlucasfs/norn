#pragma once

#include "tokenizer/token.hpp"
#include "tokenizer/token_kind.hpp"
#include <sstream>
#include <vector>

std::string stringify(std::vector<Token> result) {
    std::stringstream ss;
    bool linefeed = false;

    int tabs = 0;
    for( int i = 0; i < result.size(); i++ ) {
        auto token = result.at(i);
        if( token.kind == RIGHT_BRACE ) tabs--;

        if( linefeed ) ss << std::string(tabs, '\t');
        else if( i != 0 ) ss << " ";

        #define X(kind, val) case kind: { ss << val; }break;
        switch(token.kind) {
            case LEFT_BRACE : { tabs++; } goto __continue;
            case STRING : case IDENTIFIER:
            case INTEGER: case _FLOAT: {
                ss << token.lexeme;
            } break;
            case CARLA_EOF: case UNKNOWN_TK: break;
            default: {
                __continue:
                switch(token.kind) {
                    CARLA_FIELDS_TOKENS_KIND
                }
            } break;
        }
        #undef X

        auto amount = Token::needsLineFeed(i, result);
        linefeed = amount > 0 && i < result.size() - 2;
        if( linefeed ) ss << std::string(amount, '\n');
    }

    return ss.str();
}
