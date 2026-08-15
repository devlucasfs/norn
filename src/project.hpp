#pragma once

#include "outputs.hpp"
#include "eva.hpp"
#include <stdexcept>
#include <string>

struct ProjectData {
    std::string binary;
    std::string main;

    ProjectData() = default;

    ProjectData(std::string binary)
        : binary(binary) {};

    static ProjectData get(std::string norn, std::string path) {
        ProjectData result;
        eva driver(path);

        try { auto [_, main]   = driver.get<std::string>("target", "main");
              result.main = main;
        } catch(std::runtime_error error) {
            CompilerOutputs::Warn(error.what() + std::string("\n"));
            result.main = "./src/main.crl";
        }

        try { auto [_, binary]   = driver.get<std::string>("precompiler", "name");
              result.binary = binary;
              if( binary != norn && binary != "norn" )
                  CompilerOutputs::Warn("Your target there isn't Norn as your pre-compiler. Is recommended use the defined pre-compiler. But this time Norn will be used.\n");
        } catch(std::runtime_error error) {
            CompilerOutputs::Warn(error.what() + std::string("\n"));
            result.binary = norn;
        }

        return result;
    }
};
