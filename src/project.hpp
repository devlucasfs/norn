#pragma once

#include "outputs.hpp"
#include "eva.hpp"
#include <stdexcept>
#include <string>
#include <vector>

struct ProjectData {
    std::vector<std::string> allow;
    std::string binary;
    std::string main;

    ProjectData() = default;

    ProjectData(std::vector<std::string> allow, std::string binary)
        : allow(allow),
          binary(binary) {};

    static ProjectData get(std::string norn, std::string path) {
        ProjectData result;
        eva driver(path);

        try { auto [_, main]   = driver.get<std::string>("target", "main");
              result.main = main;
        } catch(std::runtime_error error) {
            CompilerOutputs::Warn(error.what() + std::string("\n"));
            result.main = "./src/main.crl";
        }

        try { auto [_, binary]   = driver.get<std::string>("build", "binary");
              result.binary = binary;
              if( binary != norn && binary != "norn" )
                  CompilerOutputs::Warn("Your target there isn't Norn as your pre-compiler. Is recommended use the defined pre-compiler. But this time Norn will be used.\n");
        } catch(std::runtime_error error) {
            CompilerOutputs::Warn(error.what() + std::string("\n"));
            result.binary = norn;
        }

        try { auto [exist, allow] = driver.get<eva::list>("build", "allow");
            if(! exist ) return result;
            for( size_t i = 0; i < allow.size(); i++ ) {
                  auto [_, allowed] = allow.operator[]<std::string>(i);
                  result.allow.push_back(allowed);
              }
        } catch(std::runtime_error error)
        { CompilerOutputs::Warn(error.what() + std::string("\n")); }

        return result;
    }
};
