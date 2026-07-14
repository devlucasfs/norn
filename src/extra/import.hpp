#pragma once

#include <fstream>
#include <regex>
#include <string>
#include <filesystem>
#include <vector>

#include "../ternary.hpp"
#include "../project.hpp"
#include "../file_parser.hpp"
#include "outputs.hpp"

std::vector<std::string> imports;

bool was_imported(std::string data) {
    for( auto file : imports )
    /* -> */ if( file == data ) return true;
    return false;
}

std::pair<ternary, std::string> is_a_valid_import(ProjectData& project, std::string raw, bool iterable) {
    std::string data = raw.substr(1, raw.size() - 2);

    std::regex file("[.][/](.*)");
    if( std::regex_match(data, file) ) {
        auto path = std::filesystem::absolute(project.main);
        auto file = path.parent_path() / data.substr(2);

        if( was_imported(file.string()) && iterable ) return { ternary(ternary::data::ne), std::string() };

        if(! std::filesystem::exists(file) ) CompilerOutputs::Fatal("You can't import " + data + " because it was not found.");
        if( std::filesystem::is_directory(file) ) CompilerOutputs::Fatal("You can't import " + data + " because it is a directory.");

        std::ifstream content(file, std::ios::binary | std::ios::ate);
        if(! content.is_open() ) CompilerOutputs::Fatal("Can't read " + data);

        std::streamsize size = content.tellg();
        content.seekg(0, std::ios::beg);

        std::vector<char> src(size);
        if(! content.read(src.data(), size) ) CompilerOutputs::Fatal("Can't read " + data);

        std::string code(src.begin(), src.end());
        auto fcontent = FileContent::from(code);

        if( iterable && fcontent.fpragma == FileContent::once ) imports.push_back(file.string());

        return { ternary(ternary::data::tr), fcontent.content };
    }

    return { ternary(ternary::data::fl), std::string() };
}
