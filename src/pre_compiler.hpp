#pragma once

#include "outputs.hpp"
#include "project.hpp"
#include "tokenizer/scanner.hpp"
#include "tokenizer/token.hpp"
#include "tokenizer/token_kind.hpp"
#include "extra/import.hpp"
#include <fstream>
#include <vector>

#define IGNORE return 0

std::vector<Token> tokens = {};
ProjectData data;

size_t constant(const size_t i);

std::string precomp(ProjectData pdata) {
    data = pdata;

    std::ifstream file(data.main, std::ios::binary | std::ios::ate);
    if(! file.is_open() ) CompilerOutputs::Fatal("Invalid main file path in your target.");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> src(size);
    if(! file.read(src.data(), size) ) CompilerOutputs::Fatal("Invalid main file path in your target.");

    tokens = Scanner::read(src, size);

    for( size_t i; i < tokens.size(); i++ ) {
        auto token = tokens.at(i);
        if( token.kind == _CONST ) i += constant(i);
    }

    return "-- Pre-compilador isn't working yet";
};

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

size_t semi(size_t (*func)(const size_t), const size_t i) {
    auto last = func(i);
    if( tokens.at(last + 1).kind != SEMICOLON ) CompilerOutputs::Fatal("You need end the constant expression with a semi.");

    return last + 1;
}

size_t import(const size_t i) {
    /* const std  := @import("std");     -- Get from the std packages
     * const file := @import("./files"); -- Get from the files of your project.
     *                                   -- `./` defines if will catched by the project or the std.
     */

    if(! is_open(i + 1) ) CompilerOutputs::Fatal("After an @import you need open a block.");

    auto content = tokens.at(i + 2);
    if( content.kind != STRING ) CompilerOutputs::Fatal("The 1th argument need be a string.");

    if( is_a_valid_import(content.lexeme) ) {

    }

    if(! is_close(i + 3) ) CompilerOutputs::Fatal("After an @import you need open a block.");

    return i + 3;
}

size_t constant(const size_t i) {
    if( tokens.at(i + 1).kind != IDENTIFIER  ) IGNORE;
    if( tokens.at(i + 2).kind != COLON_EQUAL ) IGNORE;
    switch( tokens.at(i + 3).kind ) {
        case IMPORT: return semi(import, i + 3);
        default: IGNORE;
    }
    IGNORE;
}
