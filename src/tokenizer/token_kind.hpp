// Isto define os tipos de tokens possiveis de se ter em carla
#pragma once

#include <string>
#define CARLA_FIELDS_TOKENS_KIND \
    X(LEFT_PAREN, "(") \
    X(RIGHT_PAREN, ")") \
    X(LEFT_BRACE, "{") \
    X(RIGHT_BRACE, "}") \
    X(LEFT_BRACKET, "[") \
    X(RIGHT_BRACKET, "]") \
 \
    X(COMMA, ",") \
    X(DOT, ".") \
    X(MINUS, "-") \
    X(PLUS, "+") \
    X(SEMICOLON, ";") \
    X(SLASH, "/") \
    X(STAR, "*") \
    X(PLUS_PLUS, "++") \
    X(MINUS_MINUS, "--") \
    X(QUAD, "::") \
\
    X(ITERABLE, "..") \
    X(COLON, ":") \
    X(COLON_EQUAL, ":=") \
 \
    X(ARROW, "->") \
 \
    X(BANG, "!") \
    X(BANG_EQUAL, "!=") \
    X(EQUAL, "=") \
    X(EQUAL_EQUAL, "==") \
    X(GREATER, ">") \
    X(GREATER_EQUAL, ">=") \
    X(SHIFT_RIGHT, ">>") \
    X(LESS, "<") \
    X(LESS_EQUAL, "<=") \
    X(SHIFT_LEFT, "<<") \
    X(SLASH_SLASH, "//") \
 \
    X(IDENTIFIER, "<identifier>") \
    X(STRING, "<string>") \
    X(_FLOAT, "<float>") \
    X(INTEGER, "<integer>") \
 \
    X(AND, "and") \
    X(OR, "or") \
    X(NIL, "nil") \
    X(_FALSE, "false") \
    X(_TRUE, "true") \
    X(XOR, "xor") \
    X(LAND, "&") \
    X(LOR, "|") \
    X(LNOT, "~") \
 \
    X(_NAMESPACE, "namespace") \
    X(RETURN, "return") \
    X(PUTS, "puts") \
    X(AUTO, "auto") \
    X(OUR, "our") \
    X(FOR, "for") \
    X(STRUCT, "struct") \
    X(ENUM, "enum") \
    X(BREAK, "break") \
    X(CONTINUE, "continue") \
    X(GPIO, "gpio") \
    X(IF, "if") \
    X(_CONST, "const") \
    X(_CONSTEXPR, "constexpr") \
    X(ELSE, "else") \
    X(LAYOUT, "layout") \
    X(LET, "let") \
    X(MUT, "mut") \
    X(END_KEYWORDS, "<end_keywords>") \
    \
    X(PLUS_EQUAL, "+=") \
    X(MINUS_EQUAL, "-=") \
    X(STAR_EQUAL, "*=") \
    X(SLASH_EQUAL, "/=") \
    \
    X(START, "@_start") \
    X(CAST, "@cast") \
    X(DETACH, "@detach") \
    \
    X(CARLA_EOF, "<eof>") \
    X(CARLA_LNREPEAT, "<LNREPEAT>") \
    X(CARLA_LNREPEAT_LITERAL, "<LNREPEAT + LITERAL>") \
    X(UNKNOWN_TK, "<unknown>") \
    X(TYPED_PLACEHOLDER, "<placeholder:input>") \
    X(PLACEHOLDER, "<placeholder>") \
    X(PUSH_F, "@pushfile") \
    X(POP_F, "@popfile") \
    X(IMPORT, "@import") \
    X(PRAGMA, "@pragma") \
    X(DEFINE, "@define")

#define X(id, _) id,
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
    #define X(id, data) case id: return data;
    switch(kind) { CARLA_FIELDS_TOKENS_KIND }
    #undef X
}
