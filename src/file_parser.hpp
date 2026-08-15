#pragma once

#include "stringify.hpp"
#include "tokenizer/scanner.hpp"
#include "tokenizer/token.hpp"
#include "tokenizer/token_kind.hpp"
#include <filesystem>
#include <string>
#include <vector>

#undef IGNORE
#define IGNORE return 0
#define CHECK(fn) {                                  \
    auto [size, data] = fn(i, tokens, &resolve);     \
    if( size == 0 ) result.push_back(token);         \
    i = size;                                        \
    auto tks = Scanner::read(data, data.size());     \
    for( Token tk : tks ) result.push_back(tk);      \
} continue

struct FileContent {
    enum FPragma { once, always };
    FPragma fpragma;
    std::string content;

    FileContent() : fpragma(once) {}
    FileContent(std::string content)
        : content(content),
          fpragma(once) {}

    static FileContent from(std::string content, std::string file = "") {
        size_t size = content.size();
        std::vector<char> src = std::vector<char>(content.begin(), content.end());
        auto tokens = Scanner::read(src, size);
        std::vector<Token> result;

        FileContent resolve;

        for( size_t i = 0; i < tokens.size(); i++ ) {
            auto token = tokens.at(i);
            switch(token.kind) {
                case PRAGMA: CHECK(pragmatic);
                default: result.push_back(token);
            }
        }

        resolve.content =
            "@pushfile \"" + std::filesystem::absolute(file).string() + "\";\n" +
            stringify(result) + "\n"
            "@popfile\n";
        return resolve;
    }

    static std::tuple<size_t, std::vector<char>> pragmatic(const size_t i, std::vector<Token>& tokens, FileContent *fcontent) {
        #undef IGNORE
        #define IGNORE return { 0, {} }

        auto identifier = tokens.at(i + 1);
        if( identifier.kind != IDENTIFIER  ) IGNORE;

        if( identifier.lexeme == "always" ) {
            fcontent->fpragma = FPragma::always;
            return { 1, std::vector<char>() };
        }

        IGNORE;
    }
};
