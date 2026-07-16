#pragma once

#include "outputs.hpp"
#include "project.hpp"
#include "tokenizer/scanner.hpp"
#include "tokenizer/token.hpp"
#include "tokenizer/token_kind.hpp"
#include "extra/import.hpp"
#include "ternary.hpp"
#include "stringify.hpp"
#include <fstream>
#include <iostream>
#include <vector>

#undef IGNORE
#define IGNORE return 0
#define CHECK(fn) {                                  \
    auto [size, data] = fn(i);                       \
    if( size == 0 ) result.push_back(token);         \
    i = size;                                        \
    auto tks = Scanner::read(data, data.size());     \
    for( Token tk : tks ) result.push_back(tk);      \
    if( size > 0 && !somechange ) somechange = true; \
} continue

std::vector<Token> result;
std::vector<Token> tokens;
ProjectData data;

std::tuple<size_t, std::vector<char>> constant(const size_t i);
std::tuple<size_t, std::vector<char>> iterable(const size_t i);

std::tuple<std::string, bool> make_it(std::vector<char> src, size_t size) {
    bool somechange = false;

    std::ifstream file(data.main, std::ios::binary | std::ios::ate);
    if(! file.is_open() ) CompilerOutputs::Fatal("Invalid main file path in your target.");

    tokens.clear();
    tokens = Scanner::read(src, size);
    for( size_t i = 0; i < tokens.size(); i++ ) {
        auto token = tokens.at(i);
        switch(token.kind) {
            case _CONST: CHECK(constant);
            case ITERABLE: CHECK(iterable);
            default: result.push_back(token);
        }
    }

    return { stringify(result), somechange };
};

std::string precomp(ProjectData pdata) {
    data = pdata;

    std::ifstream file(data.main, std::ios::binary | std::ios::ate);
    if(! file.is_open() ) CompilerOutputs::Fatal("Invalid main file path in your target.");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> src(size);
    if(! file.read(src.data(), size) ) CompilerOutputs::Fatal("Invalid main file path in your target.");
    tokens = Scanner::read(src, size);

    while(true) {
        auto [ code, changed ] = make_it(src, size);
        if(! changed ) return code;


        src = std::vector<char>(code.begin(), code.end());
        size = src.size();

        result.clear();
    }
}

/* utils */
bool is_open(const size_t i) {
    switch( tokens.at(i).kind ) {
        case LEFT_BRACKET:
        case LEFT_BRACE:
        case LEFT_PAREN: return true;
        default: return false;
    }
}

bool is_close(const size_t i) {
    switch( tokens.at(i).kind ) {
        case RIGHT_BRACKET:
        case RIGHT_BRACE:
        case RIGHT_PAREN: return true;
        default: return false;
    }
}

/* pre-compiler */

template <typename T>
std::pair<size_t, T> semi(size_t (*func)(const size_t, T*), const size_t i) {
    T value;
    auto last = func(i, &value);
    if( tokens.at(last + 1).kind != SEMICOLON ) CompilerOutputs::Fatal("You need end the constant expression with a semi.");

    return { last + 1, value };
}

size_t norn_import(const size_t i, std::string *result, bool iterable = false) {
    /* const std  := @import("std");     -- Get from the std packages
     * const file := @import("./files"); -- Get from the files of your project.
     *                                   -- `./` defines if will catched by the project or the std. */

    if(! is_open(i + 1) ) CompilerOutputs::Fatal("After an @import you need open a block.");

    auto content = tokens.at(i + 2);
    if( content.kind != STRING ) CompilerOutputs::Fatal("The 1th argument need be a string.");

    auto [ter, code] = is_a_valid_import(data, content.lexeme, iterable);
    if( ter.val == ternary::data::fl ) CompilerOutputs::Fatal(content.lexeme + " is an invalid import.");

    if(! is_close(i + 3) ) CompilerOutputs::Fatal("After an @import you need open a block.");

    *result = code;
    return i + 3;
}

size_t import_ns(const size_t i, std::string *result)
{ return norn_import(i, result, false); }

size_t import_iter(const size_t i, std::string *result)
{ return norn_import(i, result, true); }


std::tuple<size_t, std::vector<char>> constant(const size_t i) {
    #undef IGNORE
    #define IGNORE return { 0, {} }

    auto identifier = tokens.at(i + 1);
    if( identifier.kind != IDENTIFIER  ) IGNORE;
    if( tokens.at(i + 2).kind != COLON_EQUAL ) IGNORE;

    switch( tokens.at(i + 3).kind ) {
        case IMPORT: {
            auto [size, imported] = semi<std::string>(import_ns, i + 3);
            auto final = "namespace " + identifier.lexeme + " { " + imported + " }; ";
            std::vector<char> chars(final.begin(), final.end());
            return { size, chars };
        }
        default: IGNORE;
    }
    IGNORE;
}

std::tuple<size_t, std::vector<char>> iterable(const size_t i) {
    #undef IGNORE
    #define IGNORE return { 0, {} }

    switch( tokens.at(i + 1).kind ) {
        case IMPORT: {
            auto [size, imported] = semi<std::string>(import_iter, i + 1);
            std::vector<char> chars(imported.begin(), imported.end());
            return { size, chars };
        }
        default: IGNORE;
    }
    IGNORE;
}
