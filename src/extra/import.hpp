#pragma once

#include <iostream>
#include <regex>
#include <string>

#include "../ternary.hpp"

std::pair<ternary, std::string> is_a_valid_import(std::string raw) {
    std::string data = raw.substr(1, raw.size()-2);

    std::regex file("[.][/](.*)");
    if( std::regex_match(data, file) ) {
    }

    return { ternary(ternary::data::fl), "" };
}
