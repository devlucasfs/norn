// Isto define os tipos de tokens possiveis de se ter em carla
#pragma once

#include <string>
#define CARLA_FIELDS_TOKENS_KIND \
    X(LEFT_PAREN) \
    X(RIGHT_PAREN) \
    X(LEFT_BRACE) \
    X(RIGHT_BRACE) \
    X(LEFT_BRACKET) \
    X(RIGHT_BRACKET) \
    X(COMMA) \
    X(DOT) \
    X(MINUS) \
    X(PLUS) \
    X(SEMICOLON) \
    X(SLASH) \
    X(STAR) \
    X(PLUS_PLUS) \
    X(MINUS_MINUS) \
    X(QUAD) \
    X(ITERABLE) \
    X(COLON) \
    X(COLON_EQUAL) \
    X(ARROW) \
    X(BANG) \
    X(BANG_EQUAL) \
    X(EQUAL) \
    X(EQUAL_EQUAL) \
    X(GREATER) \
    X(GREATER_EQUAL) \
    X(SHIFT_RIGHT) \
    X(LESS) \
    X(LESS_EQUAL) \
    X(SHIFT_LEFT) \
    X(SLASH_SLASH) \
    X(IDENTIFIER) \
    X(STRING) \
    X(_FLOAT) \
    X(INTEGER) \
    X(INCLUDE) \
    X(AND) \
    X(OR) \
    X(NIL) \
    X(_FALSE) \
    X(_TRUE) \
    X(XOR) \
    X(LAND) \
    X(LOR) \
    X(LNOT) \
    X(_CONST) \
    X(PLUS_EQUAL) \
    X(MINUS_EQUAL) \
    X(STAR_EQUAL) \
    X(SLASH_EQUAL) \
    X(CARLA_EOF) \
    X(UNKNOWN_TK) \
    \
    /* SPECIAL PRE-COMPILER TOKENS */ \
    X(IMPORT) \


#define X(id) id,
typedef enum { CARLA_FIELDS_TOKENS_KIND } TokenKind;
#undef X

typedef enum TokenSubKind {
    OPEN_CONTEXT, CLOSE_CONTEXT,

    UNKNOWN
} TokenSubKind;

TokenSubKind getSub(TokenKind kind) {
    switch(kind) {
        case LEFT_PAREN:
        case LEFT_BRACE:
        case LEFT_BRACKET: return OPEN_CONTEXT;

        case RIGHT_BRACE:
        case RIGHT_PAREN:
        case RIGHT_BRACKET: return CLOSE_CONTEXT;

        default: return UNKNOWN;
    }
}

std::string tokenKindToString(TokenKind kind) {
    #define X(id) case id: return #id;
    switch(kind) { CARLA_FIELDS_TOKENS_KIND }
    #undef X
}
