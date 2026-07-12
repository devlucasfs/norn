#pragma once

#include <fstream>
#include <regex>
#include <string>
#include <filesystem>

#include "../ternary.hpp"
#include "../project.hpp"
#include "outputs.hpp"

std::pair<ternary, std::string> is_a_valid_import(ProjectData& project, std::string raw) {
    std::string data = raw.substr(1, raw.size() - 2);

    std::regex file("[.][/](.*)");
    if( std::regex_match(data, file) ) {
        auto path = std::filesystem::absolute(project.main);
        auto file = path.parent_path() / data.substr(2);

        if(! std::filesystem::exists(file) ) CompilerOutputs::Fatal("You can't import " + data + " because it was not found.");
        if( std::filesystem::is_directory(file) ) CompilerOutputs::Fatal("You can't import " + data + " because it is a directory.");

        std::ifstream content(file, std::ios::binary | std::ios::ate);
        if(! content.is_open() ) CompilerOutputs::Fatal("Can't read " + data);

        std::streamsize size = content.tellg();
        content.seekg(0, std::ios::beg);

        std::vector<char> src(size);
        if(! content.read(src.data(), size) ) CompilerOutputs::Fatal("Can't read " + data);

        std::string code(src.begin(), src.end());
        return { ternary(ternary::data::tr), code };
    }

    return { ternary(ternary::data::fl), "" };
}
